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

bool BaseRuntimeManager::StateManager::safetyCheckingOK(int key) {
    auto it = myManager->safetyRecords.find(key);
    simtime_t currentTime = simTime();
    if((currentTime - it->second.lastBeaconArrivalTime).dbl() > (myManager->app)->getExpectedBeaconInterval().dbl()) {
        // Connection lost to the vehicle with id key
        // Discard the record
        EV << "Connection to vehicle_" << key << "is lost. Discarding the safety records." << std::endl;

        myManager->safetyRecords.erase(key);
        return false;
    }

    return true;
}

void BaseRuntimeManager::StateManager::accStateManager() {
    if(myManager->currentState == BaseRuntimeManager::StateMachine::CACC_CAR2FRONT_ENGAGED) {
        // TODO FIRST CHECK THE SAFETY CHECK
        if(safetyCheckingOK((myManager->positionHelper)->getFrontId())) {
            myManager->switchController = BaseRuntimeManager::SwitchController::ACC_TO_CACC;
            // call the StateController to perform the transition
            myManager->stateController->accStateController();
        } else {
            myManager->currentState = BaseRuntimeManager::StateMachine::ACC_CAR2FRONT_DISENGAGED;
        }

    } else if (myManager->currentState == BaseRuntimeManager::StateMachine::CACC_CAR2FRONT_CAR2LEADER_ENGAGED) {
        // TODO FIRST CHECK THE SAFETY CHECK
        if (!safetyCheckingOK((myManager->positionHelper)->getLeaderId())) {
            myManager->currentState = BaseRuntimeManager::StateMachine::CACC_CAR2FRONT_ENGAGED;
        }

        if(safetyCheckingOK((myManager->positionHelper)->getFrontId())) {
            myManager->switchController = BaseRuntimeManager::SwitchController::ACC_TO_CACC;
            // call the StateController to perform the transition
            myManager->stateController->accStateController();
        } else {
            myManager->currentState = BaseRuntimeManager::StateMachine::ACC_CAR2FRONT_DISENGAGED;
        }

    } else if (myManager->currentState == BaseRuntimeManager::StateMachine::ACC_CAR2LEADER_ENGAGED) {
        if (!safetyCheckingOK((myManager->positionHelper)->getLeaderId())) {
            myManager->currentState = BaseRuntimeManager::StateMachine::ACC_CAR2LEADER_DISENGAGED;
        }
    }
}

void BaseRuntimeManager::StateManager::caccStateManager() {
//    std::cout << "Warning: " << __FILE__
//              << "\n\tLine: " << __LINE__
//              << "\n\tCompiled on: " << __DATE__
//              << " at " << __TIME__
//              << "\n\tfunction " << __func__
//              << " : not been implemented yet!!!"
//              << std::endl;

    if(myManager->currentState == BaseRuntimeManager::StateMachine::CACC_CAR2FRONT_ENGAGED) {
        // TODO FIRST CHECK THE SAFETY CHECK
        if(!safetyCheckingOK((myManager->positionHelper)->getFrontId())) {
            myManager->switchController = BaseRuntimeManager::SwitchController::CACC_TO_ACC;
            myManager->currentState = BaseRuntimeManager::StateMachine::ACC_CAR2FRONT_DISENGAGED;
            // call the StateController to perform the transition
            myManager->stateController->caccStateController();
        }

    } else if (myManager->currentState == BaseRuntimeManager::StateMachine::CACC_CAR2FRONT_CAR2LEADER_ENGAGED) {
        // TODO FIRST CHECK THE SAFETY CHECK
        if (!safetyCheckingOK((myManager->positionHelper)->getLeaderId())) {
            myManager->currentState = BaseRuntimeManager::StateMachine::CACC_CAR2FRONT_ENGAGED;
        }

        if(!safetyCheckingOK((myManager->positionHelper)->getFrontId())) {
            myManager->switchController = BaseRuntimeManager::SwitchController::CACC_TO_ACC;
            myManager->currentState = (myManager->currentState == BaseRuntimeManager::StateMachine::CACC_CAR2FRONT_ENGAGED) ?
                    BaseRuntimeManager::StateMachine::ACC_CAR2FRONT_DISENGAGED : BaseRuntimeManager::StateMachine::ACC_CAR2LEADER_ENGAGED;
            // call the StateController to perform the transition
            myManager->stateController->caccStateController();
        }

    }
}

//=================================================================================================================================//
// StateController's methods
//=================================================================================================================================//
BaseRuntimeManager::StateController::StateController(BaseRuntimeManager* myManager) : myManager(myManager) {

}


void BaseRuntimeManager::StateController::accStateController() {
    if(myManager->switchController == BaseRuntimeManager::SwitchController::ACC_TO_CACC) {
        // TODO TAKE APPROPRIATE ACTION FOR STABLE TRANSITION

        (myManager->traciVehicle)->setActiveController(Plexe::CACC);
#ifdef DEBUG_RUNTIMEMANAGER
        std::cout << "VehicleId: " << myManager->positionHelper->getId() << "\n\tRuntimeManager performed transition from ACC to CACC!!!" << std::endl;
#endif

    }

    // TODO ADD OTHER CONTROLLERS TRANSITION

}

void BaseRuntimeManager::StateController::caccStateController() {
    std::cout << "Warning: " << __FILE__
              << "\n\tLine: " << __LINE__
              << "\n\tCompiled on: " << __DATE__
              << " at " << __TIME__
              << "\n\tfunction " << __func__
              << " : not been implemented yet!!!"
              << std::endl;

    if(myManager->switchController == BaseRuntimeManager::SwitchController::CACC_TO_ACC) {
        (myManager->traciVehicle)->setActiveController(Plexe::ACC);
#ifdef DEBUG_RUNTIMEMANAGER
        std::cout << "VehicleId: " << myManager->positionHelper->getId() << "\n\tRuntimeManager performed transition from CACC to ACC!!!" << std::endl;
#endif
    }
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

