#include "pch.h"

/********************************************************************** <BR>
   This file is part of Crack dot Com's free source code release of
   Golgotha. <a href="http://www.crack.com/golgotha_release"> <BR> for
   information about compiling & licensing issues visit this URL</a>
   <PRE> If that doesn't help, contact Jonathan Clark at
   golgotha_source@usa.net (Subject should have "GOLG" in it)
 ***********************************************************************/

#include "sound_man.h"
#include "objs/model_id.h"
#include "objs/model_draw.h"

#include "input.h"
#include "math/pi.h"
#include "math/trig.h"
#include "math/angle.h"
#include "resources.h"
#include "g1_object.h"
#include "map_cell.h"
#include "map.h"
#include "map_man.h"
#include "sound/sfx_id.h"
#include "objs/vehic_sounds.h"
#include "objs/jet.h"
#include "objs/fire.h"
#include "object_definer.h"
#include "objs/path_object.h"

#include "image_man.h"
static g1_team_icon_ref radar_im("bitmaps/radar/plane.tga");


//const i4_float FLY_HEIGHT = 1.5f;

enum
{
	TAKE_OFF=0,
	TAKE_OFF2,
	FLYING,
	DYING
};

static g1_object_type bomb_type;
void g1_jet_init()
{
	bomb_type = g1_get_object_type("dropped_bomb");
}

g1_object_definer<g1_jet_class>
g1_jet_def("jet",
		   g1_object_definition_class::TO_MAP_PIECE |
		   g1_object_definition_class::EDITOR_SELECTABLE |
		   g1_object_definition_class::MOVABLE,
		   g1_jet_init);


g1_jet_class::g1_jet_class(g1_object_type id,
						   g1_loader_class * fp)
	: g1_map_piece_class(id, fp)
{
	radar_image=&radar_im;
	radar_type=G1_RADAR_VEHICLE;

	draw_params.setup("jet_body","jet_shadow","jet_lod");

	allocate_mini_objects(1,"Jet Mini-Objects");
	engines = &mini_objects[0];

	engines->h = engines->lh = 0.05f;
	engines->x = engines->lx = 0.01f;
	engines->y = engines->ly = 0;

	w16 ver=0,data_size=0;
	if (fp)
	{
		fp->get_version(ver,data_size);
	}
	else
	{
		ver =0;
	}
	switch (ver)
	{
		case DATA_VERSION:
			fp->read_format("1ffff4",
							&mode,
							&engines->rotation.x, &engines->rotation.y, &engines->rotation.z,
							&vspeed,
							&sway);
			engines->grab_old();
			break;
		case 2:
			mode = fp->read_8();

			engines->rotation.x = fp->read_float();
			engines->rotation.y = fp->read_float();
			engines->rotation.z = fp->read_float();
			engines->lrotation.x = fp->read_float();
			engines->lrotation.y = fp->read_float();
			engines->lrotation.z = fp->read_float();

			vspeed  = fp->read_float();
			sway = fp->read_32();
			break;
		case 1:
			mode = 0;
			if (fp->read_8())
			{
				mode=TAKE_OFF;
			}                           // take_off
			if (fp->read_8())
			{
				mode=TAKE_OFF;
			}                           // taking_off
			if (fp->read_8())
			{
				mode=FLYING;
			}                           // flying
			fp->read_8();
			fp->read_8();

			engines->rotation.x = fp->read_float();
			engines->rotation.y = fp->read_float();
			engines->rotation.z = fp->read_float();
			engines->lrotation.x = fp->read_float();
			engines->lrotation.y = fp->read_float();
			engines->lrotation.z = fp->read_float();

			vspeed  = fp->read_float();
			fp->read_float();
			sway = 0;
			break;
		default:
			if (fp)
			{
				fp->seek(fp->tell() + data_size);
			}
			mode = 0;
			engines->rotation.x = engines->lrotation.x = 0;
			engines->rotation.y = engines->lrotation.y = 0;
			engines->rotation.z = engines->lrotation.z = 0;
			vspeed = 0;
			sway = 0;
			break;
	}

	if (fp)
	{
		fp->end_version(I4_LF);
	}

	engines->defmodeltype = g1_model_list_man.find_handle("jet_engines");


	init_rumble_sound(G1_RUMBLE_JET);

	damping_fraction = 0.02f;

	radar_image=&radar_im;
	radar_type=G1_RADAR_VEHICLE;
	solveparams=SF_GRADE4;
	set_flag(BLOCKING      |
			 TARGETABLE    |
			 AERIAL        |
			 HIT_AERIAL    |
			 SELECTABLE    |
			 DANGEROUS,   1);

}

