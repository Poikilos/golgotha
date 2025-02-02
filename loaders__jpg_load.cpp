/********************************************************************** <BR>
   This file is part of Crack dot Com's free source code release of
   Golgotha. <a href="http://www.crack.com/golgotha_release"> <BR> for
   information about compiling & licensing issues visit this URL</a>
   <PRE> If that doesn't help, contact Jonathan Clark at
   golgotha_source@usa.net (Subject should have "GOLG" in it)
 ***********************************************************************/
#include "pch.h"
//#define JPEG_INTERNALS
//TODO clean up memory management
//convert types and memory and file stuff to I4
#include "loaders/load.h"
#include "loaders/jpg_load.h"
#include "time/profile.h"
#include "memory/malloc.h"
#include "palette/pal.h"
#include "status/status.h"

//i4_profile_class pf_jpg_scanline_decode("jpg_scanline_decode");
i4_profile_class pf_jpg_decode("jpg_decode");


w16 jpg_r_and;
w8 jpg_r_shift;                //r and g shifts are to the left
w16 jpg_g_and;
w8 jpg_g_shift;
w16 jpg_b_and;
w8 jpg_b_shift;                //b shift is to the right

void setup_pix_format(const i4_pixel_format * fmt)
{
	jpg_b_shift = (w8)(8-fmt->blue_bits);
	jpg_b_and   = (w8)(fmt->blue_mask);

	jpg_g_shift = (w8)(fmt->blue_bits - (8-fmt->green_bits));
	jpg_g_and   = (w8)(fmt->green_mask);

	jpg_r_shift = (w8)(fmt->green_bits + fmt->blue_bits - (8-fmt->red_bits));
	jpg_r_and   = (w8)(fmt->red_mask);
}

LOCAL(unsigned int)
jpeg_getc(j_decompress_ptr cinfo) {
/* Read next byte */
	struct jpeg_source_mgr * datasrc = cinfo->src;

	if (datasrc->bytes_in_buffer == 0)
	{
		if (!(*datasrc->fill_input_buffer)(cinfo))
		{
			ERREXIT(cinfo, JERR_CANT_SUSPEND);
		}
	}
	datasrc->bytes_in_buffer--;
	return GETJOCTET(*datasrc->next_input_byte++);
}

METHODDEF(boolean)
COM_handler(j_decompress_ptr cinfo) {
	boolean traceit = (cinfo->err->trace_level >= 1);
	INT32 length;
	unsigned int ch;
	unsigned int lastch = 0;

	length = jpeg_getc(cinfo) << 8;
	length += jpeg_getc(cinfo);
	length -= 2;        /* discount the length word itself */

	if (traceit)
	{
		fprintf(stderr, "Comment, length %ld:\n", (long) length);
	}

	while (--length >= 0)
	{
		ch = jpeg_getc(cinfo);
		if (traceit)
		{
			/* Emit the character in a readable form.
			 * Nonprintables are converted to \nnn form,
			 * while \ is converted to \\.
			 * Newlines in CR, CR/LF, or LF form will be printed as one newline.
			 */
			if (ch == '\r')
			{
				fprintf(stderr, "\n");
			}
			else if (ch == '\n')
			{
				if (lastch != '\r')
				{
					fprintf(stderr, "\n");
				}
			}
			else if (ch == '\\')
			{
				fprintf(stderr, "\\\\");
			}
			else if (isprint(ch))
			{
				putc(ch, stderr);
			}
			else
			{
				fprintf(stderr, "\\%03o", ch);
			}
			lastch = ch;
		}
	}

	if (traceit)
	{
		fprintf(stderr, "\n");
	}

	return TRUE;
}

static inline void pack_colorbgr(w16 * dest, w8 * src, sw32 num_pixels)
{
	while (num_pixels)
	{
		//shifts must go first, ands last
		*dest = ((src[0] << jpg_r_shift) & jpg_r_and) |
				((src[1] << jpg_g_shift) & jpg_g_and) |
				((src[2] >> jpg_b_shift) & jpg_b_and);

		dest++;
		src += 3;

		num_pixels--;
	}
}

