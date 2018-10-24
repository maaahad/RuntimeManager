/*
 * RuntimeManager.cc
 *
 *  Created on: Oct 18, 2018
 *      Author: ahad
 */

#include <iostream>
#include "veins/modules/application/platooning/runtimeManagers/RuntimeManager.h"
#include "veins/modules/application/platooning/apps/SimplePlatooningApp.h"

//======================================================================================================================//
//                                           RuntimeManager's methods implementation
//======================================================================================================================//

RuntimeManager::RuntimeManager(SimplePlatooningApp* app) : currentState(StateMachine::NOT_INITIALIZED) ,
switchController(SwitchController::NOT_INITIALIZED) {
    //std::cout << "Runtime Manager has been created..." << std::endl;

    stateManager    = new StateManager(this);
    stateController = new StateController(this);

    this->app            = app;
    this->positionHelper = app->getPositionHelper();
    this->mobility       = app->getMobility();
    this->traci          = app->getTraci();
    this->traciVehicle   = app->getTraciVehicle();
}

RuntimeManager::~RuntimeManager() {
    // TODO update this message using preprocessors variable for function name, file name, line etc.
    delete stateManager;
    delete stateController;
}


void RuntimeManager::monitor() {
    //std::cout << "Runtime manager is called to monitor the state!!!" << std::endl;

    switch(traciVehicle->getActiveController()) {
        case Plexe::ACC:
            stateManager->accStateManager();
            break;
        case Plexe::CACC:
            stateManager->caccStateManager();
            break;
        default:
            std::cerr << "RuntimeManager could not recognize the active controller" << std::endl;
            break;
    }
}

void RuntimeManager::record(const int sourceVehicleId, simtime_t currentSimTime) {
    if (sourceVehicleId == positionHelper->getLeaderId()) {
        // this is the leader
        updateStateMachine(sourceVehicleId);
        // now keep record of the beacon in the beaconRecordData
        updateBeaconRecord("leader", currentSimTime);
    } else if (sourceVehicleId == positionHelper->getFrontId()) {
        // this is the front vehicle
        updateStateMachine(sourceVehicleId);
        // now keep record of the beacon in the beaconRecordData
        updateBeaconRecord("front", currentSimTime);
    } else {
        // this is other vehicle
        // these records are used to determine whether a vehicle encounters a communication failure
        std::string key = std::string("vehicle_") + std::to_string(sourceVehicleId);
        updateBeaconRecord(key, currentSimTime);
    }

    // after updating the StateMachine and the stored information, call the monitor() to monitor the state stability
    monitor();

}

void RuntimeManager::updateStateMachine(const int sourceVehicleId) {
    switch(traciVehicle->getActiveController()) {
    case Plexe::ACC:
        if(sourceVehicleId == positionHelper->getLeaderId()) {
            if(currentState == RuntimeManager::StateMachine::NOT_INITIALIZED || currentState == RuntimeManager::StateMachine::CAR2X_DISENGAGED
                    || currentState == RuntimeManager::StateMachine::CAR2LEADER_DISENGAGED) {
                currentState = RuntimeManager::StateMachine::CAR2LEADER_ENGAGED;
            } else if (currentState == RuntimeManager::StateMachine::CAR2X_ENGAGED) {
                currentState = RuntimeManager::StateMachine::CAR2X_AND_CAR2LEADER_ENGAGED;
            }
        } else if (sourceVehicleId == positionHelper->getFrontId()) {
            if (currentState == RuntimeManager::StateMachine::NOT_INITIALIZED  || currentState == RuntimeManager::StateMachine::CAR2X_DISENGAGED
                    || currentState == RuntimeManager::StateMachine::CAR2LEADER_DISENGAGED) {
                currentState = RuntimeManager::StateMachine::CAR2X_ENGAGED;
            } else if (currentState == RuntimeManager::StateMachine::CAR2LEADER_ENGAGED) {
                currentState = RuntimeManager::StateMachine::CAR2X_AND_CAR2LEADER_ENGAGED;

            }
        } else {
            // TODO : use preprocessor variables related to file and function to guide the user where the error is
            std::cerr << "updateStateMachine() is called with wring sourceVehicleId. This code block have a TODO option..." << std::endl;
        }
        // TODO :: Another possibility: PLATOON_ESTABLISHED => if the vehicle know that the vehicle in front is in platoon mode
        // TODO :: LATER
        break;
    case Plexe::CACC:
        // If we are here, StateMachine::CAR2X_ENGAGED, is guaranteed, otherwise something is wrong (need double check)
        if(sourceVehicleId == positionHelper->getLeaderId()) {
            if(currentState == RuntimeManager::StateMachine::NOT_INITIALIZED) {
                currentState = RuntimeManager::StateMachine::CAR2LEADER_ENGAGED;
            } else if (currentState == RuntimeManager::StateMachine::CAR2X_ENGAGED) {
                currentState = RuntimeManager::StateMachine::CAR2X_AND_CAR2LEADER_ENGAGED;
            }
        } else if (sourceVehicleId == positionHelper->getFrontId()) {
            if(currentState == RuntimeManager::StateMachine::NOT_INITIALIZED) {
                currentState = RuntimeManager::StateMachine::CAR2X_ENGAGED;
            }
        } else {
            // TODO : use preprocessor variables related to file and function to guide the user where the error is
            std::cerr << "updateStateMachine() is called with wring sourceVehicleId. This code block have a TODO option..." << std::endl;
        }
        // TODO :: Another possibility: PLATOON_ESTABLISHED => if the vehicle know that the vehicle in front is in platoon mode
        // TODO :: LATER

        break;
    default:
        break;
    }
}




