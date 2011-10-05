/* exampleService.h */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvDataCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
#ifndef EXAMPLE_SERVICE_H
#define EXAMPLE_SERVICE_H
#include <string>
#include <cstring>
#include <stdexcept>
#include <memory>

#include <pv/pvServiceProvider.h>

namespace epics { namespace pvIOC { 

class ExamplePVTop;
class ExampleChannel;
class ExampleChannelGet;
class ExampleChannelPut;

class ExamplePVTop :
    public virtual ServicePVTop,
    public std::tr1::enable_shared_from_this<ExamplePVTop>
{
public:
    POINTER_DEFINITIONS(ExamplePVTop);
    ExamplePVTop(epics::pvData::String channelName);
    virtual ~ExamplePVTop();
    virtual epics::pvData::String getName();
    virtual PVServiceBase::shared_pointer createChannel(
        epics::pvAccess::ChannelRequester::shared_pointer const &requester,
        PVServiceProvider::shared_pointer const &provider);
    virtual void destroy();
    //following are for example
    void init();
     epics::pvData::FieldConstPtr getField();
    epics::pvData::PVStructure *createTop();
    void putData(epics::pvData::PVStructure *pvStructure);
    void getData(
        epics::pvData::PVStructure *pvStructure,
        epics::pvData::BitSet *pvBitSet);
private:
    ExamplePVTop::shared_pointer getPtrSelf()
    {
        return shared_from_this();
    }
    epics::pvData::String channelName;
    epics::pvData::PVStructure::shared_pointer pvTop;
    epics::pvData::Mutex mutex;
};

class ExampleChannel :
  public virtual PVServiceBase
{
public:
    POINTER_DEFINITIONS(ExampleChannel);
    ExampleChannel(
        PVServiceBaseProvider::shared_pointer const & channelProvider,
        epics::pvAccess::ChannelRequester::shared_pointer const & requester,
        epics::pvData::String name,
        ExamplePVTop::shared_pointer const &examplePVTop);
    virtual ~ExampleChannel();
    void init();
    virtual void getField(
        epics::pvAccess::GetFieldRequester::shared_pointer const &requester,
        epics::pvData::String subField);
    virtual epics::pvAccess::ChannelGet::shared_pointer createChannelGet(
        epics::pvAccess::ChannelGetRequester::shared_pointer const &channelGetRequester,
        epics::pvData::PVStructure::shared_pointer const &pvRequest);
    virtual epics::pvAccess::ChannelPut::shared_pointer createChannelPut(
        epics::pvAccess::ChannelPutRequester::shared_pointer const &channelPutRequester,
        epics::pvData::PVStructure::shared_pointer const &pvRequest);
    virtual void printInfo();
    virtual void printInfo(epics::pvData::StringBuilder out);
private:
    ExamplePVTop::shared_pointer examplePVTop;
};


class ExampleChannelGet :
  public virtual epics::pvAccess::ChannelGet,
  public std::tr1::enable_shared_from_this<ExampleChannelGet>
{
public:
    POINTER_DEFINITIONS(ExampleChannelGet);
    ExampleChannelGet(
        ExamplePVTop::shared_pointer const & examplePVTop,
        PVServiceBase::shared_pointer const & exampleChannel,
        epics::pvAccess::ChannelGetRequester::shared_pointer const &channelGetRequester);
    virtual ~ExampleChannelGet();
    bool init(epics::pvData::PVStructure::shared_pointer const & pvRequest);
    virtual epics::pvData::String getRequesterName();
    virtual void message(
        epics::pvData::String message,
        epics::pvData::MessageType messageType);
    virtual void destroy();
    virtual void get(bool lastRequest);
    virtual void lock();
    virtual void unlock();
private:
    ExampleChannelGet::shared_pointer getPtrSelf()
    {
        return shared_from_this();
    }
    bool firstTime;
    ExamplePVTop::shared_pointer examplePVTop;
    PVServiceBase::shared_pointer exampleChannel;
    epics::pvAccess::ChannelGetRequester::shared_pointer channelGetRequester;
    epics::pvData::PVStructure::shared_pointer pvTop;
    epics::pvData::BitSet::shared_pointer bitSet;
    epics::pvData::Mutex dataMutex;
};

class ExampleChannelPut :
  public virtual epics::pvAccess::ChannelPut,
  public std::tr1::enable_shared_from_this<ExampleChannelPut>
{
public:
    POINTER_DEFINITIONS(ExampleChannelPut);
    ExampleChannelPut(
        ExamplePVTop::shared_pointer const & examplePVTop,
        PVServiceBase::shared_pointer const & exampleChannel,
        epics::pvAccess::ChannelPutRequester::shared_pointer const & channelPutRequester);
    virtual ~ExampleChannelPut();
    bool init(epics::pvData::PVStructure::shared_pointer const & pvRequest);
    virtual epics::pvData::String getRequesterName();
    virtual void message(
        epics::pvData::String message,
        epics::pvData::MessageType messageType);
    virtual void destroy();
    virtual void put(bool lastRequest);
    virtual void get();
    virtual void lock();
    virtual void unlock();
private:
    ExampleChannelPut::shared_pointer getPtrSelf()
    {
        return shared_from_this();
    }
    ExamplePVTop::shared_pointer examplePVTop;
    PVServiceBase::shared_pointer exampleChannel;
    epics::pvAccess::ChannelPutRequester::shared_pointer channelPutRequester;
    epics::pvData::PVStructure::shared_pointer pvTop;
    epics::pvData::BitSet::shared_pointer bitSet;
    epics::pvData::Mutex dataMutex;
};

}}

#endif  /* EXAMPLE_SERVICE_H */
