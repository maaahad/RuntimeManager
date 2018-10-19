//
// Generated file, do not edit! Created by nedtool 5.4 from veins/modules/application/platooning/messages/PlatooningBeacon.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wshadow"
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wc++98-compat"
#  pragma clang diagnostic ignored "-Wunreachable-code-break"
#  pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wshadow"
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#  pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"
#  pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

#include <iostream>
#include <sstream>
#include "PlatooningBeacon_m.h"

namespace omnetpp {

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T,A>& v)
{
    int n = v.size();
    doParsimPacking(buffer, n);
    for (int i = 0; i < n; i++)
        doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T,A>& v)
{
    int n;
    doParsimUnpacking(buffer, n);
    v.resize(n);
    for (int i = 0; i < n; i++)
        doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T,A>& l)
{
    doParsimPacking(buffer, (int)l.size());
    for (typename std::list<T,A>::const_iterator it = l.begin(); it != l.end(); ++it)
        doParsimPacking(buffer, (T&)*it);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::list<T,A>& l)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        l.push_back(T());
        doParsimUnpacking(buffer, l.back());
    }
}

// Packing/unpacking an std::set
template<typename T, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::set<T,Tr,A>& s)
{
    doParsimPacking(buffer, (int)s.size());
    for (typename std::set<T,Tr,A>::const_iterator it = s.begin(); it != s.end(); ++it)
        doParsimPacking(buffer, *it);
}

template<typename T, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::set<T,Tr,A>& s)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        T x;
        doParsimUnpacking(buffer, x);
        s.insert(x);
    }
}

// Packing/unpacking an std::map
template<typename K, typename V, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::map<K,V,Tr,A>& m)
{
    doParsimPacking(buffer, (int)m.size());
    for (typename std::map<K,V,Tr,A>::const_iterator it = m.begin(); it != m.end(); ++it) {
        doParsimPacking(buffer, it->first);
        doParsimPacking(buffer, it->second);
    }
}

template<typename K, typename V, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::map<K,V,Tr,A>& m)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        K k; V v;
        doParsimUnpacking(buffer, k);
        doParsimUnpacking(buffer, v);
        m[k] = v;
    }
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimUnpacking(b, t[i]);
}

// Default rule to prevent compiler from choosing base class' doParsimPacking() function
template<typename T>
void doParsimPacking(omnetpp::cCommBuffer *, const T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp


// forward
template<typename T, typename A>
std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec);

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
inline std::ostream& operator<<(std::ostream& out,const T&) {return out;}

// operator<< for std::vector<T>
template<typename T, typename A>
inline std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec)
{
    out.put('{');
    for(typename std::vector<T,A>::const_iterator it = vec.begin(); it != vec.end(); ++it)
    {
        if (it != vec.begin()) {
            out.put(','); out.put(' ');
        }
        out << *it;
    }
    out.put('}');
    
    char buf[32];
    sprintf(buf, " (size=%u)", (unsigned int)vec.size());
    out.write(buf, strlen(buf));
    return out;
}

Register_Class(PlatooningBeacon)

PlatooningBeacon::PlatooningBeacon(const char *name, short kind) : ::omnetpp::cPacket(name,kind)
{
    this->vehicleId = 0;
    this->relayerId = 0;
    this->controllerAcceleration = 0;
    this->acceleration = 0;
    this->speed = 0;
    this->positionX = 0;
    this->positionY = 0;
    this->time = 0;
    this->sequenceNumber = 0;
    this->length = 0;
    this->speedX = 0;
    this->speedY = 0;
    this->angle = 0;
}

PlatooningBeacon::PlatooningBeacon(const PlatooningBeacon& other) : ::omnetpp::cPacket(other)
{
    copy(other);
}

PlatooningBeacon::~PlatooningBeacon()
{
}

