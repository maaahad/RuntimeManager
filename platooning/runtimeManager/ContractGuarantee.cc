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

#include "veins/modules/application/platooning/runtimeManager/RuntimeManager.h"
#include "ContractGuarantee.h"

#include <iostream>


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Contracts's Member function's implementation
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Contract_Guarantee::Contract_Guarantee(RuntimeManager *rm) : rmcg(std::make_shared<std::map<CONTRACT_TYPE, std::shared_ptr<RMContainer>>()){
Contract_Guarantee::Contract_Guarantee(RuntimeManager *rm) : rm(rm) {

    // TODO Auto-generated constructor stub
    initContractList(rm);
}

Contract_Guarantee::~Contract_Guarantee() {
    // TODO Auto-generated destructor stub
}

// This is for checking TODO will extend later
void Contract_Guarantee::evaluate(RM::RMLog_Own &state) {
    // TRYING WITH new RMContainer
    for(auto start = (state.contracts)->begin(); start != (state.contracts)->end(); ++start) {
        if((*start)->isChanged()) {
//        std::cout << (*start)->isChanged() << std::endl;
            if(std::shared_ptr<WIFIContract> contract = std::dynamic_pointer_cast<WIFIContract>(*start)) {
                // Sanity check
                ASSERT(contract->getContractType() == CONTRACT_TYPE::WIFI);
                // get the all contract-guarantee list from rmcg for CONTRACT_TYPE::WIFI
                auto match = rmcg.find(CONTRACT_TYPE::WIFI);
                if(match != rmcg.end()){
                    // We found a list of contract-guarantee for WIFI contract type
                    auto matchedCGContainer = std::static_pointer_cast<RMCGContainer<WIFIContract, Guarantees>>(match->second);

                    matchedCGContainer->provideGuarantee(contract);
                }
                continue;
            }
            // TODO check in other Contract_Gurantee list
        } else {
            // TODO If state does not change we need to check whether the current state is fulfilling the guarantees
            // We need to log in case of violation in a txt file that contains all simulation parameters
            // min safety distance
            bool distViolated = false;
            bool decelViolated = false;
            if(state.dist2pred < rm->rmParam.minSafetyDistance && rm->positionHelper->getId() != 0) {
                distViolated = true;
#if DEBUG_RM
               std::cerr << "Vehicle : " << rm->positionHelper->getId() << " :: "
                         << state.dist2pred << "<" << rm->rmParam.minSafetyDistance
                         << " ===> MinSafetyDistance violated!!!" << std::endl;
#endif
            }
            // maximum deceleration
//            if((state.maxDeceleration > -5.0 || state.maxDeceleration < -8.0) && rm->positionHelper->getId() != 0) {
//                std::cerr << "Vehicle : " << rm->positionHelper->getId()
//                                         << "state.maxDeceleration : " << state.maxDeceleration
//                                         << " ===> Deceleration condition violated!!!" << std::endl;
//            }

            if(state.maxDeceleration < -8.0 && rm->positionHelper->getId() != 0) {
                decelViolated = true;
#if DEBUG_RM
                std::cerr << "Vehicle : " << rm->positionHelper->getId() << " :: "
                                         << "state.maxDeceleration : " << state.maxDeceleration
                                         << " ===> Deceleration condition violated!!!" << std::endl;
#endif
            }
            // Report to the output file
            if(distViolated || decelViolated) {
                (rm->fileWriter)->addEntries(rm->rmParam, state, distViolated, decelViolated);
            }

        }


    }
}


template <typename C, typename G> void Contract_Guarantee::addCG(const C &c, const G *g) {
    auto cgList = rmcg.find(c.getContractType());
    if(cgList != rmcg.end()) {
        // CG list for this contractType already created
        if(c.getContractType() == CONTRACT_TYPE::WIFI) {
            // add the new element to the match
            (std::static_pointer_cast<RMCGContainer<WIFIContract, Guarantees>>(cgList->second))->addCG(c, g);
        } else if(c.getContractType() == CONTRACT_TYPE::INTERNAL_ERROR) {
            std::cout<<"Only CONTRACT_TYPE::WIFI is available right now..." << std::endl;
        }
        // TODO add for other contract type
    } else {
        // CG list for this contractType not exists, needs to be created
        if(c.getContractType() == CONTRACT_TYPE::WIFI) {
            rmcg.insert(std::make_pair(CONTRACT_TYPE::WIFI, std::make_shared<RMCGContainer<WIFIContract, Guarantees>>(c, g, CONTRACT_TYPE::WIFI)));
        } else if(c.getContractType() == CONTRACT_TYPE::INTERNAL_ERROR) {
            std::cout<<"Only CONTRACT_TYPE::WIFI is available right now..." << std::endl;
        }
    }
}


void Contract_Guarantee::initContractList(RuntimeManager *rm) {

    // TODO all dynamically allocated object should use smart pointer rather that built-in pointer :: Later

    // Creating the WIFIContract-Guarantee list
    // =============================================== StateParameters ================================================
    // StateParameters C2F : atSafetyDistance = true (default)
    C2F ok_c2f(WIFI_QUALITY::OK);
//    C2F moderate_c2f(WIFI_QUALITY::MODERATE);
    C2F poor_c2f(WIFI_QUALITY::POOR);
    C2F critical_c2f(WIFI_QUALITY::CRITICAL);
    // StateParameters C2L
    C2L ok_c2l(WIFI_QUALITY::OK);
//    C2L moderate_c2l(WIFI_QUALITY::MODERATE);
    C2L poor_c2l(WIFI_QUALITY::POOR);
    C2L critical_c2l(WIFI_QUALITY::CRITICAL);

    // ================================================== Guarantees ==================================================
    // Guarantees (ChangeController)
    Guarantees *g2acc   = new ChangeController(rm, Plexe::ACTIVE_CONTROLLER::ACC);
    Guarantees *g2ploeg = new ChangeController(rm, Plexe::ACTIVE_CONTROLLER::PLOEG);
    Guarantees *g2cacc  = new ChangeController(rm, Plexe::ACTIVE_CONTROLLER::CACC);

    // TODO Guarantees (Gap2Front)
    Guarantees *g2d_df = new AdjustGap2Front(rm, GAP2FRONT::DEFAULT);
    Guarantees *g2d_i  = new AdjustGap2Front(rm, GAP2FRONT::INCREASE);

    Guarantees *g2d_adj  = new AdjustGap2Front(rm, GAP2FRONT::ADJUST);  // TODO need to remove this



    // TODO Guarantees (ChangeControllerAndDecelerate)
    Guarantees *g2ploegN2d_i = new ChangeControllerAndAdjustGap2Front(rm, Plexe::ACTIVE_CONTROLLER::PLOEG, GAP2FRONT::INCREASE);
    Guarantees *g2caccN2d_i  = new ChangeControllerAndAdjustGap2Front(rm, Plexe::ACTIVE_CONTROLLER::CACC, GAP2FRONT::INCREASE);




    // ======================================== WIFIContract for ACC controller ========================================
    // Upgrade
    WIFIContract acc2cacc(CONTRACT_TYPE::WIFI, Plexe::ACTIVE_CONTROLLER::ACC, ok_c2f, ok_c2l);
    WIFIContract acc2ploeg(CONTRACT_TYPE::WIFI, Plexe::ACTIVE_CONTROLLER::ACC, ok_c2f, critical_c2l);
    // degrade
    // ChangeController and Gap2Front
    WIFIContract acc2ploegN2d1(CONTRACT_TYPE::WIFI, Plexe::ACTIVE_CONTROLLER::ACC, poor_c2f, ok_c2l);
    WIFIContract acc2ploegN2d2(CONTRACT_TYPE::WIFI, Plexe::ACTIVE_CONTROLLER::ACC, poor_c2f, poor_c2l);
    WIFIContract acc2ploegN2d3(CONTRACT_TYPE::WIFI, Plexe::ACTIVE_CONTROLLER::ACC, poor_c2f, critical_c2l);

    WIFIContract acc2caccN2d1(CONTRACT_TYPE::WIFI, Plexe::ACTIVE_CONTROLLER::ACC, ok_c2f, poor_c2l);

    // default


    // ======================================== WIFIContract for PLOEG controller =======================================
    // Upgrade
    WIFIContract ploeg2cacc(CONTRACT_TYPE::WIFI, Plexe::ACTIVE_CONTROLLER::PLOEG, ok_c2f, ok_c2l);
    // degrade
    WIFIContract ploeg2acc1(CONTRACT_TYPE::WIFI, Plexe::ACTIVE_CONTROLLER::PLOEG, critical_c2f, ok_c2l);
    WIFIContract ploeg2acc2(CONTRACT_TYPE::WIFI, Plexe::ACTIVE_CONTROLLER::PLOEG, critical_c2f, poor_c2l);
    WIFIContract ploeg2acc3(CONTRACT_TYPE::WIFI, Plexe::ACTIVE_CONTROLLER::PLOEG, critical_c2f, critical_c2l);
    // Gap2Front
    WIFIContract ploeg2d1(CONTRACT_TYPE::WIFI, Plexe::ACTIVE_CONTROLLER::PLOEG, poor_c2f, ok_c2l);
    WIFIContract ploeg2d2(CONTRACT_TYPE::WIFI, Plexe::ACTIVE_CONTROLLER::PLOEG, poor_c2f, poor_c2l);
    WIFIContract ploeg2d3(CONTRACT_TYPE::WIFI, Plexe::ACTIVE_CONTROLLER::PLOEG, poor_c2f, critical_c2l);

    // default Gap2Front
    WIFIContract ploeg2d_default(CONTRACT_TYPE::WIFI, Plexe::ACTIVE_CONTROLLER::PLOEG, ok_c2f, critical_c2l);
    // ChangeController and Gap2Front
    WIFIContract ploeg2caccN2d1(CONTRACT_TYPE::WIFI, Plexe::ACTIVE_CONTROLLER::PLOEG, ok_c2f, poor_c2l);


    // ======================================== WIFIContract for CACC controller ========================================
    // Degrade
    WIFIContract cacc2ploeg(CONTRACT_TYPE::WIFI, Plexe::ACTIVE_CONTROLLER::CACC, ok_c2f, critical_c2l);
    // TODO The following three should be combined in one based on WIFI_QUALITY::ALL
    WIFIContract cacc2acc1(CONTRACT_TYPE::WIFI, Plexe::ACTIVE_CONTROLLER::CACC, critical_c2f, ok_c2l);
    WIFIContract cacc2acc2(CONTRACT_TYPE::WIFI, Plexe::ACTIVE_CONTROLLER::CACC, critical_c2f, poor_c2l);
    WIFIContract cacc2acc3(CONTRACT_TYPE::WIFI, Plexe::ACTIVE_CONTROLLER::CACC, critical_c2f, critical_c2l);
    // Gap2Front
    WIFIContract cacc2d1(CONTRACT_TYPE::WIFI, Plexe::ACTIVE_CONTROLLER::CACC, ok_c2f, poor_c2l);

    // default Gap2Front
    WIFIContract cacc2d_default(CONTRACT_TYPE::WIFI, Plexe::ACTIVE_CONTROLLER::CACC, ok_c2f, ok_c2l);
    // ChangeController and Gap2Front
    WIFIContract cacc2ploegN2d1(CONTRACT_TYPE::WIFI, Plexe::ACTIVE_CONTROLLER::CACC, poor_c2f, ok_c2l);
    WIFIContract cacc2ploegN2d2(CONTRACT_TYPE::WIFI, Plexe::ACTIVE_CONTROLLER::CACC, poor_c2f, poor_c2l);
    WIFIContract cacc2ploegN2d3(CONTRACT_TYPE::WIFI, Plexe::ACTIVE_CONTROLLER::CACC, poor_c2f, critical_c2l);



    // ===================================================================================================================
    // RMCGContainer
    // ===================================================================================================================

    // upgrade
    if(rm->rmParam.upgradationEnabled) {
        // ==================== acc ====================
        // acc -> ploeg
        addCG(acc2ploeg, g2ploeg);
        // acc->cacc
        addCG(acc2cacc, g2cacc);
        // =================== ploeg ===================
        // ploeg->cacc
        addCG(ploeg2cacc, g2cacc);
    }
    // degrade
    if(rm->rmParam.degradationEnabled) {
        // =================== ploeg ===================
        // ploeg->acc
        addCG(ploeg2acc1, g2acc);
        addCG(ploeg2acc2, g2acc);
        addCG(ploeg2acc3, g2acc);
        // ===================  cacc ===================
        // cacc->ploeg
        addCG(cacc2ploeg, g2ploeg);
        // cacc->acc
        addCG(cacc2acc1, g2acc);
        addCG(cacc2acc2, g2acc);
        addCG(cacc2acc3, g2acc);
    }

    // gapcontrol
    if(rm->rmParam.gapControlEnabled) {
        // ==================== acc ====================
        // acc->ploeg->gapControl
        addCG(acc2ploegN2d1, g2ploegN2d_i);
        addCG(acc2ploegN2d2, g2ploegN2d_i);
        addCG(acc2ploegN2d3, g2ploegN2d_i);

        // acc->cacc->gapControl
        addCG(acc2caccN2d1, g2caccN2d_i);

        // =================== ploeg ===================
        // ploeg->gapControl(Default)
        addCG(ploeg2d_default, g2d_df);

        // ploeg->gapControl
        addCG(ploeg2d1, g2d_i);
        addCG(ploeg2d2, g2d_i);
        addCG(ploeg2d3, g2d_i);

        // ploeg->cacc->gapControl
        addCG(ploeg2caccN2d1, g2caccN2d_i);

        // ===================  cacc ===================
        // cacc->gapControl(Default)
        addCG(cacc2d_default, g2d_df);

        // cacc->gapControl
        addCG(cacc2d1, g2d_i);

        // cacc->ploeg->gapControl
        addCG(cacc2ploegN2d1,g2ploegN2d_i);
        addCG(cacc2ploegN2d2,g2ploegN2d_i);
        addCG(cacc2ploegN2d3,g2ploegN2d_i);
    }


    // ====================================================== [ Debug ======================================================

    // ============== Test: CHECKING WITH ADDING DUPLICATE element
//    addCG(acc2cacc, g2cacc);     // test OK


    // ============== Test: try with add different type of contract: Test OK
//    WIFIContract acc2caccC(CONTRACT_TYPE::INTERNAL_ERROR, Plexe::ACTIVE_CONTROLLER::ACC, ok_c2f, ok_c2l);
//    Guarantees g2accC(rm, true, Plexe::ACTIVE_CONTROLLER::ACC);
//    addCG(acc2caccC, g2accC);




//    auto sz = ((static_cast<RMCGContainer<WIFIContract, Guarantees> *>(rmcg->find(CONTRACT_TYPE::WIFI)->second))->cgs)->size();
//    auto sz2 = rmcg->size();
//
//
//    auto cc = ((static_cast<RMCGContainer<WIFIContract, Guarantees> *>(rmcg->find(CONTRACT_TYPE::WIFI)->second))->cgs)->find(cacc2d1);
//    std::cout << cc->first <<std::endl;
    // ====================================================== Debug ] =======================================================

}

