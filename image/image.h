/********************************************************************** <BR>
   This file is part of Crack dot Com's free source code release of
   Golgotha. <a href="http://www.crack.com/golgotha_release"> <BR> for
   information about compiling & licensing issues visit this URL</a>
   <PRE> If that doesn't help, contact Jonathan Clark at
   golgotha_source@usa.net (Subject should have "GOLG" in it)
 ***********************************************************************/

#ifndef __I4IMAGE_HPP_
#define __I4IMAGE_HPP_


class i4_pal;
class i4_rect_list_class;
class i4_draw_context_class;

#include "arch.h"
#include "rotation.h"

class i4_image_class
{
public:
	const i4_pal * pal;
	// raw data,
	// depends on pal.pal->source.pixel_depth==(I4_32BIT,I4_16BIT,I4_8BIT,I4_4BIT,I4_2BIT)
	w8 * data;
	int w, h, bpl;
	i4_bool dont_free_data;

	const i4_pal *get_pal()
	{
		return pal;
	}
	void set_pal(const i4_pal * which)
	{
		pal=which;
	}


	virtual w16 width()
	{
		return w;
	}
	virtual w16 height()
	{
		return h;
	}

	// these do color conversion to & from 32bit
	virtual i4_color get_pixel(i4_coord x, i4_coord y) = 0;
	virtual void put_pixel(i4_coord x, i4_coord y, w32 color) = 0;

	// put pixel does clipping and window offset moving and then calls put_pixel(x,y,c)
	void put_pixel(i4_coord x, i4_coord y, w32 color, i4_draw_context_class &context);

	w32  get_pixel(i4_coord x, i4_coord y,  i4_draw_context_class &context);

	virtual void xor_bar(i4_coord x1, i4_coord y1,
						 i4_coord x2, i4_coord y2,
						 i4_color xor_color, i4_draw_context_class &context);

	virtual void bar(i4_coord x1,    i4_coord y1,
					 i4_coord x2,    i4_coord y2,
					 i4_color color, i4_draw_context_class &context);

	virtual void line(i4_coord x1, i4_coord y1,
					  i4_coord x2, i4_coord y2,
					  i4_color color, i4_draw_context_class &context);

	void add_single_dirty(i4_coord x1, i4_coord y1, i4_coord x2,
						  i4_coord y2, i4_draw_context_class &context);

	void add_dirty(i4_coord x1, i4_coord y1, i4_coord x2,
				   i4_coord y2, i4_draw_context_class &context);


	//! Copies a part of this image to the destination image.
	//! put_part transfers a rectanguar image of yourself to location x,y on image 'to'
	//! \param to Target image
	//! \param _x Where to put the image relative to the target upper left corner, x coordinate
	//! \param _y y coordinate
	//! \param x1 Upper left corner of source rectangle
	//! \param y1 Upper left corner of source rectangle
	//! \param x2 Lower right corner of source rectangle
	//! \param y2 Lower right corner of source rectangle
	//! \param context The context to use (for clipping purposes)
	virtual void put_part(i4_image_class * to,
						  i4_coord _x,  i4_coord _y,
						  i4_coord x1, i4_coord y1, i4_coord x2, i4_coord y2,
						  i4_draw_context_class &context);

	virtual void put_part_trans(i4_image_class * to,
								i4_coord x,  i4_coord y,
								i4_coord x1, i4_coord y1, i4_coord x2, i4_coord y2,
								i4_color trans_color, i4_draw_context_class &context);

	// copy is not expanded by the image template, it needs to be implemented per image
	virtual i4_image_class *copy();

	//! Copies this image to a specific location in another image.
	//! Non-virtual functions, these are implemented as a convience
	//! not as a primative to be derived from
	//! \param to Target image
	//! \param x Where to put the image relative to the target upper left corner, x coordinate
	//! \param y y coordinate
	//! \param context The context to use
	void put_image(i4_image_class * to, i4_coord x, i4_coord y, i4_draw_context_class &context)
	{
		put_part(to,x,y,0,0,width()-1,height()-1,context);
	}

	void put_image(i4_image_class * to, i4_coord x, i4_coord y);

	void put_image_trans(i4_image_class * to,
						 i4_coord x, i4_coord y, i4_color trans_color,
						 i4_draw_context_class &context)
	{
		put_part_trans(to,x,y,0,0,width()-1,height()-1,trans_color,context);
	}

	virtual void clear(i4_color color, i4_draw_context_class &context)
	{
		bar(0,0,width()-1,height()-1,color,context);
	}

	virtual void rectangle(i4_coord x1, i4_coord y1, i4_coord x2, i4_coord y2, i4_color color,
						   i4_draw_context_class &context);
	void widget(i4_coord x1, i4_coord y1, i4_coord x2, i4_coord y2, i4_color bright, i4_color med, i4_color dark, i4_draw_context_class &context);


	//! Scales the current image to size (newx,newy).
	//! If to==0, a new image is created. if to is supplied, its size must match newx,newy.
	//! If to==0 and pal==0, the destination pal is same as source.
	//! If to==0 and pal!=0, the new pal is pal.
	//! If to!=0 pal is ignored
	i4_image_class *scale_image(i4_image_class * to,i4_coord newx, i4_coord newy,
								const i4_pal * pal);

	//! Rotates and mirrors the given image.
	//! This rotates the given image by 90, 180 or 270 degress and/or mirrors it about the
	//! diagonal axis.
	i4_image_class *rotate_image(g1_rotation_type rotation, i4_bool mirror);

	//! Copies this image to a specific location in image to, with a new size and possibly a new orientation.
	void copy_image_to(i4_image_class * to, i4_coord new_xpos, i4_coord new_ypos,
					   sw32 xsize, sw32 ysize, g1_rotation_type rotation, i4_bool mirror);

	i4_image_class()
	{
		dont_free_data=i4_F;
	}
	virtual ~i4_image_class()
	{
		;
	}
};

i4_image_class *i4_create_image(int width, int height, const i4_pal * pal);

i4_image_class *i4_create_image(int width, int height,
								const i4_pal * pal,
								void * data,
								int bytes_per_line);

#endif