void g1_jet_class::save(g1_saver_class * fp)
{
	g1_map_piece_class::save(fp);

	fp->start_version(DATA_VERSION);

	fp->write_format("1ffff4",
					 &mode,
					 &engines->rotation.x, &engines->rotation.y, &engines->rotation.z,
					 &vspeed,
					 &sway);

	fp->end_version();
}

void g1_jet_class::load(g1_loader_class * fp)
{
	g1_map_piece_class::load(fp);
	fp->check_version(DATA_VERSION);
	fp->read_format("1ffff4",&mode,&engines->rotation.x,&engines->rotation.y,&engines->rotation.z,
					&vspeed,&sway);
	fp->end_version(I4_LF);
};

void g1_jet_class::skipload(g1_loader_class * fp)
{
	g1_map_piece_class::skipload(fp);
	fp->check_version(DATA_VERSION);
	w8 m;
	w32 s;
	float x,y,z,vs;
	fp->read_format("1ffff4",&m,&x,&y,&z,&vs,&s);
	fp->end_version(I4_LF);
};

void g1_jet_class::fire()
{
	fire_delay = g1_jet_def.defaults->fire_delay;

	i4_3d_vector p(x,y,h), dir(attack_target->x, attack_target->y, attack_target->h);

	dir -= p;

	g1_fire(defaults->fire_type,
			this, attack_target.get(), p, dir);
}

i4_bool g1_jet_class::move(i4_float x_amount, i4_float y_amount, i4_float z_amount)
{
	unoccupy_location();
	x+=x_amount;
	y+=y_amount;
	h+=z_amount;

	if (occupy_location())
	{
		g1_add_to_sound_average(rumble_type, i4_3d_vector(x,y,h));
		return i4_T;
	}
	return i4_F;
}


/* DLL stuff */

