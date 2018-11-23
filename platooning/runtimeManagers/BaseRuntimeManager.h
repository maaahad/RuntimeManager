/*
 * BaseRuntimeManager.h
 *
 *  Created on: Oct 30, 2018
 *      Author: ahad
 */

#ifndef RUNTIMEMANAGERS_BASERUNTIMEMANAGER_H_
#define RUNTIMEMANAGERS_BASERUNTIMEMANAGER_H_

#include<atomic>

#include "veins/modules/application/platooning/utilities/BasePositionHelper.h"
#include "veins/modules/mobility/traci/TraCIMobility.h"

#define DEBUG_RUNTIMEMANAGER

//=================================================================================================================================//
// Forward declaration of Application
//=================================================================================================================================//
class BaseApp;

//=================================================================================================================================//
// BaseRuntimeManager
//=================================================================================================================================//
class BaseRuntimeManager {
public:
    BaseRuntimeManager(BaseApp *app);
    virtual ~BaseRuntimeManager();

    /**
     * This method analyze the stored beacon related information by calling appropriate StateManager
     * This method is called by the application during handleSelfMessage()
     * This method is also called within RuntimaManager from record()
     */
    virtual void monitor() = 0;
    /**
     * This method store the beacon information such as, beacon received time, beacon delay to be used by StateManager
     * This method is called by the application during onPlatoonBeacon()
     */
    virtual void record(const int sourceVehicleId, simtime_t currentSimTime) = 0;

    /**
     * This method is called by the handleSefMsg to perform degradation
     */
    virtual void triggerDegradation() = 0;

protected:
    /**
     * This method is called by class method member record
     * to update the StateMachine
     */
    virtual void updateStateMachine(const int sourceVehicleId, const simtime_t currentSimTime) = 0;
    /**
     * This method is called by class method member record
     * to update the beacon record
     */
    virtual void updateSafetyRecords(const int key, simtime_t currentSimTime) = 0;

    /**
     * This struct type use to record the safety related information on receiving beacon from another vehicles
     */
    struct SafetyRecords {

        simtime_t lastBeaconArrivalTime;
        simtime_t timeIntervalBetweenBeacon;

        simtime_t firstBeaconArrivalTime;
        int nbeaconReceived;                // WE CAN KEEP COUNTING AFTER CONNECTION TO USE THIS TO COMPUTE packetLossRate
        double avgBeaconInterval;

        double packetLossRate;
    };

    /**
     * This is the state machine for the runtime manager
     * to store/identify the state of a vehicle requires
     * for checking active controller's stability
     */
    enum class RTStateMachine {
        CAR2FRONT_CAR2LEADER_DISENGAGED,
        CAR2LEADER_ENGAGED,
        CAR2FRONT_ENGAGED,
        CAR2FRONT_CAR2LEADER_ENGAGED,
    };

    /**
     * This enum class is used by StateManager to propose controller transition
     * StateController use this information to perform actual controller transition
     * TODO PLATOON ??
     */
    enum class SwitchController {
        NOT_INITIALIZED,
        ACC_TO_CACC,
        ACC_TO_PLOEG,
        BACK_TO_PLOEG,
        CACC_TO_ACC,
        CACC_TO_PLOEG,
        PLOEG_TO_CACC,
        PLOEG_TO_ACC,
        BACK_TO_CACC,
    };

    enum class DegradationState {
        DEGRADATION_NOT_INITIATED,
        DEGRADATION_INITIATED,
        DEGRADATION_COMPLETED,
    };

    /**
     * The instance of class StateManager is responsible for analyzing the SafetyRecords
     * And propose the possible controller transition to the StateController
     */
    class StateManager {
    public:
        StateManager(BaseRuntimeManager*);
        void upgradationStateManager();
        void degradationStateManager();
        void accStateManager();
        void caccStateManager();
        void ploegStateManager();
    private:
        // TODO a method for safeChecking
        void transitionCheck();
        bool connectionOK(int key);
        BaseRuntimeManager *myManager;
    };

    /**
     * The instance of StateController is to take appropriate action
     * for stable transition proposed by StateManager and impose transition
     */
    class StateController {
    public:
        StateController (BaseRuntimeManager*);
        void accStateController();
        void caccStateController();
        void ploegStateController();
    private:
        void adjust() const;
        BaseRuntimeManager *myManager;
    };


    //===============================================================//
    // Protected Data
    //===============================================================//
    std::atomic<RTStateMachine> rtState{RTStateMachine::CAR2FRONT_CAR2LEADER_DISENGAGED};
    std::atomic<SwitchController> switchController{SwitchController::NOT_INITIALIZED};

    // Degradation
    std::atomic<DegradationState> degState{DegradationState::DEGRADATION_NOT_INITIATED};
    std::atomic<bool> abortDegradation{false};


    StateManager *stateManager;
    StateController *stateController;

    std::map<int, SafetyRecords> safetyRecords;


    BaseApp *app;
    Veins::TraCIMobility* mobility;
    Veins::TraCICommandInterface* traci;
    Veins::TraCICommandInterface::Vehicle* traciVehicle;
    BasePositionHelper* positionHelper;

private:


};

#endif /* RUNTIMEMANAGERS_BASERUNTIMEMANAGER_H_ */
