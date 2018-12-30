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

Contract_Guarantee::Contract_Guarantee(RuntimeManager *rm) : wifiCG(std::make_shared<contract_guarantee_type>()) ,
rmcg(std::make_shared<std::map<CONTRACT_TYPE, RMContainer *>>()){
    // TODO Auto-generated constructor stub
    initContractList(rm);
}

Contract_Guarantee::~Contract_Guarantee() {
    // TODO Auto-generated destructor stub
}

// This is for checking TODO will extend later
void Contract_Guarantee::evaluate(RM::RMLog_Own &state) {

//    // OLD Iterate through every single contract of the state and perform the associate guarantees
//    for(auto start = (state.contracts)->begin(); start != (state.contracts)->end(); ++start) {
//        if(auto contract = dynamic_cast<WIFIContract *>(*start)) {
//            // match should be unique
//            auto match = wifiCG->find(*contract);
//            if(match != wifiCG->end()) {
//                // Match found and call the provideGuarantee method of the found Guarantee
//                Guarantees guarantee = match->second;
//                guarantee.provideGuarantee(*start);
//            } else {
//                std::cout << "Not match contract found. No action needs to be taken...." << std::endl;
//            }
//            // each contract of a vehicle should be exclusive to only one particular Contract_Guarantee list
//            continue;
//        }
//
//        // TODO check in other Contract_Gurantee list
//    }



    // TRYING WITH new RMContainer
    for(auto start = (state.contracts)->begin(); start != (state.contracts)->end(); ++start) {
        if(auto contract = dynamic_cast<WIFIContract *>(*start)) {
            // Sanity check
            ASSERT(contract->getContractType() == CONTRACT_TYPE::WIFI);
            // get the all contract-guarantee list from rmcg for CONTRACT_TYPE::WIFI
            auto match = rmcg->find(CONTRACT_TYPE::WIFI);
            if(match != rmcg->end()){
                // We found a list of contract-guarantee for WIFI contract type
                auto matchedCGContainer = static_cast<RMCGContainer<WIFIContract, Guarantees> *>(match->second);

                matchedCGContainer->provideGuarantee(contract);

//                auto matchedCG = (matchedCGContainer->cgs)->find(*contract); //TODO Delegate this to Container.. LATER
//                if(matchedCG != (matchedCGContainer->cgs)->end()) {
//                    // Match found and call the provideGuarantee method of the found Guarantee
//                    Guarantees guarantee = matchedCG->second;
//                    guarantee.provideGuarantee(*start);
//                } else {
//                    std::cout << "Not match contract found. No action needs to be taken...." << std::endl;
//                }

            }
            continue;
        }

        // TODO check in other Contract_Gurantee list
    }
}


template <typename C, typename G> void Contract_Guarantee::addCG(const C &c, const G &g) {
    auto cgList = rmcg->find(c.getContractType());
    if(cgList != rmcg->end()) {
        if(c.getContractType() == CONTRACT_TYPE::WIFI) {
            // add the new element to the match
            (static_cast<RMCGContainer<WIFIContract, Guarantees> *>(cgList->second))->addCG(c, g);
        } else if(c.getContractType() == CONTRACT_TYPE::INTERNAL_ERROR) {
            std::cout<<"Only CONTRACT_TYPE::WIFI is available right now..." << std::endl;
        }
        // TODO add for other contract type
    } else {
        if(c.getContractType() == CONTRACT_TYPE::WIFI) {
            rmcg->insert(std::make_pair(CONTRACT_TYPE::WIFI, new RMCGContainer<WIFIContract, Guarantees>(c, g, CONTRACT_TYPE::WIFI)));
        } else if(c.getContractType() == CONTRACT_TYPE::INTERNAL_ERROR) {
            std::cout<<"Only CONTRACT_TYPE::WIFI is available right now..." << std::endl;
        }
    }
}


