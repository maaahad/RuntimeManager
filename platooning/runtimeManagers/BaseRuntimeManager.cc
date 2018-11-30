/*
 * BaseRuntimeManager.cc
 *
 *  Created on: Oct 30, 2018
 *      Author: ahad
 */

#include <iostream>

#include "veins/modules/application/platooning/apps/BaseApp.h"
#include "BaseRuntimeManager.h"


//=================================================================================================================================//
// Constructor
//=================================================================================================================================//
BaseRuntimeManager::BaseRuntimeManager(BaseApp *app) {
    // TODO Auto-generated constructor stub
    monitor    = new Monitor(this);
    reactor = new Reactor(this);

    this->app      = app;
    mobility       = app->getMobility();
    traci          = app->getTraci();
    traciVehicle   = app->getTraciVehicle();
    positionHelper = app->getPositionHelper();
}

//=================================================================================================================================//
// Destructor
//=================================================================================================================================//
BaseRuntimeManager::~BaseRuntimeManager() {
    // TODO Auto-generated destructor stub
    delete monitor;
    delete reactor;
}

//=================================================================================================================================//
// StateManager's methods
//=================================================================================================================================//
BaseRuntimeManager::Monitor::Monitor(BaseRuntimeManager* myManager) : myManager(myManager) {

}

bool BaseRuntimeManager::Monitor::connected(int key) {
    auto it = myManager->safetyRecords.find(key);
    simtime_t currentTime = simTime();
    int beaconMissed = (currentTime.dbl() - it->second.lastBeaconArrivalTime.dbl()) / (myManager->app)->getAcceptedAvgBeaconInterval();


//    int myId = myManager->positionHelper->getId();      // Debug

    // Debug
    //std::cout << "beaconMissed: "<< beaconMissed << ", it->second.avgBeaconInterval: " << it->second.avgBeaconInterval << std::endl;
//    if(it->second.nbeaconReceived <= 0) {
//        std::cerr << "Warning: \n\tVehicleId: " << myManager->positionHelper->getId()
//                  << "\n\tit->second.nbeaconReceived: " << it->second.nbeaconReceived
//                  << std::endl;
//    }

//    if (beaconMissed > (myManager->app)->getNAcceptedBeaconMiss() ||
//            it->second.avgBeaconInterval > (myManager->app)->getAcceptedAvgBeaconInterval()) {
    if (beaconMissed > (myManager->app)->getNAcceptedBeaconMiss()) {
        // Assume Connection lost to the vehicle with id key
        // Discard the record
        EV << "Connection to vehicle_" << key << " is lost. Discarding the safety records." << std::endl;
        myManager->safetyRecords.erase(key);
        return false;
    }
    return true;
}

void BaseRuntimeManager::Monitor::checkLog(){
    if(myManager->rmState == BaseRuntimeManager::RMStateMachine::CAR2FRONT_ENGAGED) {
        // Sanity check
        ASSERT((myManager->positionHelper)->getLeaderId() != (myManager->positionHelper)->getFrontId());

        if(!connected((myManager->positionHelper)->getFrontId())) {
            myManager->rmState = BaseRuntimeManager::RMStateMachine::CAR2FRONT_CAR2LEADER_DISENGAGED;
        }
    } else if (myManager->rmState == BaseRuntimeManager::RMStateMachine::CAR2LEADER_ENGAGED) {
        // Sanity check
        ASSERT((myManager->positionHelper)->getLeaderId() != (myManager->positionHelper)->getFrontId());

        if (!connected((myManager->positionHelper)->getLeaderId())) {
            myManager->rmState = BaseRuntimeManager::RMStateMachine::CAR2FRONT_CAR2LEADER_DISENGAGED;
        }
    } else if (myManager->rmState == BaseRuntimeManager::RMStateMachine::CAR2FRONT_CAR2LEADER_ENGAGED){
        if (!connected((myManager->positionHelper)->getLeaderId())) {
            myManager->rmState = ((myManager->positionHelper)->getLeaderId() == (myManager->positionHelper)->getFrontId()) ?
                    BaseRuntimeManager::RMStateMachine::CAR2FRONT_CAR2LEADER_DISENGAGED :
                    BaseRuntimeManager::RMStateMachine::CAR2FRONT_ENGAGED;
        }

        if((myManager->positionHelper)->getLeaderId() != (myManager->positionHelper)->getFrontId()) {
            if(!connected((myManager->positionHelper)->getFrontId())) {
                myManager->rmState = (myManager->rmState == BaseRuntimeManager::RMStateMachine::CAR2FRONT_ENGAGED) ?
                        BaseRuntimeManager::RMStateMachine::CAR2FRONT_CAR2LEADER_DISENGAGED :
                        BaseRuntimeManager::RMStateMachine::CAR2LEADER_ENGAGED;
            }
        }
    } else {
        std::cerr << "Error : wrong rtState"
                             << "\n\tFile: "
                             << __FILE__
                             << "\n\tFunction: "
                             << __func__
                             << "\n\tLine: "
                             << __LINE__
                             << std::endl;
    }
}


