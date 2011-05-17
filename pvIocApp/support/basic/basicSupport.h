/* basicSupport.h */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvDataCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
#ifndef BASICSUPPORT_H
#define BASICSUPPORT_H

#include "support.h"

namespace epics { namespace pvIOC { 

extern SupportCreate noopSupportCreate;
extern SupportCreate genericSupportCreate;
extern SupportCreate scanFieldSupportCreate;

}}

#endif  /* BASICSUPPORT_H */
