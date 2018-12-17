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

void StateParameter::evaluate(const RMParameters &rmParam, const rm_log &rmLog, const bool onPlatoonBeacon, const int index) {
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
C2X::C2X(ROLE role) : quality(QUALITY::CRITICAL), role(role) {

}

C2X::C2X(QUALITY quality, ROLE role) : quality(quality), role(role) {

}

template <typename T> void C2X::c2xQualityCheck(const RMParameters &rmParam, const T &other) {
    // c2xInitiated ensures that we have logged the last received beacon
    if(other.common.c2xInitiated) {
        SimTime currentTime = simTime();
        int nBeaconMiss = (int)((currentTime.dbl() - other.common.lastBeaconArrivalTime) / rmParam.expectedBeconInterval);

        std::cout << "nBeaconMiss: " << nBeaconMiss << std::endl;

        if(nBeaconMiss >= rmParam.nPacketLossCritical) {
            quality = QUALITY::CRITICAL;
        } else if (nBeaconMiss < rmParam.nPacketLossCritical && nBeaconMiss >= rmParam.nPacketLossPoor) {
            quality = QUALITY::POOR;
        } else if (nBeaconMiss < rmParam.nPacketLossPoor && nBeaconMiss >= rmParam.nPacketLossModerate) {
            quality = QUALITY::MODERATE;
        } else {
            quality = QUALITY::OK;
        }

    } else {
        // Sanity check
        ASSERT(quality == QUALITY::CRITICAL);
        return;
    }
}

void C2X::evaluate(const RMParameters &rmParam, const rm_log &rmLog, const bool onPlatoonBeacon, const int index) {
    if(onPlatoonBeacon) {
        // Right now there is nothing to do here!!!
    } else {
        // This is called during monitoring from self message
        if(role == ROLE::FRONT) {
//            const auto &other = std::get<1>(rmLog);
            const RMLog_Front &other = std::get<1>(rmLog);
            c2xQualityCheck(rmParam, other);
        } else if (role == ROLE::LEADER) {
//            const auto &other = std::get<2>(rmLog);
            const RMLog_Leader &other = std::get<2>(rmLog);
            c2xQualityCheck(rmParam, other);
        } else {
            std::cerr << "Error: " << __FILE__
                      << "\n\tLine: " << __LINE__
                      << "\n\tCompiled on: " << __DATE__
                      << " at " << __TIME__
                      << "\n\tfunction " << __func__
                      << " Wrong vehicle type!!!"
                      << std::endl;
        }
    }
}

// Operator overloaded

bool C2X::operator==(const C2X &rhs) const{
    if(quality != rhs.quality) return false;
    if(role != rhs.role) return false;
}

std::ostream &operator<<(std::ostream &os, const C2X &c2x){
    os << "C2X: \n\tQuality : " << (int)c2x.quality << "\n\trole: " << (int)c2x.role;
    return os;
}
