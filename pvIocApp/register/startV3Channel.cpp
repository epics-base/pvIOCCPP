/*startV3Channel.cpp */

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

static const iocshArg *startArgs[] = {};
static const iocshFuncDef startV3ChannelFuncDef = {
    "startV3Channel", 0, startArgs
};
static void startV3ChannelCallFunc(const iocshArgBuf *args)
{
    V3ChannelProvider::getChannelProvider();
}

static const iocshArg *stopArgs[] = {};
static const iocshFuncDef stopV3ChannelFuncDef = {
    "stopV3Channel", 0, stopArgs
};
static void stopV3ChannelCallFunc(const iocshArgBuf *args)
{
    V3ChannelProvider::getChannelProvider().destroy();
}

static void startV3ChannelRegister(void)
{
    static int firstTime = 1;
    if (firstTime) {
        firstTime = 0;
        iocshRegister(&startV3ChannelFuncDef, startV3ChannelCallFunc);
    }
}

static void stopV3ChannelRegister(void)
{
    static int firstTime = 1;
    if (firstTime) {
        firstTime = 0;
        iocshRegister(&stopV3ChannelFuncDef, stopV3ChannelCallFunc);
    }
}

epicsExportRegistrar(startV3ChannelRegister);
epicsExportRegistrar(stopV3ChannelRegister);
