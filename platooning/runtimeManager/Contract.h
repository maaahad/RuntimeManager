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

#ifndef SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_CONTRACT_H_
#define SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_CONTRACT_H_

#include "veins/modules/application/platooning/CC_Const.h"
#include "veins/modules/application/platooning/runtimeManager/RMUtility.h"

/**
 * This is the super class for Contract
 */


enum CONTRACT_TYPE{
    WIFI,
    INTERNAL_ERROR,
};

class Contract {
    friend bool operator==(const Contract &cr1, const Contract &cr2 );
public:
    Contract(CONTRACT_TYPE type, Plexe::ACTIVE_CONTROLLER controller);
    virtual ~Contract();
    virtual void evaluate(const RMParameters &rmParam, const rm_log &rmLog, const bool onPlatoonBeacon = false, const int index = -1) = 0;
    virtual bool equal(const Contract &contract) const;
    virtual void updateOnTransition(Plexe::ACTIVE_CONTROLLER to) = 0;
    Plexe::ACTIVE_CONTROLLER getController() const;

protected:
    CONTRACT_TYPE type;
    Plexe::ACTIVE_CONTROLLER controller;
};



#endif /* SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_CONTRACT_H_ */
