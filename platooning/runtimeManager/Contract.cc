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
#include "Contract.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Contract's Member function's implementation
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Contract::Contract() {
    // TODO Auto-generated constructor stub

}

Contract::~Contract() {
    // TODO Auto-generated destructor stub
}

void Contract::evaluate() {
    std::cerr << "Error: " << __FILE__
              << "\n\tLine: " << __LINE__
              << "\n\tCompiled on: " << __DATE__
              << " at " << __TIME__
              << "\n\tfunction " << __func__
              << " must have to be overriden by the derived class!!!"
              << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// C2X's Member function's implementation
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
C2X::C2X() {

}

C2X::~C2X() {

}

void C2X::evaluate() {
    std::cout << "Warning: " << __FILE__
              << "\n\tLine: " << __LINE__
              << "\n\tCompiled on: " << __DATE__
              << " at " << __TIME__
              << "\n\tfunction " << __func__
              << " not implemented yet!!!"
              << std::endl;
}



