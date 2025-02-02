/********************************************************************** <BR>
   This file is part of Crack dot Com's free source code release of
   Golgotha. <a href="http://www.crack.com/golgotha_release"> <BR> for
   information about compiling & licensing issues visit this URL</a>
   <PRE> If that doesn't help, contact Jonathan Clark at
   golgotha_source@usa.net (Subject should have "GOLG" in it)
 ***********************************************************************/

#ifndef G1_MAKE_TLIST_HH
#define G1_MAKE_TLIST_HH

#include "memory/malloc.h"
#include "string/string.h"
class li_object;
class g1_loader_class;

li_object *g1_get_load_info(g1_loader_class * map_file,
							i4_file_class * * fp, int t_files,
							i4_array<i4_str *> &texture_name_array,
							i4_array<i4_str *> &model_name_array,
							int &total_tiles,
							int include_model_textures);

void g1_make_texture_list_for_model(const i4_const_str model_name,
									i4_array<i4_str *> &t_arr);

#endif
