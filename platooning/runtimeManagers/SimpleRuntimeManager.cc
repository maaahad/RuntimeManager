/*
 * SimpleRuntimeManager.cc
 *
 *  Created on: Oct 30, 2018
 *      Author: ahad
 */

#include "veins/modules/application/platooning/apps/BaseApp.h"
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
    // Nothing to delete here. BaseRuntimeManager will delete the allocated memory
    // As this derived class does not have any dynamically allocated memory
}

//=================================================================================================================================//
// override virtual mehtods
//=================================================================================================================================//
void SimpleRuntimeManager::monitor() {
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

    updateSafetyRecords(sourceVehicleId, currentSimTime);

    if (sourceVehicleId == positionHelper->getLeaderId() || sourceVehicleId == positionHelper->getFrontId()) {
        // this is the leader or the front vehicle
        updateStateMachine(sourceVehicleId, currentSimTime);
    }
    // after updating the StateMachine and the stored information, call the monitor() to monitor the check state stability
    monitor();
}


void SimpleRuntimeManager::updateStateMachine(const int sourceVehicleId, const simtime_t currentSimTime) {
    switch(traciVehicle->getActiveController()) {
    case Plexe::ACC:
        if(sourceVehicleId == positionHelper->getLeaderId()) {
            auto safetyData = safetyRecords.find(sourceVehicleId);
            if(safetyData->second.nbeaconReceived >= app->getNBeaconToAcknoledgeConnectionEstd() &&
                    (currentSimTime - safetyData->second.firstBeaconArrivalTime.dbl()) <= app->getWaitTimeToAcknoledgeConnectionEstd()) {

                if(currentState == BaseRuntimeManager::StateMachine::ACC_CAR2FRONT_CAR2LEADER_DISENGAGED ||
                   currentState == BaseRuntimeManager::StateMachine::ACC_CAR2LEADER_DISENGAGED ||
                   currentState == BaseRuntimeManager::StateMachine::ACC_CAR2FRONT_DISENGAGED) {

                    if (sourceVehicleId == positionHelper->getFrontId()) {
                        currentState = BaseRuntimeManager::StateMachine::CACC_CAR2FRONT_CAR2LEADER_ENGAGED;
                    } else {
                        currentState = BaseRuntimeManager::StateMachine::ACC_CAR2LEADER_ENGAGED;
                    }
                }
            }

        } else if (sourceVehicleId == positionHelper->getFrontId()) {
            auto safetyData = safetyRecords.find(sourceVehicleId);
            if(safetyData->second.nbeaconReceived >= app->getNBeaconToAcknoledgeConnectionEstd() &&
                                (currentSimTime - safetyData->second.firstBeaconArrivalTime.dbl()) <= app->getWaitTimeToAcknoledgeConnectionEstd()) {
                if(currentState == BaseRuntimeManager::StateMachine::ACC_CAR2FRONT_CAR2LEADER_DISENGAGED ||
                   currentState == BaseRuntimeManager::StateMachine::ACC_CAR2LEADER_DISENGAGED ||
                   currentState == BaseRuntimeManager::StateMachine::ACC_CAR2FRONT_DISENGAGED) {


                    if (sourceVehicleId == positionHelper->getLeaderId()) {
                        currentState = BaseRuntimeManager::StateMachine::CACC_CAR2FRONT_CAR2LEADER_ENGAGED;
                    } else {
                        currentState = BaseRuntimeManager::StateMachine::CACC_CAR2FRONT_ENGAGED;
                    }

                } else if (currentState == BaseRuntimeManager::StateMachine::ACC_CAR2LEADER_ENGAGED) {
                    // Sanity check
                    assert(sourceVehicleId != positionHelper->getLeaderId());

                    currentState = BaseRuntimeManager::StateMachine::CACC_CAR2FRONT_CAR2LEADER_ENGAGED;
                }
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
        // in CACC mode, this is guaranteed that the connection to front is established
        // We don't have to think about update state machine while getting beacon from front vehicle
        // In case the simulaiton start with CACC, there is a possibility that the current state will be ACC_CAR2FRONT_CAR2LEADER_DISENGAGED
        // In this circumstance, we assume that the conection to front established as the simulation kernel already set active controller to CACC

        if(sourceVehicleId == positionHelper->getLeaderId()) {

            if(currentState == BaseRuntimeManager::StateMachine::CACC_CAR2FRONT_ENGAGED ||
               currentState == BaseRuntimeManager::StateMachine::ACC_CAR2FRONT_CAR2LEADER_DISENGAGED ||
               currentState == BaseRuntimeManager::StateMachine::CACC_CAR2LEADER_DISENGAGED) {

                auto safetyData = safetyRecords.find(sourceVehicleId);
                if(safetyData->second.nbeaconReceived >= app->getNBeaconToAcknoledgeConnectionEstd() &&
                   (currentSimTime - safetyData->second.firstBeaconArrivalTime.dbl()) <= app->getWaitTimeToAcknoledgeConnectionEstd()) {
//                    currentState = BaseRuntimeManager::StateMachine::CACC_CAR2FRONT_CAR2LEADER_ENGAGED;
                    currentState = (currentState == BaseRuntimeManager::StateMachine::CACC_CAR2FRONT_ENGAGED) ?
                            BaseRuntimeManager::StateMachine::CACC_CAR2FRONT_CAR2LEADER_ENGAGED :
                            BaseRuntimeManager::StateMachine::CACC_CAR2LEADER_ENGAGED;
                }
            }

        } else if (sourceVehicleId == positionHelper->getFrontId()) {
            if(currentState == BaseRuntimeManager::StateMachine::CACC_CAR2LEADER_ENGAGED ||
               currentState == BaseRuntimeManager::StateMachine::ACC_CAR2FRONT_CAR2LEADER_DISENGAGED ||
               currentState == BaseRuntimeManager::StateMachine::CACC_CAR2LEADER_DISENGAGED) {

                auto safetyData = safetyRecords.find(sourceVehicleId);
                if(safetyData->second.nbeaconReceived >= app->getNBeaconToAcknoledgeConnectionEstd() &&
                   (currentSimTime - safetyData->second.firstBeaconArrivalTime.dbl()) <= app->getWaitTimeToAcknoledgeConnectionEstd()) {

                    currentState = (currentState == BaseRuntimeManager::StateMachine::CACC_CAR2LEADER_ENGAGED) ?
                                                BaseRuntimeManager::StateMachine::CACC_CAR2FRONT_CAR2LEADER_ENGAGED :
                                                BaseRuntimeManager::StateMachine::CACC_CAR2FRONT_ENGAGED;
                }

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
void SimpleRuntimeManager::updateSafetyRecords(const int key, simtime_t currentSimTime) {

    // TODO IN CASE OF CONNECTION LOST WE CAN DISCARD THE RECORD TO RESTART THE NEW CONNECTION PROCEDURE
    if(safetyRecords.find(key) == safetyRecords.end()) {
        // This is the first time called of this method for this key during the simulation
        // or after connection to key is lost
        // create a new records for this key and insert it to the safetyRecords
        // TODO ADD ADDITIONAL SAFETY DATA IF REQUIRED
        BaseRuntimeManager::SafetyRecords safetyData;
        safetyData.lastBeaconArrivalTime = currentSimTime;
        safetyData.firstBeaconArrivalTime = currentSimTime;
        safetyData.nbeaconReceived = 0;
        safetyRecords.insert({key, safetyData});
    } else {
        // The record for the key is already exist
        auto safetyData = safetyRecords.find(key);
        safetyData->second.timeIntervalBetweenBeacon = currentSimTime - safetyData->second.lastBeaconArrivalTime;
        safetyData->second.lastBeaconArrivalTime = currentSimTime;
        safetyData->second.nbeaconReceived += 1;

#ifdef DEBUG_RUNTIMEMANAGER
//        std::cout << "vehicle_" << positionHelper->getId() << " from vehicle_"
//                         << key << ":\n\tlastBeaconArrivalTime: " << safetyData->second.lastBeaconArrivalTime.dbl()
//                         << "\n\ttimeIntervalBetweenBeacon: "<< safetyData->second.timeIntervalBetweenBeacon.dbl()
//                         << "\n\tnbeaconReceived: " << safetyData->second.nbeaconReceived
//                         << std::endl;
#endif

    }

//#ifdef DEBUG_RUNTIMEMANAGER
//        std::cout << "Total No. of vehicle in recordData: " << safetyRecords.size() << std::endl;
//#endif

}









