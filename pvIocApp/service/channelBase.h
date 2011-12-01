/* channelBase.h */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvDataCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
#ifndef CHANNELBASE_H
#define CHANNELBASE_H
#include <string>
#include <cstring>
#include <stdexcept>
#include <memory>

#include <pv/lock.h>

#include <pv/status.h>
#include <pv/monitor.h>
#include <pv/linkedList.h>
#include <pv/pvIntrospect.h>
#include <pv/pvData.h>
#include <pv/noDefaultMethods.h>
#include <pv/lock.h>
#include <pv/pvAccess.h>

namespace epics { namespace pvAccess { 

class ChannelBaseProvider;

class ChannelBase;
class ChannelBaseProcess;
class ChannelBaseGet;
class ChannelBasePut;
class ChannelBasePutGet;
class ChannelBaseMonitor;
class ChannelBaseRPC;
class ChannelBaseArray;

/* ChannelBase is a base class for implementing Channel
 * ChannelBase implements all Channel methods.
 * A derived class  need only implement the methods it supports and lets
 * the base class handle the others.
 */

class ChannelBase :
  public Channel,
  public std::tr1::enable_shared_from_this<ChannelBase>
{
public:
    POINTER_DEFINITIONS(ChannelBase);
    ChannelBase(
        ChannelProvider::shared_pointer const &channelProvider,
        ChannelRequester::shared_pointer const & requester,
        epics::pvData::String channelName
    );
    virtual ~ChannelBase();
    virtual void destroy();
    virtual epics::pvData::String getRequesterName();
    virtual void message(
        epics::pvData::String message,
        epics::pvData::MessageType messageType);
    virtual ChannelProvider::shared_pointer const & getProvider();
    virtual epics::pvData::String getRemoteAddress();
    virtual Channel::ConnectionState getConnectionState();
    virtual epics::pvData::String getChannelName();
    virtual ChannelRequester::shared_pointer const &
       getChannelRequester();
    virtual bool isConnected();
    virtual void getField(
        GetFieldRequester::shared_pointer const &requester,
        epics::pvData::String subField);
    virtual AccessRights getAccessRights(
        epics::pvData::PVField::shared_pointer const &pvField);
    virtual ChannelProcess::shared_pointer createChannelProcess(
        ChannelProcessRequester::shared_pointer const &requester,
        epics::pvData::PVStructure::shared_pointer const &pvRequest);
    virtual ChannelGet::shared_pointer createChannelGet(
        ChannelGetRequester::shared_pointer const &requester,
        epics::pvData::PVStructure::shared_pointer const &pvRequest);
    virtual ChannelPut::shared_pointer createChannelPut(
        ChannelPutRequester::shared_pointer const &requester,
        epics::pvData::PVStructure::shared_pointer const &pvRequest);
    virtual ChannelPutGet::shared_pointer createChannelPutGet(
        ChannelPutGetRequester::shared_pointer const &requester,
        epics::pvData::PVStructure::shared_pointer const &pvRequest);
    virtual ChannelRPC::shared_pointer createChannelRPC(
        ChannelRPCRequester::shared_pointer const &requester,
        epics::pvData::PVStructure::shared_pointer const &pvRequest);
    virtual epics::pvData::Monitor::shared_pointer createMonitor(
        epics::pvData::MonitorRequester::shared_pointer const &requester,
        epics::pvData::PVStructure::shared_pointer const &pvRequest);
    virtual ChannelArray::shared_pointer createChannelArray(
        ChannelArrayRequester::shared_pointer const &requester,
        epics::pvData::PVStructure::shared_pointer const &pvRequest);
    virtual void printInfo();
    virtual void printInfo(epics::pvData::StringBuilder out);
    // following called by derived classes
    void addChannelProcess(ChannelProcess &);
    void addChannelGet(ChannelGet &);
    void addChannelPut(ChannelPut &);
    void addChannelPutGet(ChannelPutGet &);
    void addChannelMonitor(epics::pvData::Monitor &);
    void addChannelRPC(ChannelRPC &);
    void addChannelArray(ChannelArray &);
    void removeChannelProcess(ChannelProcess &);
    void removeChannelGet(ChannelGet &);
    void removeChannelPut(ChannelPut &);
    void removeChannelPutGet(ChannelPutGet &);
    void removeChannelMonitor(epics::pvData::Monitor &);
    void removeChannelRPC(ChannelRPC &);
    void removeChannelArray(ChannelArray &);
protected:
    ChannelBase::shared_pointer getPtrSelf()
    {
        return shared_from_this();
    }
private:
    ChannelProvider::shared_pointer  provider;
    ChannelRequester::shared_pointer requester;
    epics::pvData::String channelName;
    epics::pvData::LinkedList<ChannelProcess> channelProcessList;
    epics::pvData::LinkedList<ChannelGet> channelGetList;
    epics::pvData::LinkedList<ChannelPut> channelPutList;
    epics::pvData::LinkedList<ChannelPutGet> channelPutGetList;
    epics::pvData::LinkedList<epics::pvData::Monitor> channelMonitorList;
    epics::pvData::LinkedList<ChannelRPC> channelRPCList;
    epics::pvData::LinkedList<ChannelArray> channelArrayList;
    bool beingDestroyed;
    epics::pvData::Mutex mutex;
};

class ChannelBaseProvider :
    public ChannelProvider,
    public std::tr1::enable_shared_from_this<ChannelBaseProvider>
{
public:
    POINTER_DEFINITIONS(ChannelBaseProvider);
    ChannelBaseProvider(
        epics::pvData::String providerName
    );
    void init();
    virtual ~ChannelBaseProvider();
    virtual epics::pvData::String getProviderName();
    virtual void destroy();
    virtual ChannelFind::shared_pointer channelFind(
        epics::pvData::String channelName,
        ChannelFindRequester::shared_pointer const &
            channelFindRequester) = 0;
    virtual Channel::shared_pointer createChannel(
        epics::pvData::String channelName,
        ChannelRequester::shared_pointer const &requester,
        short priority);
    virtual Channel::shared_pointer createChannel(
        epics::pvData::String channelName,
        ChannelRequester::shared_pointer  const &
             channelRequester,
        short priority,
        epics::pvData::String address) = 0;
    // following called by derived class
    void channelFound(
        bool found,
        ChannelFindRequester::shared_pointer const & requester);
    void channelNotCreated(
        ChannelRequester::shared_pointer const & requester);
    void channelCreated(Channel::shared_pointer channel);
    void removeChannel(ChannelBase &channel);
protected:
    ChannelBaseProvider::shared_pointer getPtrSelf()
    {
        return shared_from_this();
    }
private:
    epics::pvData::String providerName;
    epics::pvData::LinkedList<ChannelBase> channelList;
    bool beingDestroyed;
    epics::pvData::Mutex mutex;
};

}}

#endif  /* CHANNELBASE_H */
