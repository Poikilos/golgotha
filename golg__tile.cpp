/********************************************************************** <BR>
  This file is part of Crack dot Com's free source code release of
  Golgotha. <a href="http://www.crack.com/golgotha_release"> <BR> for
  information about compiling & licensing issues visit this URL</a> 
  <PRE> If that doesn't help, contact Jonathan Clark at 
  golgotha_source@usa.net (Subject should have "GOLG" in it) 
***********************************************************************/

#include "file/file.h"
#include "load3d.h"
#include "tile.h"
#include "error/alert.h"
#include "obj3d.h"
#include "map.h"
#include "resources.h"
#include "string/str_checksum.h"
#include "render/r1_api.h"
#include "checksum/checksum.h"
#include "saver.h"
#include "file/file.h"
#include "lisp/lisp.h"
#include "g1_render.h"
#include "render/tmanage.h"
#include "string/str_checksum.h"
#include "map_cell.h"
#include "memory/hashtable.h"

g1_tile_man_class g1_tile_man;


static i4_array<w16> select_remap(0,128);


void g1_tile_class::set_friction(float uB)
{
  friction_fraction = uB;
  damping_fraction = 1.0f/uB;
  damping_e = (float)exp(-uB *0.1f);
}


int g1_tile_man_class::get_remap(int tile_num)
{
  return select_remap[tile_num];
}

g1_tile_man_class::g1_tile_man_class()
:original_checksums(256,i4_hashtable<w32>::WAITFORINIT|i4_hashtable<w32>::KEYSONLY)
{
  t_tiles=0;
  max_tiles=0;
  array=0;
  sorted_by_checksum=0;
}



int g1_tile_man_class::remap_size()
{
  return select_remap.size();
}

void g1_tile_man_class::init()
	{
	original_checksums.init();
	}

void g1_tile_man_class::reset(int _max_tiles) 
{ 
  if (array)
    i4_free(array);
    
  max_tiles=_max_tiles+1;
  t_tiles=0;
  if (max_tiles)
    array=(g1_tile_class *)I4_MALLOC(sizeof(g1_tile_class) * max_tiles,"");
  else
    array=0;

  array[0].init();
  array[0].texture=0;
  array[0].filename_checksum=0;
  original_checksums.reset(i4_F);
  sorted_by_checksum=0;
}

void g1_tile_class::apply_to_cell(g1_map_cell_class &cell)
{
  if (flags & BLOCKING)
    cell.flags&=~g1_map_cell_class::IS_GROUND;
  else
    cell.flags|=g1_map_cell_class::IS_GROUND;
}

void g1_tile_class::init()
{
  flags=SELECTABLE;
  set_friction(g1_resources.damping_friction); 
}

/*
static i4_bool test_closest(const i4_3d_point_class &p1,
                         const i4_3d_point_class &p2,
                         i4_float &dist)              
{
  i4_float d=((p1.x - p2.x) * (p1.x - p2.x)) +
    ((p1.y - p2.y) * (p1.y - p2.y));
 

  if (fabs(d-dist)<0.005)
  {
    if (p1.z>p2.z)
    {
      dist=d;
      return i4_T;
    }
  } else if (d<dist)
  {
    dist=d;
    return i4_T;
  }
  return i4_F;
}
*/

int g1_tile_man_class::get_tile_from_name(char *name)
{
  sw32 len = strlen(name);
  return get_tile_from_checksum(i4_check_sum32(name,len));
}

int g1_tile_man_class::get_tile_from_name(i4_const_str &name)
{
  return get_tile_from_checksum(i4_str_checksum(name));
}


int g1_tile_compare(const void *a, const void *b)
{
  if (((g1_tile_class *)a)->filename_checksum<((g1_tile_class *)b)->filename_checksum)
    return -1;
  else if (((g1_tile_class *)a)->filename_checksum>((g1_tile_class *)b)->filename_checksum)
    return 1;
  else return 0;
}

void g1_tile_man_class::finished_load()
{
 if (!sorted_by_checksum)
  {
    int i,order_on=0;

    for (i=0; i<t_tiles; i++)
    {
      if (array[i].flags & g1_tile_class::SELECTABLE)
        array[i].selection_order=order_on++;      
      else
        array[i].selection_order=0;
    }

    qsort(array, t_tiles, sizeof(g1_tile_class), g1_tile_compare);
    sorted_by_checksum=1;

    select_remap.clear();
    for (i=0; i<order_on; i++)
      select_remap.add();

    for (i=0; i<t_tiles;i++)
      if (array[i].flags & g1_tile_class::SELECTABLE)
        select_remap[array[i].selection_order]=i;

    pink=array[get_tile_from_name("pink")].texture;
    default_tile_type=get_tile_from_name("tron_grid");
  }
}

