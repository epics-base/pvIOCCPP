/*exampleChannelRegister.cpp */

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

#include <pv/pvIntrospect.h>
#include <pv/pvData.h>
#include <pv/pvAccess.h>
#include <pv/serverContext.h>

#include <pv/pvDatabase.h>
#include <pv/exampleChannel.h>

using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvIOC;

class ExampleChannelRun : public Runnable {
public:
    ExampleChannelRun(const char * channelName);
    ~ExampleChannelRun();
    virtual void run();
private:
    String channelName;
    Event event;
    ServerContextImpl::shared_pointer ctx;
    Thread *thread;
};

ExampleChannelRun::ExampleChannelRun(const char * channelName)
: channelName(channelName),
  event(),
  ctx(ServerContextImpl::create()),
  thread(new Thread(String("exampleService"),lowerPriority,this))
{}

ExampleChannelRun::~ExampleChannelRun()
{
    ctx->shutdown();
    // we need thead.waitForCompletion()
    event.wait();
    epicsThreadSleep(1.0);
    delete thread;
}

void ExampleChannelRun::run()
{
    ExampleChannelProvider *exampleChannelProvider
        = new ExampleChannelProvider(channelName);
    ChannelProvider::shared_pointer channelProvider(exampleChannelProvider);
    exampleChannelProvider->init();
    registerChannelProvider(channelProvider);
    ctx->setChannelProviderName(channelProvider->getProviderName());
    ctx->initialize(getChannelAccess());
    ctx->printInfo();
    ctx->run(0);
    ctx->destroy();
    event.signal();
}

static ExampleChannelRun *myRun = 0;

static const iocshArg startExampleChannelArg0 = {"channelName", iocshArgString};
static const iocshArg *const startExampleChannelArgs[] = {
    &startExampleChannelArg0};

static const iocshFuncDef startExampleChannelFuncDef = {
    "startExampleChannel", 1, startExampleChannelArgs};
static void startExampleChannelCallFunc(const iocshArgBuf *args)
{
    const char * channelName = args[0].sval;
    if(channelName==0 || channelName[0]==0) {
        printf("illegal channelName\n");
        return;
    }
    if(myRun!=0) {
        printf("server already started\n");
        return;
    }
    myRun = new ExampleChannelRun(channelName);
}

static const iocshFuncDef stopExampleChannelFuncDef = {
    "stopExampleChannel", 0, 0
};
static void stopExampleChannelCallFunc(const iocshArgBuf *args)
{
   printf("stopPVAccessServer\n");
   if(myRun!=0) delete myRun;
   myRun = 0;
}

static void startExampleChannelRegister(void)
{
    static int firstTime = 1;
    if (firstTime) {
        firstTime = 0;
        iocshRegister(&startExampleChannelFuncDef, startExampleChannelCallFunc);
    }
}

static void stopExampleChannelRegister(void)
{
    static int firstTime = 1;
    if (firstTime) {
        firstTime = 0;
        iocshRegister(&stopExampleChannelFuncDef, stopExampleChannelCallFunc);
    }
}

epicsExportRegistrar(startExampleChannelRegister);
epicsExportRegistrar(stopExampleChannelRegister);
