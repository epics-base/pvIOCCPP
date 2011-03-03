/*testV3Local.cpp */

/* Author: Marty Kraimer */

#include <cstddef>
#include <cstdlib>
#include <cstddef>
#include <string>
#include <cstdio>
#include <memory>

#include <cantProceed.h>
#include <epicsStdio.h>
#include <epicsMutex.h>
#include <epicsEvent.h>
#include <epicsThread.h>
#include <iocsh.h>

#include <epicsExport.h>

#include <pvIntrospect.h>
#include <pvData.h>
#include <pvAccess.h>

#include "pvDatabase.h"
#include "v3Channel.h"

using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvIOC;

class FindRequester : public ChannelFindRequester {
public:
    FindRequester(){}
    virtual ~FindRequester(){}
    virtual void channelFindResult(
        const Status &status,ChannelFind *channelFind,bool wasFound);
};

void FindRequester::channelFindResult(
    const Status &status,ChannelFind *channelFind,bool wasFound)
{
    String message = status.getMessage();
    printf("channelFindResult status %s wasFound %s\n",
        message.c_str(),
        (wasFound ? "true" : "false"));
}

class MyRequester : public ChannelRequester
{
public:
    MyRequester()
    : name(String("testV3Local")),channel(0)
    { }
    Channel *getChannel() {return channel;}
    virtual ~MyRequester() { }
    virtual String getRequesterName()
    {
        return name;
    }
    virtual void message(String message,MessageType messageType)
    {
        String typeName = messageTypeName[messageType];
        printf("ChannelRequester message %s messageType %s\n",
            message.c_str(),typeName.c_str());
    }
    virtual void channelCreated(const epics::pvData::Status &status, Channel *channel)
    {
        this->channel = channel;
        String message = status.getMessage();
        bool isOK = status.isOK();
        printf("channelCreated status %s statusOK %s\n",
        message.c_str(),
        (isOK ? "true" : "false"));
    }
    virtual void channelStateChange(Channel *c, Channel::ConnectionState connectionState)
    {
        String state = Channel::ConnectionStateNames[connectionState];
        printf("channelStateChange %s\n",state.c_str());
    }
private:
    String name;
    Channel *channel;
};

static const iocshArg testArg0 = { "pvName", iocshArgString };
static const iocshArg *testArgs[] = {
    &testArg0};

static const iocshFuncDef testV3LocalFuncDef = {
    "testV3Local", 1, testArgs};
static void testV3LocalCallFunc(const iocshArgBuf *args)
{
    char *pvName = args[0].sval;
    printf("testV3Local pvName %s\n",pvName);
    V3ChannelProvider &channelProvider = V3ChannelProvider::getChannelProvider();
    String providerName = channelProvider.getProviderName();
    printf("providerName %s\n",providerName.c_str());
    std::auto_ptr<FindRequester> findRequester(new FindRequester());
    channelProvider.channelFind(String(pvName),findRequester.get());
    std::auto_ptr<MyRequester> myRequester(new MyRequester());
    Channel *channel = channelProvider.createChannel(String(pvName),myRequester.get(),0,String(""));
    if(channel!=0) {
        channel->destroy();
    }
}

static void testV3LocalRegister(void)
{
    static int firstTime = 1;
    if (firstTime) {
        firstTime = 0;
        iocshRegister(&testV3LocalFuncDef, testV3LocalCallFunc);
    }
}
epicsExportRegistrar(testV3LocalRegister);
