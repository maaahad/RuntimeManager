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
    friend class std::hash<const WIFIContract>;
public:
//    WIFIContract(CONTRACT_TYPE type, Plexe::ACTIVE_CONTROLLER controller, C2X c2f, C2X c2l);
    WIFIContract(CONTRACT_TYPE type, Plexe::ACTIVE_CONTROLLER controller, C2F c2f, C2L c2l);

    virtual ~WIFIContract();
    virtual void evaluate(const RM::RMParameters &rmParam, const RM::rm_log &rmLog, const bool onPlatoonBeacon = false, const int index = -1) override;
    virtual bool equal(const Contract &contract) const override;
    virtual void updateOnTransition(Plexe::ACTIVE_CONTROLLER to) override;
private:
//    C2X c2f;
//    C2X c2l;

    C2F c2f;
    C2L c2l;
};

// Defining template specialization for std::hash<WIFIContract> to use in unordered map
// Not using the base class in Contract-Guarantee map

namespace std{
template <> struct hash<const WIFIContract> {
   typedef size_t result_type;
   typedef WIFIContract argument_type;
   result_type operator()(const argument_type &wifi_contract) const {
//       return std::hash<std::underlying_type<CONTRACT_TYPE>::type>()(wifi_contract.type) ^
//              std::hash<std::underlying_type<Plexe::ACTIVE_CONTROLLER>::type>()(wifi_contract.controller) ^
//              std::hash<C2X>()(wifi_contract.c2f) ^
//              std::hash<C2X>()(wifi_contract.c2l);
       return std::hash<std::underlying_type<CONTRACT_TYPE>::type>()(wifi_contract.type) ^
              std::hash<std::underlying_type<Plexe::ACTIVE_CONTROLLER>::type>()(wifi_contract.controller) ^
              std::hash<C2F>()(wifi_contract.c2f) ^
              std::hash<C2L>()(wifi_contract.c2l);
   }
};
}

#endif /* SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_CONTRACT_WIFICONTRACT_H_ */
