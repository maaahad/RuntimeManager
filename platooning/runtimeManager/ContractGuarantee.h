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

#ifndef SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_CONTRACTGUARANTEE_H_
#define SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_CONTRACTGUARANTEE_H_

#include <vector>
#include <memory>
#include <unordered_map>
#include <map>

#include "contracts/WIFIContract.h"
#include "veins/modules/application/platooning/CC_Const.h"
#include "veins/modules/application/platooning/runtimeManager/StateParameter.h"
#include "veins/modules/application/platooning/runtimeManager/Guarantees.h"
#include "veins/modules/application/platooning/runtimeManager/rmContainer/RMCGContainer.h"





class RuntimeManager;

class Contract_Guarantee {
public:
    Contract_Guarantee(RuntimeManager *rm);
    virtual ~Contract_Guarantee();
    void evaluate(RM::RMLog_Own &state);
private:
    void initContractList(RuntimeManager *rm);
    template <typename C, typename G> void addCG(const C &c, const G &g);
    using contract_guarantee_type = std::unordered_map<const WIFIContract, const Guarantees>;
    std::shared_ptr<contract_guarantee_type> wifiCG;

    // Use of new RMContainer
    std::shared_ptr<std::map<CONTRACT_TYPE, RMContainer *>> rmcg;
};

#endif /* SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_CONTRACTGUARANTEE_H_ */
