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
#include "WIFIContract.h"

WIFIContract::WIFIContract(CONTRACT_TYPE type, Plexe::ACTIVE_CONTROLLER controller, C2X c2f, C2X c2l) : Contract(type, controller), c2f(c2f), c2l(c2l) {
    // TODO Auto-generated constructor stub

}

WIFIContract::~WIFIContract() {
    // TODO Auto-generated destructor stub
}

void WIFIContract::evaluate(const RMParameters &rmParam, const rm_log &rmLog, const bool onPlatoonBeacon, const int index) {
    c2f.evaluate(rmParam, rmLog, onPlatoonBeacon, index);
    c2l.evaluate(rmParam, rmLog, onPlatoonBeacon, index);

    // [ debug
    std::cout << c2f << std::endl;
    std::cout << c2l << std::endl;
    // debug ]
}

