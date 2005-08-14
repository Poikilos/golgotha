/********************************************************************** <BR>
  This file is part of Crack dot Com's free source code release of
  Golgotha. <a href="http://www.crack.com/golgotha_release"> <BR> for
  information about compiling & licensing issues visit this URL</a> 
  <PRE> If that doesn't help, contact Jonathan Clark at 
  golgotha_source@usa.net (Subject should have "GOLG" in it) 
***********************************************************************/

#ifndef G1_JET_HH
#define G1_JET_HH

#include "objs/map_piece.h"

class g1_jet_class : public g1_map_piece_class
{
protected:
  // internal modes for jet
  w8 mode;

  i4_float            target_height;
  g1_mini_object        *engines;

  w32 sway;
public:
  // version of save file
  enum { DATA_VERSION=3 };

  // modes for jet

  g1_jet_class(g1_object_type id, g1_loader_class *fp);
  virtual void save(g1_saver_class *fp);  
  virtual void load(g1_loader_class *fp);
  virtual void skipload(g1_loader_class *fp);

  i4_bool move(i4_float x_amount, i4_float y_amount, i4_float z_amount);

  virtual void fire();
  virtual void think();
  //This virtual function doesn't exist.
  //virtual void note_leave_range(g1_object_class *who, 
  //                              g1_fire_range_type _range) {}
  
  void damage(g1_object_class *obj, int hp, i4_3d_vector _damage_dir);
};

#endif