void BaseRuntimeManager::Monitor::upgrade() {
    switch((myManager->traciVehicle)->getActiveController()) {
    case Plexe::ACC:
        if(myManager->rmState == BaseRuntimeManager::RMStateMachine::CAR2FRONT_ENGAGED) {
            myManager->switchController = BaseRuntimeManager::SwitchController::ACC_TO_PLOEG;
        } else if (myManager->rmState == BaseRuntimeManager::RMStateMachine::CAR2FRONT_CAR2LEADER_ENGAGED) {
            myManager->switchController = BaseRuntimeManager::SwitchController::ACC_TO_CACC;
        }

        // call the accController for perform upgradation
        std::cout << "UPGRADATION => ";
        (myManager->reactor)->accStateController();
        break;
    case Plexe::PLOEG:
        if(myManager->rmState == BaseRuntimeManager::RMStateMachine::CAR2FRONT_ENGAGED) {
            // There is a possibility that during monitoring, front was assumed disconnected
            // That means reconnection has just happened
            if(myManager->degState == BaseRuntimeManager::DegradationState::DEGRADATION_INITIATED) {
                // Degradation was initiated, but not triggered yet
                // Abort the degradation
                myManager->abortDegradation = true;
                myManager->degState = BaseRuntimeManager::DegradationState::DEGRADATION_NOT_INITIATED;
            } else if (myManager->degState == BaseRuntimeManager::DegradationState::DEGRADATION_COMPLETED) {
                // Degradation performed just after connection establishment
                // Regain the controller
                myManager->switchController = BaseRuntimeManager::SwitchController::BACK_TO_PLOEG;
                myManager->degState = BaseRuntimeManager::DegradationState::DEGRADATION_NOT_INITIATED;
            }
        } else if (myManager->rmState == BaseRuntimeManager::RMStateMachine::CAR2FRONT_CAR2LEADER_ENGAGED) {
            myManager->switchController = BaseRuntimeManager::SwitchController::PLOEG_TO_CACC;
        }
        std::cout << "UPGRADATION / RETAIN => ";
        (myManager->reactor)->ploegStateController();
        break;
    case Plexe::CACC:
        if(myManager->rmState == BaseRuntimeManager::RMStateMachine::CAR2FRONT_CAR2LEADER_ENGAGED) {
           // There is a possibility that during monitoring, front was assumed disconnected
           // That means reconnection has just happened
           if(myManager->degState == BaseRuntimeManager::DegradationState::DEGRADATION_INITIATED) {
               // Degradation was initiated, but not triggered yet
               // Abort the degradation
               myManager->abortDegradation = true;
               myManager->degState = BaseRuntimeManager::DegradationState::DEGRADATION_NOT_INITIATED;
           } else if (myManager->degState == BaseRuntimeManager::DegradationState::DEGRADATION_COMPLETED) {
               // Degradation performed just after connection establishment
               // Regain the controller
               myManager->switchController = BaseRuntimeManager::SwitchController::BACK_TO_CACC;
               myManager->degState = BaseRuntimeManager::DegradationState::DEGRADATION_NOT_INITIATED;
           }
       }

        std::cout << "UPGRADATION / RETAIN => ";
       (myManager->reactor)->caccStateController();
       break;
    default:
        std::cerr << "Error : Unrecognizable Active Controller +/ not considered yet in : "
                  << "\n\tFile: "
                  << __FILE__
                  << "\n\tFunction: "
                  << __func__
                  << "\n\tLine: "
                  << __LINE__
                  << std::endl;
        break;
    }
}

