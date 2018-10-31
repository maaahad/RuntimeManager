//
// Copyright (c) 2012-2018 Michele Segata <segata@ccs-labs.org>
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

#ifndef SIMPLEPLATOONINGAPP_H_
#define SIMPLEPLATOONINGAPP_H_

#include "veins/modules/application/platooning/apps/BaseApp.h"

class SimplePlatooningApp : public BaseApp {

public:
    SimplePlatooningApp() {}
    virtual ~SimplePlatooningApp();

//================================ Ahad :: Start of Runtime Manager ============================//

    /** override from BaseApp */
    virtual void initialize(int stage) override;
    // ================================= Moved to BaseApp =====================================//
//    BasePositionHelper* getPositionHelper() override
//    {
//        return positionHelper;
//    }
//    Veins::TraCIMobility* getMobility() override
//    {
//        return mobility;
//    }
//    Veins::TraCICommandInterface* getTraci() override
//    {
//        return traci;
//    }
//    Veins::TraCICommandInterface::Vehicle* getTraciVehicle() override
//    {
//        return traciVehicle;
//    }

protected:
    virtual void handleSelfMsg(cMessage* msg) override;

    /**
     * Handles PlatoonBeacons
     *
     * @param PlatooningBeacon pb to handle
     */
    virtual void onPlatoonBeacon(const PlatooningBeacon* pb) override;

private:
    // self message to activate runtime manager.
    // TODO : CALLBACK TIME SHOULD BE COME FROM CONFIGURATION FILE. BaseScenario.ned, BBaseScenario.ned and .ini
    cMessage* callBackRuntimeManager;
    // Runtime manager
    RuntimeManager *runtimeManager;

//================================ Ahad :: End of Runtime Manager ==============================//

};

#endif /* SIMPLEPLATOONINGAPP_H_ */
