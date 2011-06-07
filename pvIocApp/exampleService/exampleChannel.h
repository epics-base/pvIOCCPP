/* exampleChannel.h */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvDataCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
#ifndef EXAMPLE_CHANNEL_H
#define EXAMPLE_CHANNEL_H
#include <string>
#include <cstring>
#include <stdexcept>
#include <memory>

#include <pv/pvServiceBase.h>

namespace epics { namespace pvIOC { 

class ExampleChannelProvider;
class ExampleChannel;
class ExampleChannelGet;
class ExampleChannelPut;

class ExampleChannelProvider : public PVServiceBaseProvider {
public:
    POINTER_DEFINITIONS(ExampleChannelProvider);
    ExampleChannelProvider(epics::pvData::String channelName);
    virtual ~ExampleChannelProvider();
    void init();
    virtual epics::pvAccess::ChannelFind::shared_pointer channelFind(
        epics::pvData::String channelName,
        epics::pvAccess::ChannelFindRequester::shared_pointer const & channelFindRequester);
    virtual epics::pvAccess::Channel::shared_pointer createChannel(
        epics::pvData::String channelName,
        epics::pvAccess::ChannelRequester::shared_pointer  const &channelRequester,
        short priority,
        epics::pvData::String address);
    // following are for example
    epics::pvData::FieldConstPtr getField();
    epics::pvData::PVStructure *createTop();
    void putData(epics::pvData::PVStructure *pvStructure);
    void getData(
        epics::pvData::PVStructure *pvStructure,
        epics::pvData::BitSet *pvBitSet);
private:
    epics::pvData::String channelName;
    epics::pvData::PVStructure::shared_pointer pvTop;
};

class ExampleChannel :
  public virtual PVServiceBase
{
public:
    POINTER_DEFINITIONS(ExampleChannel);
    ExampleChannel(
        PVServiceBaseProvider::shared_pointer const & channelProvider,
        epics::pvAccess::ChannelRequester::shared_pointer const & requester,
        epics::pvData::String name);
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
    ExampleChannelProvider *exampleChannelProvider;
};


class ExampleChannelGet :
  public virtual epics::pvAccess::ChannelGet,
  public std::tr1::enable_shared_from_this<ExampleChannelGet>
{
public:
    POINTER_DEFINITIONS(ExampleChannelGet);
    ExampleChannelGet(
        ExampleChannelProvider *exampleChannelProvider,
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
private:
    ExampleChannelGet::shared_pointer getPtrSelf()
    {
        return shared_from_this();
    }
    ExampleChannelProvider *exampleChannelProvider;
    PVServiceBase::shared_pointer exampleChannel;
    epics::pvAccess::ChannelGetRequester::shared_pointer channelGetRequester;
    epics::pvData::PVStructure::shared_pointer pvTop;
    epics::pvData::BitSet::shared_pointer bitSet;
};

class ExampleChannelPut :
  public virtual epics::pvAccess::ChannelPut,
  public std::tr1::enable_shared_from_this<ExampleChannelPut>
{
public:
    POINTER_DEFINITIONS(ExampleChannelPut);
    ExampleChannelPut(
        ExampleChannelProvider *exampleChannelProvider,
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
private:
    ExampleChannelPut::shared_pointer getPtrSelf()
    {
        return shared_from_this();
    }
    ExampleChannelProvider *exampleChannelProvider;
    PVServiceBase::shared_pointer exampleChannel;
    epics::pvAccess::ChannelPutRequester::shared_pointer channelPutRequester;
    epics::pvData::PVStructure::shared_pointer pvTop;
    epics::pvData::BitSet::shared_pointer bitSet;
};


}}

#endif  /* EXAMPLE_CHANNEL_H */
