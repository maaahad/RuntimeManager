//
// Generated file, do not edit! Created by nedtool 5.4 from veins/modules/application/platooning/messages/UpdatePlatoonFormationAck.msg.
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
#include "UpdatePlatoonFormationAck_m.h"

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

Register_Class(UpdatePlatoonFormationAck)

UpdatePlatoonFormationAck::UpdatePlatoonFormationAck(const char *name, short kind) : ::ManeuverMessage(name,kind)
{
    this->platoonSpeed = 0;
    this->platoonLane = 0;
    platoonFormation_arraysize = 0;
    this->platoonFormation = 0;
}

UpdatePlatoonFormationAck::UpdatePlatoonFormationAck(const UpdatePlatoonFormationAck& other) : ::ManeuverMessage(other)
{
    platoonFormation_arraysize = 0;
    this->platoonFormation = 0;
    copy(other);
}

UpdatePlatoonFormationAck::~UpdatePlatoonFormationAck()
{
    delete [] this->platoonFormation;
}

UpdatePlatoonFormationAck& UpdatePlatoonFormationAck::operator=(const UpdatePlatoonFormationAck& other)
{
    if (this==&other) return *this;
    ::ManeuverMessage::operator=(other);
    copy(other);
    return *this;
}

void UpdatePlatoonFormationAck::copy(const UpdatePlatoonFormationAck& other)
{
    this->platoonSpeed = other.platoonSpeed;
    this->platoonLane = other.platoonLane;
    delete [] this->platoonFormation;
    this->platoonFormation = (other.platoonFormation_arraysize==0) ? nullptr : new int[other.platoonFormation_arraysize];
    platoonFormation_arraysize = other.platoonFormation_arraysize;
    for (unsigned int i=0; i<platoonFormation_arraysize; i++)
        this->platoonFormation[i] = other.platoonFormation[i];
}

void UpdatePlatoonFormationAck::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::ManeuverMessage::parsimPack(b);
    doParsimPacking(b,this->platoonSpeed);
    doParsimPacking(b,this->platoonLane);
    b->pack(platoonFormation_arraysize);
    doParsimArrayPacking(b,this->platoonFormation,platoonFormation_arraysize);
}

void UpdatePlatoonFormationAck::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::ManeuverMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->platoonSpeed);
    doParsimUnpacking(b,this->platoonLane);
    delete [] this->platoonFormation;
    b->unpack(platoonFormation_arraysize);
    if (platoonFormation_arraysize==0) {
        this->platoonFormation = 0;
    } else {
        this->platoonFormation = new int[platoonFormation_arraysize];
        doParsimArrayUnpacking(b,this->platoonFormation,platoonFormation_arraysize);
    }
}

double UpdatePlatoonFormationAck::getPlatoonSpeed() const
{
    return this->platoonSpeed;
}

void UpdatePlatoonFormationAck::setPlatoonSpeed(double platoonSpeed)
{
    this->platoonSpeed = platoonSpeed;
}

int UpdatePlatoonFormationAck::getPlatoonLane() const
{
    return this->platoonLane;
}

void UpdatePlatoonFormationAck::setPlatoonLane(int platoonLane)
{
    this->platoonLane = platoonLane;
}

void UpdatePlatoonFormationAck::setPlatoonFormationArraySize(unsigned int size)
{
    int *platoonFormation2 = (size==0) ? nullptr : new int[size];
    unsigned int sz = platoonFormation_arraysize < size ? platoonFormation_arraysize : size;
    for (unsigned int i=0; i<sz; i++)
        platoonFormation2[i] = this->platoonFormation[i];
    for (unsigned int i=sz; i<size; i++)
        platoonFormation2[i] = 0;
    platoonFormation_arraysize = size;
    delete [] this->platoonFormation;
    this->platoonFormation = platoonFormation2;
}

unsigned int UpdatePlatoonFormationAck::getPlatoonFormationArraySize() const
{
    return platoonFormation_arraysize;
}

