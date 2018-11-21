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
BaseRuntimeManager::BaseRuntimeManager(BaseApp *app) : rtState(BaseRuntimeManager::RTStateMachine::CAR2FRONT_CAR2LEADER_DISENGAGED),
switchController(BaseRuntimeManager::SwitchController::NOT_INITIALIZED){
    // TODO Auto-generated constructor stub
    stateManager    = new StateManager(this);
    stateController = new StateController(this);

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
    delete stateManager;
    delete stateController;
}

//=================================================================================================================================//
// StateManager's methods
//=================================================================================================================================//
BaseRuntimeManager::StateManager::StateManager(BaseRuntimeManager* myManager) : myManager(myManager) {

}

bool BaseRuntimeManager::StateManager::connectionOK(int key) {
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

    if (beaconMissed > (myManager->app)->getNAcceptedBeaconMiss() ||
            it->second.avgBeaconInterval > (myManager->app)->getAcceptedAvgBeaconInterval()) {
        // Assume Connection lost to the vehicle with id key
        // Discard the record
        EV << "Connection to vehicle_" << key << "is lost. Discarding the safety records." << std::endl;
        myManager->safetyRecords.erase(key);
        return false;
    }
    return true;
}

void BaseRuntimeManager::StateManager::transitionCheck(){
    if(myManager->rtState == BaseRuntimeManager::RTStateMachine::CAR2FRONT_ENGAGED) {
        // Sanity check
        ASSERT((myManager->positionHelper)->getLeaderId() != (myManager->positionHelper)->getFrontId());

        if(!connectionOK((myManager->positionHelper)->getFrontId())) {
            myManager->rtState = BaseRuntimeManager::RTStateMachine::CAR2FRONT_CAR2LEADER_DISENGAGED;
        }
    } else if (myManager->rtState == BaseRuntimeManager::RTStateMachine::CAR2LEADER_ENGAGED) {
        // Sanity check
        ASSERT((myManager->positionHelper)->getLeaderId() != (myManager->positionHelper)->getFrontId());

        if (!connectionOK((myManager->positionHelper)->getLeaderId())) {
            myManager->rtState = BaseRuntimeManager::RTStateMachine::CAR2FRONT_CAR2LEADER_DISENGAGED;
        }
    } else if (myManager->rtState == BaseRuntimeManager::RTStateMachine::CAR2FRONT_CAR2LEADER_ENGAGED){
        if (!connectionOK((myManager->positionHelper)->getLeaderId())) {
            myManager->rtState = ((myManager->positionHelper)->getLeaderId() == (myManager->positionHelper)->getFrontId()) ?
                    BaseRuntimeManager::RTStateMachine::CAR2FRONT_CAR2LEADER_DISENGAGED :
                    BaseRuntimeManager::RTStateMachine::CAR2FRONT_ENGAGED;
        }

        if((myManager->positionHelper)->getLeaderId() != (myManager->positionHelper)->getFrontId()) {
            if(!connectionOK((myManager->positionHelper)->getFrontId())) {
                myManager->rtState = (myManager->rtState == BaseRuntimeManager::RTStateMachine::CAR2FRONT_ENGAGED) ?
                        BaseRuntimeManager::RTStateMachine::CAR2FRONT_CAR2LEADER_DISENGAGED :
                        BaseRuntimeManager::RTStateMachine::CAR2LEADER_ENGAGED;
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


void BaseRuntimeManager::StateManager::accStateManager() {

    transitionCheck();

    if(myManager->rtState == BaseRuntimeManager::RTStateMachine::CAR2FRONT_ENGAGED) {
        myManager->switchController = BaseRuntimeManager::SwitchController::ACC_TO_PLOEG;
    } else if (myManager->rtState == BaseRuntimeManager::RTStateMachine::CAR2FRONT_CAR2LEADER_ENGAGED) {
        myManager->switchController = BaseRuntimeManager::SwitchController::ACC_TO_CACC;
    }

//
//
//
//    if(myManager->rtState == BaseRuntimeManager::RTStateMachine::CAR2FRONT_ENGAGED) {
//        // Sanity check
//        ASSERT((myManager->positionHelper)->getLeaderId() != (myManager->positionHelper)->getFrontId());
//
//        if(connectionOK((myManager->positionHelper)->getFrontId())) {
//            myManager->switchController = BaseRuntimeManager::SwitchController::ACC_TO_PLOEG;
//        } else {
//            myManager->rtState = BaseRuntimeManager::RTStateMachine::CAR2FRONT_CAR2LEADER_DISENGAGED;
//        }
//    } else if (myManager->rtState == BaseRuntimeManager::RTStateMachine::CAR2LEADER_ENGAGED) {
//        // Sanity check
//        ASSERT((myManager->positionHelper)->getLeaderId() != (myManager->positionHelper)->getFrontId());
//
//        if (!connectionOK((myManager->positionHelper)->getLeaderId())) {
//            myManager->rtState = BaseRuntimeManager::RTStateMachine::CAR2FRONT_CAR2LEADER_DISENGAGED;
//        }
//    } else if (myManager->rtState == BaseRuntimeManager::RTStateMachine::CAR2FRONT_CAR2LEADER_ENGAGED){
//        if (!connectionOK((myManager->positionHelper)->getLeaderId())) {
//            myManager->rtState = ((myManager->positionHelper)->getLeaderId() == (myManager->positionHelper)->getFrontId()) ?
//                    BaseRuntimeManager::RTStateMachine::CAR2FRONT_CAR2LEADER_DISENGAGED :
//                    BaseRuntimeManager::RTStateMachine::CAR2FRONT_ENGAGED;
//        }
//
//        if((myManager->positionHelper)->getLeaderId() != (myManager->positionHelper)->getFrontId()) {
//            if(!connectionOK((myManager->positionHelper)->getFrontId())) {
//                myManager->rtState = (myManager->rtState == BaseRuntimeManager::RTStateMachine::CAR2FRONT_ENGAGED) ?
//                        BaseRuntimeManager::RTStateMachine::CAR2FRONT_CAR2LEADER_DISENGAGED :
//                        BaseRuntimeManager::RTStateMachine::CAR2LEADER_ENGAGED;
//            }
//        }
//
//        if (myManager->rtState == BaseRuntimeManager::RTStateMachine::CAR2FRONT_CAR2LEADER_ENGAGED) {
//            myManager->switchController = BaseRuntimeManager::SwitchController::ACC_TO_CACC;
//        } else if(myManager->rtState == BaseRuntimeManager::RTStateMachine::CAR2FRONT_ENGAGED) {
//            myManager->switchController = BaseRuntimeManager::SwitchController::ACC_TO_PLOEG;
//        }
//    } else {
//        std::cerr << "Error : wrong rtState"
//                             << "\n\tFile: "
//                             << __FILE__
//                             << "\n\tFunction: "
//                             << __func__
//                             << "\n\tLine: "
//                             << __LINE__
//                             << std::endl;
//    }
    // Call the stateController
    myManager->stateController->accStateController();

}


void BaseRuntimeManager::StateManager::ploegStateManager(){

    transitionCheck();
    if (myManager->rtState == BaseRuntimeManager::RTStateMachine::CAR2FRONT_CAR2LEADER_ENGAGED) {
        myManager->switchController = BaseRuntimeManager::SwitchController::PLOEG_TO_CACC;
    }else if(myManager->rtState == BaseRuntimeManager::RTStateMachine::CAR2LEADER_ENGAGED ||
             myManager->rtState == BaseRuntimeManager::RTStateMachine::CAR2FRONT_CAR2LEADER_DISENGAGED) {
        myManager->switchController = BaseRuntimeManager::SwitchController::PLOEG_TO_ACC;
    }

//    if(myManager->rtState == BaseRuntimeManager::RTStateMachine::CAR2FRONT_ENGAGED) {
//
//        // Sanity check
//        ASSERT((myManager->positionHelper)->getLeaderId() != (myManager->positionHelper)->getFrontId());
//
//        if(!connectionOK((myManager->positionHelper)->getFrontId())) {
//            myManager->rtState = BaseRuntimeManager::RTStateMachine::CAR2FRONT_CAR2LEADER_DISENGAGED;
//            myManager->switchController = BaseRuntimeManager::SwitchController::PLOEG_TO_ACC;
//        }
//
//    } else if (myManager->rtState == BaseRuntimeManager::RTStateMachine::CAR2FRONT_CAR2LEADER_ENGAGED) {
//
//
//        if (!connectionOK((myManager->positionHelper)->getLeaderId())) {
//            myManager->rtState = (myManager->positionHelper)->getLeaderId() == (myManager->positionHelper)->getFrontId() ?
//                    BaseRuntimeManager::RTStateMachine::CAR2FRONT_CAR2LEADER_DISENGAGED :
//                    BaseRuntimeManager::RTStateMachine::CAR2FRONT_ENGAGED;
//        }
//
//        if((myManager->positionHelper)->getLeaderId() != (myManager->positionHelper)->getFrontId()) {
//            if(!connectionOK((myManager->positionHelper)->getFrontId())) {
//                myManager->rtState = (myManager->rtState == BaseRuntimeManager::RTStateMachine::CAR2FRONT_ENGAGED) ?
//                        BaseRuntimeManager::RTStateMachine::CAR2FRONT_CAR2LEADER_DISENGAGED :
//                        BaseRuntimeManager::RTStateMachine::CAR2LEADER_ENGAGED;
//            }
//        }
//
//        if (myManager->rtState == BaseRuntimeManager::RTStateMachine::CAR2FRONT_CAR2LEADER_ENGAGED) {
//            myManager->switchController = BaseRuntimeManager::SwitchController::PLOEG_TO_CACC;
//        }else if(myManager->rtState == BaseRuntimeManager::RTStateMachine::CAR2LEADER_ENGAGED ||
//                 myManager->rtState == BaseRuntimeManager::RTStateMachine::CAR2FRONT_CAR2LEADER_DISENGAGED) {
//            myManager->switchController = BaseRuntimeManager::SwitchController::PLOEG_TO_ACC;
//        }
//
//    } else if (myManager->rtState == BaseRuntimeManager::RTStateMachine::CAR2LEADER_ENGAGED) {
//        // Sanity check
//        ASSERT((myManager->positionHelper)->getLeaderId() != (myManager->positionHelper)->getFrontId());
//
//        if (!connectionOK((myManager->positionHelper)->getLeaderId())) {
//            myManager->rtState = BaseRuntimeManager::RTStateMachine::CAR2FRONT_CAR2LEADER_DISENGAGED;
//        }
//        // Does not matter whether connection is ok or not, a vehicle should use ACC if
//        // it is connected to the leader only. Better to use radar information rather than 0 acceleration
//        myManager->switchController = BaseRuntimeManager::SwitchController::PLOEG_TO_ACC;
//
//    } else {
//        std::cerr << "Error : wrong rtState"
//                             << "\n\tFile: "
//                             << __FILE__
//                             << "\n\tFunction: "
//                             << __func__
//                             << "\n\tLine: "
//                             << __LINE__
//                             << std::endl;
//    }

    // Call the stateController
    myManager->stateController->ploegStateController();
}

void BaseRuntimeManager::StateManager::caccStateManager() {

    transitionCheck();
    if (myManager->rtState == BaseRuntimeManager::RTStateMachine::CAR2FRONT_ENGAGED) {
        myManager->switchController = BaseRuntimeManager::SwitchController::CACC_TO_PLOEG;
    }else if(myManager->rtState == BaseRuntimeManager::RTStateMachine::CAR2LEADER_ENGAGED ||
             myManager->rtState == BaseRuntimeManager::RTStateMachine::CAR2FRONT_CAR2LEADER_DISENGAGED) {
        myManager->switchController = BaseRuntimeManager::SwitchController::CACC_TO_ACC;
    }


//    if(myManager->rtState == BaseRuntimeManager::RTStateMachine::CAR2FRONT_CAR2LEADER_ENGAGED) {
//        if (!connectionOK((myManager->positionHelper)->getLeaderId())) {
//            myManager->rtState = (myManager->positionHelper)->getLeaderId() == (myManager->positionHelper)->getFrontId() ?
//                    BaseRuntimeManager::RTStateMachine::CAR2FRONT_CAR2LEADER_DISENGAGED :
//                    BaseRuntimeManager::RTStateMachine::CAR2FRONT_ENGAGED;
//        }
//
//        if((myManager->positionHelper)->getLeaderId() != (myManager->positionHelper)->getFrontId()) {
//            if(!connectionOK((myManager->positionHelper)->getFrontId())) {
//                myManager->rtState = (myManager->rtState == BaseRuntimeManager::RTStateMachine::CAR2FRONT_ENGAGED) ?
//                        BaseRuntimeManager::RTStateMachine::CAR2FRONT_CAR2LEADER_DISENGAGED :
//                        BaseRuntimeManager::RTStateMachine::CAR2LEADER_ENGAGED;
//            }
//        }
//
//        if (myManager->rtState == BaseRuntimeManager::RTStateMachine::CAR2FRONT_ENGAGED) {
//            myManager->switchController = BaseRuntimeManager::SwitchController::CACC_TO_PLOEG;
//        }else if(myManager->rtState == BaseRuntimeManager::RTStateMachine::CAR2LEADER_ENGAGED ||
//                 myManager->rtState == BaseRuntimeManager::RTStateMachine::CAR2FRONT_CAR2LEADER_DISENGAGED) {
//            myManager->switchController = BaseRuntimeManager::SwitchController::CACC_TO_ACC;
//        }
//
//    } else if (myManager->rtState == BaseRuntimeManager::RTStateMachine::CAR2FRONT_ENGAGED) {
//        // Sanity check
//        ASSERT((myManager->positionHelper)->getLeaderId() != (myManager->positionHelper)->getFrontId());
//
//        if(connectionOK((myManager->positionHelper)->getFrontId())) {
//            myManager->switchController = BaseRuntimeManager::SwitchController::CACC_TO_PLOEG;
//        } else {
//            myManager->rtState = BaseRuntimeManager::RTStateMachine::CAR2FRONT_CAR2LEADER_DISENGAGED;
//            myManager->switchController = BaseRuntimeManager::SwitchController::CACC_TO_ACC;
//        }
//    } else if (myManager->rtState == BaseRuntimeManager::RTStateMachine::CAR2LEADER_ENGAGED) {
//        // Sanity check
//        ASSERT((myManager->positionHelper)->getLeaderId() != (myManager->positionHelper)->getFrontId());
//
//        if (!connectionOK((myManager->positionHelper)->getLeaderId())) {
//            myManager->rtState = BaseRuntimeManager::RTStateMachine::CAR2FRONT_CAR2LEADER_DISENGAGED;
//        }
//        // Does not matter whether connection is ok or not, a vehicle should use ACC if
//        // it is connected to the leader only. Better to use radar information rather than 0 acceleration
//        myManager->switchController = BaseRuntimeManager::SwitchController::CACC_TO_ACC;
//
//    } else {
//        std::cerr << "Error : wrong rtState"
//                             << "\n\tFile: "
//                             << __FILE__
//                             << "\n\tFunction: "
//                             << __func__
//                             << "\n\tLine: "
//                             << __LINE__
//                             << std::endl;
//    }
    // Call the stateController
    myManager->stateController->caccStateController();

}

//=================================================================================================================================//
// StateController's methods
//=================================================================================================================================//
BaseRuntimeManager::StateController::StateController(BaseRuntimeManager* myManager) : myManager(myManager) {

}

void BaseRuntimeManager::StateController::adjust() const {
//    std::cout << "Warning: " << __FILE__
//                  << "\n\tLine: " << __LINE__
//                  << "\n\tCompiled on: " << __DATE__
//                  << " at " << __TIME__
//                  << "\n\tfunction " << __func__
//                  << " not implemented yet!!!"
//                  << std::endl;
}

void BaseRuntimeManager::StateController::accStateController() {

    // adjust the parameter and controller's configuration : if required
    adjust();

    if(myManager->switchController == BaseRuntimeManager::SwitchController::ACC_TO_CACC) {
        // TODO TAKE APPROPRIATE ACTION FOR STABLE TRANSITION. For example: providing deceleration
        // Set constant spacing
        //traciVehicle->setCACCConstantSpacing(5.0);
        //double ctd = (myManager->traciVehicle)->getCACCConstantSpacing();
        //std::cout << "CTD: " << ctd << std::endl;


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


void BaseRuntimeManager::StateController::ploegStateController(){
    // adjust the parameter and controller's configuration : if required
    adjust();

    if(myManager->switchController == BaseRuntimeManager::SwitchController::PLOEG_TO_CACC) {
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


void BaseRuntimeManager::StateController::caccStateController() {
    // adjust the parameter and controller's configuration : if required
    adjust();

    if(myManager->switchController == BaseRuntimeManager::SwitchController::CACC_TO_ACC) {
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
// virtual mehtods
//=================================================================================================================================//
void BaseRuntimeManager::monitor() {
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


void BaseRuntimeManager::updateStateMachine(const int sourceVehicleId, const simtime_t currentSimTime){
    std::cerr << "Error: " << __FILE__
              << "\n\tLine: " << __LINE__
              << "\n\tCompiled on: " << __DATE__
              << " at " << __TIME__
              << "\n\tfunction " << __func__
              << " must have to override by the derived class!!!"
              << std::endl;
}

void BaseRuntimeManager::updateSafetyRecords(const int key, simtime_t currentSimTime){
    std::cerr << "Error: " << __FILE__
              << "\n\tLine: " << __LINE__
              << "\n\tCompiled on: " << __DATE__
              << " at " << __TIME__
              << "\n\tfunction " << __func__
              << " must have to override by the derived class!!!"
              << std::endl;
}