i4_bool i4_jpg_loader_class::convert_to_raw16(i4_file_class * src_fp,
											  FILE * dst_fp,
											  i4_status_class * status,
											  const i4_pal * pal,
											  sw32 expected_width,
											  sw32 expected_height)
{
	djpeg_dest_ptr dest_mgr = NULL;

	thread_sync->wait_signal();
	JDIMENSION num_scanlines;
	int i; //,x;

	//setup this conversion stuff
	setup_pix_format(&pal->source);

	/* Initialize the JPEG decompression object with default error handling. */
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);
	/* Add some application-specific error messages (from cderror.h) */
	jerr.addon_message_table = cdjpeg_message_table;
	jerr.first_addon_message = JMSG_FIRSTADDONCODE;
	jerr.last_addon_message = JMSG_LASTADDONCODE;
	/* Insert custom COM marker processor. */
	jpeg_set_marker_processor(&cinfo, JPEG_COM, COM_handler);
	/* Specify data source for decompression */
	jpeg_stdio_src(&cinfo, src_fp);

	/* Read file header, set default decompression parameters */
	(void) jpeg_read_header(&cinfo, TRUE);

	dest_mgr = jinit_write_targa(&cinfo);

	/* Start decompressor */
	(void) jpeg_start_decompress(&cinfo);

	w16 * data = (w16 *)I4_MALLOC(cinfo.image_width*2, "jpeg 16-bit buffer");

	w32 scn=0;
	/* Process data */
	while (cinfo.output_scanline < cinfo.output_height)
	{
		//pf_jpg_scanline_decode.start();
		num_scanlines = jpeg_read_scanlines(&cinfo, dest_mgr->buffer,dest_mgr->buffer_height);
		//pf_jpg_scanline_decode.stop();

		for(i=0; i<(int)num_scanlines; i++)
		{
			if (status)
			{
				status->update(i/(float)num_scanlines);
			}

			pack_colorbgr(data,dest_mgr->buffer[i],cinfo.image_width);
			fwrite(data,cinfo.image_width*2,1,dst_fp);
			/*
			   for(x=0;x<cinfo.image_width;x++)
			   {
			 *((w16 *)(data+x)) = pack_colorbgr((((w8 *)((dest_mgr->buffer[i])+(x*3)))));
			   }

			   scn += cinfo.image_width;
			 */
		}
	}

	/* Finish decompression and release memory.
	 * I must do it in this order because output module has allocated memory
	 * of lifespan JPOOL_IMAGE; it needs to finish before releasing memory.
	 */
	i4_free(data);

	(void) jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	thread_sync->signal();
	return i4_T;
}

i4_bool i4_jpg_loader_class::special_load16(i4_file_class * src_fp,
											w16 * dst_tex,
											sw32 base_width,
											const i4_pal * pal)
{
	pf_jpg_decode.start();
	thread_sync->wait_signal();
	djpeg_dest_ptr dest_mgr = NULL;
	JDIMENSION num_scanlines;
	int i; //,x;

	//setup this conversion stuff
	setup_pix_format(&pal->source);

	/* Initialize the JPEG decompression object with default error handling. */
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);
	/* Add some application-specific error messages (from cderror.h) */
	jerr.addon_message_table = cdjpeg_message_table;
	jerr.first_addon_message = JMSG_FIRSTADDONCODE;
	jerr.last_addon_message = JMSG_LASTADDONCODE;
	/* Insert custom COM marker processor. */
	jpeg_set_marker_processor(&cinfo, JPEG_COM, COM_handler);
	/* Specify data source for decompression */
	jpeg_stdio_src(&cinfo, src_fp);

	/* Read file header, set default decompression parameters */
	(void) jpeg_read_header(&cinfo, TRUE);

	dest_mgr = jinit_write_targa(&cinfo);

	/* Start decompressor */
	(void) jpeg_start_decompress(&cinfo);

	w16 * data = dst_tex;

	w32 scn=0;
	/* Process data */
	while (cinfo.output_scanline < cinfo.output_height)
	{
		//pf_jpg_scanline_decode.start();
		num_scanlines = jpeg_read_scanlines(&cinfo, dest_mgr->buffer,dest_mgr->buffer_height);
		//pf_jpg_scanline_decode.stop();

		for(i=0; i<(int)num_scanlines; i++,data += base_width)
		{
			pack_colorbgr(data,dest_mgr->buffer[i],cinfo.image_width);
		}
	}

	/* Finish decompression and release memory.
	 * I must do it in this order because output module has allocated memory
	 * of lifespan JPOOL_IMAGE; it needs to finish before releasing memory.
	 */

	(void) jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	thread_sync->signal();
	pf_jpg_decode.stop();
	return i4_T;
}

void i4_jpg_loader_class::init()
{
	i4_image_loader_class::init();
	thread_sync=new i4_signal_object("GOLGOTHA_JPG_LOADER_Thread_synchronizer_object1",1,1);
}

void i4_jpg_loader_class::uninit()
{
	delete thread_sync;
	thread_sync=0;
	i4_image_loader_class::uninit();
}