void RuntimeManager::updateBeaconRecord(const std::string &key, simtime_t currentSimTime) {
    if (vehicleBeaconData.find(key) == vehicleBeaconData.end()) {
        // this is the first time for this key
        RuntimeManager::BeaconData bData;
        bData.previousBeaconArrivalTime = currentSimTime;  // this is for checking
        //std::cout << "Previous beacon arrival time from  " << key<<" : " << bData.previousBeaconArrivalTime << std::endl;
        vehicleBeaconData.insert({key, bData});
    } else {
        auto iter = vehicleBeaconData.find(key);
        iter->second.timeIntervalBetweenBeacon = currentSimTime - iter->second.previousBeaconArrivalTime;
        iter->second.previousBeaconArrivalTime = currentSimTime;
//#ifdef DEBUG_RUNTIMEMANAGER
//        std::cout << key << "\n" << "\tpreviousBeaconArrivalTime: " << iter->second.previousBeaconArrivalTime.dbl()
//                         << "\n\ttimeIntervalBetweenBeacon: "<< iter->second.timeIntervalBetweenBeacon.dbl()
//                         << std::endl;
//#endif

    }

//#ifdef DEBUG_RUNTIMEMANAGER
//        std::cout << "Total No. of vehicle in recordData: " << vehicleBeaconData.size() << std::endl;
//#endif


}

//======================================================================================================================//
//                                  RuntimeManager::StateManager's methods implementation
//======================================================================================================================//
RuntimeManager::StateManager::StateManager(RuntimeManager* runtimeManager) {
    this->myManager = runtimeManager;
}

RuntimeManager::StateManager::~StateManager(){

}

void RuntimeManager::StateManager::accStateManager() {
    if (myManager->currentState == RuntimeManager::StateMachine::CAR2X_ENGAGED || myManager->currentState == RuntimeManager::StateMachine::CAR2X_AND_CAR2LEADER_ENGAGED) {
        // TODO need to check all safety requirements stored in beaconRecordData

        // Then store the possible controller to switch to for the state controller
        myManager->switchController = RuntimeManager::SwitchController::ACC_TO_CACC;
        myManager->stateController->accStateController();
    } else if (myManager->currentState == RuntimeManager::StateMachine::PLATOON_ESTABLISHED_AND_CAR2X_ENGAGED ||
            myManager->currentState == RuntimeManager::StateMachine::PLATOON_ESTABLISHED) {
        // TODO need to check all the safe requirements

        myManager->switchController == RuntimeManager::SwitchController::ACC_TO_PLATOON;
        myManager->stateController->accStateController();
    }

}

