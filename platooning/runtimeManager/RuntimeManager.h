//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#ifndef SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_RUNTIMEMANAGER_H_
#define SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_RUNTIMEMANAGER_H_

#include "veins/base/modules/BaseApplLayer.h"
#include "veins/modules/application/platooning/messages/PlatooningBeacon_m.h"
#include "veins/modules/mobility/traci/TraCIMobility.h"
#include "veins/modules/application/platooning/utilities/BasePositionHelper.h"



class RuntimeManager : public Veins::BaseApplLayer {
public:
    RuntimeManager();
    virtual ~RuntimeManager();
    virtual void initialize(int stage) override;

    /**
     * This method is called by the application on receiving platooning beacon
     * RM log the required information in PlatooningBeacon pb
     */
    void onPlatoonBeacon(const PlatooningBeacon *pb);

    /**
     * This method return true if RM is enabled
     */
    bool isRMEnabled() {
        return rmParam.rmEnabled;
    }

protected:
    virtual void handleSelfMsg(cMessage* msg) override;

    /**
     * This structure represents the RMLog, that is the information that RM
     * stores onPlatoonBeacon from leader or front vehicle
     */
    struct RMParameters {
        bool rmEnabled;
        double rmMonitorInterval;

        int nPacketLossModerate;
        int nPacketLossPoor;
        int nPacketLossCritical;

        double minSafetyDistance;
    };

    /**
     * These structures represents logs for different vehicles (own, front, leader)
     *
     *
     */

    /**
     * This is common parameters of front and leader vehicle
     */
    struct RMLog {
        double acceleration;
        double controllerAcceleration;
        double ede;     // end to end delay ,, don't know yet how to define and use this
        // TODO more parameters
    };

    /**
     * This is the log for the own vehicle
     */
    struct RMLog_Own {
        Plexe::ACTIVE_CONTROLLER activeController;
        // TODO more parameters
    };
    /**
     * This is the log for the front vehicle
     */
    struct RMLog_Front {
      RMLog common;
      double distance;
      // TODO more parameters
    };

    /**
     * This is the log for the leader vehicle
     */
    struct RMLog_Leader {
      RMLog common;
      double distance;
      // TODO more parameters
    };


private:

    RMParameters rmParam;
    std::tuple<RMLog_Own, RMLog_Front, RMLog_Leader> rmLog;

    Veins::TraCIMobility *mobility;
    Veins::TraCICommandInterface *traci;
    Veins::TraCICommandInterface::Vehicle *traciVehicle;
    BasePositionHelper* positionHelper;

    cMessage *checkMsg;
};

#endif /* SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_RUNTIMEMANAGER_H_ */
