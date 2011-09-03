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
#include "test_support.h"
#include <stdio.h>

static char * cmdname;

void show_help()
{
    fprintf (stderr, "usage: %s <video_file> [output_dir]\n", cmdname);
}

int main (int argc, char **argv)
{
  int counter = 0;

  char out_file[128];
  char * out_dir;

  fas_error_type video_error;
  fas_context_ref_type context;
  fas_raw_image_type image_buffer;

  cmdname = argv[0];

  if (argc < 2) {
    show_help();
    return -1;
  }

  fas_initialize (FAS_FALSE, FAS_RGB24);

  fprintf(stderr, "Opening %s : ",argv[1]);
  video_error = fas_open_video (&context, argv[1]);
  if (video_error != FAS_SUCCESS)
    fail("failed to open\n");

  fprintf(stderr, "OK. \n");


  if (argc >= 3) {
    out_dir = argv[2];
    create_dir(out_dir);
  } else {
    out_dir = ".";
  }

  while (fas_frame_available (context))
  {


    if (FAS_SUCCESS != fas_get_frame (context, &image_buffer))
      fail("failed on rgb image\n");

    sprintf(out_file, "%s/frame_%04d.ppm", out_dir, counter);

    fprintf(stderr, "Writing %s (counter=%d frame_index=%d)\n", out_file, counter, fas_get_frame_index(context));
    ppm_save(&image_buffer, out_file);

    fas_free_frame (image_buffer);

    video_error = fas_step_forward (context);
    counter++;
  }

  success();

}
