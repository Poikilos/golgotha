/********************************************************************** <BR>
   This file is part of Crack dot Com's free source code release of
   Golgotha. <a href="http://www.crack.com/golgotha_release"> <BR> for
   information about compiling & licensing issues visit this URL</a>
   <PRE> If that doesn't help, contact Jonathan Clark at
   golgotha_source@usa.net (Subject should have "GOLG" in it)
 ***********************************************************************/

#ifndef G1_HELICOPTER_HH
#define G1_HELICOPTER_HH

#include "objs/map_piece.h"

class g1_helicopter_class :
	public g1_map_piece_class
{
protected:
	w8 mode;                                      //internal modes
	i4_float upaccel;                             //upwards acceleration
	i4_float bladespeed;                          //blade speed
	g1_mini_object * blades;                       // = &mini_objects[0];
public:
	g1_helicopter_class(g1_object_type id, g1_loader_class * fp);
	virtual void save(g1_saver_class * fp);
	virtual void load(g1_loader_class * fp);
	virtual void skipload(g1_loader_class * fp);
private:
	i4_bool move(i4_float x_amount,i4_float y_amount, i4_float z_amount);
public:
	virtual void fire();
	virtual void think();

	short get_min_health()
	{
		return 20;
	}

	i4_bool can_strafe()
	{
		return i4_T;
	}

	void damage(g1_object_class * obj, int hp, i4_3d_vector _damage_dir);
	int get_chunk_names(char * *&list);
};

#endif
