/*
 * Copyright (C) 2003-2012 FreeIPMI Core Team
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <stdlib.h>
#ifdef STDC_HEADERS
#include <string.h>
#endif /* STDC_HEADERS */
#include <assert.h>
#include <errno.h>

#include "freeipmi/sel-parse/ipmi-sel-parse.h"
#include "freeipmi/fiid/fiid.h"

#include "ipmi-sel-parse-defs.h"
#include "ipmi-sel-parse-trace.h"
#include "ipmi-sel-parse-util.h"

#include "freeipmi-portability.h"

void
sel_parse_set_sel_parse_errnum_by_errno (ipmi_sel_parse_ctx_t ctx, int __errno)
{
  assert (ctx && ctx->magic == IPMI_SEL_PARSE_CTX_MAGIC);
  
  switch (__errno)
    {
    case 0:
      ctx->errnum = IPMI_SEL_PARSE_ERR_SUCCESS;
      break;
    case ENOMEM:
      ctx->errnum = IPMI_SEL_PARSE_ERR_OUT_OF_MEMORY;
      break;
    default:
      ctx->errnum = IPMI_SEL_PARSE_ERR_INTERNAL_ERROR;
    }
}

void
sel_parse_set_sel_parse_errnum_by_fiid_object (ipmi_sel_parse_ctx_t ctx, fiid_obj_t obj)
{
  assert (ctx && ctx->magic == IPMI_SEL_PARSE_CTX_MAGIC);
  
  switch (fiid_obj_errnum (obj))
    {
    case FIID_ERR_SUCCESS:
      ctx->errnum = IPMI_SEL_PARSE_ERR_SUCCESS;
      break;
    case FIID_ERR_OUT_OF_MEMORY:
      ctx->errnum = IPMI_SEL_PARSE_ERR_OUT_OF_MEMORY;
      break;
    case FIID_ERR_DATA_NOT_AVAILABLE:
      ctx->errnum = IPMI_SEL_PARSE_ERR_SYSTEM_ERROR;
      break;
    case FIID_ERR_FIELD_NOT_FOUND:
    case FIID_ERR_DATA_NOT_BYTE_ALIGNED:
    case FIID_ERR_REQUIRED_FIELD_MISSING:
    case FIID_ERR_FIXED_LENGTH_FIELD_INVALID:
    case FIID_ERR_NOT_IDENTICAL:
      ctx->errnum = IPMI_SEL_PARSE_ERR_PARAMETERS;
      break;
    default:
      ctx->errnum = IPMI_SEL_PARSE_ERR_INTERNAL_ERROR;
    }
}
