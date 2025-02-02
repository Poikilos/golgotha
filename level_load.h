/********************************************************************** <BR>
   This file is part of Crack dot Com's free source code release of
   Golgotha. <a href="http://www.crack.com/golgotha_release"> <BR> for
   information about compiling & licensing issues visit this URL</a>
   <PRE> If that doesn't help, contact Jonathan Clark at
   golgotha_source@usa.net (Subject should have "GOLG" in it)
 ***********************************************************************/

#ifndef G1_LEVEL_LOAD_HH
#define G1_LEVEL_LOAD_HH



#include "string/string.h"

i4_bool g1_load_level(const i4_const_str &filename,
					  int load_res_stuff=1,
					  w32 exclude_flags=0);
class g1_loader_class;
w32 g1_load_res_info(g1_loader_class * map_file, i4_file_class * fp,w32 exclude_flags);
i4_str *g1_get_res_filnename(const i4_const_str &map_filename);


#endif
