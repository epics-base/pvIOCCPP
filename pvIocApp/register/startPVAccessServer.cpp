/*startPVAccessServer.cpp */

/* Author: Matej Sekoranja */

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
#include <thread.h>
#include <pvAccess.h>
#include <serverContext.h>

#include "pvDatabase.h"
#include "v3Channel.h"

using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvIOC;

class MyRun : public Runnable {
public:
    MyRun();
    ~MyRun();
    virtual void run();
private:
    Thread *thread;
    ServerContextImpl ctx;
    Event event;
};

MyRun::MyRun()
: thread(new Thread(String("pvAccessServer"),lowerPriority,this))
{}

MyRun::~MyRun()
{
    ctx.shutdown();
    // we need thead.waitForCompletion()
    event.wait();
    epicsThreadSleep(1.0);
    delete thread;
}

void MyRun::run()
{
    V3ChannelProvider &channelProvider = V3ChannelProvider::getChannelProvider();
    registerChannelProvider(&channelProvider);

    ctx.setChannelProviderName(channelProvider.getProviderName());
    ctx.initialize(getChannelAccess());
    ctx.printInfo();
    ctx.run(0);
    ctx.destroy();
    event.signal();
}

static MyRun *myRun = 0;

static const iocshFuncDef startPVAccessServerFuncDef = {
    "startPVAccessServer", 0, 0};
static void startPVAccessServerCallFunc(const iocshArgBuf *args)
{
    printf("startPVAccessServer\n");
    if(myRun!=0) {
        printf("server already started\n");
        return;
    }
    myRun = new MyRun();
}

static const iocshFuncDef stopPVAccessServerFuncDef = {
    "stopPVAccessServer", 0, 0};
static void stopPVAccessServerCallFunc(const iocshArgBuf *args)
{
    printf("stopPVAccessServer\n");
   if(myRun!=0) delete myRun;
   myRun = 0; 
}

static void startPVAccessServerRegister(void)
{
    static int firstTime = 1;
    if (firstTime) {
        firstTime = 0;
        iocshRegister(&startPVAccessServerFuncDef, startPVAccessServerCallFunc);
    }
}

static void stopPVAccessServerRegister(void)
{
    static int firstTime = 1;
    if (firstTime) {
        firstTime = 0;
        iocshRegister(&stopPVAccessServerFuncDef, stopPVAccessServerCallFunc);
    }
}

epicsExportRegistrar(startPVAccessServerRegister);
epicsExportRegistrar(stopPVAccessServerRegister);
