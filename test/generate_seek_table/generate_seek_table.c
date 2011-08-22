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

#include <stdio.h>
#include <string.h>
#include "ffmpeg_fas.h"
#include "seek_indices.h"

/* This executable is used for creating experimental seek tables.
   The show_seek_table executable will show the seek-table as ffmpeg_fas
   currently creates them.
 */

int main (int argc, char **argv)
{
  //av_log_level = AV_LOG_QUIET;

  seek_table_type table;

  if (argc < 2) {
    fprintf (stderr, "usage: %s <video_file>\n", argv[0]);
    return -1;
  }

  //we just need this to init avcodec....
  fas_initialize (FAS_FALSE, FAS_RGB24);

  table = seek_init_table(16);
  generate_seek_table(argv[1], &table);

  return 1;
}
