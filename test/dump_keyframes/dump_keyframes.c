/*****************************************************************************
 * Copyright 2008. Pittsburgh Pattern Recognition, Inc.
 *
 * This file is part of the Frame Accurate Seeking extension library to
 * ffmpeg (ffmpeg-fas).
 *
 * ffmpeg-fas is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * The ffmpeg-fas library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with the ffmpeg-fas library.  If not, see <http://www.gnu.org/licenses/>.
 *
 ******************************************************************************/

#include "ffmpeg_fas.h"
#include "seek_indices.h"
#include "test_support.h"
#include <stdio.h>

static char * cmdname;

void show_help()
{
    fprintf (stderr, "usage: %s <video_file> <seek_table> [output_dir]\n", cmdname);
}

int main (int argc, char **argv)
{
  char * out_dir;
  char out_file[128];
  int i;
  seek_table_type table;
  fas_error_type video_error;
  fas_context_ref_type context, seek_context;

  cmdname = argv[0];

  if (argc < 3) {
    show_help();
    fail("arguments\n");
  }

  table = read_table_file(argv[2]);
  if (table.num_entries == 0)
    fail("bad table\n");

  fas_initialize (FAS_FALSE, FAS_RGB24);

  video_error = fas_open_video (&context, argv[1]);
  if (video_error != FAS_SUCCESS)    fail("fail on open\n");

  video_error = fas_put_seek_table(context, table);
  if (video_error != FAS_SUCCESS)    fail("fail on put_seek_table\n");

  video_error = fas_open_video (&seek_context, argv[1]);
  if (video_error != FAS_SUCCESS)    fail("fail on open\n");


  if (argc >= 4) {
    out_dir = argv[3];
    create_dir(out_dir);
  } else {
    out_dir = ".";
  }

  for(i=0;i<table.num_entries;i++)
    {
      fas_raw_image_type image_buffer;
      //      int frame_index = table.array[table.num_entries - i - 1].display_index;
      int frame_index = table.array[i].display_index;
      if (FAS_SUCCESS != fas_seek_to_frame(context, frame_index))
	fail("failed on seek");

      if (FAS_SUCCESS != fas_get_frame (context, &image_buffer))
	fail("failed on rgb image\n");

      sprintf(out_file, "%s/frame_%04d.ppm", out_dir, frame_index);

      fprintf(stderr, "Writing %s (seek_table_value=%d frame_index=%d)\n", out_file, frame_index, fas_get_frame_index(context));
      ppm_save(&image_buffer, out_file);

      fas_free_frame (image_buffer);
    }

  success();
}