void BaseRuntimeManager::Monitor::degrade() {
    switch((myManager->traciVehicle)->getActiveController()) {
    case Plexe::ACC:
        checkLog();
        //std::cout << "Right now there are no degradation from ACC mode !!!" << std::endl;
        break;
    case Plexe::PLOEG:
        checkLog();
        if(myManager->rmState == BaseRuntimeManager::RMStateMachine::CAR2FRONT_CAR2LEADER_DISENGAGED ||
                myManager->rmState == BaseRuntimeManager::RMStateMachine::CAR2LEADER_ENGAGED ) {

            myManager->switchController = BaseRuntimeManager::SwitchController::PLOEG_TO_ACC;
            // trigger self message or degrade straight away
            if((myManager->app)->getTimeToTransition() > 0.0) {
                // initiate degradation
                // adjust the parameters and controller's configuration
                myManager->reactor->adjust();

                // We have to wait timeToTrasition to perform transition
                // Recall app to trigger a self message
                myManager->degState = BaseRuntimeManager::DegradationState::DEGRADATION_INITIATED;
                (myManager->app)->triggerTransitionSelfMsg();
            } else {
                // perform degradation straight away
                std::cout << "DEGRADATION => ";
                myManager->reactor->ploegStateController();
            }
        }

        break;
    case Plexe::CACC:
        checkLog();
        if (myManager->rmState == BaseRuntimeManager::RMStateMachine::CAR2FRONT_ENGAGED) {
            myManager->switchController = BaseRuntimeManager::SwitchController::CACC_TO_PLOEG;
        }else if(myManager->rmState == BaseRuntimeManager::RMStateMachine::CAR2LEADER_ENGAGED ||
                 myManager->rmState == BaseRuntimeManager::RMStateMachine::CAR2FRONT_CAR2LEADER_DISENGAGED) {
            myManager->switchController = BaseRuntimeManager::SwitchController::CACC_TO_ACC;
        }

        // trigger self message or degrade straight away
        if((myManager->app)->getTimeToTransition() > 0.0) {
            // initiate degradation
            // adjust the parameters and controller's configuration
            myManager->reactor->adjust();
            // We have to wait timeToTrasition to perform transition
            // Recall app to trigger a self message
            myManager->degState = BaseRuntimeManager::DegradationState::DEGRADATION_INITIATED;
            (myManager->app)->triggerTransitionSelfMsg();
        } else {
            // perform degradation straight away
            std::cout << "DEGRADATION => ";
            myManager->reactor->caccStateController();
        }

        break;
    default:
        std::cerr << "Error : Unrecognizable Active Controller +/ not considered yet in : "
                  << "\n\tFile: "
                  << __FILE__
                  << "\n\tFunction: "
                  << __func__
                  << "\n\tLine: "
                  << __LINE__
                  << std::endl;
        break;
    }
}


//void BaseRuntimeManager::StateManager::accStateManager() {
//
//    checkLog();
//
//    if(myManager->rtState == BaseRuntimeManager::RTStateMachine::CAR2FRONT_ENGAGED) {
//        myManager->switchController = BaseRuntimeManager::SwitchController::ACC_TO_PLOEG;
//    } else if (myManager->rtState == BaseRuntimeManager::RTStateMachine::CAR2FRONT_CAR2LEADER_ENGAGED) {
//        myManager->switchController = BaseRuntimeManager::SwitchController::ACC_TO_CACC;
//    }
//    // Call the stateController
//    myManager->stateController->accStateController();
//
//}
//
//
//void BaseRuntimeManager::StateManager::ploegStateManager(){
//
//    checkLog();
//    if (myManager->rtState == BaseRuntimeManager::RTStateMachine::CAR2FRONT_CAR2LEADER_ENGAGED) {
//        myManager->switchController = BaseRuntimeManager::SwitchController::PLOEG_TO_CACC;
//    }else if(myManager->rtState == BaseRuntimeManager::RTStateMachine::CAR2LEADER_ENGAGED ||
//             myManager->rtState == BaseRuntimeManager::RTStateMachine::CAR2FRONT_CAR2LEADER_DISENGAGED) {
//        myManager->switchController = BaseRuntimeManager::SwitchController::PLOEG_TO_ACC;
//    }
//    // Call the stateController
//    myManager->stateController->ploegStateController();
//}
//
//void BaseRuntimeManager::StateManager::caccStateManager() {
//
//    checkLog();
//    if (myManager->rtState == BaseRuntimeManager::RTStateMachine::CAR2FRONT_ENGAGED) {
//        myManager->switchController = BaseRuntimeManager::SwitchController::CACC_TO_PLOEG;
//    }else if(myManager->rtState == BaseRuntimeManager::RTStateMachine::CAR2LEADER_ENGAGED ||
//             myManager->rtState == BaseRuntimeManager::RTStateMachine::CAR2FRONT_CAR2LEADER_DISENGAGED) {
//        myManager->switchController = BaseRuntimeManager::SwitchController::CACC_TO_ACC;
//    }
//    // Call the stateController
//    myManager->stateController->caccStateController();
//
//}

