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

#ifndef SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_RMCONTAINER_RMCGCONTAINER_H_
#define SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_RMCONTAINER_RMCGCONTAINER_H_

#include <iostream>
#include <unordered_map>
#include <memory>
#include "veins/modules/application/platooning/runtimeManager/rmContainer/RMContainer.h"
#include "veins/modules/application/platooning/runtimeManager/Contract.h"
#include "veins/modules/application/platooning/runtimeManager/contracts/WIFIContract.h"
#include "veins/modules/application/platooning/runtimeManager/Guarantees.h"
#include "veins/modules/application/platooning/runtimeManager/guarantees/ChangeController.h"





template <typename C, typename G> class RMCGContainer;

template <typename C, typename G> std::ostream &operator<<(std::ostream &os, const RMCGContainer<C,G> &container) {
    return os << "Will add later..........";
}

template <typename C, typename G> class RMCGContainer : public RMContainer{
    friend std::ostream &operator<<<C,G>(std::ostream &os, const RMCGContainer<C,G> &container);
public:
    using container_type = std::unordered_map<const C, const G *>;
    RMCGContainer(const C &c, const G *g, CONTRACT_TYPE ctype);
    ~RMCGContainer();
    void addCG(const C &c, const G *g);
    void provideGuarantee(std::shared_ptr<C> c) const;
//private:
    std::shared_ptr<container_type> cgs;
};

template <typename C, typename G> RMCGContainer<C,G>::RMCGContainer(const C &c, const G *g, CONTRACT_TYPE ctype) : RMContainer(ctype) ,
        cgs(std::make_shared<container_type>()){
    cgs->insert(std::make_pair(c,g));
}

template <typename C, typename G> RMCGContainer<C,G>::~RMCGContainer() {
    // TODO need to destroy the dynamically allocated G objects
    // This is not possible, as the lists may contain the same pointer to G object for multiple entries
    // TODO replace with shared pointer,,, It's not possible to delete where they were created, as they were created locally
    // Another way to use the following is to create the pointer to G as a temporary variable in addCG(), on the fly
    // Having an error while deleting the elements

//    for(auto it = cgs->begin(); it != cgs->end(); ++it) {
//        if(it->second) {
//            delete it->second;
//            it->second = nullptr;
//        }
//    }

    // This is to check whether the container is destroyed
    std::cout << "RMCGContainer is destroyed...." << std::endl;

}

template <typename C, typename G> void RMCGContainer<C,G>::addCG(const C &c, const G *g) {
    cgs->insert(std::make_pair(c,g));
}

template <typename C, typename G> void RMCGContainer<C,G>::provideGuarantee(std::shared_ptr<C> c) const {
    auto match = cgs->find(*c);
    if(match != cgs->end()) {
        (*(match->second))(c);
    } else {
        //std::cout << "Not matched contract found. No action needs to be taken...." << std::endl;
    }
}

#endif /* SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_RMCONTAINER_RMCGCONTAINER_H_ */
