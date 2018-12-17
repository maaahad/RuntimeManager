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

#ifndef SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_CONTRACT_WIFICONTRACT_H_
#define SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_CONTRACT_WIFICONTRACT_H_

#include "veins/modules/application/platooning/runtimeManager/Contract.h"
#include "veins/modules/application/platooning/runtimeManager/StateParameter.h"


class WIFIContract : public Contract {
public:
    WIFIContract(CONTRACT_TYPE type, Plexe::ACTIVE_CONTROLLER controller, C2X c2f, C2X c2l);
    virtual ~WIFIContract();
    virtual void evaluate(const RMParameters &rmParam, const rm_log &rmLog, const bool onPlatoonBeacon = false, const int index = -1) override;

private:
    C2X c2f;
    C2X c2l;
};

#endif /* SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_CONTRACT_WIFICONTRACT_H_ */
