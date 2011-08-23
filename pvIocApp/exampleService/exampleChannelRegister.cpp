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
#include <pv/pvServiceProvider.h>

using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvIOC;

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
    PVServiceProvider::shared_pointer serviceProvider
         = PVServiceProvider::getPVServiceProvider();
    ExamplePVTop::shared_pointer examplePVTop(new ExamplePVTop(channelName));
    examplePVTop->init();
    serviceProvider->addRecord(examplePVTop);
}

static void startExampleChannelRegister(void)
{
    static int firstTime = 1;
    if (firstTime) {
        firstTime = 0;
        iocshRegister(&startExampleChannelFuncDef, startExampleChannelCallFunc);
    }
}

epicsExportRegistrar(startExampleChannelRegister);