void Contract_Guarantee::initContractList(RuntimeManager *rm) {
    // Creating the WIFIContract-Guarantee list

    // StateParameters
//    C2X ok_c2f(WIFI_QUALITY::OK, ROLE::FRONT);
//    C2X poor_c2f(WIFI_QUALITY::POOR, ROLE::FRONT);
//    C2X critical_c2f(WIFI_QUALITY::CRITICAL, ROLE::FRONT);
//
//    C2X ok_c2l(WIFI_QUALITY::OK, ROLE::LEADER);
//    C2X poor_c2l(WIFI_QUALITY::POOR, ROLE::LEADER);
//    C2X critical_c2l(WIFI_QUALITY::CRITICAL, ROLE::LEADER);

    // StateParameters C2F : atSafetyDistance = true (default)
    C2F ok_c2f(WIFI_QUALITY::OK);
    C2F poor_c2f(WIFI_QUALITY::POOR);
    C2F critical_c2f(WIFI_QUALITY::CRITICAL);
    // StateParameters C2L
    C2L ok_c2l(WIFI_QUALITY::OK);
    C2L poor_c2l(WIFI_QUALITY::POOR);
    C2L critical_c2l(WIFI_QUALITY::CRITICAL);

    // Guarantees
    Guarantees g2acc(rm, true, Plexe::ACTIVE_CONTROLLER::ACC);
    Guarantees g2ploeg(rm, true, Plexe::ACTIVE_CONTROLLER::PLOEG);
    Guarantees g2cacc(rm, true, Plexe::ACTIVE_CONTROLLER::CACC);


    // WIFIContract for ACC controller
    // Upgrade
    WIFIContract acc2cacc(CONTRACT_TYPE::WIFI, Plexe::ACTIVE_CONTROLLER::ACC, ok_c2f, ok_c2l);
    WIFIContract acc2ploeg(CONTRACT_TYPE::WIFI, Plexe::ACTIVE_CONTROLLER::ACC, ok_c2f, critical_c2l);

    // WIFIContract for PLOEG controller
    // Upgrade
    WIFIContract ploeg2cacc(CONTRACT_TYPE::WIFI, Plexe::ACTIVE_CONTROLLER::PLOEG, ok_c2f, ok_c2l);
    // degrade
    WIFIContract ploeg2acc1(CONTRACT_TYPE::WIFI, Plexe::ACTIVE_CONTROLLER::PLOEG, critical_c2f, ok_c2l);
    WIFIContract ploeg2acc2(CONTRACT_TYPE::WIFI, Plexe::ACTIVE_CONTROLLER::PLOEG, critical_c2f, critical_c2l);
    // decelerate TODO need to define the contracts by only contract as we are concerned with the quality of c2f, not c2l


    //WIFIContract for CACC
    // Degrade
    WIFIContract cacc2ploeg(CONTRACT_TYPE::WIFI, Plexe::ACTIVE_CONTROLLER::CACC, ok_c2f, critical_c2l);
    WIFIContract cacc2acc1(CONTRACT_TYPE::WIFI, Plexe::ACTIVE_CONTROLLER::CACC, critical_c2f, ok_c2l);
    WIFIContract cacc2acc2(CONTRACT_TYPE::WIFI, Plexe::ACTIVE_CONTROLLER::CACC, critical_c2f, critical_c2l);

    // mapping between WIFIContract and Guarantee
    wifiCG->insert(std::make_pair(acc2cacc, g2cacc));
    wifiCG->insert(std::make_pair(acc2ploeg, g2ploeg));

    wifiCG->insert(std::make_pair(ploeg2cacc, g2cacc));
    wifiCG->insert(std::make_pair(ploeg2acc1, g2acc));
    wifiCG->insert(std::make_pair(ploeg2acc2, g2acc));

    wifiCG->insert(std::make_pair(cacc2ploeg, g2ploeg));
    wifiCG->insert(std::make_pair(cacc2acc1, g2acc));
    wifiCG->insert(std::make_pair(cacc2acc2, g2acc));

    contract_guarantee_type::size_type size = wifiCG->size();

    // ==============================================================
    // RMCGContainer
    // ==============================================================
    addCG(acc2cacc, g2cacc);
    addCG(acc2ploeg, g2ploeg);

    addCG(ploeg2cacc, g2cacc);
    addCG(ploeg2acc1, g2acc);
    addCG(ploeg2acc2, g2acc);

    addCG(cacc2ploeg, g2ploeg);
    addCG(cacc2acc1, g2acc);
    addCG(cacc2acc2, g2acc);

    // TODO Test: CHECKING WITH ADDING DUPLICATE element
//    addCG(acc2cacc, g2cacc);     // test OK


    // TODO Test: try with add different type of contract: Test OK
//    WIFIContract acc2caccC(CONTRACT_TYPE::INTERNAL_ERROR, Plexe::ACTIVE_CONTROLLER::ACC, ok_c2f, ok_c2l);
//    Guarantees g2accC(rm, true, Plexe::ACTIVE_CONTROLLER::ACC);
//    addCG(acc2caccC, g2accC);



//
//    auto sz = ((static_cast<RMCGContainer<WIFIContract, Guarantees> *>(rmcg->find(CONTRACT_TYPE::WIFI)->second))->cgs)->size();
//    auto sz2 = rmcg->size();
//
//
//    auto cc = ((static_cast<RMCGContainer<WIFIContract, Guarantees> *>(rmcg->find(CONTRACT_TYPE::WIFI)->second))->cgs)->find(acc2cacc);
//    std::cout << cc->first <<std::endl;
}

