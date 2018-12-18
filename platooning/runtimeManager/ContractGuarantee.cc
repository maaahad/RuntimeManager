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
#include "ContractGuarantee.h"

#include <iostream>


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Contracts's Member function's implementation
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Contract_Guarantee::Contract_Guarantee() : contractList(std::make_shared<std::multimap<Plexe::ACTIVE_CONTROLLER,std::vector<StateParameter*>>>()) ,
data(std::make_shared<contract_guarantee>()), cgList(std::make_shared<contract_guarantee_type>()){
    // TODO Auto-generated constructor stub
    initContractList();
}

Contract_Guarantee::~Contract_Guarantee() {
    // TODO Auto-generated destructor stub
}

// This is for checking TODO will extend later
void Contract_Guarantee::evaluate(RMLog_Own &state) {
    std::pair<contract_guarantee::iterator, contract_guarantee::iterator> it = data->equal_range(state.activeController);

    // Looks like iterator is working
    for( ; it.first != it.second; ++it.first) {
//        for (auto itS = ((it.first)->second).first.begin(); itS != ((it.first)->second).first.end(); ++itS) {
//
//        }
    }
}

void Contract_Guarantee::initContractList() {
    // Downgrade
    std::vector<StateParameter *> c2f_down = {new C2X(QUALITY::CRITICAL, ROLE::FRONT)};
    std::vector<StateParameter *> c2l_down = {new C2X(QUALITY::CRITICAL, ROLE::LEADER)};
    std::vector<StateParameter *> c2fc2l_down = {new C2X(QUALITY::CRITICAL, ROLE::FRONT), new C2X(QUALITY::CRITICAL, ROLE::LEADER)};

    // Upgrade
    std::vector<StateParameter *> c2f_up = {new C2X(QUALITY::OK, ROLE::FRONT)};
    std::vector<StateParameter *> c2l_up = {new C2X(QUALITY::OK, ROLE::LEADER)};
    std::vector<StateParameter *> c2fc2l_up = {new C2X(QUALITY::OK, ROLE::FRONT), new C2X(QUALITY::OK, ROLE::LEADER)};

//    (*contractList).insert(std::make_pair(Plexe::ACTIVE_CONTROLLER::CACC,c2f_down));
//    (*contractList).insert(std::make_pair(Plexe::ACTIVE_CONTROLLER::CACC,c2fc2l_down));
//
//
//    (*contractList).insert(std::make_pair(Plexe::ACTIVE_CONTROLLER::PLOEG,c2f_down));
//
//
//
//    (*contractList).insert(std::make_pair(Plexe::ACTIVE_CONTROLLER::ACC, c2f_up));
//    (*contractList).insert(std::make_pair(Plexe::ACTIVE_CONTROLLER::ACC, c2fc2l_up));
//
//    (*contractList).insert(std::make_pair(Plexe::ACTIVE_CONTROLLER::PLOEG, c2l_up));

    // Down grade
    data->insert(std::make_pair(Plexe::ACTIVE_CONTROLLER::CACC, std::make_pair(c2f_down, Guarantees(true, Plexe::ACTIVE_CONTROLLER::ACC))));
    data->insert(std::make_pair(Plexe::ACTIVE_CONTROLLER::CACC, std::make_pair(c2fc2l_down, Guarantees(true, Plexe::ACTIVE_CONTROLLER::ACC))));
    data->insert(std::make_pair(Plexe::ACTIVE_CONTROLLER::CACC, std::make_pair(c2l_down, Guarantees(true, Plexe::ACTIVE_CONTROLLER::PLOEG))));


    data->insert(std::make_pair(Plexe::ACTIVE_CONTROLLER::PLOEG, std::make_pair(c2f_down, Guarantees(true, Plexe::ACTIVE_CONTROLLER::ACC))));



//    (*data).insert(std::make_pair(Plexe::ACTIVE_CONTROLLER::PLOEG,std::make_pair(c2f_down, Guarantees(true))));

    // cgList
//    Contract *cr1 = new WIFIContract(CONTRACT_TYPE::WIFI, Plexe::ACTIVE_CONTROLLER::ACC, C2X(ROLE::FRONT), C2X(ROLE::LEADER));
//    Contract *cr2 = new WIFIContract(CONTRACT_TYPE::WIFI, Plexe::ACTIVE_CONTROLLER::ACC, C2X(ROLE::FRONT), C2X(ROLE::LEADER));
//    cgList->insert(std::make_pair(cr1, Guarantees(true, Plexe::ACTIVE_CONTROLLER::ACC)));
//    cgList->insert(std::make_pair(cr2, Guarantees(true, Plexe::ACTIVE_CONTROLLER::CACC)));

    WIFIContract cr1(CONTRACT_TYPE::WIFI, Plexe::ACTIVE_CONTROLLER::ACC, C2X(QUALITY::OK, ROLE::FRONT), C2X(ROLE::LEADER));
    WIFIContract cr2(CONTRACT_TYPE::WIFI, Plexe::ACTIVE_CONTROLLER::ACC, C2X(QUALITY::OK, ROLE::FRONT), C2X(ROLE::LEADER));
    WIFIContract cr3(CONTRACT_TYPE::WIFI, Plexe::ACTIVE_CONTROLLER::ACC, C2X(ROLE::FRONT), C2X(ROLE::LEADER));

    cgList->insert(std::make_pair(cr1, Guarantees(true, Plexe::ACTIVE_CONTROLLER::ACC)));
    cgList->insert(std::make_pair(cr2, Guarantees(true, Plexe::ACTIVE_CONTROLLER::CACC)));
    cgList->insert(std::make_pair(cr3, Guarantees(true, Plexe::ACTIVE_CONTROLLER::CACC)));


    contract_guarantee_type ::size_type size = cgList->size();
    int check = (cr1 == cr2);
    std::cout << "  " <<std::endl;

    // downgrade
}

