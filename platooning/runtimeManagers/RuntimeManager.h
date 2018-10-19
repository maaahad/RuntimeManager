/*
 * RuntimeManager.h
 *
 *  Created on: Oct 18, 2018
 *      Author: ahad
 */

#ifndef RUNTIMEMANAGER_H_
#define RUNTIMEMANAGER_H_

// The runtime manager requires to have a reference to the Application under observation
class BaseApp;

#include "veins/modules/application/platooning/utilities/BasePositionHelper.h"
#include "veins/modules/mobility/traci/TraCIMobility.h"

class RuntimeManager {
public:
    RuntimeManager(BaseApp* app);

    // We want all derived classes to override the destructor
    virtual ~RuntimeManager();

    // method called by the application periodically
    // This is a virtual method. Complex derived class of RuntimeManager can implement their own version (May be!!!!! Not sure yet)
    virtual void monitor();

protected:

    enum class ControllerStabilityState {
        NOT_SET_YET,
        STABLE,
        UNSTABLE,
    };

    enum class StateMachine {
        NOT_SET_YET,
        CONNECTED_TO_FRONT_VEHICLE,
        COMPROMISED_WITH_FRONT_VEHICLE,
        CACC_ESTABLISHED,
        CONNECDTED_TO_PLATOON_LEADER,
        PLATOON_ESTABLISHED,
        CONNECTED_TO_BOTH_FRONT_VEHILE_AND_PLATOON_LEADER,
    };

    class StateManager {
    public:
        StateManager(RuntimeManager*);
        ~StateManager();
        void accStateManager();
        void caccStateManager();
        void platoonStateManager();
    private:
        RuntimeManager* myManager;
    };


    class StateController {
    public:
        StateController (RuntimeManager*);
        void accStateController();
        void caccStateController();
        void platoonController();
    private:
        RuntimeManager* myManager;
    };

    // Controller state
    ControllerStabilityState controllerStabilityState;

    //record the current state of the vehicle within the StateMachine
    enum StateMachine currentState;

    // StateManager:: It's responsibility is to check the safety requirements and propose the transition if required
    StateManager* stateManager;

    // StateController: this guy perform state transition based on instruction from the StateManager
    // And take appropriate action for stable state transition
    StateController* stateController;

    // required module. for example: PositionHelper, TraCI interface etc
    BaseApp* app;
    BasePositionHelper* positionHelper;
    Veins::TraCIMobility* mobility;
    Veins::TraCICommandInterface* traci;
    Veins::TraCICommandInterface::Vehicle* traciVehicle;


    //friend class StateManager;
    //friend class StateController;
};



#endif /* RUNTIMEMANAGER_H_ */
