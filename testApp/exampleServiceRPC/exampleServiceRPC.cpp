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

#include <pv/ntfield.h>
#include <pv/nttable.h>
#include <pv/ntnameValue.h>

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
    builder += "pvArgument ";
    bool is = NTNameValue::isNTNameValue(pvArgument.get());
    if(is) {
        builder += "is a NTNameValue\n";
    } else {
        builder += "is not a NTNameValue\n ";
    }
    pvArgument->toString(&builder);
    printf("%s\n",builder.c_str());
    FieldCreate * fieldCreate = getFieldCreate();
    NTField *ntField = NTField::get();
    PVNTField *pvntField = PVNTField::get();
    int n = 2;
    FieldConstPtr fields[2];
    fields[0] = fieldCreate->createScalarArray("position",pvDouble);
    fields[1] = ntField->createAlarmArray("alarms");
    PVStructure::shared_pointer pvStructure = NTTable::create(
        true,true,true,n,fields);
//builder.clear();
//pvStructure->toString(&builder);
//printf("%s\n",builder.c_str());
//builder.clear();
//pvStructure->getStructure()->toString(&builder);
//printf("%s\n",builder.c_str());
    NTTable ntTable(pvStructure);
    PVDoubleArray *pvPositions
        = static_cast<PVDoubleArray *>(ntTable.getPVField(0));
    double positions[2];
    positions[0] = 1.0;
    positions[1] = 2.0;
    pvPositions->put(0,n,positions,0);
    PVStructureArray *pvAlarms
        = static_cast<PVStructureArray *>(ntTable.getPVField(1));
    PVAlarm pvAlarm;
    Alarm alarm;
    PVStructurePtr palarms[n];
    for(int i=0; i<n; i++) {
        palarms[i] = pvntField->createAlarm(0);
        pvAlarm.attach(palarms[i]);
        alarm.setMessage("test");
        alarm.setSeverity(majorAlarm);
        alarm.setStatus(clientStatus);
        pvAlarm.set(alarm);
    }
    pvAlarms->put(0,n,palarms,0);
    String labels[n];
    labels[0] = pvPositions->getField()->getFieldName();
    labels[1] = pvAlarms->getField()->getFieldName();
    PVStringArray *label = ntTable.getLabel();
    label->put(0,n,labels,0);
    channelRPCRequester->requestDone(Status::OK,pvStructure);
}

}}

