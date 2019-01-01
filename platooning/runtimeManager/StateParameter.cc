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



bool StateParameter::equal(const StateParameter &stateParameter) const {
    std::cerr << "Error: " << __FILE__
              << "\n\tLine: " << __LINE__
              << "\n\tCompiled on: " << __DATE__
              << " at " << __TIME__
              << "\n\tfunction " << __func__
              << " This method should never be called!!!"
              << std::endl;
    return true;
}

void StateParameter::evaluate(const RM::RMParameters &rmParam, const RM::rm_log &rmLog, const bool onPlatoonBeacon, const int index) {
    std::cerr << "Error: " << __FILE__
              << "\n\tLine: " << __LINE__
              << "\n\tCompiled on: " << __DATE__
              << " at " << __TIME__
              << "\n\tfunction " << __func__
              << " must have to be overriden by the derived class!!!"
              << std::endl;
}

// Free == operator
bool operator==(const StateParameter &sp1, const StateParameter &sp2) {
    return (typeid(sp1) == typeid(sp2)) && sp1.equal(sp2);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// C2X's Member function's implementation
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
C2X::C2X(ROLE role) : quality(WIFI_QUALITY::CRITICAL), role(role) {

}

C2X::C2X(WIFI_QUALITY quality) : quality(quality){

}

C2X::C2X(WIFI_QUALITY quality, ROLE role) : quality(quality), role(role) {

}

template <typename T> void C2X::c2xQualityCheck(const RM::RMParameters &rmParam, const T &other) {
    // c2xInitiated ensures that we have logged the last received beacon
    if(other.common.c2xInitiated) {
        SimTime currentTime = simTime();
        int nBeaconMiss = (int)((currentTime.dbl() - other.common.lastBeaconArrivalTime) / rmParam.expectedBeconInterval);


//        std::cout << "nBeaconMiss: " << nBeaconMiss << std::endl;


        if(nBeaconMiss >= rmParam.nPacketLossCritical) {
            quality = WIFI_QUALITY::CRITICAL;
        } else if (nBeaconMiss < rmParam.nPacketLossCritical && nBeaconMiss >= rmParam.nPacketLossPoor) {
            quality = WIFI_QUALITY::POOR;
        } else if (nBeaconMiss < rmParam.nPacketLossPoor && nBeaconMiss >= rmParam.nPacketLossModerate) {
            quality = WIFI_QUALITY::MODERATE;
        } else {
            quality = WIFI_QUALITY::OK;
        }

    } else {
        // Sanity check
        ASSERT(quality == WIFI_QUALITY::CRITICAL);
        return;
    }
}

void C2X::evaluate(const RM::RMParameters &rmParam, const RM::rm_log &rmLog, const bool onPlatoonBeacon, const int index) {
//    if(onPlatoonBeacon) {
//        // Right now there is nothing to do here!!!
//    } else {
//        // This is called during monitoring from self message
//        if(role == ROLE::FRONT) {
////            const auto &other = std::get<1>(rmLog);
//            const RM::RMLog_Front &other = std::get<1>(rmLog);
//            c2xQualityCheck(rmParam, other);
//
//            // TODO In case of c2f, we need to check the distance to the front vehicle
//        } else if (role == ROLE::LEADER) {
////            const auto &other = std::get<2>(rmLog);
//            const RM::RMLog_Leader &other = std::get<2>(rmLog);
//            c2xQualityCheck(rmParam, other);
//        } else {
//            std::cerr << "Error: " << __FILE__
//                      << "\n\tLine: " << __LINE__
//                      << "\n\tCompiled on: " << __DATE__
//                      << " at " << __TIME__
//                      << "\n\tfunction " << __func__
//                      << " Wrong vehicle type!!!"
//                      << std::endl;
//        }
//    }

    std::cerr << "Error: " << __FILE__
                          << "\n\tLine: " << __LINE__
                          << "\n\tCompiled on: " << __DATE__
                          << " at " << __TIME__
                          << "\n\tfunction " << __func__
                          << " Should be overriden by the derived class C2F and C2L!!!"
                          << std::endl;
}

// Operator overloaded

bool C2X::equal(const StateParameter &stateParameter) const {
    auto rhs = dynamic_cast<const C2X &>(stateParameter);
    return (quality == rhs.quality) &&
           (role == rhs.role);
}


std::ostream &operator<<(std::ostream &os, const C2X &c2x){
    os << "C2X: \n\tQuality : " << (int)c2x.quality << "\n\trole: " << (int)c2x.role;
    return os;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// C2F's Member function's implementation
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

C2F::C2F(WIFI_QUALITY quality, bool atSafeDistance) : C2X(quality), atSafeDistance(atSafeDistance) {

}


bool C2F::equal(const StateParameter &stateParameter) const {
    auto rhs = dynamic_cast<const C2F &>(stateParameter);
    return (quality == rhs.quality) &&
           (atSafeDistance == rhs.atSafeDistance);
}

void C2F::evaluate(const RM::RMParameters &rmParam, const RM::rm_log &rmLog, const bool onPlatoonBeacon, const int index) {
    if(onPlatoonBeacon) {
        // Right now there is nothing to do here!!!
    } else {
//      const auto &other = std::get<1>(rmLog);
        const RM::RMLog_Front &other = std::get<1>(rmLog);
        c2xQualityCheck(rmParam, other);
        // TODO we need to check the distance to the front vehicle
    }
}

std::ostream &operator<<(std::ostream &os, const C2F &c2f) {
    os << "C2F: \n\t\t\tQuality : " << (int)c2f.quality << "\n\t\t\tatSafeDistance: " << c2f.atSafeDistance;
    return os;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// C2L's Member function's implementation
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

C2L::C2L(WIFI_QUALITY quality) : C2X(quality) {

}

bool C2L::equal(const StateParameter &stateParameter) const {
    auto rhs = dynamic_cast<const C2L &>(stateParameter);
    return (quality == rhs.quality);
}

void C2L::evaluate(const RM::RMParameters &rmParam, const RM::rm_log &rmLog, const bool onPlatoonBeacon, const int index) {
    if(onPlatoonBeacon) {
        // Right now there is nothing to do here!!!
    } else {
//      const auto &other = std::get<2>(rmLog);
        const RM::RMLog_Leader &other = std::get<2>(rmLog);
        c2xQualityCheck(rmParam, other);
    }
}

std::ostream &operator<<(std::ostream &os, const C2L &c2l) {
    os << "C2L: \n\t\t\tQuality : " << (int)c2l.quality;
    return os;
}
