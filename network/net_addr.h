/********************************************************************** <BR>
   This file is part of Crack dot Com's free source code release of
   Golgotha. <a href="http://www.crack.com/golgotha_release"> <BR> for
   information about compiling & licensing issues visit this URL</a>
   <PRE> If that doesn't help, contact Jonathan Clark at
   golgotha_source@usa.net (Subject should have "GOLG" in it)
 ***********************************************************************/

#ifndef I4_NET_ADDR_HH
#define I4_NET_ADDR_HH

#include "arch.h"

class i4_net_protocol;

class i4_net_address
{
public:
	i4_net_protocol * protocol;
	virtual i4_net_address *copy() = 0;
	virtual void set_port(int port) = 0;
	virtual i4_bool equals(i4_net_address * other) = 0;
};

#endif
