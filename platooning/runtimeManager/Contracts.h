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

#ifndef SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_CONTRACTS_H_
#define SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_CONTRACTS_H_

#include<vector>
#include<memory>
#include<map>

#include "veins/modules/application/platooning/CC_Const.h"
#include "veins/modules/application/platooning/runtimeManager/StateParameter.h"
#include "veins/modules/application/platooning/runtimeManager/Guarantees.h"



class Contracts {
public:
    Contracts();
    virtual ~Contracts();
    void evaluate(RMLog_Own &state);
private:
    // A companion type
    using contract_guarantee = std::multimap<Plexe::ACTIVE_CONTROLLER, std::pair<std::vector<StateParameter*>, Guarantees>>;
    void initContractList();
    std::shared_ptr<std::multimap<Plexe::ACTIVE_CONTROLLER, std::vector<StateParameter*>>> contractList;
    std::shared_ptr<contract_guarantee> data;
};

#endif /* SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_CONTRACTS_H_ */
