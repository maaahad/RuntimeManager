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

RuntimeManager::RuntimeManager(BaseApp* app) : controllerStabilityState(ControllerStabilityState::NOT_SET_YET),
currentState(StateMachine::NOT_SET_YET) {
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
    std::cout << "Warning:: Derived classes should implement it's own version of destructor of RuntimeManager"
              << std::endl;
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
    if(myManager->currentState == RuntimeManager::StateMachine::NOT_SET_YET) {
        // TODO :: NEED TO CHECK COMPROMISATION WITH THE FRONT VEHICLE ????
        // THIS IS ALREADY FROM THE FRONT VEHICLE => I AM THE FOLLOWER
        // RADIO CONNECTION ESTABLISHED, SO WE CAN SWITCH TO CACC
        // This means there is at least one beacon from the front vehicle
        myManager->currentState = RuntimeManager::StateMachine::CONNECTED_TO_FRONT_VEHICLE;
        myManager->stateController->accStateController();
    }

}

void RuntimeManager::StateManager::caccStateManager() {
    // here the status/stability of the current controller will be analyzed,
    // and transition will be proposed to TODO  StateController if required
    std::cout << "caccStateManager is called to monitor the state. Changing state to ACC!!!" << std::endl;
    myManager->traciVehicle->setActiveController(Plexe::ACC);
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
    if(myManager->currentState == RuntimeManager::StateMachine::CONNECTED_TO_FRONT_VEHICLE) {
        // this means connection to front vehicle established. Controller can switch to CACC

        // TODO :: First take appropriate action for stable transition

        // Perform state transition
        myManager->traciVehicle->setActiveController(Plexe::CACC);
        myManager->currentState = RuntimeManager::StateMachine::CACC_ESTABLISHED;

        std::cout << "RuntimeManager performed transition from ACC to CACC!!!" << std::endl;
    }
}

void RuntimeManager::StateController::caccStateController() {

}

void RuntimeManager::StateController::platoonController() {

}









