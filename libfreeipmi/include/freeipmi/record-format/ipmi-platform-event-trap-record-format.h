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

#ifndef _IPMI_PLATFORM_EVENT_TRAP_RECORD_FORMAT_H
#define _IPMI_PLATFORM_EVENT_TRAP_RECORD_FORMAT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <freeipmi/fiid/fiid.h>

/*
 * achu:
 *
 * Not in IPMI spec.  In "Platform Event Trap Specification" document.
 */

#define IPMI_PLATFORM_EVENT_TRAP_MIN_VARIABLE_BINDINGS_LENGTH 47
#define IPMI_PLATFORM_EVENT_TRAP_MAX_VARIABLE_BINDINGS_LENGTH 110

#define IPMI_PLATFORM_EVENT_TRAP_SPECIFIC_TRAP_SENSOR_TYPE_MASK  0x00FF0000
#define IPMI_PLATFORM_EVENT_TRAP_SPECIFIC_TRAP_SENSOR_TYPE_SHIFT 16

#define IPMI_PLATFORM_EVENT_TRAP_SPECIFIC_TRAP_EVENT_TYPE_MASK  0x0000FF00
#define IPMI_PLATFORM_EVENT_TRAP_SPECIFIC_TRAP_EVENT_TYPE_SHIFT 8

#define IPMI_PLATFORM_EVENT_TRAP_SPECIFIC_TRAP_EVENT_DIRECTION_MASK  0x00000080
#define IPMI_PLATFORM_EVENT_TRAP_SPECIFIC_TRAP_EVENT_DIRECTION_SHIFT 7

#define IPMI_PLATFORM_EVENT_TRAP_SPECIFIC_TRAP_EVENT_OFFSET_MASK  0x0000000F
#define IPMI_PLATFORM_EVENT_TRAP_SPECIFIC_TRAP_EVENT_OFFSET_SHIFT 0
#define IPMI_PLATFORM_EVENT_TRAP_SPECIFIC_TRAP_EVENT_OFFSET_UNSPECIFIED 0x0F

#define IPMI_PLATFORM_EVENT_TRAP_VARIABLE_BINDINGS_GUID_INDEX_START 0
#define IPMI_PLATFORM_EVENT_TRAP_VARIABLE_BINDINGS_GUID_INDEX_END   15

#define IPMI_PLATFORM_EVENT_TRAP_VARIABLE_BINDINGS_SEQUENCE_NUMBER_INDEX_START 16
#define IPMI_PLATFORM_EVENT_TRAP_VARIABLE_BINDINGS_SEQUENCE_NUMBER_INDEX_END   17

#define IPMI_PLATFORM_EVENT_TRAP_VARIABLE_BINDINGS_COOKIE_INDEX_START IPMI_PLATFORM_EVENT_TRAP_VARIABLE_BINDINGS_SEQUENCE_NUMBER_INDEX_START
#define IPMI_PLATFORM_EVENT_TRAP_VARIABLE_BINDINGS_COOKIE_INDEX_END   IPMI_PLATFORM_EVENT_TRAP_VARIABLE_BINDINGS_SEQUENCE_NUMBER_INDEX_END

#define IPMI_PLATFORM_EVENT_TRAP_VARIABLE_BINDINGS_LOCAL_TIMESTAMP_INDEX_START 18
#define IPMI_PLATFORM_EVENT_TRAP_VARIABLE_BINDINGS_LOCAL_TIMESTAMP_INDEX_END   21
#define IPMI_PLATFORM_EVENT_TRAP_VARIABLE_BINDINGS_LOCAL_TIMESTAMP_UNSPECIFIED 0x0000 

#define IPMI_PLATFORM_EVENT_TRAP_VARIABLE_BINDINGS_UTC_OFFSET_INDEX_START 22
#define IPMI_PLATFORM_EVENT_TRAP_VARIABLE_BINDINGS_UTC_OFFSET_INDEX_END   23
#define IPMI_PLATFORM_EVENT_TRAP_VARIABLE_BINDINGS_UTC_OFFSET_UNSPECIFIED 0xFFFF

#define IPMI_PLATFORM_EVENT_TRAP_VARIABLE_BINDINGS_TRAP_SOURCE_TYPE_INDEX 24
#define IPMI_PLATFORM_EVENT_TRAP_VARIABLE_BINDINGS_TRAP_SOURCE_UNSPECIFIED 0xFF

#define IPMI_PLATFORM_EVENT_TRAP_VARIABLE_BINDINGS_EVENT_SOURCE_TYPE_INDEX 25
#define IPMI_PLATFORM_EVENT_TRAP_VARIABLE_BINDINGS_EVENT_SOURCE_UNSPECIFIED 0xFF

