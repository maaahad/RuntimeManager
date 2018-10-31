/*
 * BaseRuntimeManager.cc
 *
 *  Created on: Oct 30, 2018
 *      Author: ahad
 */

#include <iostream>

#include "veins/modules/application/platooning/apps/BaseApp.h"
#include "BaseRuntimeManager.h"

//using namespace Veins;

//=================================================================================================================================//
// Constructor
//=================================================================================================================================//
BaseRuntimeManager::BaseRuntimeManager(BaseApp *app) : currentState(BaseRuntimeManager::StateMachine::ACC_CAR2FRONT_CAR2LEADER_DISENGAGED),
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

void BaseRuntimeManager::StateManager::accStateManager() {
    std::cout << "Warning: " << __FILE__
              << "\n\tLine: " << __LINE__
              << "\n\tCompiled on: " << __DATE__
              << " at " << __TIME__
              << "\n\tfunction " << __func__
              << " : not been implemented yet!!!"
              << std::endl;
}

void BaseRuntimeManager::StateManager::caccStateManager() {
    std::cout << "Warning: " << __FILE__
              << "\n\tLine: " << __LINE__
              << "\n\tCompiled on: " << __DATE__
              << " at " << __TIME__
              << "\n\tfunction " << __func__
              << " : not been implemented yet!!!"
              << std::endl;
}

//=================================================================================================================================//
// StateController's methods
//=================================================================================================================================//
BaseRuntimeManager::StateController::StateController(BaseRuntimeManager* myManager) : myManager(myManager) {

}


void BaseRuntimeManager::StateController::accStateController() {
    std::cout << "Warning: " << __FILE__
              << "\n\tLine: " << __LINE__
              << "\n\tCompiled on: " << __DATE__
              << " at " << __TIME__
              << "\n\tfunction " << __func__
              << " : not been implemented yet!!!"
              << std::endl;
}

void BaseRuntimeManager::StateController::caccStateController() {
    std::cout << "Warning: " << __FILE__
              << "\n\tLine: " << __LINE__
              << "\n\tCompiled on: " << __DATE__
              << " at " << __TIME__
              << "\n\tfunction " << __func__
              << " : not been implemented yet!!!"
              << std::endl;
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


void BaseRuntimeManager::updateStateMachine(const int sourceVehicleId){
    std::cerr << "Error: " << __FILE__
              << "\n\tLine: " << __LINE__
              << "\n\tCompiled on: " << __DATE__
              << " at " << __TIME__
              << "\n\tfunction " << __func__
              << " must have to override by the derived class!!!"
              << std::endl;
}

void BaseRuntimeManager::updateSafetyRecords(const std::string &key, simtime_t currentSimTime){
    std::cerr << "Error: " << __FILE__
              << "\n\tLine: " << __LINE__
              << "\n\tCompiled on: " << __DATE__
              << " at " << __TIME__
              << "\n\tfunction " << __func__
              << " must have to override by the derived class!!!"
              << std::endl;
}

