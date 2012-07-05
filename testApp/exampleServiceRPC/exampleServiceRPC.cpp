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
using std::tr1::static_pointer_cast;

ExampleServiceRPC::ExampleServiceRPC()
{
printf("ExampleServiceRPC::ExampleServiceRPC()\n");
}

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
printf("ExampleServiceRPC::request\n");
    String builder;
    PVStringPtr pvfunction = pvArgument->getStringField("function");
    PVStringArrayPtr pvnames = static_pointer_cast<PVStringArray>
        (pvArgument->getScalarArrayField("names",pvString));
    PVStringArrayPtr pvvalues = static_pointer_cast<PVStringArray>
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
    StandardFieldPtr standardField = getStandardField();
    StandardPVFieldPtr standardPVField = getStandardPVField();
    FieldCreatePtr  fieldCreate = getFieldCreate();
    PVDataCreatePtr  pvDataCreate = getPVDataCreate();
    size_t n = 5;
    FieldConstPtrArray fields;
    StringArray names;
    fields.reserve(n);
    names.reserve(n);
    names.push_back("alarm");
    names.push_back("timeStamp");
    names.push_back("label");
    names.push_back("position");
    names.push_back("alarms");
    fields.push_back(standardField->alarm());
    fields.push_back(standardField->timeStamp());
    fields.push_back(fieldCreate->createScalarArray(pvString));
    fields.push_back(fieldCreate->createScalarArray(pvDouble));
    fields.push_back(fieldCreate->createStructureArray(standardField->alarm()));
    StructureConstPtr structure = fieldCreate->createStructure(names,fields);
    PVStructurePtr pvStructure = pvDataCreate->createPVStructure(structure);
    PVTimeStamp pvTimeStamp;
    TimeStamp timeStamp;
    pvTimeStamp.attach(pvStructure->getStructureField("timeStamp"));
    timeStamp.getCurrent();
    pvTimeStamp.set(timeStamp);
    StringArray label;
    label.reserve(2);
    for(int i=0; i<2; i++) {
        label.push_back(names[i+3]);
    }
    PVStringArrayPtr pvLabel = static_pointer_cast<PVStringArray>
        (pvStructure->getScalarArrayField("label",pvString));
    pvLabel->put(0,2,label,0);
    PVDoubleArrayPtr pvPositions = static_pointer_cast<PVDoubleArray>
        (pvStructure->getScalarArrayField("position",pvDouble));
    double positions[2];
    positions[0] = 1.0;
    positions[1] = 2.0;
    pvPositions->put(0,2,positions,0);
    PVStructureArrayPtr pvAlarms = static_pointer_cast<PVStructureArray>
        (pvStructure->getStructureArrayField("alarms"));
    PVAlarm pvAlarm;
    Alarm alarm;
    PVStructurePtrArray palarms;
    size_t na=2;
    palarms.reserve(na);
    for(size_t i=0; i<na; i++) {
        palarms.push_back(pvDataCreate->createPVStructure(standardField->alarm()));
    }
    for(size_t i=0; i<na; i++) {
        pvAlarm.attach(palarms[i]);
        alarm.setMessage("test");
        alarm.setSeverity(majorAlarm);
        alarm.setStatus(clientStatus);
        pvAlarm.set(alarm);
    }
    PVStructurePtr *xxx = &palarms[0];
    pvAlarms->put(0,2,xxx,0);
    String labels[2];
    labels[0] = pvPositions->getFieldName();
    labels[1] = pvAlarms->getFieldName();
    pvLabel->put(0,2,labels,0);
builder.clear();
pvStructure->toString(&builder);
printf("%s\n",builder.c_str());
//builder.clear();
//pvStructure->getStructure()->toString(&builder);
//printf("%s\n",builder.c_str());
    channelRPCRequester->requestDone(Status::Ok,pvStructure);
}

}}