void g1_jet_class::think()
{
	if (!check_life())
	{
		return;
	}

	find_target();

	if (fire_delay>0)
	{
		fire_delay--;
	}

	h      += vspeed;

	switch (mode)
	{
		case TAKE_OFF:
			{
				// rotate engines
				if (i4_rotate_to(engines->rotation.y,3*i4_pi()/2,0.1f)==0.0)
				{
					mode = TAKE_OFF2;
				}
			} break;

		case TAKE_OFF2:
			{
				if (next_path.valid())
				{
					dest_x = next_path->x - path_cos*path_len;
					dest_y = next_path->y - path_sin*path_len;
					dest_z = next_path->h - path_tan_phi*path_len;
				}

				//then raise the jet
				i4_float dist_to_go = dest_z - h;
				if (dist_to_go>0.05)
				{
					//if he is more than halfway away, accelerate down
					//otherwise accelerate up
					if (dist_to_go > (FLY_HEIGHT * 0.5))
					{
						vspeed = (vspeed<0.05f) ? vspeed+0.005f : vspeed;
					}
					else
					{
						vspeed+=0.005f;
					}
				}
				else
				{
					//lock these in case there were any small errors
					h = dest_z;
					vspeed = 0;

					//think one more time so the l* variables catch up
					//(otherwise he'll bob up and down)
					if (h == lh)
					{
						mode = FLYING;
					}
				}
			} break;

		case 47: //Heli-Flying
			{
				find_target();

				if (next_path.valid())
				{
					dest_x = next_path->x;
					dest_y = next_path->y;
					dest_z = next_path->h;
				}

				i4_float dist, dtheta;
				i4_3d_vector d;
				suggest_air_move(dist, dtheta, d);
				g1_object_class * blocking=0;
				i4_float save_z=d.z;
				if (check_move(d.x,d.y,d.z,blocking))
				{
					//if we're colliding with the airbase we just go up vertically.
					if (blocking && (blocking->id==g1_get_object_type("airbase")))
					{
						d.x=0;
						d.y=0;
						d.z=save_z>0.05f ? save_z : 0.05f;
					}
					if (my_solver)
					{
						if (h>terrain_height+0.3)
						{
							//fly vertical upwards if just above the floor
							move(d.x,d.y,d.z);
						}
						else
						{
							move(0,0,d.z);
						}
					}
					else
					{
						move(d.x,d.y,d.z);
					}
				}

				if (h<terrain_height)
				{
					damage(this,health,i4_3d_vector(-d.x,-d.y,-d.z));
					break;
				}
				if (dist<speed)
				{
					advance_path();
				}

				i4_float roll_to = -i4_pi()/4 * dtheta;

				i4_normalize_angle(roll_to);

				if (roll_to)
				{
					i4_rotate_to(roll,roll_to,defaults->turn_speed/4);
				}
				else
				{
					i4_rotate_to(roll,0,defaults->turn_speed/2);
				}
				//make the heli pitch forward if it moves. The amount is just pure guess, since speed is
				//in units per tick and pitch is an angle. But it seems to work
				i4_float pitch_to=speed*1.5f;
				if (i4_fabs(pitch-pitch_to)>0.05f)
				{
					//this avoids swinging.
					i4_rotate_to(pitch,pitch_to,0.05f);
				}

				if (attack_target.valid() && !fire_delay)
				{
					fire();
				}

				groundpitch = 0; //no ground when in the air (duh)
				groundroll  = 0;
			} break;

		case FLYING:
			{
				i4_3d_vector d;

				// sway over terrain
				sway++;

				if (attack_target.valid())
				{
					if (fire_delay==0)
					{
						fire();
					}
				}

				if (next_path.valid())
				{
					dest_x = next_path->x;
					dest_y = next_path->y;
					dest_z = next_path->h;
				}

				i4_float roll_to  = 0.02f*(float)sin(i4_pi()*sway/17.0f);
				i4_float pitch_to = 0.02f*(float)sin(i4_pi()*sway/13.0f);

				i4_float dist=0, dtheta=0;
				suggest_air_move(dist, dtheta, d);
				g1_object_class * blocking=0;
				if (!controled())
				{
					d.z+=0.02f*(float)sin(i4_pi()*sway/15.0f);
				}                                  //doing this while controled causes "jumpy" movement
				i4_float save_z=d.z;
				if (check_move(d.x,d.y,d.z,blocking))
				{
					//if we're colliding with the airbase we just go up vertically.
					if (blocking && (blocking->id==g1_get_object_type("airbase")))
					{
						d.x=0;
						d.y=0;
						d.z=save_z>0.05f ? save_z : 0.05f;
					}
					if (my_solver)
					{
						if (h>terrain_height+0.3)
						{
							//fly vertical upwards if just above the floor
							move(d.x,d.y,d.z);
						}
						else
						{
							move(0,0,d.z);
						}
					}
					else
					{
						move(d.x,d.y,d.z);
					}
				}

				if (h<terrain_height)
				{
					damage(this,health,i4_3d_vector(-d.x,-d.y,-d.z));
					break;
				}

				if (dist<speed)
				{
					advance_path();
				}

				i4_normalize_angle(roll_to);

				if (speed>0.001 || dtheta!=0.0)
				{
					roll_to = -i4_pi()/4 * dtheta;
					pitch_to = -i4_pi()/8 * speed;
					i4_rotate_to(engines->rotation.y,0,0.1f);
				}
				else
				{
					i4_rotate_to(engines->rotation.y,3*i4_pi()/2,0.1f);
				}

				i4_rotate_to(roll,roll_to,defaults->turn_speed/4);
				if (i4_fabs(pitch-pitch_to)>0.05f)
				{
					//this avoids swinging.
					i4_rotate_to(pitch,pitch_to,defaults->turn_speed/4);
				}

				groundpitch = 0; //no ground in the air (duh)
				groundroll  = 0;
			} break;

		case DYING:
			{
				i4_float &roll_speed  = dest_x;
				i4_float &pitch_speed = dest_y;

				pitch_speed += 0.004f;
				pitch += pitch_speed;

				roll_speed -= 0.012f;
				roll += roll_speed;

				vspeed -= (g1_resources.gravity * 0.1f);

				i4_float dx,dy;
				dx = speed*(float)cos(theta);
				dy = speed*(float)sin(theta);
				move(dx,dy,0);

				if (h<=terrain_height)
				{
					g1_map_piece_class::damage(0,health,i4_3d_vector(0,0,1));
				}                                                               // die somehow!!!
			} break;
	}

	// have to keep thinking to sway
	request_think();
}

void g1_jet_class::damage(g1_object_class * obj, int hp, i4_3d_vector _damage_dir)
{
	//we dont want to explode if ppl shoot us while we're dying.. we want to
	//smash into the ground and create a nice explosion
	if (mode != DYING)
	{
		g1_map_piece_class::damage(obj,hp,_damage_dir);
		if (health<20)
		{
			i4_float &roll_speed  = dest_x;
			i4_float &pitch_speed = dest_y;

			health = 20;
			set_flag(DANGEROUS,0);
			roll_speed  = 0;
			pitch_speed = 0;
			mode = DYING;
		}
	}
}
