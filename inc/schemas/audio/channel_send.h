/*
 * Copyright (C) 2020-2021 Alexandros Theodotou <alex at zrythm dot org>
 *
 * This file is part of Zrythm
 *
 * Zrythm is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, version 3.
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
 * Channel send schema.
 */

#ifndef __SCHEMAS_AUDIO_CHANNEL_SEND_H__
#define __SCHEMAS_AUDIO_CHANNEL_SEND_H__

#include <stdbool.h>

#include "schemas/audio/port_identifier.h"
#include "utils/yaml.h"

typedef struct ChannelSend_v1
{
  int            schema_version;
  int            track_pos;
  int            slot;
  float          amount;
  bool           on;
  bool           is_empty;
  bool           is_sidechain;
  PortIdentifier_v1 dest_l_id;
  PortIdentifier_v1 dest_r_id;
  PortIdentifier_v1 dest_midi_id;
} ChannelSend_v1;

static const cyaml_schema_field_t
channel_send_fields_schema_v1[] =
{
  YAML_FIELD_INT (ChannelSend_v1, schema_version),
  YAML_FIELD_INT (ChannelSend_v1, track_pos),
  YAML_FIELD_INT (ChannelSend_v1, slot),
  YAML_FIELD_FLOAT (ChannelSend_v1, amount),
  YAML_FIELD_INT (ChannelSend_v1, on),
  YAML_FIELD_INT (ChannelSend_v1, is_empty),
  YAML_FIELD_INT (ChannelSend_v1, is_sidechain),
  YAML_FIELD_MAPPING_EMBEDDED (
    ChannelSend_v1, dest_l_id,
    port_identifier_fields_schema_v1),
  YAML_FIELD_MAPPING_EMBEDDED (
    ChannelSend_v1, dest_r_id,
    port_identifier_fields_schema_v1),
  YAML_FIELD_MAPPING_EMBEDDED (
    ChannelSend_v1, dest_midi_id,
    port_identifier_fields_schema_v1),

  CYAML_FIELD_END
};

static const cyaml_schema_value_t
channel_send_schema_v1 =
{
  YAML_VALUE_DEFAULT (
    ChannelSend_v1, channel_send_fields_schema_v1),
};

#endif