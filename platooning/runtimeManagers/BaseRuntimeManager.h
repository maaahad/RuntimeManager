/*
 * BaseRuntimeManager.h
 *
 *  Created on: Oct 30, 2018
 *      Author: ahad
 */

#ifndef RUNTIMEMANAGERS_BASERUNTIMEMANAGER_H_
#define RUNTIMEMANAGERS_BASERUNTIMEMANAGER_H_

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
        int vehicleId;
        simtime_t lastBeaconArrivalTime;
        simtime_t timeIntervalBetweenBeacon;

        simtime_t firstBeaconArrivalTime;
        int nbeaconReceived;                // WE CAN KEEP COUNTING AFTER CONNECTION TO USE THIS TO COMPUTE packetLossRate
        double avgBeaconInterval;
        //bool connectionEstablished;       // WE DON'T NEED THIS RIGHT NOW

        int nbeaconMissed;
        double packetLossRate;
    };

    /**
     * This is the state machine for the runtime manager
     * to store/identify the state of a vehicle requires
     * for checking active controller's stability
     * TODO PLATOON ??
     */
    enum class StateMachine {
        CAR2FRONT_CAR2LEADER_DISENGAGED,
        ACC_CAR2LEADER_ENGAGED,
        ACC_CAR2LEADER_DISENGAGED,
        ACC_CAR2FRONT_DISENGAGED,
        CACC_CAR2FRONT_ENGAGED,
        CACC_CAR2LEADER_ENGAGED,
        CACC_CAR2LEADER_DISENGAGED,
        CACC_CAR2FRONT_CAR2LEADER_ENGAGED,
    };

    /**
     * This enum class is used by StateManager to propose controller transition
     * StateController use this information to perform actual controller transition
     * TODO PLATOON ??
     */
    enum class SwitchController {
        NOT_INITIALIZED,
        ACC_TO_CACC,
        ACC_TO_PLATOON,
        CACC_TO_ACC,
        CACC_TO_PLATOON,
        PLATOON_TO_CACC,
        PLATOON_TO_ACC,
    };

    /**
     * The instance of class StateManager is responsible for analyzing the SafetyRecords
     * And propose the possible controller transition to the StateController
     */
    class StateManager {
    public:
        StateManager(BaseRuntimeManager*);
        void accStateManager();
        void caccStateManager();
        void platoonStateManager();
    private:
        bool safetyCheckingOK(int key);
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
        void platoonController();
    private:
        BaseRuntimeManager *myManager;
    };


    //===============================================================//
    // Protected Data
    //===============================================================//
    StateMachine currentState;
    SwitchController switchController;

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