void RuntimeManager::StateManager::caccStateManager() {
    // TODO  here the status/stability of the current controller will be analyzed,
    // and transition will be proposed to StateController if required
    // monitor the recorded data
    if (myManager->currentState == RuntimeManager::StateMachine::PLATOON_ESTABLISHED_AND_CAR2X_ENGAGED) {
        // upgrade to Platoon
        std::cout << "caccStateManager() has not implemented the upgrade from CACC yet..." << std::endl;

    } else if (myManager->currentState == RuntimeManager::StateMachine::CAR2X_ENGAGED || myManager->currentState == RuntimeManager::StateMachine::CAR2X_AND_CAR2LEADER_ENGAGED) {
        // No upgrade , but check the stability of the current state
        auto iter = myManager->vehicleBeaconData.find("front");

        // second operand of operator || is used by considering that the monitor() is called from the handleSelfMessage() of app !!!
        // TODO :: (NOT 100% CONVINCED :: NEED TO DOUBLE CHECK)
        // TODO :: exceptedBeaconInterval is hard coded right now. Need to define in the configuration file .ned and .ini
        if(iter->second.timeIntervalBetweenBeacon > 0.11 || (simTime() - iter->second.previousBeaconArrivalTime) > 0.11) {
            // update the state machine
            myManager->currentState = (myManager->currentState == RuntimeManager::StateMachine::CAR2X_ENGAGED) ?
                    RuntimeManager::StateMachine::CAR2X_DISENGAGED :
                    RuntimeManager::StateMachine::CAR2LEADER_ENGAGED;

            // we assume the connection to front vehicles lost or not good enough for CACC. downgrade from CACC to ACC
            myManager->switchController = RuntimeManager::SwitchController::CACC_TO_ACC;
            myManager->stateController->caccStateController();
        }
    }
}

void RuntimeManager::StateManager::platoonStateManager() {
    // here the status/stability of the current controller will be analyzed,
    // and transition will be proposed to TODO StateController if required

}


//======================================================================================================================//
//                                  RuntimeManager::StateController's methods implementation
//======================================================================================================================//
RuntimeManager::StateController::StateController(RuntimeManager* runtimeManager) {
    this->myManager = runtimeManager;
}

void RuntimeManager::StateController::accStateController() {
    if(myManager->switchController == RuntimeManager::SwitchController::ACC_TO_CACC) {
        // this means connection to front vehicle established. Controller can switch to CACC

        // TODO :: First take appropriate action for stable transition. For example acceleration to reduce time gap to the front vehicle

        // Perform state transition
        myManager->traciVehicle->setActiveController(Plexe::CACC);
        //myManager->currentState = RuntimeManager::StateMachine::CACC_ACTIVATED;

#ifdef DEBUG_RUNTIMEMANAGER
        std::cout << "VehicleId: " << myManager->positionHelper->getId() << "\n\tRuntimeManager performed transition from ACC to CACC!!!" << std::endl;
#endif

    } else if (myManager->switchController == RuntimeManager::SwitchController::ACC_TO_PLATOON) {
        // TODO Implement the transition
        std::cerr << "accStateController() has not implemented the ACC_PLATOON yet!!!" << std::endl;
    }
}

void RuntimeManager::StateController::caccStateController() {
    if(myManager->switchController == RuntimeManager::SwitchController::CACC_TO_ACC) {
        // TODO :: First take appropriate action for stable transition. For example deceleration to increase time gap to the front vehicle

        myManager->traciVehicle->setActiveController(Plexe::ACC);

#ifdef DEBUG_RUNTIMEMANAGER
        std::cout << "VehicleId: " << myManager->positionHelper->getId() << "\n\tRuntimeManager performed transition from CACC to ACC!!!" << std::endl;
#endif
    } else if (myManager->switchController == RuntimeManager::SwitchController::CACC_TO_PLATOON) {
        // TODO :: First take appropriate action for stable transition. For example deceleration to increase time gap to the front vehicle
        std::cout << "caccStateController() has not implemented the transition from CACC to Platoon yet..." << std::endl;
    }

}

void RuntimeManager::StateController::platoonController() {

}









