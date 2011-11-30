/* pvServerBase.h */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvDataCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
#ifndef PVSERVERBASE_H
#define PVSERVERBASE_H
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

namespace epics { namespace pvIOC { 

class PVServiceBaseProvider;
class PVServiceBase;
class PVServiceBaseProcess;
class PVServiceBaseGet;
class PVServiceBasePut;
class PVServiceBasePutGet;
class PVServiceBaseMonitor;
class PVServiceBaseRPC;
class PVServiceBaseArray;



/* PVServiceBase is a base class for implementing pvAccess services.
 * A service must implement Channel.
 * PVServiceBase implements all Channel methods.
 * A service need only implement the methods it supports and lets
 * the base class handle the others.
 */

class PVServiceBase :
  public epics::pvAccess::Channel,
  public std::tr1::enable_shared_from_this<PVServiceBase>
{
public:
    POINTER_DEFINITIONS(PVServiceBase);
    PVServiceBase(
        std::tr1::shared_ptr<PVServiceBaseProvider> const &channelProvider,
        epics::pvAccess::ChannelRequester::shared_pointer const & requester,
        epics::pvData::String channelName
    );
    virtual ~PVServiceBase();
    virtual void destroy();
    virtual epics::pvData::String getRequesterName();
    virtual void message(
        epics::pvData::String message,
        epics::pvData::MessageType messageType);
    virtual epics::pvAccess::ChannelProvider::shared_pointer const & getProvider();
    virtual epics::pvData::String getRemoteAddress();
    virtual epics::pvAccess::Channel::ConnectionState getConnectionState();
    virtual epics::pvData::String getChannelName();
    virtual epics::pvAccess::ChannelRequester::shared_pointer const &
       getChannelRequester();
    virtual bool isConnected();
    virtual void getField(
        epics::pvAccess::GetFieldRequester::shared_pointer const &requester,
        epics::pvData::String subField);
    virtual epics::pvAccess::AccessRights getAccessRights(
        epics::pvData::PVField::shared_pointer const &pvField);
    virtual epics::pvAccess::ChannelProcess::shared_pointer createChannelProcess(
        epics::pvAccess::ChannelProcessRequester::shared_pointer const &requester,
        epics::pvData::PVStructure::shared_pointer const &pvRequest);
    virtual epics::pvAccess::ChannelGet::shared_pointer createChannelGet(
        epics::pvAccess::ChannelGetRequester::shared_pointer const &requester,
        epics::pvData::PVStructure::shared_pointer const &pvRequest);
    virtual epics::pvAccess::ChannelPut::shared_pointer createChannelPut(
        epics::pvAccess::ChannelPutRequester::shared_pointer const &requester,
        epics::pvData::PVStructure::shared_pointer const &pvRequest);
    virtual epics::pvAccess::ChannelPutGet::shared_pointer createChannelPutGet(
        epics::pvAccess::ChannelPutGetRequester::shared_pointer const &requester,
        epics::pvData::PVStructure::shared_pointer const &pvRequest);
    virtual epics::pvAccess::ChannelRPC::shared_pointer createChannelRPC(
        epics::pvAccess::ChannelRPCRequester::shared_pointer const &requester,
        epics::pvData::PVStructure::shared_pointer const &pvRequest);
    virtual epics::pvData::Monitor::shared_pointer createMonitor(
        epics::pvData::MonitorRequester::shared_pointer const &requester,
        epics::pvData::PVStructure::shared_pointer const &pvRequest);
    virtual epics::pvAccess::ChannelArray::shared_pointer createChannelArray(
        epics::pvAccess::ChannelArrayRequester::shared_pointer const &requester,
        epics::pvData::PVStructure::shared_pointer const &pvRequest);
    virtual void printInfo();
    virtual void printInfo(epics::pvData::StringBuilder out);
    // following called by derived classes
    void addChannelProcess(epics::pvAccess::ChannelProcess &);
    void addChannelGet(epics::pvAccess::ChannelGet &);
    void addChannelPut(epics::pvAccess::ChannelPut &);
    void addChannelPutGet(epics::pvAccess::ChannelPutGet &);
    void addChannelMonitor(epics::pvData::Monitor &);
    void addChannelRPC(epics::pvAccess::ChannelRPC &);
    void addChannelArray(epics::pvAccess::ChannelArray &);
    void removeChannelProcess(epics::pvAccess::ChannelProcess &);
    void removeChannelGet(epics::pvAccess::ChannelGet &);
    void removeChannelPut(epics::pvAccess::ChannelPut &);
    void removeChannelPutGet(epics::pvAccess::ChannelPutGet &);
    void removeChannelMonitor(epics::pvData::Monitor &);
    void removeChannelRPC(epics::pvAccess::ChannelRPC &);
    void removeChannelArray(epics::pvAccess::ChannelArray &);
protected:
    PVServiceBase::shared_pointer getPtrSelf()
    {
        return shared_from_this();
    }
private:
    epics::pvAccess::ChannelProvider::shared_pointer  provider;
    epics::pvAccess::ChannelRequester::shared_pointer requester;
    epics::pvData::String channelName;
    epics::pvData::LinkedList<epics::pvAccess::ChannelProcess> channelProcessList;
    epics::pvData::LinkedList<epics::pvAccess::ChannelGet> channelGetList;
    epics::pvData::LinkedList<epics::pvAccess::ChannelPut> channelPutList;
    epics::pvData::LinkedList<epics::pvAccess::ChannelPutGet> channelPutGetList;
    epics::pvData::LinkedList<epics::pvData::Monitor> channelMonitorList;
    epics::pvData::LinkedList<epics::pvAccess::ChannelRPC> channelRPCList;
    epics::pvData::LinkedList<epics::pvAccess::ChannelArray> channelArrayList;
    bool beingDestroyed;
    epics::pvData::Mutex mutex;
};

class PVServiceBaseProvider :
    public epics::pvAccess::ChannelProvider
{
public:
    POINTER_DEFINITIONS(PVServiceBaseProvider);
    PVServiceBaseProvider(
        epics::pvData::String providerName
    );
    virtual ~PVServiceBaseProvider();
    virtual epics::pvData::String getProviderName();
    virtual void destroy();
    virtual epics::pvAccess::ChannelFind::shared_pointer channelFind(
        epics::pvData::String channelName,
        epics::pvAccess::ChannelFindRequester::shared_pointer const &
            channelFindRequester) = 0;
    virtual epics::pvAccess::Channel::shared_pointer createChannel(
        epics::pvData::String channelName,
        epics::pvAccess::ChannelRequester::shared_pointer const &requester,
        short priority);
    virtual epics::pvAccess::Channel::shared_pointer createChannel(
        epics::pvData::String channelName,
        epics::pvAccess::ChannelRequester::shared_pointer  const &
             channelRequester,
        short priority,
        epics::pvData::String address) = 0;
    PVServiceBaseProvider::shared_pointer getChannelProvider()
    {
        return channelProviderPtr;
    }
    // following called by derived class
    void channelFound(
        bool found,
        epics::pvAccess::ChannelFindRequester::shared_pointer const & requester);
    void channelNotCreated(
        epics::pvAccess::ChannelRequester::shared_pointer const & requester);
    void channelCreated(PVServiceBase::shared_pointer channel);
    void removeChannel(PVServiceBase &channel);
private:
    epics::pvData::String providerName;
    PVServiceBaseProvider::shared_pointer channelProviderPtr;
    epics::pvData::LinkedList<PVServiceBase> channelList;
    bool beingDestroyed;
    epics::pvData::Mutex mutex;
};

}}

#endif  /* PVSERVERBASE_H */
