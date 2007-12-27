/* 
   Copyright (C) 2003, 2004, 2005 FreeIPMI Core Team

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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>

#include "freeipmi/spec/ipmi-sensor-types-spec.h"

const char *const ipmi_sensor_types[] = 
  {
    "Reserved", 
    "Temperature",
    "Voltage",
    "Current",
    "Fan",
    "Platform Chassis Intrusion",
    "Platform Security Violation",
    "Processor",
    "Power Supply",
    "Power Unit",
    "Cooling Device",
    "FRU Sensor",
    "Memory",
    "Drive Slot",
    "Post Memory Resize",
    "System Firmware",
    "Event Logging Disabled",
    "Watchdog 1",
    "System Event",
    "Critical Interrupt",
    "Button",
    "Module/Board",
    "Microcontroller",
    "Add In Card",
    "Chassis",
    "Chip Set",
    "Other Fru",
    "Cable Interconnect",
    "Terminator",
    "System Boot Initiated",
    "Boot Error",
    "OS Boot",
    "OS Critical Stop",
    "Slot Connector",
    "ACPI Power State",
    "Watchdog 2",
    "Platform Alert",
    "Entity Presence",
    "Monitor Asic",
    "LAN",
    "Management Subsystem Health",
    "Battery",
    "Session Audit",
    "Version Change",
    "FRU State",
    NULL
  };

const char *const ipmi_oem_sensor_type = "OEM Reserved";

