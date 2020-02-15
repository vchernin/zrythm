/*
 * Copyright (C) 2019 Alexandros Theodotou <alex at zrythm dot org>
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

/**
 * \file
 *
 * Backend for faders or other volume/gain controls.
 */

#ifndef __AUDIO_FADER_H__
#define __AUDIO_FADER_H__

#include "audio/port.h"
#include "utils/types.h"
#include "utils/yaml.h"

typedef struct StereoPorts StereoPorts;
typedef struct Port Port;
typedef struct Channel Channel;

/**
 * @addtogroup audio
 *
 * @{
 */

typedef enum FaderType
{
  FADER_TYPE_NONE,

  /** Audio fader for the monitor. */
  FADER_TYPE_MONITOR,

  /** Audio fader for Channel's. */
  FADER_TYPE_AUDIO_CHANNEL,

  /* MIDI fader for Channel's. */
  FADER_TYPE_MIDI_CHANNEL,

  /** For generic uses. */
  FADER_TYPE_GENERIC,
} FaderType;

/**
 * A Fader is a processor that is used for volume
 * controls and pan.
 *
 * It does not necessarily have to correspond to
 * a FaderWidget. It can be used as a backend to
 * KnobWidget's.
 */
typedef struct Fader
{
  /**
   * Volume in dBFS. (-inf ~ +6)
   */
  float            volume;

  /** Used by the phase knob (0.0 ~ 360.0). */
  float            phase;

  /** 0.0 ~ 1.0 for widgets. */
  float            fader_val;

  /**
   * A control port that controls the volume in
   * amplitude (0.0 ~ 1.5)
   */
  Port *           amp;

  /** A control Port that controls the balance
   * (0.0 ~ 1.0) 0.5 is center. */
  Port *           balance;

  /**
   * L & R audio input ports, if audio.
   */
  StereoPorts *    stereo_in;

  /**
   * L & R audio output ports, if audio.
   */
  StereoPorts *    stereo_out;

  /**
   * MIDI in port, if MIDI.
   */
  Port *           midi_in;

  /**
   * MIDI out port, if MIDI.
   */
  Port *           midi_out;

  /**
   * Current dBFS after procesing each output port.
   *
   * Transient variables only used by the GUI.
   */
  float            l_port_db;
  float            r_port_db;

  FaderType        type;

  /** Track position, if channel fader. */
  int              track_pos;
} Fader;

static const cyaml_strval_t
fader_type_strings[] =
{
  { "none",           FADER_TYPE_NONE    },
  { "monitor channel", FADER_TYPE_MONITOR   },
  { "audio channel",  FADER_TYPE_AUDIO_CHANNEL   },
  { "midi channel",   FADER_TYPE_MIDI_CHANNEL   },
  { "generic",        FADER_TYPE_GENERIC   },
};

static const cyaml_schema_field_t
fader_fields_schema[] =
{
  CYAML_FIELD_ENUM (
    "type", CYAML_FLAG_DEFAULT,
    Fader, type, fader_type_strings,
    CYAML_ARRAY_LEN (fader_type_strings)),
  CYAML_FIELD_FLOAT (
    "volume", CYAML_FLAG_DEFAULT,
    Fader, volume),
  CYAML_FIELD_MAPPING_PTR (
    "amp",
    CYAML_FLAG_POINTER,
    Fader, amp, port_fields_schema),
  CYAML_FIELD_FLOAT (
    "phase", CYAML_FLAG_DEFAULT,
    Fader, phase),
  CYAML_FIELD_MAPPING_PTR (
    "balance",
    CYAML_FLAG_POINTER,
    Fader, balance, port_fields_schema),
  CYAML_FIELD_MAPPING_PTR (
    "midi_in",
    CYAML_FLAG_POINTER | CYAML_FLAG_OPTIONAL,
    Fader, midi_in,
    port_fields_schema),
  CYAML_FIELD_MAPPING_PTR (
    "midi_out",
    CYAML_FLAG_POINTER | CYAML_FLAG_OPTIONAL,
    Fader, midi_out,
    port_fields_schema),
  CYAML_FIELD_MAPPING_PTR (
    "stereo_in",
    CYAML_FLAG_POINTER | CYAML_FLAG_OPTIONAL,
    Fader, stereo_in,
    stereo_ports_fields_schema),
  CYAML_FIELD_MAPPING_PTR (
    "stereo_out",
    CYAML_FLAG_POINTER | CYAML_FLAG_OPTIONAL,
    Fader, stereo_out,
    stereo_ports_fields_schema),
  CYAML_FIELD_INT (
    "track_pos", CYAML_FLAG_DEFAULT,
    Fader, track_pos),

  CYAML_FIELD_END
};

static const cyaml_schema_value_t
fader_schema =
{
  CYAML_VALUE_MAPPING (
    CYAML_FLAG_POINTER,
    Fader, fader_fields_schema),
};

/**
 * Inits fader after a project is loaded.
 */
void
fader_init_loaded (
  Fader * self);

/**
 * Inits fader to default values.
 *
 * @param self The Fader to init.
 * @param type The FaderType.
 * @param ch Channel, if this is a channel Fader.
 */
void
fader_init (
  Fader * self,
  FaderType type,
  Channel * ch);

/**
 * Sets the amplitude of the fader. (0.0 to 2.0)
 */
void
fader_set_amp (
  void * self,
  float   amp);

/**
 * Adds (or subtracts if negative) to the amplitude
 * of the fader (clamped at 0.0 to 2.0).
 */
void
fader_add_amp (
  void * self,
  float   amp);

/**
 * Gets the fader amplitude (not db)
 * FIXME is void * necessary? do it in the caller.
 */
float
fader_get_amp (
  void * self);

float
fader_get_fader_val (
  void * self);

Channel *
fader_get_channel (
  Fader * self);

Track *
fader_get_track (
  Fader * self);

void
fader_update_volume_and_fader_val (
  Fader * self);

/**
 * Clears all buffers.
 */
void
fader_clear_buffers (
  Fader * self);

/**
 * Sets the fader levels from a normalized value
 * 0.0-1.0 (such as in widgets).
 */
void
fader_set_fader_val (
  Fader * self,
  float   fader_val);

/**
 * Disconnects all ports connected to the fader.
 */
void
fader_disconnect_all (
  Fader * self);

/**
 * Copy the fader values from source to dest.
 *
 * Used when cloning channels.
 */
void
fader_copy_values (
  Fader * src,
  Fader * dest);

/**
 * Process the Fader.
 *
 * @param nframes The number of frames to process.
 */
void
fader_process (
  Fader *         self,
  nframes_t local_offset,
  const nframes_t nframes);

/**
 * @}
 */

#endif
