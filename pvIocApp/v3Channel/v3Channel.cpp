/* v3Channel.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvDataCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
#include <string>
#include <stdexcept>
#include <memory>

#include <pvIntrospect.h>
#include <pvData.h>
#include <noDefaultMethods.h>

#include "support.h"
#include "pvDatabase.h"
#include "v3Channel.h"

namespace epics { namespace pvIOC { 

using namespace epics::pvData;
using namespace epics::pvAccess;

V3Channel::V3Channel(
    V3ChannelProvider &provider,
    ChannelRequester &requester,
    String name,
    std::auto_ptr<DbAddr> addr)
:   provider(provider),
    requester(requester),name(name),
    addr(addr),
    channelProcessList(),
    channelGetList(),
    channelPutList(),
    channelMonitorList(),
    channelArrayList()
{
}

V3Channel::~V3Channel()
{
}

void V3Channel::destroy()
{
    delete this;
}

void V3Channel::removeChannelProcess(ChannelProcessListNode &node)
{
    if(node.isOnList()) channelProcessList.remove(node);
}

void V3Channel::removeChannelGet(ChannelGetListNode &node)
{
    if(node.isOnList()) channelGetList.remove(node);
}

void V3Channel::removeChannelPut(ChannelPutListNode &node)
{
    if(node.isOnList()) channelPutList.remove(node);
}

void V3Channel::removeChannelMonitor(ChannelMonitorListNode &node)
{
    if(node.isOnList()) channelMonitorList.remove(node);
}

void V3Channel::removeChannelArray(ChannelArrayListNode &node)
{
    if(node.isOnList()) channelArrayList.remove(node);
}

String V3Channel::getRequesterName()
{
    return requester.getRequesterName();
}

void V3Channel::message(
        String message,
        MessageType messageType)
{
    requester.message(message,messageType);
}

ChannelProvider *V3Channel::getProvider()
{
    return &provider;
}

String V3Channel::getRemoteAddress()
{
    throw std::logic_error(String("Not Implemented"));
}

Channel::ConnectionState V3Channel::getConnectionState()
{
    return Channel::CONNECTED;
}

String V3Channel::getChannelName()
{
    return name;
}

ChannelRequester *V3Channel::getChannelRequester()
{
    return &requester;
}

bool V3Channel::isConnected()
{
    return true;
}

void V3Channel::getField(GetFieldRequester *requester,
        String subField)
{
    throw std::logic_error(String("Not Implemented"));
}

AccessRights V3Channel::getAccessRights(
        PVField *pvField)
{
    throw std::logic_error(String("Not Implemented"));
}

ChannelProcess *V3Channel::createChannelProcess(
        ChannelProcessRequester *channelProcessRequester,
        PVStructure *pvRequest)
{
    V3ChannelProcess *v3ChannelProcess = new V3ChannelProcess(
        *this,*channelProcessRequester,*(addr.get()));
    ChannelProcessListNode * node = v3ChannelProcess->init();
    if(node!=0) channelProcessList.addTail(*node);
    return v3ChannelProcess;
}

ChannelGet *V3Channel::createChannelGet(
        ChannelGetRequester *channelGetRequester,
        PVStructure *pvRequest)
{
    V3ChannelGet *v3ChannelGet = new V3ChannelGet(
        *this,*channelGetRequester,*(addr.get()));
    ChannelGetListNode * node = v3ChannelGet->init(*pvRequest);
    if(node!=0) channelGetList.addTail(*node);
    return v3ChannelGet;
}

ChannelPut *V3Channel::createChannelPut(
        ChannelPutRequester *channelPutRequester,
        PVStructure *pvRequest)
{
    V3ChannelPut *v3ChannelPut = new V3ChannelPut(
        *this,*channelPutRequester,*(addr.get()));
    ChannelPutListNode * node = v3ChannelPut->init(*pvRequest);
    if(node!=0) channelPutList.addTail(*node);
    return v3ChannelPut;
}

ChannelPutGet *V3Channel::createChannelPutGet(
        ChannelPutGetRequester *channelPutGetRequester,
        PVStructure *pvRequest)
{
    Status status(Status::STATUSTYPE_ERROR,
        String("ChannelPutGet not supported for V3 Records"));
    channelPutGetRequester->channelPutGetConnect(status,0,0,0);
    return 0;
}

ChannelRPC *V3Channel::createChannelRPC(
        ChannelRPCRequester *channelRPCRequester,
        PVStructure *pvRequest)
{
    Status status(Status::STATUSTYPE_ERROR,
        String("ChannelRPC not supported for V3 Records"));
    channelRPCRequester->channelRPCConnect(status,0,0,0);
    return 0;
}

Monitor *V3Channel::createMonitor(
        MonitorRequester *monitorRequester,
        PVStructure *pvRequest)
{
    Status status(Status::STATUSTYPE_ERROR,
        String("ChannelMonitor not implemented for V3 Records"));
    monitorRequester->monitorConnect(status,0,0);
    return 0;
}

ChannelArray *V3Channel::createChannelArray(
        ChannelArrayRequester *channelArrayRequester,
        PVStructure *pvRequest)
{
    Status status(Status::STATUSTYPE_ERROR,
        String("ChannelArray not implemented for V3 Records"));
    channelArrayRequester->channelArrayConnect(status,0,0);
    return 0;
}

void V3Channel::printInfo()
{
    printf("V3Channel provides access to V3 records\n");
}

void V3Channel::printInfo(StringBuilder out)
{
    *out += "V3Channel provides access to V3 records";
}



}}