i4_const_str *g1_tile_man_class::get_name_from_tile(w32 tileno)
	{
	if (tileno>=(w32)t_tiles) return 0;
	w32 checksum=array[tileno].filename_checksum;
	i4_const_str *nm=r1_get_texture_name(checksum);
	if (!nm)//not found?
		{
		//then try using the hash-table to find overwritten
		//names
		checksum=get_original_checksum(checksum);
		nm=r1_get_texture_name(checksum);
		}
	return nm;
	}

int g1_tile_man_class::get_tile_from_checksum(w32 checksum)
{
  if (!t_tiles)
    return 0;

 
  sw32 lo=0,hi=t_tiles-1,mid;

  mid=(lo+hi+1)/2;
  for(;;)
  {
    if (checksum==array[mid].filename_checksum)
      return mid;
    else if (checksum<array[mid].filename_checksum)
      hi=mid-1;
    else 
      lo=mid+1;
    
    w32 last_mid=mid;
    mid=(hi+lo)/2;

    if (last_mid==(w32)mid)
      return 0;
  }

  return 0;
}


void g1_tile_man_class::add(li_object *o, li_environment *env)
{
  if (t_tiles==max_tiles)
    i4_error("WARNING: Too many tile textures in level.");

  li_object *prop=0;

  li_string *tname=0;  
  if (o->type()==LI_STRING)
    tname=li_string::get(o,env);
  else
  {
    prop=li_cdr(o,env);
    tname=li_string::get(li_car(o,env),env);
  }


  r1_texture_manager_class *tman=g1_render.r_api->get_tmanager();


  i4_const_str i4_tname=i4_const_str(tname->value());
  array[t_tiles].texture=tman->register_texture(i4_tname, i4_tname); 
  array[t_tiles].filename_checksum=i4_str_checksum(i4_tname);


  array[t_tiles++].get_properties(prop, env);
  sorted_by_checksum=0;
}


static li_symbol *g1_block=0, *g1_wave=0, *g1_selectable=0, *g1_friction=0, *g1_save_name=0,
  *g1_damage=0;

// format of ("texture_name" (property_name prop_value)..)
void g1_tile_class::get_properties(li_object *properties, li_environment *env)
{
  init();

  while (properties)
  {
    li_symbol *sym=li_symbol::get(li_car(li_car(properties,env),env),env);
	li_object *temp=li_cdr(li_car(properties,env),env);
	if (!temp)
		li_error(env,"USER: Missing texture property argument");
    li_object *value=li_car(temp,env);

    if (sym==li_get_symbol("block", g1_block))
    {
      if (li_get_bool(value,env)) flags|=BLOCKING; else flags&=~BLOCKING;
    }
    else if (sym==li_get_symbol("wave", g1_wave))
    {
      if (li_get_bool(value,env)) flags|=WAVE; else flags&=~WAVE;
    }
    else if (sym==li_get_symbol("selectable", g1_selectable))
    {
      if (li_get_bool(value, env)) flags|=SELECTABLE; else flags&=~SELECTABLE;
    }
    else if (sym==li_get_symbol("friction", g1_friction))
      set_friction((float)(li_float::get(value,env)->value()));      
    else if (sym==li_get_symbol("damage", g1_damage))
      damage = li_int::get(value,env)->value();
    else if (sym==li_get_symbol("save_name", g1_save_name))
		{
		w32 newkey=i4_str_checksum(i4_const_str(li_string::get(value,env)->value()));
		//must fake the insertion of a pointer
		g1_tile_man.store_alternate_checksum(newkey,filename_checksum);
		filename_checksum=newkey;
		}
    else
      i4_error("bad texture flag '%s' should be block, wave, selectable, friction, save_name", 
               sym->name()->value());

    properties=li_cdr(properties,env);
  }
}
void g1_tile_man_class::store_alternate_checksum(w32 nchk, w32 oldchk)
	{
	original_checksums.insert_if_single(nchk,(w32*)oldchk);
	}

w32 g1_tile_man_class::get_original_checksum(w32 newchecksum)
	{
	w32 *c=original_checksums.get(newchecksum);
	return (w32)c;
	}

void g1_tile_man_class::uninit()
{
  select_remap.uninit();
  if (array)
    i4_free(array);
  array=0;
  original_checksums.reset();
}
