/*
  Copyright (C) 2003-2009 FreeIPMI Core Team

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2, or (at your option)
  any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software Foundation,
  Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA.
*/

#ifndef _IPMI_SENSORS_H
#define _IPMI_SENSORS_H

#include <freeipmi/freeipmi.h>

#include "tool-cmdline-common.h"
#include "tool-sdr-cache-common.h"
#include "tool-sensor-common.h"
#include "pstdout.h"

enum ipmi_sensors_argp_option_keys
  {
    VERBOSE_KEY = 'v',
    QUIET_READINGS_KEY = 'q',
    SDR_INFO_KEY = 'i',
    LIST_GROUPS_KEY = 'L',
    GROUP_KEY = 160,              /* legacy */
    GROUPS_KEY = 'g',
    EXCLUDE_GROUPS_KEY = 161,
    SENSORS_KEY = 's',          /* legacy */
    RECORD_IDS_KEY = 'r',
    BRIDGE_SENSORS_KEY = 'b',
    INTERPRET_OEM_DATA = 162,
    COMMA_SEPARATED_OUTPUT_KEY = 163,
    NON_ABBREVIATED_UNITS_KEY = 164,
    LEGACY_OUTPUT_KEY = 165,
  };

struct ipmi_sensors_arguments
{
  struct common_cmd_args common;
  struct sdr_cmd_args sdr;
  struct hostrange_cmd_args hostrange;
  int verbose_count;
  int quiet_readings;
  int sdr_info;
  int list_groups;
  char groups[MAX_SENSOR_GROUPS][MAX_SENSOR_GROUPS_STRING_LENGTH+1];
  unsigned int groups_length;
  char exclude_groups[MAX_SENSOR_GROUPS][MAX_SENSOR_GROUPS_STRING_LENGTH+1];
  unsigned int exclude_groups_length;
  unsigned int record_ids[MAX_SENSOR_RECORD_IDS];
  unsigned int record_ids_length;
  int bridge_sensors;
  int interpret_oem_data;
  int comma_separated_output;
  int non_abbreviated_units;
  int legacy_output;
};

typedef struct ipmi_sensors_prog_data
{
  char *progname;
  struct ipmi_sensors_arguments *args;
} ipmi_sensors_prog_data_t;

typedef struct ipmi_sensors_state_data
{
  ipmi_sensors_prog_data_t *prog_data;
  ipmi_ctx_t ipmi_ctx;
  pstdout_state_t pstate;
  char *hostname;
  ipmi_sdr_cache_ctx_t sdr_cache_ctx;
  ipmi_sdr_parse_ctx_t sdr_parse_ctx;
  ipmi_sensor_read_ctx_t sensor_read_ctx;
  int output_headers;
  struct sensor_column_width column_width;
  uint32_t manufacturer_id;
  uint16_t product_id;
} ipmi_sensors_state_data_t;

#endif
