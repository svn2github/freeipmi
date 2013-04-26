/*
 * Copyright (C) 2005-2013 FreeIPMI Core Team
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

#ifndef IPMI_RAW_ARGP_H
#define IPMI_RAW_ARGP_H

#include "ipmi-raw.h"

void ipmi_raw_argp_parse (int argc, char **argv, struct ipmi_raw_arguments *cmd_args);

#endif /* IPMI_RAW_ARGP_H */
