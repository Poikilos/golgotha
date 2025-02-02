/********************************************************************** <BR>
   This file is part of Crack dot Com's free source code release of
   Golgotha. <a href="http://www.crack.com/golgotha_release"> <BR> for
   information about compiling & licensing issues visit this URL</a>
   <PRE> If that doesn't help, contact Jonathan Clark at
   golgotha_source@usa.net (Subject should have "GOLG" in it)
 ***********************************************************************/

#ifndef G1_BOMBER_HH
#define G1_BOMBER_HH

#include "objs/map_piece.h"

class g1_bomber_class :
	public g1_map_piece_class
{
protected:
	// internal modes for bomber
	w8 mode;
	i4_float target_height;
	w32 sway;
	g1_typed_reference_class<g1_object_class> last_target;
	w32 ticks_still_bombing;
public:
	g1_bomber_class(g1_object_type id, g1_loader_class * fp);
	virtual void save(g1_saver_class * fp);
	void load(g1_loader_class * fp);
	void skipload(g1_loader_class * fp);

	i4_bool move(i4_float x_amount, i4_float y_amount);

	virtual void fire();
	virtual void think();

	i4_bool can_attack(g1_object_class * who);

	void damage(g1_object_class * obj, int hp, i4_3d_vector _damage_dir);
};

#endif
