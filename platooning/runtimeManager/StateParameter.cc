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
#include "StateParameter.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// StateParameter's Member function's implementation
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
StateParameter::StateParameter() {
    // TODO Auto-generated constructor stub

}

StateParameter::~StateParameter() {
    // TODO Auto-generated destructor stub
}

void StateParameter::evaluate(const RMParameters &rmParam, rm_log &rmLog, const bool onPlatoonBeacon, const int index) {
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
C2X::C2X(Role role) : quality(Quality::CRITICAL), role(role) {

}

C2X::C2X(Quality quality, Role role) : quality(quality), role(role) {

}

void C2X::evaluate(const RMParameters &rmParam, rm_log &rmLog, const bool onPlatoonBeacon, const int index) {
    std::cerr << "Error: " << __FILE__
              << "\n\tLine: " << __LINE__
              << "\n\tCompiled on: " << __DATE__
              << " at " << __TIME__
              << "\n\tfunction " << __func__
              << " Implemenation started...!!!"
              << std::endl;
    if(onPlatoonBeacon) {
        // This is related to upgrade the connection
        // If connection is ok, nothing to do

        // We need to set the quality to ok for degradaton to work

        // [debug checking
//        Quality check = (Quality)((int)Quality::CRITICAL + 1);
//        int x = 0;
        // debug ]
    } else {
        // This is called during monitoring from self message

        auto &ego   = std::get<0>(rmLog);

        // DON'T NEED THIS CASTING. AS C2X IS HERE !!!!!!!?????????????!!!!!!!!!!!!!
        StateParameter *stateParameter = dynamic_cast<C2X *>((*ego.stateParameters)[index - 1]);


        // We need dynamic cast as StateParameters can be of different type
        if(C2X *c2x = dynamic_cast<C2X *>(stateParameter)) {
            if(c2x->quality == Quality::CRITICAL) return;       // There is now way of degradation anymore

            if (index == 1) {
                auto &other = std::get<1>(rmLog);


            } else if (index == 2) {
                auto &other = std::get<2>(rmLog);

            } else {
                std::cerr << "Error: " << __FILE__
                          << "\n\tLine: " << __LINE__
                          << "\n\tCompiled on: " << __DATE__
                          << " at " << __TIME__
                          << "\n\tfunction " << __func__
                          << " wrong vehicle index in rmLog"
                          << std::endl;
            }
        }
        // TODO Add code for other stateParameter



        // need to reset the received beacon to 0 once we are here from monitor/self-message
    }
}