//=================================================================================================================================//
// StateController's methods
//=================================================================================================================================//
BaseRuntimeManager::Reactor::Reactor(BaseRuntimeManager* myManager) : myManager(myManager) {

}

void BaseRuntimeManager::Reactor::adjust() const {
    // Checking
    //(myManager->traciVehicle)->setFixedAcceleration(1, -10.0);

//    if(myManager->switchController == BaseRuntimeManager::SwitchController::ACC_TO_PLOEG) {
//        (myManager->traciVehicle)->setACCHeadwayTime(.6);
//    } else if(myManager->switchController == BaseRuntimeManager::SwitchController::ACC_TO_CACC) {
//        (myManager->traciVehicle)->setCACCConstantSpacing(6.0);
//    } else if(myManager->switchController == BaseRuntimeManager::SwitchController::PLOEG_TO_ACC) {
//        (myManager->traciVehicle)->setACCHeadwayTime(1.5);
//    } else if(myManager->switchController == BaseRuntimeManager::SwitchController::PLOEG_TO_CACC) {
//        (myManager->traciVehicle)->setCACCConstantSpacing(6.0);
//    } else if(myManager->switchController == BaseRuntimeManager::SwitchController::CACC_TO_ACC) {
//        (myManager->traciVehicle)->setACCHeadwayTime(1.5);
//    } else if(myManager->switchController == BaseRuntimeManager::SwitchController::CACC_TO_PLOEG) {
//        (myManager->traciVehicle)->setACCHeadwayTime(.6);
//    }
}

/**
 * Perform state transition within ACC
 */
void BaseRuntimeManager::Reactor::accStateController() {

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // There is the possibility of the followings
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // 1.
    //  front -->c2f ---------------------->to_ploeg----->transition_performed_to_ploeg--> reset switchController
    //      leader-->c2f_c2l---------------------->to_cacc-----------------------------------> No transition ()

    // 2.
    // front-->c2f------------->(see the changes by the following beacon)--> to_cacc----->transition_performed--> reset switchController
    //      leader--> c2f_c2l--> to_cacc--------------------------------------------------------------------------->no_transition (fix it in monitor())

    // 3.
    //  front -->c2f ---------------------->to_ploeg----->transition_performed_to_ploeg--> reset switchController
    //      leader-->c2f_c2l---------------------->to_cacc-----------------------> performed transition ---< reset switchController (again)


    if(myManager->switchController == BaseRuntimeManager::SwitchController::ACC_TO_CACC) {
        // TODO TAKE APPROPRIATE ACTION FOR STABLE TRANSITION. For example: providing deceleration
        // Set constant spacing
        //traciVehicle->setCACCConstantSpacing(5.0);
        //double ctd = (myManager->traciVehicle)->getCACCConstantSpacing();
        //std::cout << "CTD: " << ctd << std::endl;

        if((myManager->traciVehicle)->getActiveController() == Plexe::CACC) return;

        (myManager->traciVehicle)->setActiveController(Plexe::CACC);

#ifdef DEBUG_RUNTIMEMANAGER
        std::cout << "VehicleId: " << myManager->positionHelper->getId() << "\n\tRuntimeManager performed transition from ACC to CACC!!!" << std::endl;
#endif

    } else if(myManager->switchController == BaseRuntimeManager::SwitchController::ACC_TO_PLOEG) {
        // TODO TAKE APPROPRIATE ACTION FOR STABLE TRANSITION. For example: providing deceleration
        // reset headway time gap for Ploeg



        (myManager->traciVehicle)->setActiveController(Plexe::PLOEG);

#ifdef DEBUG_RUNTIMEMANAGER
        std::cout << "VehicleId: " << myManager->positionHelper->getId() << "\n\tRuntimeManager performed transition from ACC to PLOEG!!!" << std::endl;
#endif
    }

    // reset the switchController
    myManager->switchController = BaseRuntimeManager::SwitchController::NOT_INITIALIZED;
}

/**
 * Perform state transition within PLOEG
 */

