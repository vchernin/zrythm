/*
 * Copyright (C) 2019-2022 Alexandros Theodotou <alex at zrythm dot org>
 *
 * This file is part of Zrythm
 *
 * Zrythm is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Zrythm is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with Zrythm.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <math.h>
#include <stdlib.h>

#include "zrythm-config.h"
#include "audio/encoder.h"
#include "audio/engine.h"
#include "project.h"
#include "utils/objects.h"
#include "zrythm_app.h"

#include <gtk/gtk.h>

#include <samplerate.h>

/**
 * Creates a new instance of an AudioEncoder from
 * the given file, that can be used for encoding.
 *
 * It converts the file into a float array in its
 * original sample rate and prepares the instance
 * for decoding it into the project's sample rate.
 */
AudioEncoder *
audio_encoder_new_from_file (
  const char *    filepath)
{
  AudioEncoder * self = object_new (AudioEncoder);

  audec_set_log_level (AUDEC_LOG_LEVEL_DEBUG);
  audec_set_log_func (audio_audec_log_func);

  /* read info */
  self->audec_handle =
    audec_open (filepath, &self->nfo);
  if (!self->audec_handle)
    {
      g_critical (
        "An error has occurred opening the file %s",
        filepath);
      free (self);
      return NULL;
    }
  self->file = g_strdup (filepath);

  return self;
}

/**
 * Decodes the information in the AudioEncoder
 * instance and stores the results there.
 *
 * Resamples the input float array to match the
 * project's sample rate.
 *
 * Assumes that the input array is already filled in.
 *
 * @param show_progress Display a progress dialog.
 */
void
audio_encoder_decode (
  AudioEncoder * self,
  int            samplerate,
  bool           show_progress)
{
  g_message ("--audio decoding start--");

  self->out_frames = NULL;
  ssize_t num_out_frames =
    audec_read (
      self->audec_handle, &self->out_frames,
      samplerate);
  if (num_out_frames < 0)
    {
      g_critical (
        "An error has occurred during reading of "
        "the audio file %s", self->file);
    }
  self->num_out_frames =
    (unsigned_frame_t) num_out_frames;
  g_message (
    "num out frames %lu",
    self->num_out_frames);
  self->channels = self->nfo.channels;
  audec_close (self->audec_handle);
  g_message ("--audio decoding end--");
}

/**
 * Free's the AudioEncoder and its members.
 */
void
audio_encoder_free (
  AudioEncoder * self)
{
  if (self->out_frames)
    free (self->out_frames);
  if (self->file)
    g_free (self->file);

  free (self);
}
