/* exampleServiceRPC.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvDataCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/* Marty Kraimer 2011.03 */
/* This connects to a V3 record and presents the data as a PVStructure
 * It provides access to  value, alarm, display, and control.
 */

#include <cstddef>
#include <cstdlib>
#include <cstddef>
#include <string>
#include <cstdio>
#include <stdexcept>
#include <memory>

#include <exampleServiceRPC.h>


namespace epics { namespace pvIOC { 

using namespace epics::pvData;
using namespace epics::pvAccess;

ExampleServiceRPC::ExampleServiceRPC()
{}

ExampleServiceRPC::~ExampleServiceRPC()
{
printf("ExampleServiceRPC::~ExampleServiceRPC()\n");
}

void ExampleServiceRPC::destroy()
{
printf("ExampleServiceRPC::destroy()\n");
}

void ExampleServiceRPC::request(
    ChannelRPCRequester::shared_pointer const & channelRPCRequester,
    epics::pvData::PVStructure::shared_pointer const & pvArgument)
{
    String builder;
    PVString *pvfunction = pvArgument->getStringField("function");
    PVStringArray *pvnames = static_cast<PVStringArray *>
        (pvArgument->getScalarArrayField("names",pvString));
    PVStringArray *pvvalues = static_cast<PVStringArray *>
        (pvArgument->getScalarArrayField("values",pvString));
    builder += "pvArgument ";
    bool is = true;
    if(pvfunction==0) is = false;
    if(pvnames==0) is = false;
    if(pvvalues==0) is = false;
    if(is) {
        builder += "is a NTNameValue\n";
    } else {
        builder += "is not a NTNameValue\n ";
    }
pvArgument->toString(&builder);
printf("%s\n",builder.c_str());
    StandardField *standardField = getStandardField();
    StandardPVField *standardPVField = getStandardPVField();
    FieldCreate * fieldCreate = getFieldCreate();
    PVDataCreate * pvDataCreate = getPVDataCreate();
    int n = 5;
    FieldConstPtrArray fields = new FieldConstPtr[5];
    fields[0] = standardField->alarm();
    fields[1] = standardField->timeStamp();
    fields[2] = fieldCreate->createScalarArray("label",pvString);
    fields[3] = fieldCreate->createScalarArray("position",pvDouble);
    fields[4] = fieldCreate->createStructureArray(
        "alarms",standardField->alarm());
    PVStructure::shared_pointer pvStructure = PVStructure::shared_pointer(
        pvDataCreate->createPVStructure(0,"NTTable",n,fields));
    PVTimeStamp pvTimeStamp;
    TimeStamp timeStamp;
    pvTimeStamp.attach(pvStructure->getStructureField("timeStamp"));
    timeStamp.getCurrent();
    pvTimeStamp.set(timeStamp);
    String label[2];
    for(int i=0; i<2; i++) {
        FieldConstPtr field = fields[i + 3];
        label[i] = field->getFieldName();
    }
    PVStringArray *pvLabel = static_cast<PVStringArray *>
        (pvStructure->getScalarArrayField("label",pvString));
    pvLabel->put(0,2,label,0);
    PVDoubleArray *pvPositions = static_cast<PVDoubleArray *>
        (pvStructure->getScalarArrayField("position",pvDouble));
    double positions[2];
    positions[0] = 1.0;
    positions[1] = 2.0;
    pvPositions->put(0,2,positions,0);
    PVStructureArray *pvAlarms = static_cast<PVStructureArray *>
        (pvStructure->getStructureArrayField("alarms"));
    PVAlarm pvAlarm;
    Alarm alarm;
    PVStructurePtr palarms[n];
    for(int i=0; i<2; i++) {
        palarms[i] = standardPVField->alarm(0);
        pvAlarm.attach(palarms[i]);
        alarm.setMessage("test");
        alarm.setSeverity(majorAlarm);
        alarm.setStatus(clientStatus);
        pvAlarm.set(alarm);
    }
    pvAlarms->put(0,2,palarms,0);
    String labels[2];
    labels[0] = pvPositions->getField()->getFieldName();
    labels[1] = pvAlarms->getField()->getFieldName();
    pvLabel->put(0,2,labels,0);
builder.clear();
pvStructure->toString(&builder);
printf("%s\n",builder.c_str());
//builder.clear();
//pvStructure->getStructure()->toString(&builder);
//printf("%s\n",builder.c_str());
    channelRPCRequester->requestDone(Status::OK,pvStructure);
}

}}