#define IPMI_PLATFORM_EVENT_TRAP_VARIABLE_BINDINGS_EVENT_SEVERITY_INDEX 26
#define IPMI_PLATFORM_EVENT_TRAP_VARIABLE_BINDINGS_EVENT_SEVERITY_UNSPECIFIED 0x00

#define IPMI_PLATFORM_EVENT_TRAP_VARIABLE_BINDINGS_EVENT_SEVERITY_MONITOR                   0x01
#define IPMI_PLATFORM_EVENT_TRAP_VARIABLE_BINDINGS_EVENT_SEVERITY_INFORMATION               0x02
#define IPMI_PLATFORM_EVENT_TRAP_VARIABLE_BINDINGS_EVENT_SEVERITY_OK                        0x04
#define IPMI_PLATFORM_EVENT_TRAP_VARIABLE_BINDINGS_EVENT_SEVERITY_NON_CRITICAL_CONDITION    0x08
#define IPMI_PLATFORM_EVENT_TRAP_VARIABLE_BINDINGS_EVENT_SEVERITY_CRITICAL_CONDITION        0x10
#define IPMI_PLATFORM_EVENT_TRAP_VARIABLE_BINDINGS_EVENT_SEVERITY_NON_RECOVERABLE_CONDITION 0x20

#define IPMI_PLATFORM_EVENT_TRAP_VARIABLE_BINDINGS_SENSOR_DEVICE_INDEX 27
#define IPMI_PLATFORM_EVENT_TRAP_VARIABLE_BINDINGS_SENSOR_DEVICE_UNSPECIFIED 0xFF

#define IPMI_PLATFORM_EVENT_TRAP_VARIABLE_BINDINGS_SENSOR_NUMBER_INDEX 28
#define IPMI_PLATFORM_EVENT_TRAP_VARIABLE_BINDINGS_SENSOR_NUMBER_UNSPECIFIED_A 0x00 
#define IPMI_PLATFORM_EVENT_TRAP_VARIABLE_BINDINGS_SENSOR_NUMBER_UNSPECIFIED_B 0xFF

#define IPMI_PLATFORM_EVENT_TRAP_VARIABLE_BINDINGS_ENTITY_INDEX 29
#define IPMI_PLATFORM_EVENT_TRAP_VARIABLE_BINDINGS_ENTITY_UNSPECIFIED 0x00

#define IPMI_PLATFORM_EVENT_TRAP_VARIABLE_BINDINGS_ENTITY_INSTANCE_INDEX 30
#define IPMI_PLATFORM_EVENT_TRAP_VARIABLE_BINDINGS_ENTITY_INSTANCE_UNSPECIFIED 0x00

#define IPMI_PLATFORM_EVENT_TRAP_VARIABLE_BINDINGS_EVENT_DATA_INDEX_START 31
#define IPMI_PLATFORM_EVENT_TRAP_VARIABLE_BINDINGS_EVENT_DATA_INDEX_END   38
#define IPMI_PLATFORM_EVENT_TRAP_VARIABLE_BINDINGS_EVENT_DATA_LENGTH      8

#define IPMI_PLATFORM_EVENT_TRAP_VARIABLE_BINDINGS_LANGUAGE_CODE_INDEX 39

#define IPMI_PLATFORM_EVENT_TRAP_VARIABLE_BINDINGS_MANUFACTURER_ID_INDEX_START 40
#define IPMI_PLATFORM_EVENT_TRAP_VARIABLE_BINDINGS_MANUFACTURER_ID_INDEX_END   43

#define IPMI_PLATFORM_EVENT_TRAP_VARIABLE_BINDINGS_SYSTEM_ID_INDEX_START 44
#define IPMI_PLATFORM_EVENT_TRAP_VARIABLE_BINDINGS_SYSTEM_ID_INDEX_END   45

#define IPMI_PLATFORM_EVENT_TRAP_VARIABLE_BINDINGS_OEM_CUSTOM_FIELDS_INDEX_START 46
#define IPMI_PLATFORM_EVENT_TRAP_VARIABLE_BINDINGS_OEM_CUSTOM_FIELDS_INDEX_END   109
#define IPMI_PLATFORM_EVENT_TRAP_VARIABLE_BINDINGS_OEM_CUSTOM_FIELDS_LENGTH      64



#ifdef __cplusplus
}
#endif

#endif /* _IPMI_PLATFORM_EVENT_TRAP_RECORD_FORMAT_H */