PlatooningBeacon& PlatooningBeacon::operator=(const PlatooningBeacon& other)
{
    if (this==&other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void PlatooningBeacon::copy(const PlatooningBeacon& other)
{
    this->vehicleId = other.vehicleId;
    this->relayerId = other.relayerId;
    this->controllerAcceleration = other.controllerAcceleration;
    this->acceleration = other.acceleration;
    this->speed = other.speed;
    this->positionX = other.positionX;
    this->positionY = other.positionY;
    this->time = other.time;
    this->sequenceNumber = other.sequenceNumber;
    this->length = other.length;
    this->speedX = other.speedX;
    this->speedY = other.speedY;
    this->angle = other.angle;
}

void PlatooningBeacon::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    doParsimPacking(b,this->vehicleId);
    doParsimPacking(b,this->relayerId);
    doParsimPacking(b,this->controllerAcceleration);
    doParsimPacking(b,this->acceleration);
    doParsimPacking(b,this->speed);
    doParsimPacking(b,this->positionX);
    doParsimPacking(b,this->positionY);
    doParsimPacking(b,this->time);
    doParsimPacking(b,this->sequenceNumber);
    doParsimPacking(b,this->length);
    doParsimPacking(b,this->speedX);
    doParsimPacking(b,this->speedY);
    doParsimPacking(b,this->angle);
}

void PlatooningBeacon::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->vehicleId);
    doParsimUnpacking(b,this->relayerId);
    doParsimUnpacking(b,this->controllerAcceleration);
    doParsimUnpacking(b,this->acceleration);
    doParsimUnpacking(b,this->speed);
    doParsimUnpacking(b,this->positionX);
    doParsimUnpacking(b,this->positionY);
    doParsimUnpacking(b,this->time);
    doParsimUnpacking(b,this->sequenceNumber);
    doParsimUnpacking(b,this->length);
    doParsimUnpacking(b,this->speedX);
    doParsimUnpacking(b,this->speedY);
    doParsimUnpacking(b,this->angle);
}

int PlatooningBeacon::getVehicleId() const
{
    return this->vehicleId;
}

void PlatooningBeacon::setVehicleId(int vehicleId)
{
    this->vehicleId = vehicleId;
}

int PlatooningBeacon::getRelayerId() const
{
    return this->relayerId;
}

void PlatooningBeacon::setRelayerId(int relayerId)
{
    this->relayerId = relayerId;
}

double PlatooningBeacon::getControllerAcceleration() const
{
    return this->controllerAcceleration;
}

void PlatooningBeacon::setControllerAcceleration(double controllerAcceleration)
{
    this->controllerAcceleration = controllerAcceleration;
}

double PlatooningBeacon::getAcceleration() const
{
    return this->acceleration;
}

void PlatooningBeacon::setAcceleration(double acceleration)
{
    this->acceleration = acceleration;
}

double PlatooningBeacon::getSpeed() const
{
    return this->speed;
}

void PlatooningBeacon::setSpeed(double speed)
{
    this->speed = speed;
}

double PlatooningBeacon::getPositionX() const
{
    return this->positionX;
}

void PlatooningBeacon::setPositionX(double positionX)
{
    this->positionX = positionX;
}

double PlatooningBeacon::getPositionY() const
{
    return this->positionY;
}

void PlatooningBeacon::setPositionY(double positionY)
{
    this->positionY = positionY;
}

double PlatooningBeacon::getTime() const
{
    return this->time;
}

void PlatooningBeacon::setTime(double time)
{
    this->time = time;
}

int PlatooningBeacon::getSequenceNumber() const
{
    return this->sequenceNumber;
}

void PlatooningBeacon::setSequenceNumber(int sequenceNumber)
{
    this->sequenceNumber = sequenceNumber;
}

double PlatooningBeacon::getLength() const
{
    return this->length;
}

void PlatooningBeacon::setLength(double length)
{
    this->length = length;
}

double PlatooningBeacon::getSpeedX() const
{
    return this->speedX;
}

void PlatooningBeacon::setSpeedX(double speedX)
{
    this->speedX = speedX;
}

double PlatooningBeacon::getSpeedY() const
{
    return this->speedY;
}

void PlatooningBeacon::setSpeedY(double speedY)
{
    this->speedY = speedY;
}

double PlatooningBeacon::getAngle() const
{
    return this->angle;
}

void PlatooningBeacon::setAngle(double angle)
{
    this->angle = angle;
}

class PlatooningBeaconDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    PlatooningBeaconDescriptor();
    virtual ~PlatooningBeaconDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(PlatooningBeaconDescriptor)

PlatooningBeaconDescriptor::PlatooningBeaconDescriptor() : omnetpp::cClassDescriptor("PlatooningBeacon", "omnetpp::cPacket")
{
    propertynames = nullptr;
}

PlatooningBeaconDescriptor::~PlatooningBeaconDescriptor()
{
    delete[] propertynames;
}

bool PlatooningBeaconDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<PlatooningBeacon *>(obj)!=nullptr;
}

