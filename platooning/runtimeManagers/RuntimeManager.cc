/*
 * RuntimeManager.cc
 *
 *  Created on: Oct 18, 2018
 *      Author: ahad
 */

#include <iostream>
#include "veins/modules/application/platooning/runtimeManagers/RuntimeManager.h"
#include "veins/modules/application/platooning/apps/BaseApp.h"

//======================================================================================================================//
//                                           RuntimeManager's methods implementation
//======================================================================================================================//

RuntimeManager::RuntimeManager(BaseApp* app) : currentState(StateMachine::NOT_INITIALIZED) ,
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
            //stateManager->caccStateManager();
            break;
        default:
            std::cerr << "RuntimeManager could not recognize the active controller" << std::endl;
            break;
    }
}

void RuntimeManager::record(int vehicleId) {
    if (vehicleId == positionHelper->getLeaderId()) {
        // this is the leader

    } else if (vehicleId == positionHelper->getFrontId()) {
        // this is the front vehicle
        if (currentState != StateMachine::CAR2X_ENGAGED || currentState != StateMachine::CAR2X_ENGAGED_AND_PLATOON_ESTABLISHED) {
            // this is the first call to record for front vehicle :: We need to update the state machine
            updateStateMachine();
        }
        // now keep record of the beacon in the beaconRecordData

    } else {
        // this is other vehicle
        // these records are used to determine whether a vehicle encounters a communication faulire
    }
}

void RuntimeManager::updateStateMachine() {
    switch(traciVehicle->getActiveController()) {
    case Plexe::ACC:
        currentState = (currentState == StateMachine::NOT_INITIALIZED) ? StateMachine::CAR2X_ENGAGED :
                StateMachine::CAR2X_ENGAGED_AND_PLATOON_ESTABLISHED;
        break;
    case Plexe::CACC:
        // If we are here, StateMachine::CAR2X_ENGAGED, is guaranteed, otherwise something is wrong (need double check)

        break;
    default:
        break;
    }
}




void RuntimeManager::updateBeaconRecord(const std::string &key) {
    //RuntimeManager::BeaconData tmp; // TODO :: TRY WITH
    //beaconData.insert({key, tmp});

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
    // here the status/stability of the current controller will be analyzed,
    // and transition will be proposed to TODO StateController if required

    //std::cout << "accStateManager is called to monitor the state. Changing state to CACC!!!" << std::endl;

    // If the current active controller is ACC and this is the first beacon from the vehicle in front
//    if(myManager->currentState == RuntimeManager::StateMachine::NOT_INITIALIZED) {
//        // TODO :: NEED TO CHECK COMPROMISATION WITH THE FRONT VEHICLE ????
//        // THIS IS ALREADY FROM THE FRONT VEHICLE => I AM THE FOLLOWER
//        // RADIO CONNECTION ESTABLISHED, SO WE CAN SWITCH TO CACC
//        // This means there is at least one beacon from the front vehicle
//
//        // TODO: recored the received time of the beacon from the front vehicle.
//        // This will periodically be used by the upper controller
//
//        myManager->currentState = RuntimeManager::StateMachine::CONNECTED_TO_FRONT_VEHICLE;
//        myManager->stateController->accStateController();
//    }

    // If currentState is, at least CAR2X_ENGAGED, we require to switch to CACC mode
    if (myManager->currentState == RuntimeManager::StateMachine::CAR2X_ENGAGED) {
        // TODO need to check all safety requirements stored in beaconRecordData

        // Then store the possible controller to switch to for the state controller
        myManager->switchController = RuntimeManager::SwitchController::ACC_TO_CACC;
        myManager->stateController->accStateController();
    }


}

void RuntimeManager::StateManager::caccStateManager() {
    // here the status/stability of the current controller will be analyzed,
    // and transition will be proposed to TODO  StateController if required

}

void RuntimeManager::StateManager::platoonStateManager() {
    // here the status/stability of the current controller will be analyzed,
    // and transition will be proposed to TODO StateController if required
    std::cout << "platoonStateManager is called to monitor the state!!!" << std::endl;

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

        std::cout << "RuntimeManager performed transition from ACC to CACC!!!" << std::endl;
    }
}

void RuntimeManager::StateController::caccStateController() {

}

void RuntimeManager::StateController::platoonController() {

}









