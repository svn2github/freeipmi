/*
 * Copyright (C) 2008-2011 FreeIPMI Core Team
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <stdlib.h>
#if STDC_HEADERS
#include <string.h>
#endif /* STDC_HEADERS */
#if HAVE_ARGP_H
#include <argp.h>
#else /* !HAVE_ARGP_H */
#include "freeipmi-argp.h"
#endif /* !HAVE_ARGP_H */
#include <errno.h>

#include "ipmi-oem.h"
#include "ipmi-oem-argp.h"

#include "freeipmi-portability.h"
#include "tool-cmdline-common.h"
#include "tool-config-file-common.h"

const char *argp_program_version =
  "ipmi-oem - " PACKAGE_VERSION "\n"
  "Copyright (C) 2008-2011 FreeIPMI Core Team\n"
  "This program is free software; you may redistribute it under the terms of\n"
  "the GNU General Public License.  This program has absolutely no warranty.";

const char *argp_program_bug_address =
  "<" PACKAGE_BUGREPORT ">";

static char cmdline_doc[] =
  "ipmi-oem - IPMI OEM utility";

static char cmdline_args_doc[] =
  "<OEMID> <OEMCOMMAND> [OEMOPTION...]";

static struct argp_option cmdline_options[] =
  {
    ARGP_COMMON_OPTIONS_DRIVER,
    ARGP_COMMON_OPTIONS_INBAND,
    ARGP_COMMON_OPTIONS_OUTOFBAND_HOSTRANGED,
    ARGP_COMMON_OPTIONS_AUTHENTICATION_TYPE,
    ARGP_COMMON_OPTIONS_CIPHER_SUITE_ID,
    ARGP_COMMON_OPTIONS_PRIVILEGE_LEVEL,
    ARGP_COMMON_OPTIONS_CONFIG_FILE,
    ARGP_COMMON_OPTIONS_WORKAROUND_FLAGS,
    ARGP_COMMON_SDR_OPTIONS,
    ARGP_COMMON_HOSTRANGED_OPTIONS,
    ARGP_COMMON_OPTIONS_DEBUG,
    { "list", LIST_KEY, 0, 0,
      "List supported OEM IDs and Commands.", 30},
    { "verbose", VERBOSE_KEY, 0, 0,
      "Increase verbosity in output.  May be specified multiple times.", 31},
    { NULL, 0, NULL, 0, NULL, 0}
  };

static error_t cmdline_parse (int key, char *arg, struct argp_state *state);

static struct argp cmdline_argp = { cmdline_options,
                                    cmdline_parse,
                                    cmdline_args_doc,
                                    cmdline_doc };

static struct argp cmdline_config_file_argp = { cmdline_options,
                                                cmdline_config_file_parse,
                                                cmdline_args_doc,
                                                cmdline_doc };

static error_t
cmdline_parse (int key, char *arg, struct argp_state *state)
{
  struct ipmi_oem_arguments *cmd_args = state->input;
  error_t ret;

  switch (key)
    {
    case LIST_KEY:
      cmd_args->list = 1;
      break;
    case VERBOSE_KEY:
      cmd_args->verbose_count++;
      break;
    case ARGP_KEY_ARG:
      {
        if (!cmd_args->oem_id)
          {
            if (!(cmd_args->oem_id = strdup (arg)))
              {
                perror ("strdup");
                exit (1);
              }
            break;
          }
        else if (!cmd_args->oem_command)
          {
            if (!(cmd_args->oem_command = strdup (arg)))
              {
                perror ("strdup");
                exit (1);
              }
            break;
          }
        else
          {
            if (cmd_args->oem_options_count < IPMI_OEM_MAX_ARGS)
              {
                if (!(cmd_args->oem_options[cmd_args->oem_options_count] = strdup (arg)))
                  {
                    perror ("strdup");
                    exit (1);
                  }
                cmd_args->oem_options_count++;
                break;
              }
	    else
	      {
		fprintf (stderr, "Too many arguments specified\n");
		exit (1);
	      }
          }
        break;
      }
    case ARGP_KEY_END:
      break;
    default:
      ret = common_parse_opt (key, arg, &(cmd_args->common));
      if (ret == ARGP_ERR_UNKNOWN) 
        ret = sdr_parse_opt (key, arg, &(cmd_args->sdr));
      if (ret == ARGP_ERR_UNKNOWN)
        ret = hostrange_parse_opt (key, arg, &(cmd_args->hostrange));
      return (ret);
    }

  return (0);
}

static void
_ipmi_oem_config_file_parse (struct ipmi_oem_arguments *cmd_args)
{
  struct config_file_data_ipmi_oem config_file_data;

  memset (&config_file_data,
          '\0',
          sizeof (struct config_file_data_ipmi_oem));

  if (config_file_parse (cmd_args->common.config_file,
                         0,
                         &(cmd_args->common),
                         &(cmd_args->sdr),
                         &(cmd_args->hostrange),
                         CONFIG_FILE_INBAND | CONFIG_FILE_OUTOFBAND | CONFIG_FILE_SDR | CONFIG_FILE_HOSTRANGE,
                         CONFIG_FILE_TOOL_IPMI_OEM,
                         &config_file_data) < 0)
    {
      fprintf (stderr, "config_file_parse: %s\n", strerror (errno));
      exit (1);
    }

  if (config_file_data.verbose_count_count)
    cmd_args->verbose_count = config_file_data.verbose_count;
}

void
ipmi_oem_argp_parse (int argc, char **argv, struct ipmi_oem_arguments *cmd_args)
{
  init_common_cmd_args_admin (&(cmd_args->common));
  init_sdr_cmd_args (&(cmd_args->sdr));
  init_hostrange_cmd_args (&(cmd_args->hostrange));

  cmd_args->list = 0;
  cmd_args->verbose_count = 0;
  cmd_args->oem_id = NULL;
  cmd_args->oem_command = NULL;
  memset (cmd_args->oem_options,
	  '\0',
	  sizeof (char *) * IPMI_OEM_MAX_ARGS);
  cmd_args->oem_options_count = 0;

  argp_parse (&cmdline_config_file_argp,
              argc,
              argv,
              ARGP_IN_ORDER,
              NULL,
              &(cmd_args->common));

  _ipmi_oem_config_file_parse (cmd_args);

  argp_parse (&cmdline_argp,
              argc,
              argv,
              ARGP_IN_ORDER,
              NULL,
              cmd_args);

  verify_common_cmd_args (&(cmd_args->common));
  verify_hostrange_cmd_args (&(cmd_args->hostrange));
}