const char **PlatooningBeaconDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *PlatooningBeaconDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int PlatooningBeaconDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 13+basedesc->getFieldCount() : 13;
}

unsigned int PlatooningBeaconDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<13) ? fieldTypeFlags[field] : 0;
}

const char *PlatooningBeaconDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "vehicleId",
        "relayerId",
        "controllerAcceleration",
        "acceleration",
        "speed",
        "positionX",
        "positionY",
        "time",
        "sequenceNumber",
        "length",
        "speedX",
        "speedY",
        "angle",
    };
    return (field>=0 && field<13) ? fieldNames[field] : nullptr;
}

int PlatooningBeaconDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='v' && strcmp(fieldName, "vehicleId")==0) return base+0;
    if (fieldName[0]=='r' && strcmp(fieldName, "relayerId")==0) return base+1;
    if (fieldName[0]=='c' && strcmp(fieldName, "controllerAcceleration")==0) return base+2;
    if (fieldName[0]=='a' && strcmp(fieldName, "acceleration")==0) return base+3;
    if (fieldName[0]=='s' && strcmp(fieldName, "speed")==0) return base+4;
    if (fieldName[0]=='p' && strcmp(fieldName, "positionX")==0) return base+5;
    if (fieldName[0]=='p' && strcmp(fieldName, "positionY")==0) return base+6;
    if (fieldName[0]=='t' && strcmp(fieldName, "time")==0) return base+7;
    if (fieldName[0]=='s' && strcmp(fieldName, "sequenceNumber")==0) return base+8;
    if (fieldName[0]=='l' && strcmp(fieldName, "length")==0) return base+9;
    if (fieldName[0]=='s' && strcmp(fieldName, "speedX")==0) return base+10;
    if (fieldName[0]=='s' && strcmp(fieldName, "speedY")==0) return base+11;
    if (fieldName[0]=='a' && strcmp(fieldName, "angle")==0) return base+12;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *PlatooningBeaconDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "int",
        "double",
        "double",
        "double",
        "double",
        "double",
        "double",
        "int",
        "double",
        "double",
        "double",
        "double",
    };
    return (field>=0 && field<13) ? fieldTypeStrings[field] : nullptr;
}

const char **PlatooningBeaconDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *PlatooningBeaconDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int PlatooningBeaconDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    PlatooningBeacon *pp = (PlatooningBeacon *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *PlatooningBeaconDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    PlatooningBeacon *pp = (PlatooningBeacon *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string PlatooningBeaconDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    PlatooningBeacon *pp = (PlatooningBeacon *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getVehicleId());
        case 1: return long2string(pp->getRelayerId());
        case 2: return double2string(pp->getControllerAcceleration());
        case 3: return double2string(pp->getAcceleration());
        case 4: return double2string(pp->getSpeed());
        case 5: return double2string(pp->getPositionX());
        case 6: return double2string(pp->getPositionY());
        case 7: return double2string(pp->getTime());
        case 8: return long2string(pp->getSequenceNumber());
        case 9: return double2string(pp->getLength());
        case 10: return double2string(pp->getSpeedX());
        case 11: return double2string(pp->getSpeedY());
        case 12: return double2string(pp->getAngle());
        default: return "";
    }
}

bool PlatooningBeaconDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    PlatooningBeacon *pp = (PlatooningBeacon *)object; (void)pp;
    switch (field) {
        case 0: pp->setVehicleId(string2long(value)); return true;
        case 1: pp->setRelayerId(string2long(value)); return true;
        case 2: pp->setControllerAcceleration(string2double(value)); return true;
        case 3: pp->setAcceleration(string2double(value)); return true;
        case 4: pp->setSpeed(string2double(value)); return true;
        case 5: pp->setPositionX(string2double(value)); return true;
        case 6: pp->setPositionY(string2double(value)); return true;
        case 7: pp->setTime(string2double(value)); return true;
        case 8: pp->setSequenceNumber(string2long(value)); return true;
        case 9: pp->setLength(string2double(value)); return true;
        case 10: pp->setSpeedX(string2double(value)); return true;
        case 11: pp->setSpeedY(string2double(value)); return true;
        case 12: pp->setAngle(string2double(value)); return true;
        default: return false;
    }
}

const char *PlatooningBeaconDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

void *PlatooningBeaconDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    PlatooningBeacon *pp = (PlatooningBeacon *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}


