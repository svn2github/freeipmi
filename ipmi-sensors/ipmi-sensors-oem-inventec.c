/*
 * Copyright (C) 2003-2015 FreeIPMI Core Team
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
#include <assert.h>
#include <errno.h>

#include <freeipmi/freeipmi.h>

#include "ipmi-sensors.h"
#include "ipmi-sensors-oem-inventec.h"
#include "ipmi-sensors-oem-inventec-5441.h"
#include "ipmi-sensors-oem-inventec-5442.h"

#include "freeipmi-portability.h"
#include "pstdout.h"
#include "tool-sensor-common.h"

/* return (0) - no OEM match
 * return (1) - OEM match
 * return (-1) - error, cleanup and return error
 */
int
ipmi_sensors_oem_inventec_output_oem_record (ipmi_sensors_state_data_t *state_data,
					     uint32_t oem_record_manufacturer_id,
					     const uint8_t *oem_data,
					     unsigned int oem_data_len)
{
  int ret;

  assert (state_data);
  assert (oem_data);
  assert (oem_data_len);
  assert (state_data->prog_data->args->verbose_count >= 2);
  assert (state_data->prog_data->args->interpret_oem_data);
  assert (state_data->oem_data.manufacturer_id == IPMI_IANA_ENTERPRISE_ID_INVENTEC);
  
  /*
   * Inventec 5441/Dell Xanadu II
   */
  if (state_data->oem_data.product_id == IPMI_INVENTEC_PRODUCT_ID_5441)
    {
      if ((ret = ipmi_sensors_oem_inventec_5441_output_oem_record (state_data,
								   oem_record_manufacturer_id,
								   oem_data,
								   oem_data_len)) < 0)
	return (-1);

      if (ret)
	return (1);
    }

  /*
   * Inventec 5442/Dell Xanadu III
   */
  if (state_data->oem_data.product_id == IPMI_INVENTEC_PRODUCT_ID_5442)
    {
      if ((ret = ipmi_sensors_oem_inventec_5442_output_oem_record (state_data,
								   oem_record_manufacturer_id,
								   oem_data,
								   oem_data_len)) < 0)
	return (-1);

      if (ret)
	return (1);
    }
  
  return (0);
}
