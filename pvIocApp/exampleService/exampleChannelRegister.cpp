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
#include <epicsThread.h>
#include <iocsh.h>

#include <epicsExport.h>

#include <pv/pvIntrospect.h>
#include <pv/pvData.h>
#include <pv/pvAccess.h>
#include <pv/serverContext.h>

#include <pv/pvDatabase.h>
#include <pv/exampleService.h>
#include <pv/pvServiceProvider.h>

using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvIOC;

/* define arguments for the following command:
 * startExampleChannel channelName
 */
static const iocshArg startExampleChannelArg0 = {"channelName", iocshArgString};
static const iocshArg *const startExampleChannelArgs[] = {
    &startExampleChannelArg0};
static const iocshFuncDef startExampleChannelFuncDef = {
    "startExampleChannel", 1, startExampleChannelArgs};
/* This is the function that creates examplePVTop
 */
static void startExampleChannel(const iocshArgBuf *args)
{
    // the channel name is the single argument which is a string
    const char * channelName = args[0].sval;
    if(channelName==0 || channelName[0]==0) {
        printf("illegal channelName\n");
        return;
    }
    // Use the standard ChannelProvider for services.
    PVServiceProvider::shared_pointer serviceProvider
         = PVServiceProvider::getPVServiceProvider();

    // create and initialize examplePVTop
    ExamplePVTop::shared_pointer examplePVTop(new ExamplePVTop(channelName));
    examplePVTop->init();
    // add it to the "records" that clients can access via serviceProvider
    serviceProvider->addRecord(examplePVTop);
}

static void startExampleChannelRegister(void)
{
    static int firstTime = 1;
    if (firstTime) {
        firstTime = 0;
        iocshRegister(&startExampleChannelFuncDef, startExampleChannel);
    }
}

epicsExportRegistrar(startExampleChannelRegister);
