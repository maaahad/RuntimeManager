/*
 * SimpleRuntimeManager.cc
 *
 *  Created on: Oct 30, 2018
 *      Author: ahad
 */

#include "SimpleRuntimeManager.h"


//=================================================================================================================================//
// Constructor
//=================================================================================================================================//
SimpleRuntimeManager::SimpleRuntimeManager(BaseApp *app) : BaseRuntimeManager(app) {
    // TODO Auto-generated constructor stub

}


//=================================================================================================================================//
// Destructor
//=================================================================================================================================//
SimpleRuntimeManager::~SimpleRuntimeManager() {
    // TODO Auto-generated destructor stub
}

//=================================================================================================================================//
// override virtual mehtods
//=================================================================================================================================//
void SimpleRuntimeManager::monitor() {
//    std::cout << "Warning: " << __FILE__
//              << "\n\tLine: " << __LINE__
//              << "\n\tCompiled on: " << __DATE__
//              << " at " << __TIME__
//              << "\n\tfunction " << __func__
//              << " : not been implemented yet!!!"
//              << std::endl;

    switch(traciVehicle->getActiveController()) {
    case Plexe::ACC:
        stateManager->accStateManager();
        break;
    case Plexe::CACC:
        stateManager->caccStateManager();
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

void SimpleRuntimeManager::record(const int sourceVehicleId, simtime_t currentSimTime){
//    std::cout << "Warning: "
//              << __FILE__
//              << "\n\tLine: "
//              << __LINE__
//              << "\n\tCompiled on: "
//              << __DATE__
//              << " at "
//              << __TIME__
//              << "\n\tfunction "
//              << __func__
//              << " : not been implemented yet!!!"
//              << std::endl;


    if (sourceVehicleId == positionHelper->getLeaderId()) {

        // this is the leader
        updateStateMachine(sourceVehicleId);
        // now keep record of the beacon in the beaconRecordData
        //updateSafetyRecords("leader", currentSimTime);
    } else if (sourceVehicleId == positionHelper->getFrontId()) {
        // this is the front vehicle
        updateStateMachine(sourceVehicleId);
        // now keep record of the beacon in the beaconRecordData
        //updateSafetyRecords("front", currentSimTime);
    } else {
        // this is other vehicle
        // these records are used to determine whether a vehicle encounters a communication failure
        std::string key = std::string("vehicle_") + std::to_string(sourceVehicleId);
        //updateSafetyRecords(key, currentSimTime);
    }

    //===================================================== Test ======================================================//
    // this is for testing purpose
    if(currentState == BaseRuntimeManager::StateMachine::CACC_CAR2FRONT_ENGAGED ||
            currentState == BaseRuntimeManager::StateMachine::CACC_CAR2FRONT_CAR2LEADER_ENGAGED) {
        if(traciVehicle->getActiveController() != Plexe::CACC) {
            traciVehicle->setActiveController(Plexe::CACC);
//            if (positionHelper->getId() == 2)
            std::cout << "VehicleId: " << positionHelper->getId() << "\n\tRuntimeManager performed transition from ACC to CACC!!!" << std::endl;
        }

    }
    //===================================================== Test ======================================================//


    // after updating the StateMachine and the stored information, call the monitor() to monitor the state stability
    //monitor();
}


void SimpleRuntimeManager::updateStateMachine(const int sourceVehicleId) {
//    std::cout << "Warning: "
//              << __FILE__
//              << "\n\tLine: "
//              << __LINE__
//              << "\n\tCompiled on: "
//              << __DATE__
//              << " at "
//              << __TIME__
//              << "\n\tfunction "
//              << __func__
//              << " : under implementation!!"
//              << std::endl;

    switch(traciVehicle->getActiveController()) {
    case Plexe::ACC:
        if(sourceVehicleId == positionHelper->getLeaderId()) {
            if(currentState == BaseRuntimeManager::StateMachine::ACC_CAR2FRONT_CAR2LEADER_DISENGAGED ||
               currentState == BaseRuntimeManager::StateMachine::ACC_CAR2LEADER_DISENGAGED ||
               currentState == BaseRuntimeManager::StateMachine::ACC_CAR2FRONT_DISENGAGED) {

                if (positionHelper->getId() == 1) {
                    currentState = BaseRuntimeManager::StateMachine::CACC_CAR2FRONT_CAR2LEADER_ENGAGED;
                } else {
                    currentState = BaseRuntimeManager::StateMachine::ACC_CAR2LEADER_ENGAGED;
                }
            }
        } else if (sourceVehicleId == positionHelper->getFrontId()) {
            if(currentState == BaseRuntimeManager::StateMachine::ACC_CAR2FRONT_CAR2LEADER_DISENGAGED ||
               currentState == BaseRuntimeManager::StateMachine::ACC_CAR2LEADER_DISENGAGED ||
               currentState == BaseRuntimeManager::StateMachine::ACC_CAR2FRONT_DISENGAGED) {


                if (positionHelper->getId() == 1) {
                    currentState = BaseRuntimeManager::StateMachine::CACC_CAR2FRONT_CAR2LEADER_ENGAGED;
                } else {
                    currentState = BaseRuntimeManager::StateMachine::CACC_CAR2FRONT_ENGAGED;
                }

            } else if (currentState == BaseRuntimeManager::StateMachine::ACC_CAR2LEADER_ENGAGED) {

//                if (positionHelper->getId() == 1 && sourceVehicleId == positionHelper->getFrontId()) {
//                    std::cout << "I am here an I am confused here ...My front and leader is the same vehicle!!!!!!!!!!!" << std::endl;
//                }
                currentState = BaseRuntimeManager::StateMachine::CACC_CAR2FRONT_CAR2LEADER_ENGAGED;
            }
        } else {
            std::cerr << "Error : wrong Vehicle Id"
                      << "\n\tFile: "
                      << __FILE__
                      << "\n\tFunction: "
                      << __func__
                      << "\n\tLine: "
                      << __LINE__
                      << std::endl;
        }
        break;
    case Plexe::CACC:
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
void SimpleRuntimeManager::updateSafetyRecords(const std::string &key, simtime_t currentSimTime) {
    std::cout << "Warning: "
              << __FILE__
              << "\n\tLine: "
              << __LINE__
              << "\n\tCompiled on: "
              << __DATE__
              << " at " << __TIME__
              << "\n\tfunction " << __func__
              << " : not been implemented yet!!!"
              << std::endl;
}









