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

#ifndef SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_STATEPARAMETER_H_
#define SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_STATEPARAMETER_H_

#include "veins/base/utils/MiXiMDefs.h"         // This is required for SimTime, ASSERT

#include "veins/modules/application/platooning/runtimeManager/RMUtility.h"


enum QUALITY {
    CRITICAL,
    POOR,
    MODERATE,
    OK,
};


class StateParameter {
    friend bool operator==(const StateParameter &sp1, const StateParameter &sp2);
public:
    StateParameter();
    virtual ~StateParameter();
    virtual void evaluate(const RM::RMParameters &rmParam, const RM::rm_log &rmLog, const bool onPlatoonBeacon = false, const int index = -1) = 0;
    virtual bool equal(const StateParameter &stateParameter) const = 0;
};

enum ROLE {
    FRONT,
    LEADER,
};

class C2X : public StateParameter {
private:
    template <typename T> void c2xQualityCheck(const RM::RMParameters &rmParam, const T &other);
public:
    C2X(ROLE role);
    C2X(QUALITY quality);
    C2X(QUALITY quality, ROLE role);
    virtual void evaluate(const RM::RMParameters &rmParam, const RM::rm_log &rmLog, const bool onPlatoonBeacon = false, const int index = -1) override;
    virtual bool equal(const StateParameter &stateParameter) const override;

protected:
    QUALITY quality;
    ROLE role;

    friend std::ostream &operator<<(std::ostream &os, const C2X &c2x);
    friend std::hash<C2X>;
};

// template specialization for hash<C2X>
// TODO DONOT FORGET ABOOUT BASE CLASS
namespace std {
template <> struct hash<C2X> {
  using result_type = size_t;
  using argument_type = C2X;
  result_type operator()(const argument_type &c2x) const {
      return std::hash<std::underlying_type<QUALITY>::type>()(c2x.quality) ^
             std::hash<std::underlying_type<ROLE>::type>()(c2x.role);
  }
};
}


/**
 * Subclass of C2X
 */
class C2F : public C2X {
public:
    C2F(QUALITY quality, bool atSafeDistance) : C2X(quality), atSafeDistance(atSafeDistance) {

    }
    virtual bool equal(const StateParameter &stateParameter) const override;
private:
    bool atSafeDistance;

    friend std::hash<C2F>;
};

// template specialization for hash<C2F>

namespace std{
template <> struct hash<C2F> {
  using return_type   = size_t;
  using argument_type = C2F;
  return_type operator()(const argument_type &c2f) const {
      return std::hash<std::underlying_type<QUALITY>::type>()(c2f.quality) ^
             std::hash<bool>()(c2f.atSafeDistance);
  }
};

}

/**
 * Subclass of C2X
 */
class C2L : public C2X {
public:
    C2L(QUALITY quality) : C2X(quality) {

    }
    virtual bool equal(const StateParameter &stateParameter) const override;

    friend std::hash<C2L>;
};

// Template specialization for hash<C2L>
namespace std {
template <> struct hash<C2L> {
  using return_type   = size_t;
  using argument_type = C2L;
  return_type operator()(const C2L &c2l) const {
      return std::hash<std::underlying_type<QUALITY>::type>()(c2l.quality);
  }
};
}

#endif /* SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_STATEPARAMETER_H_ */