i4_bool i4_jpg_loader_class::special_load24(i4_file_class * src_fp,
											w8 * dst_tex,
											sw32 * dst_width,
											sw32 * dst_height)
{

	pf_jpg_decode.start();
	thread_sync->wait_signal();

	djpeg_dest_ptr dest_mgr = NULL;
	JDIMENSION num_scanlines;
	int i; //,x;

	/* Initialize the JPEG decompression object with default error handling. */
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);
	/* Add some application-specific error messages (from cderror.h) */
	jerr.addon_message_table = cdjpeg_message_table;
	jerr.first_addon_message = JMSG_FIRSTADDONCODE;
	jerr.last_addon_message = JMSG_LASTADDONCODE;
	/* Insert custom COM marker processor. */
	jpeg_set_marker_processor(&cinfo, JPEG_COM, COM_handler);
	/* Specify data source for decompression */
	jpeg_stdio_src(&cinfo, src_fp);

	/* Read file header, set default decompression parameters */
	(void) jpeg_read_header(&cinfo, TRUE);

	dest_mgr = jinit_write_targa(&cinfo);

	/* Start decompressor */
	(void) jpeg_start_decompress(&cinfo);

	w8 * data = dst_tex;
	sw32 data_add = cinfo.image_width*3;

	*dst_width  = cinfo.image_width;
	*dst_height = cinfo.image_height;

	/* Process data */
	while (cinfo.output_scanline < cinfo.output_height)
	{
		//pf_jpg_scanline_decode.start();
		num_scanlines = jpeg_read_scanlines(&cinfo, dest_mgr->buffer,dest_mgr->buffer_height);
		//pf_jpg_scanline_decode.stop();

		for(i=0; i<(int)num_scanlines; i++)
		{
			memcpy(data,dest_mgr->buffer[i],cinfo.image_width*3);
			data += data_add;
		}
	}

	/* Finish decompression and release memory.
	 * I must do it in this order because output module has allocated memory
	 * of lifespan JPOOL_IMAGE; it needs to finish before releasing memory.
	 */

	(void) jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);

	pf_jpg_decode.stop();
	thread_sync->signal();

	return i4_T;
}


i4_image_class * i4_jpg_loader_class::load(i4_file_class * fp,
										   i4_status_class * status)
{
	thread_sync->wait_signal();
	djpeg_dest_ptr dest_mgr = NULL;
	JDIMENSION num_scanlines;
	int i,x;

	// Initialize the JPEG decompression object with default error handling.
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);
	// Add some application-specific error messages (from cderror.h)
	jerr.addon_message_table = cdjpeg_message_table;
	jerr.first_addon_message = JMSG_FIRSTADDONCODE;
	jerr.last_addon_message = JMSG_LASTADDONCODE;
	// Insert custom COM marker processor.
	jpeg_set_marker_processor(&cinfo, JPEG_COM, COM_handler);

	/* Specify data source for decompression */
	jpeg_stdio_src(&cinfo, fp);

	/* Read file header, set default decompression parameters */
	(void) jpeg_read_header(&cinfo, TRUE);

	dest_mgr = jinit_write_targa(&cinfo);

	/* Start decompressor */
	(void) jpeg_start_decompress(&cinfo);


	w32 * data=(w32 *) new w8[cinfo.image_width*cinfo.image_height*4]; //(w32 *)I4_MALLOC(cinfo.image_width*cinfo.image_height*4,"jpeg image");
	w32 scn=0;
	/* Process data */
	while (cinfo.output_scanline < cinfo.output_height)
	{
		num_scanlines = jpeg_read_scanlines(&cinfo, dest_mgr->buffer,
											dest_mgr->buffer_height);
		for(i=0; i<(int)num_scanlines; i++)
		{
			if (status)
			{
				status->update(i/(float)num_scanlines);
			}



			for(x=0; x<(int)cinfo.image_width; x++)
			{
				*((w32 *)(data+x+scn))=read_colorbgr((((w8 *)((dest_mgr->buffer[i])+(x*3)))));
			}
			scn+=cinfo.image_width;
			//i4_thread_yield();
		}
	}

	/* Finish decompression and release memory.
	 * I must do it in this order because output module has allocated memory
	 * of lifespan JPOOL_IMAGE; it needs to finish before releasing memory.
	 */
	(void) jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);


	i4_pixel_format fmt;
	fmt.default_format();

	fmt.alpha_mask=0;         // no alpha in jpegs?
	fmt.calc_shift();


	const i4_pal * pal=i4_pal_man.register_pal(&fmt);

	i4_image_class * im=i4_create_image(
		cinfo.image_width,
		cinfo.image_height,           // width & height
		pal,                        // palette (should be 32 bit by default)
		data,
		cinfo.image_width*4);                       // bytes per line

	im->dont_free_data=i4_F;

	thread_sync->signal();
	return im;
}


class i4_jpg_loader_class i4_jpg_loader_instance;
