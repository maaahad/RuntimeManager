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
#include <iostream>
#include "Contracts.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Contracts's Member function's implementation
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Contracts::Contracts() : contractList(std::make_shared<std::multimap<Plexe::ACTIVE_CONTROLLER,std::vector<StateParameter*>>>()) ,
data(std::make_shared<std::multimap<Plexe::ACTIVE_CONTROLLER, std::pair<std::vector<StateParameter *>, Guarantees>>>()){
    // TODO Auto-generated constructor stub
    initContractList();
}

Contracts::~Contracts() {
    // TODO Auto-generated destructor stub
}

void Contracts::initContractList() {
    std::cerr << "Error: " << __FILE__
              << "\n\tLine: " << __LINE__
              << "\n\tCompiled on: " << __DATE__
              << " at " << __TIME__
              << "\n\tfunction " << __func__
              << " Has not been implemented yet...!!!"
              << std::endl;
    // c2f
    std::vector<StateParameter *> c2f = {new C2X(Quality::CRITICAL, Role::FRONT)};
    std::vector<StateParameter *> c2fc2l = {new C2X(Quality::CRITICAL, Role::FRONT), new C2X(Quality::CRITICAL, Role::FRONT)};


    (*contractList).insert(std::make_pair(Plexe::ACTIVE_CONTROLLER::CACC,c2f));
    (*contractList).insert(std::make_pair(Plexe::ACTIVE_CONTROLLER::CACC,c2fc2l));


    (*contractList).insert(std::make_pair(Plexe::ACTIVE_CONTROLLER::PLOEG,c2f));

    // Checking Replace the previous container by the following container
    (*data).insert(std::make_pair(Plexe::ACTIVE_CONTROLLER::PLOEG,std::make_pair(c2f, Guarantees())));


}

