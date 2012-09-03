/* pvServiceProvider.h */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 */
#ifndef PVSERVICEPROVIDER_H
#define PVSERVICEPROVIDER_H
#include <pv/channelBase.h>
#include <pv/thread.h>
#include <pv/event.h>
#include <pv/serverContext.h>

namespace epics { namespace pvIOC { 

class PVServiceProvider;
class ServicePVTop;
class ServicePVTopBase;

typedef std::tr1::shared_ptr<ServicePVTop> ServicePVTopPtr;
typedef std::tr1::shared_ptr<ServicePVTopBase> ServicePVTopBasePtr;
typedef std::set<ServicePVTopBasePtr> ServicePVTopBaseList;

class ServicePVTop 
{
public:
    POINTER_DEFINITIONS(ServicePVTop);
    virtual ~ServicePVTop(){}
    virtual epics::pvData::String getName() = 0;
    virtual epics::pvAccess::ChannelBase::shared_pointer createChannel(
        epics::pvAccess::ChannelRequester::shared_pointer const &requester,
        epics::pvAccess::ChannelProvider::shared_pointer const &provider) = 0;
    virtual void destroy() = 0;
};


class PVServiceProvider :
    public epics::pvAccess::ChannelBaseProvider
{
public:
    POINTER_DEFINITIONS(PVServiceProvider);
    static epics::pvAccess::ChannelBaseProvider::shared_pointer getPVServiceProvider();
    virtual ~PVServiceProvider();
    virtual void destroy();
    virtual epics::pvAccess::ChannelFind::shared_pointer channelFind(
        epics::pvData::String const & channelName,
        epics::pvAccess::ChannelFindRequester::shared_pointer const & channelFindRequester);
    virtual epics::pvAccess::Channel::shared_pointer createChannel(
        epics::pvData::String const & channelName,
        epics::pvAccess::ChannelRequester::shared_pointer  const & channelRequester,
        short priority,
        epics::pvData::String const & address);
    void addRecord(ServicePVTop::shared_pointer const & servicePVTop);
    void removeRecord(ServicePVTop::shared_pointer & servicePVTop);
private:
    PVServiceProvider();
    ServicePVTopBaseList topList;
    epics::pvData::Mutex mutex;
};

class PVServiceChannelCTX :
    public epics::pvData::Runnable,
    public std::tr1::enable_shared_from_this<PVServiceChannelCTX>
{
public:
    POINTER_DEFINITIONS(PVServiceChannelCTX);
    PVServiceChannelCTX();
    virtual ~PVServiceChannelCTX();
    virtual void run();
private:
    shared_pointer getPtrSelf()
    {
        return shared_from_this();
    }
    epics::pvData::Event event;
    epics::pvAccess::ServerContextImpl::shared_pointer ctx;
    epics::pvData::Thread *thread;
};

}}

#endif  /* PVSERVICEPROVIDER_H */
