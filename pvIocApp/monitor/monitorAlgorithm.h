/* monitorAlgorithm.h */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvDataCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/* Marty Kraimer 2011.03 */
#ifndef MONITORALGORITHM_H
#define MONITORALGORITHM_H
#include <string>
#include <stdexcept>
#include <memory>
#include <vector>

#include <pvData.h>
#include <monitor.h>
#include <sharedPtr.h>

namespace epics { namespace pvIOC { 

class MonitorAlgorithm {
public:
     MonitorAlgorithm(){}
     virtual ~MonitorAlgorithm(){}
     virtual String getAlgorithmName() = 0;
     virtual bool causeMonitor() = 0;
     void monitorIssued() = 0;
}}

#endif  /* MONITORALGORITHM_H */
