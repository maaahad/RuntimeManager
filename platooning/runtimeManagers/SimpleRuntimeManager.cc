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
        //stateManager->caccStateManager();
        break;
    case Plexe::PLOEG:
        //stateManager->ploegStateManager();
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
        // sanity check
        ASSERT(rtState != RTStateMachine::CAR2FRONT_ENGAGED);
        ASSERT(rtState != RTStateMachine::CAR2FRONT_CAR2LEADER_ENGAGED);

        // Update the RTStateMachine
        if(rtState == RTStateMachine::CAR2FRONT_CAR2LEADER_DISENGAGED) {
            if (sourceVehicleId == positionHelper->getLeaderId()) {
                rtState = (sourceVehicleId == positionHelper->getFrontId()) ? RTStateMachine::CAR2FRONT_CAR2LEADER_ENGAGED :
                        RTStateMachine::CAR2LEADER_ENGAGED;
            } else if(sourceVehicleId == positionHelper->getFrontId()) {
                rtState = (sourceVehicleId == positionHelper->getLeaderId()) ? RTStateMachine::CAR2FRONT_CAR2LEADER_ENGAGED :
                                        RTStateMachine::CAR2FRONT_ENGAGED;
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
        } else if(rtState == RTStateMachine::CAR2LEADER_ENGAGED) {
            // Sanity check
            ASSERT(positionHelper->getFrontId() != positionHelper->getLeaderId());

            if (sourceVehicleId == positionHelper->getLeaderId()) {
                // DO NOTHING
            } else if(sourceVehicleId == positionHelper->getFrontId()) {
                rtState = RTStateMachine::CAR2FRONT_CAR2LEADER_ENGAGED;
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
//        if(sourceVehicleId == positionHelper->getLeaderId()) {
//            auto safetyData = safetyRecords.find(sourceVehicleId);
//            if(safetyData->second.nbeaconReceived >= app->getNBeaconToAcknoledgeConnectionEstd() &&
//               safetyData->second.avgBeaconInterval <= app->getAcceptedAvgBeaconInterval()) {
//
//                if(currentState == BaseRuntimeManager::StateMachine::CAR2FRONT_CAR2LEADER_DISENGAGED ||
//                   currentState == BaseRuntimeManager::StateMachine::ACC_CAR2LEADER_DISENGAGED ||
//                   currentState == BaseRuntimeManager::StateMachine::ACC_CAR2FRONT_DISENGAGED) {
//
//                    if (sourceVehicleId == positionHelper->getFrontId()) {
//                        currentState = BaseRuntimeManager::StateMachine::CACC_CAR2FRONT_CAR2LEADER_ENGAGED;
//                    } else {
//                        currentState = BaseRuntimeManager::StateMachine::ACC_CAR2LEADER_ENGAGED;
//                    }
//                }
//            }
//
//        } else if (sourceVehicleId == positionHelper->getFrontId()) {
//            auto safetyData = safetyRecords.find(sourceVehicleId);
//            if(safetyData->second.nbeaconReceived >= app->getNBeaconToAcknoledgeConnectionEstd() &&
//               safetyData->second.avgBeaconInterval <= app->getAcceptedAvgBeaconInterval()) {
//                if(currentState == BaseRuntimeManager::StateMachine::CAR2FRONT_CAR2LEADER_DISENGAGED ||
//                   currentState == BaseRuntimeManager::StateMachine::ACC_CAR2LEADER_DISENGAGED ||
//                   currentState == BaseRuntimeManager::StateMachine::ACC_CAR2FRONT_DISENGAGED) {
//
//
//                    if (sourceVehicleId == positionHelper->getLeaderId()) {
//                        currentState = BaseRuntimeManager::StateMachine::CACC_CAR2FRONT_CAR2LEADER_ENGAGED;
//                    } else {
//                        currentState = BaseRuntimeManager::StateMachine::CACC_CAR2FRONT_ENGAGED;
//                    }
//
//                } else if (currentState == BaseRuntimeManager::StateMachine::ACC_CAR2LEADER_ENGAGED) {
//                    // Sanity check
//                    assert(sourceVehicleId != positionHelper->getLeaderId());
//
//                    currentState = BaseRuntimeManager::StateMachine::CACC_CAR2FRONT_CAR2LEADER_ENGAGED;
//                }
//            }
//
//        } else {
//            std::cerr << "Error : wrong Vehicle Id"
//                      << "\n\tFile: "
//                      << __FILE__
//                      << "\n\tFunction: "
//                      << __func__
//                      << "\n\tLine: "
//                      << __LINE__
//                      << std::endl;
//        }
        break;
    case Plexe::CACC:
        if(sourceVehicleId == positionHelper->getLeaderId()) {
            auto safetyData = safetyRecords.find(sourceVehicleId);
            if(safetyData->second.nbeaconReceived >= app->getNBeaconToAcknoledgeConnectionEstd() &&
               safetyData->second.avgBeaconInterval <= app->getAcceptedAvgBeaconInterval()) {

                if(currentState == BaseRuntimeManager::StateMachine::CACC_CAR2FRONT_ENGAGED ||
                   currentState == BaseRuntimeManager::StateMachine::CAR2FRONT_CAR2LEADER_DISENGAGED ||
                   currentState == BaseRuntimeManager::StateMachine::CACC_CAR2LEADER_DISENGAGED) {

                    if(currentState == BaseRuntimeManager::StateMachine::CACC_CAR2FRONT_ENGAGED ||
                       sourceVehicleId == positionHelper->getFrontId()) {

                        currentState = BaseRuntimeManager::StateMachine::CACC_CAR2FRONT_CAR2LEADER_ENGAGED;
                    } else {
                        currentState = BaseRuntimeManager::StateMachine::CACC_CAR2LEADER_ENGAGED;
                    }

                 }
            }


        } else if (sourceVehicleId == positionHelper->getFrontId()) {
            auto safetyData = safetyRecords.find(sourceVehicleId);
            if(safetyData->second.nbeaconReceived >= app->getNBeaconToAcknoledgeConnectionEstd() &&
               safetyData->second.avgBeaconInterval <= app->getAcceptedAvgBeaconInterval()) {

                if(currentState == BaseRuntimeManager::StateMachine::CACC_CAR2LEADER_ENGAGED ||
                   currentState == BaseRuntimeManager::StateMachine::CAR2FRONT_CAR2LEADER_DISENGAGED ||
                   currentState == BaseRuntimeManager::StateMachine::CACC_CAR2LEADER_DISENGAGED) {


                    if(currentState == BaseRuntimeManager::StateMachine::CACC_CAR2LEADER_ENGAGED ||
                       sourceVehicleId == positionHelper->getLeaderId()) {

                        currentState = BaseRuntimeManager::StateMachine::CACC_CAR2FRONT_CAR2LEADER_ENGAGED;
                    } else {
                        currentState = BaseRuntimeManager::StateMachine::CACC_CAR2FRONT_ENGAGED;
                    }

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
    case Plexe::PLOEG:
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
        safetyData->second.avgBeaconInterval = (currentSimTime.dbl() - safetyData->second.firstBeaconArrivalTime.dbl()) / (double)safetyData->second.nbeaconReceived;


#ifdef DEBUG_RUNTIMEMANAGER
//        if(positionHelper->getId() == 1 && key == 0)
//        std::cout << "vehicle_" << positionHelper->getId() << " from vehicle_"
//                         << key << ":\n\tlastBeaconArrivalTime: " << safetyData->second.lastBeaconArrivalTime.dbl()
//                         << "\n\ttimeIntervalBetweenBeacon: "<< safetyData->second.timeIntervalBetweenBeacon.dbl()
//                         << "\n\tnbeaconReceived: " << safetyData->second.nbeaconReceived
//                         << "\n\tavgBeaconInterval: " << safetyData->second.avgBeaconInterval
//                         << std::endl;
#endif

    }

//#ifdef DEBUG_RUNTIMEMANAGER
//        std::cout << "Total No. of vehicle in recordData: " << safetyRecords.size() << std::endl;
//#endif

}