int UpdatePlatoonFormationAck::getPlatoonFormation(unsigned int k) const
{
    if (k>=platoonFormation_arraysize) throw omnetpp::cRuntimeError("Array of size %d indexed by %d", platoonFormation_arraysize, k);
    return this->platoonFormation[k];
}

void UpdatePlatoonFormationAck::setPlatoonFormation(unsigned int k, int platoonFormation)
{
    if (k>=platoonFormation_arraysize) throw omnetpp::cRuntimeError("Array of size %d indexed by %d", platoonFormation_arraysize, k);
    this->platoonFormation[k] = platoonFormation;
}

class UpdatePlatoonFormationAckDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    UpdatePlatoonFormationAckDescriptor();
    virtual ~UpdatePlatoonFormationAckDescriptor();

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

Register_ClassDescriptor(UpdatePlatoonFormationAckDescriptor)

UpdatePlatoonFormationAckDescriptor::UpdatePlatoonFormationAckDescriptor() : omnetpp::cClassDescriptor("UpdatePlatoonFormationAck", "ManeuverMessage")
{
    propertynames = nullptr;
}

UpdatePlatoonFormationAckDescriptor::~UpdatePlatoonFormationAckDescriptor()
{
    delete[] propertynames;
}

bool UpdatePlatoonFormationAckDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<UpdatePlatoonFormationAck *>(obj)!=nullptr;
}

const char **UpdatePlatoonFormationAckDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *UpdatePlatoonFormationAckDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int UpdatePlatoonFormationAckDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 3+basedesc->getFieldCount() : 3;
}

unsigned int UpdatePlatoonFormationAckDescriptor::getFieldTypeFlags(int field) const
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
        FD_ISARRAY | FD_ISEDITABLE,
    };
    return (field>=0 && field<3) ? fieldTypeFlags[field] : 0;
}

const char *UpdatePlatoonFormationAckDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "platoonSpeed",
        "platoonLane",
        "platoonFormation",
    };
    return (field>=0 && field<3) ? fieldNames[field] : nullptr;
}

int UpdatePlatoonFormationAckDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='p' && strcmp(fieldName, "platoonSpeed")==0) return base+0;
    if (fieldName[0]=='p' && strcmp(fieldName, "platoonLane")==0) return base+1;
    if (fieldName[0]=='p' && strcmp(fieldName, "platoonFormation")==0) return base+2;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *UpdatePlatoonFormationAckDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "double",
        "int",
        "int",
    };
    return (field>=0 && field<3) ? fieldTypeStrings[field] : nullptr;
}

const char **UpdatePlatoonFormationAckDescriptor::getFieldPropertyNames(int field) const
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

const char *UpdatePlatoonFormationAckDescriptor::getFieldProperty(int field, const char *propertyname) const
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

int UpdatePlatoonFormationAckDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    UpdatePlatoonFormationAck *pp = (UpdatePlatoonFormationAck *)object; (void)pp;
    switch (field) {
        case 2: return pp->getPlatoonFormationArraySize();
        default: return 0;
    }
}

const char *UpdatePlatoonFormationAckDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    UpdatePlatoonFormationAck *pp = (UpdatePlatoonFormationAck *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string UpdatePlatoonFormationAckDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    UpdatePlatoonFormationAck *pp = (UpdatePlatoonFormationAck *)object; (void)pp;
    switch (field) {
        case 0: return double2string(pp->getPlatoonSpeed());
        case 1: return long2string(pp->getPlatoonLane());
        case 2: return long2string(pp->getPlatoonFormation(i));
        default: return "";
    }
}

bool UpdatePlatoonFormationAckDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    UpdatePlatoonFormationAck *pp = (UpdatePlatoonFormationAck *)object; (void)pp;
    switch (field) {
        case 0: pp->setPlatoonSpeed(string2double(value)); return true;
        case 1: pp->setPlatoonLane(string2long(value)); return true;
        case 2: pp->setPlatoonFormation(i,string2long(value)); return true;
        default: return false;
    }
}

const char *UpdatePlatoonFormationAckDescriptor::getFieldStructName(int field) const
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

void *UpdatePlatoonFormationAckDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    UpdatePlatoonFormationAck *pp = (UpdatePlatoonFormationAck *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}