void BaseRuntimeManager::Reactor::ploegStateController(){

    if (myManager->switchController == BaseRuntimeManager::SwitchController::BACK_TO_PLOEG) {
        (myManager->traciVehicle)->setActiveController(Plexe::PLOEG);
#ifdef DEBUG_RUNTIMEMANAGER
        std::cout << "VehicleId: " << myManager->positionHelper->getId() << "\n\tRuntimeManager regain the controller PLOEG from degradation!!!" << std::endl;
#endif
    } else if(myManager->switchController == BaseRuntimeManager::SwitchController::PLOEG_TO_CACC) {
        // TODO TAKE APPROPRIATE ACTION FOR STABLE TRANSITION. For example: providing deceleration
        // Set constant spacing

        (myManager->traciVehicle)->setActiveController(Plexe::CACC);

#ifdef DEBUG_RUNTIMEMANAGER
        std::cout << "VehicleId: " << myManager->positionHelper->getId() << "\n\tRuntimeManager performed transition from PLOEG to CACC!!!" << std::endl;
#endif

    } else if(myManager->switchController == BaseRuntimeManager::SwitchController::PLOEG_TO_ACC) {
        // reset headway time gap for acc


        (myManager->traciVehicle)->setActiveController(Plexe::ACC);

#ifdef DEBUG_RUNTIMEMANAGER
        std::cout << "VehicleId: " << myManager->positionHelper->getId() << "\n\tRuntimeManager performed transition from PLOEG to ACC!!!" << std::endl;
#endif
    }

    // reset the switchController
    myManager->switchController = BaseRuntimeManager::SwitchController::NOT_INITIALIZED;
}

/**
 * Perform state transition within CACC
 */
void BaseRuntimeManager::Reactor::caccStateController() {

    if(myManager->switchController == BaseRuntimeManager::SwitchController::BACK_TO_CACC) {
            (myManager->traciVehicle)->setActiveController(Plexe::CACC);

    #ifdef DEBUG_RUNTIMEMANAGER
            std::cout << "VehicleId: " << myManager->positionHelper->getId() << "\n\tRuntimeManager regain the controller CACC from degradation!!!" << std::endl;
    #endif

    } else if(myManager->switchController == BaseRuntimeManager::SwitchController::CACC_TO_ACC) {
        (myManager->traciVehicle)->setActiveController(Plexe::ACC);

#ifdef DEBUG_RUNTIMEMANAGER
        std::cout << "VehicleId: " << myManager->positionHelper->getId() << "\n\tRuntimeManager performed transition from CACC to ACC!!!" << std::endl;
#endif

    } else if(myManager->switchController == BaseRuntimeManager::SwitchController::CACC_TO_PLOEG) {
        // TODO TAKE APPROPRIATE ACTION FOR STABLE TRANSITION. For example: providing deceleration
        // reset headway time gap for Ploeg


        (myManager->traciVehicle)->setActiveController(Plexe::PLOEG);

#ifdef DEBUG_RUNTIMEMANAGER
        std::cout << "VehicleId: " << myManager->positionHelper->getId() << "\n\tRuntimeManager performed transition from CACC to PLOEG!!!" << std::endl;
#endif
    }

    // reset the switchController
    myManager->switchController = BaseRuntimeManager::SwitchController::NOT_INITIALIZED;
}



//=================================================================================================================================//
// virtual methods
//=================================================================================================================================//
void BaseRuntimeManager::observe() {
    std::cerr << "Error: " << __FILE__
              << "\n\tLine: " << __LINE__
              << "\n\tCompiled on: " << __DATE__
              << " at " << __TIME__
              << "\n\tfunction " << __func__
              << " must have to override by the derived class!!!"
              << std::endl;
}

void BaseRuntimeManager::record(const int sourceVehicleId, simtime_t currentSimTime) {
    std::cerr << "Error: " << __FILE__
              << "\n\tLine: " << __LINE__
              << "\n\tCompiled on: " << __DATE__
              << " at " << __TIME__
              << "\n\tfunction " << __func__
              << " must have to override by the derived class!!!"
              << std::endl;
}


void BaseRuntimeManager::triggerDegradation() {
    std::cerr << "Error: " << __FILE__
              << "\n\tLine: " << __LINE__
              << "\n\tCompiled on: " << __DATE__
              << " at " << __TIME__
              << "\n\tfunction " << __func__
              << " must have to override by the derived class!!!"
              << std::endl;
}

void BaseRuntimeManager::updateStateMachine(const int sourceVehicleId, const simtime_t currentSimTime){
    std::cerr << "Error: " << __FILE__
              << "\n\tLine: " << __LINE__
              << "\n\tCompiled on: " << __DATE__
              << " at " << __TIME__
              << "\n\tfunction " << __func__
              << " must have to override by the derived class!!!"
              << std::endl;
}

void BaseRuntimeManager::logSafetyRecords(const int key, simtime_t currentSimTime){
    std::cerr << "Error: " << __FILE__
              << "\n\tLine: " << __LINE__
              << "\n\tCompiled on: " << __DATE__
              << " at " << __TIME__
              << "\n\tfunction " << __func__
              << " must have to override by the derived class!!!"
              << std::endl;
}

