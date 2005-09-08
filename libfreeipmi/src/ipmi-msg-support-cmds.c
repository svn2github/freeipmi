/* 
   ipmi-msg-support-cmds.c - IPMI Message Support Commands

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
   Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  
*/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* AIX requires this to be the first thing in the file.  */
#ifndef __GNUC__
# if HAVE_ALLOCA_H
#  include <alloca.h>
# else
#  ifdef _AIX
 #pragma alloca
#  else
#   ifndef alloca /* predefined by HP cc +Olibcalls */
char *alloca ();
#   endif
#  endif
# endif
#endif

#include <stdio.h>

#ifdef STDC_HEADERS
#include <string.h>
#else
# include <sys/types.h>
# ifndef HAVE_MEMCPY
static void*
memcpy (void *dest, const void *src, size_t n)
{
  while (0 <= --n) ((unsigned char*)dest) [n] = ((unsigned char*)src) [n];
  return dest;
}
# endif
# ifndef HAVE_MEMSET
static void*
memset (void *s, int c, size_t n)
{
  while (0 <= --n) ((unsigned char*)s) [n] = (unsigned char) c;
  return s;
}
# endif
#endif

#include <errno.h>
#ifdef __FreeBSD__
#include <sys/types.h>
#endif
#include <netinet/in.h>

#include "freeipmi.h"

fiid_template_t tmpl_cmd_get_channel_auth_caps_rq =
  {
    {8, "cmd"},
    {4, "channel_num"},
    {4, "reserved1"},
    {4, "max_priv_level"},
    {4, "reserved2"},
    {0, ""}
  };

fiid_template_t tmpl_cmd_get_channel_auth_caps_rs = 
  {
    {8, "cmd"},
    {8, "comp_code"},
    {8, "channel_num"},
    {1, "auth_type.none"},
    {1, "auth_type.md2"},
    {1, "auth_type.md5"},
    {1, "auth_type.reserved1"},
    {1, "auth_type.straight_passwd_key"},
    {1, "auth_type.oem_prop"},
    {2, "auth_type.reserved2"},
    {1, "auth_status.anonymous_login"},
    {1, "auth_status.null_username"},
    {1, "auth_status.non_null_username"},
    {1, "auth_status.user_level_auth"},
    {1, "auth_status.per_message_auth"},
    {3, "auth_status.reserved"},
    {8, "reserved1"},
    {24, "oem_id"},
    {8, "oem_aux"},
    {0, ""}
  };

fiid_template_t tmpl_cmd_get_session_challenge_rq =
  {
    {8, "cmd"},
    {4, "auth_type"},
    {4, "reserved1"},
    {128, "username"},
    {0, ""}
  };

fiid_template_t tmpl_get_channel_access_rq =
  {
    {8, "cmd"},

    {4, "channel_number"},
    {4, "reserved"},

    {6, "reserved2"},
    {2, "channel_access_set_flag"},

    {0, ""}
  };

fiid_template_t tmpl_get_channel_access_rs =
  {
    {8, "cmd"},

    {8, "comp_code"},

    {3, "ipmi_messaging_access_mode"}, 
    {1, "user_level_authentication"}, 
    {1, "per_message_authentication"}, 
    {1, "pef_alerting"}, 
    {2, "reserved"}, 

    {4, "channel_privilege_level_limit"},
    {4, "reserved2"},

    {0, ""}
  };

fiid_template_t tmpl_cmd_get_session_challenge_rs =
  {
    {8, "cmd"},
    {8, "comp_code"},
    {32, "tmp_session_id"}, /* LS byte first */
    {128, "challenge_str"},
    {0, ""}
  };

fiid_template_t tmpl_cmd_activate_session_rq =
  {
    {8, "cmd"},
    {4, "auth_type"},
    {4, "reserved1"},
    {4, "max_priv_level"},
    {4, "reserved2"},
    {128, "challenge_str"},
    {32, "initial_outbound_seq_num"},
    {0, ""}
  };

fiid_template_t tmpl_cmd_activate_session_rs =
  {
    {8, "cmd"},
    {8, "comp_code"},
    {4, "auth_type"},
    {4, "reserved1"},
    {32, "session_id"},
    {32, "initial_inbound_seq_num"},
    {4, "max_priv_level"},
    {4, "reserved2"},
    {0, ""}
  };

fiid_template_t tmpl_cmd_set_session_priv_level_rq =
  {
    {8, "cmd"},
    {4, "priv_level"},
    {4, "reserved1"},
    {0, ""}
  };

fiid_template_t tmpl_cmd_set_session_priv_level_rs =
  {
    {8, "cmd"},
    {8, "comp_code"},
    {4, "new_priv_level"},
    {4, "reserved1"},
    {0, ""}
  };

fiid_template_t tmpl_cmd_close_session_rq =
  {
    {8, "cmd"},
    {32, "session_id"},
    {0, ""}
  };

fiid_template_t tmpl_cmd_close_session_rs =
  {
    {8, "cmd"},
    {8, "comp_code"},
    {0, ""}
  };

fiid_template_t tmpl_set_channel_access_rq =
  {
    {8, "cmd"}, 
    
    {4, "channel_number"}, 
    {4, "reserved1"}, 
    
    {3, "ipmi_messaging_access_mode"}, 
    {1, "user_level_authentication"}, 
    {1, "per_message_authentication"}, 
    {1, "pef_alerting"}, 
    {2, "channel_access_set_flag"}, 
    
    {4, "channel_privilege_level_limit"}, 
    {2, "reserved2"}, 
    {2, "channel_privilege_level_limit_set_flag"}, 
    
    {0, ""}
  };

fiid_template_t tmpl_set_channel_access_rs =
  {
    {8,  "cmd"}, 
    {8,  "comp_code"}, 
    {0,  ""}
  };

fiid_template_t tmpl_set_user_name_rq =
  {
    {8, "cmd"}, 
    
    {6, "user_id"}, 
    {2, "user_id.reserved"}, 
    
    {128, "user_name"}, 
    
    {0, ""}
  };

fiid_template_t tmpl_set_user_name_rs =
  {
    {8,  "cmd"}, 
    {8,  "comp_code"}, 
    {0,  ""}
  };

fiid_template_t tmpl_get_user_name_rq =
  {
    {8, "cmd"}, 
    
    {6, "user_id"}, 
    {2, "user_id.reserved"}, 
    
    {0, ""}
  };

fiid_template_t tmpl_get_user_name_rs =
  {
    {8,  "cmd"}, 
    {8,  "comp_code"}, 
    
    {128, "user_name"}, 
    
    {0,  ""}
  };

fiid_template_t tmpl_set_user_password_rq =
  {
    {8, "cmd"}, 
    
    {6, "user_id"}, 
    {2, "user_id.reserved"}, 
    
    {2, "operation"}, 
    {6, "operation.reserved"}, 
    
    {128, "password"}, 
    
    {0, ""}
  };

fiid_template_t tmpl_set_user_password_rs =
  {
    {8,  "cmd"}, 
    {8,  "comp_code"}, 
    {0,  ""}
  };

fiid_template_t tmpl_set_user_access_rq =
  {
    {8, "cmd"},

    {4, "channel_number"},
    {1, "user_flags.enable_ipmi_msgs"},
    {1, "user_flags.enable_link_auth"},
    {1, "user_flags.restrict_to_callback"},
    {1, "modify_flag"},

    {6, "user_id"},
    {2, "reserved"},

    {4, "user_privilege_level_limit"},
    {4, "reserved2"},

    {4, "user_session_number_limit"},
    {4, "reserved3"},

    {0, ""}
  };

fiid_template_t tmpl_set_user_access_rs =
  {
    {8,  "cmd"}, 
    {8,  "comp_code"}, 
    {0,  ""}
  };

fiid_template_t tmpl_get_user_access_rq =
  {
    {8, "cmd"},

    {4, "channel_number"},
    {4, "reserved"},

    {6, "user_id"},
    {2, "reserved2"},

    {0, ""}
  };

fiid_template_t tmpl_get_user_access_rs =
  {
    {8, "cmd"},
    {8, "comp_code"},

    {6, "max_channel_user_ids"},
    {2, "reserved"},

    {6, "current_channel_user_ids"},
    {2, "reserved2"},

    {6, "current_channel_fixed_user_names"},
    {2, "reserved3"},

    {4, "user_privilege_level_limit"},
    {1, "user_flags.enable_ipmi_msgs"},
    {1, "user_flags.enable_link_auth"},
    {1, "user_flags.restrict_to_callback"},
    {1, "reserved4"},

    {0, ""}
  };
    
fiid_template_t tmpl_get_channel_info_rq =
  {
    {8, "cmd"},
    
    {4, "channel_number"},
    {4, "reserved"},
    
    {0, ""}
  };

fiid_template_t tmpl_get_channel_info_rs =
  {
    {8, "cmd"},
    {8, "comp_code"},
    
    {4, "actual_channel_number"},
    {4, "actual_channel_number.reserved"},
    
    {7, "channel_medium_type"}, 
    {1, "channel_medium_type.reserved"},
    
    {5, "channel_protocol_type"}, 
    {3, "channel_protocol_type.reserved"},
    
    {6, "active_session_count"}, 
    {2, "session_support"},
    
    {24, "vendor_id"}, 
    
    {16, "auxiliary_channel_info"}, 
    
    {0, ""}
  };


int8_t 
fill_cmd_get_channel_auth_caps (u_int8_t max_priv_level, 
				fiid_obj_t obj_cmd)
{
  if (!obj_cmd || !IPMI_PRIV_LEVEL_VALID(max_priv_level))
    {
      errno = EINVAL;
      return (-1);
    }
  
  FIID_OBJ_SET (obj_cmd, tmpl_cmd_get_channel_auth_caps_rq, "cmd", 
		IPMI_CMD_GET_CHANNEL_AUTH_CAPS);

  FIID_OBJ_SET (obj_cmd, tmpl_cmd_get_channel_auth_caps_rq,
		"channel_num", 0x0E); 
  /* retrieve information for channel this request was
     issued on. */

  FIID_OBJ_SET (obj_cmd, tmpl_cmd_get_channel_auth_caps_rq, "max_priv_level", 
		max_priv_level);
  return (0);
}

int8_t 
ipmi_lan_get_channel_auth_caps (int sockfd, 
				struct sockaddr *hostaddr, 
				size_t hostaddr_len, 
				u_int8_t rq_seq, 
				fiid_obj_t obj_cmd_rs)
{
  fiid_obj_t obj_cmd_rq;

  if (!(hostaddr && sockfd && hostaddr_len && obj_cmd_rs))
    {
      errno = EINVAL;
      return (-1);
    }
  
  obj_cmd_rq = alloca (fiid_obj_len_bytes (tmpl_cmd_get_channel_auth_caps_rq));
  memset (obj_cmd_rq, 0, fiid_obj_len_bytes (tmpl_cmd_get_channel_auth_caps_rq));
  ERR (obj_cmd_rq);

  ERR (fill_cmd_get_channel_auth_caps (IPMI_PRIV_LEVEL_USER, obj_cmd_rq) != -1);

  ERR (ipmi_lan_cmd (sockfd, hostaddr, hostaddr_len, IPMI_SESSION_AUTH_TYPE_NONE,
		     0, 0, NULL, 0, IPMI_NET_FN_APP_RQ, IPMI_BMC_IPMB_LUN_BMC, rq_seq,
		     obj_cmd_rq, tmpl_cmd_get_channel_auth_caps_rq,
		     obj_cmd_rs, tmpl_cmd_get_channel_auth_caps_rs) != -1);
  
  /* INFO: you can also do auth type check here */
  return (0);
}

int8_t 
ipmi_cmd_get_channel_auth_caps2 (ipmi_device_t *dev, 
				 fiid_obj_t obj_cmd_rs)
{
  ipmi_device_t local_dev;
  fiid_obj_t obj_cmd_rq;
  
  if (!(dev->private.local_sockfd && 
	dev->private.remote_host && 
	dev->private.remote_host_len && 
	obj_cmd_rs))
    {
      errno = EINVAL;
      return (-1);
    }
  
  memcpy (&local_dev, dev, sizeof (ipmi_device_t));
  local_dev.io.outofband.rq.obj_hdr_rmcp = NULL;
  local_dev.io.outofband.rs.obj_hdr_rmcp = NULL;
  local_dev.io.outofband.rq.obj_hdr_session = NULL;
  local_dev.io.outofband.rs.obj_hdr_session = NULL;
  local_dev.io.outofband.rq.obj_msg_hdr = NULL;
  local_dev.io.outofband.rs.obj_msg_hdr = NULL;
  
  local_dev.io.outofband.rq.tmpl_hdr_session_ptr = 
    local_dev.io.outofband.rs.tmpl_hdr_session_ptr = &tmpl_hdr_session;
  
  FIID_OBJ_ALLOCA (local_dev.io.outofband.rq.obj_hdr_rmcp,
		   *(local_dev.io.outofband.rq.tmpl_hdr_rmcp_ptr));
  FIID_OBJ_ALLOCA (local_dev.io.outofband.rs.obj_hdr_rmcp,
		   *(local_dev.io.outofband.rs.tmpl_hdr_rmcp_ptr));
  FIID_OBJ_ALLOCA (local_dev.io.outofband.rq.obj_hdr_session,
		   *(local_dev.io.outofband.rq.tmpl_hdr_session_ptr));
  FIID_OBJ_ALLOCA (local_dev.io.outofband.rs.obj_hdr_session,
		   *(local_dev.io.outofband.rs.tmpl_hdr_session_ptr));
  FIID_OBJ_ALLOCA (local_dev.io.outofband.rq.obj_msg_hdr,
		   *(local_dev.io.outofband.rq.tmpl_msg_hdr_ptr));
  FIID_OBJ_ALLOCA (local_dev.io.outofband.rs.obj_msg_hdr,
		   *(local_dev.io.outofband.rs.tmpl_msg_hdr_ptr));
  
  obj_cmd_rq = alloca (fiid_obj_len_bytes (tmpl_cmd_get_channel_auth_caps_rq));
  memset (obj_cmd_rq, 0, fiid_obj_len_bytes (tmpl_cmd_get_channel_auth_caps_rq));
  ERR (obj_cmd_rq);
  
  ERR (fill_cmd_get_channel_auth_caps (IPMI_PRIV_LEVEL_USER, obj_cmd_rq) != -1);
  
  local_dev.private.lun = IPMI_BMC_IPMB_LUN_BMC;
  local_dev.private.net_fn = IPMI_NET_FN_APP_RQ;
  ERR (ipmi_lan_cmd2 (&local_dev, 
		      obj_cmd_rq, 
		      tmpl_cmd_get_channel_auth_caps_rq, 
		      obj_cmd_rs, 
		      tmpl_cmd_get_channel_auth_caps_rs) != -1);
  
  /* Note: We need to copy local_dev.io.outofband.rs.obj_hdr_session
     content to dev->io.outofband.rs.obj_hdr_session here -- Bala.A */
  
  /* INFO: you can also do auth type check here */
  return (0);
}

int8_t 
fill_cmd_get_session_challenge (u_int8_t auth_type, 
				char *username, 
				u_int32_t username_len, 
				fiid_obj_t obj_cmd)
{
  /* achu: username can be IPMI_SESSION_MAX_USERNAME_LEN length.  Null
   * termination in IPMI packet not required
   */
  if (!obj_cmd 
      || !IPMI_SESSION_AUTH_TYPE_VALID(auth_type)
      || (username && username_len > IPMI_SESSION_MAX_USERNAME_LEN))
    {
      errno = EINVAL;
      return (-1);
    }

  FIID_OBJ_SET (obj_cmd, tmpl_cmd_get_session_challenge_rq, "cmd", 
		IPMI_CMD_GET_SESSION_CHALLENGE);
  FIID_OBJ_SET (obj_cmd, tmpl_cmd_get_session_challenge_rq, "auth_type",
		auth_type);

  /* achu: The BMC may ignore any '\0' characters that indicate the
   * end of the string.  So we need to guarantee the buffer is
   * completely cleared before setting anything.
   */
  ERR_EXIT (fiid_obj_memset_field(obj_cmd, '\0',
                                  tmpl_cmd_get_session_challenge_rq,
                                  "username") == 0);

  if (username)
    ERR_EXIT (fiid_obj_set_data (obj_cmd, 
                                 tmpl_cmd_get_session_challenge_rq, 
                                 "username", 
                                 username, 
                                 username_len) == 0);

  return (0);
}

int8_t 
ipmi_lan_get_session_challenge (int sockfd, 
				struct sockaddr *hostaddr, 
				size_t hostaddr_len, 
				u_int8_t auth_type, 
				char *username, 
				u_int8_t rq_seq, 
				fiid_obj_t obj_cmd_rs)
{
  fiid_obj_t obj_cmd_rq;

  if (!(hostaddr && sockfd && hostaddr_len && obj_cmd_rs))
    {
      errno = EINVAL;
      return (-1);
    }
  
  obj_cmd_rq = alloca (fiid_obj_len_bytes (tmpl_cmd_get_session_challenge_rq));
  ERR (obj_cmd_rq);
  memset (obj_cmd_rq, 0, fiid_obj_len_bytes (tmpl_cmd_get_session_challenge_rq));

  ERR (fill_cmd_get_session_challenge (auth_type, username, 
                                       (username) ? strlen(username) : 0, obj_cmd_rq) != -1);

  ERR (ipmi_lan_cmd (sockfd, hostaddr, hostaddr_len, IPMI_SESSION_AUTH_TYPE_NONE,
		     0, 0, NULL, 0, IPMI_NET_FN_APP_RQ, IPMI_BMC_IPMB_LUN_BMC, rq_seq,
		     obj_cmd_rq, tmpl_cmd_get_session_challenge_rq,
		     obj_cmd_rs, tmpl_cmd_get_session_challenge_rs) != -1);
  return (0);
}

int8_t 
ipmi_cmd_get_session_challenge2 (ipmi_device_t *dev, 
				 fiid_obj_t obj_cmd_rs)
{
  ipmi_device_t local_dev;
  fiid_obj_t obj_cmd_rq;
  
  if (!(dev->private.local_sockfd && 
	dev->private.remote_host && 
	dev->private.remote_host_len && 
	obj_cmd_rs))
    {
      errno = EINVAL;
      return (-1);
    }
  
  memcpy (&local_dev, dev, sizeof (ipmi_device_t));
  local_dev.io.outofband.rq.obj_hdr_rmcp = NULL;
  local_dev.io.outofband.rs.obj_hdr_rmcp = NULL;
  local_dev.io.outofband.rq.obj_hdr_session = NULL;
  local_dev.io.outofband.rs.obj_hdr_session = NULL;
  local_dev.io.outofband.rq.obj_msg_hdr = NULL;
  local_dev.io.outofband.rs.obj_msg_hdr = NULL;
  
  local_dev.io.outofband.rq.tmpl_hdr_session_ptr = 
    local_dev.io.outofband.rs.tmpl_hdr_session_ptr = &tmpl_hdr_session;
  
  FIID_OBJ_ALLOCA (local_dev.io.outofband.rq.obj_hdr_rmcp,
		   *(local_dev.io.outofband.rq.tmpl_hdr_rmcp_ptr));
  FIID_OBJ_ALLOCA (local_dev.io.outofband.rs.obj_hdr_rmcp,
		   *(local_dev.io.outofband.rs.tmpl_hdr_rmcp_ptr));
  FIID_OBJ_ALLOCA (local_dev.io.outofband.rq.obj_hdr_session,
		   *(local_dev.io.outofband.rq.tmpl_hdr_session_ptr));
  FIID_OBJ_ALLOCA (local_dev.io.outofband.rs.obj_hdr_session,
		   *(local_dev.io.outofband.rs.tmpl_hdr_session_ptr));
  FIID_OBJ_ALLOCA (local_dev.io.outofband.rq.obj_msg_hdr,
		   *(local_dev.io.outofband.rq.tmpl_msg_hdr_ptr));
  FIID_OBJ_ALLOCA (local_dev.io.outofband.rs.obj_msg_hdr,
		   *(local_dev.io.outofband.rs.tmpl_msg_hdr_ptr));
  
  obj_cmd_rq = alloca (fiid_obj_len_bytes (tmpl_cmd_get_session_challenge_rq));
  ERR (obj_cmd_rq);
  memset (obj_cmd_rq, 0, fiid_obj_len_bytes (tmpl_cmd_get_session_challenge_rq));
  
  ERR (fill_cmd_get_session_challenge (dev->private.auth_type, 
				       dev->private.username, 
                                       ((dev->private.username) ? 
					strlen (dev->private.username) : 0), 
				       obj_cmd_rq) != -1);
  
  local_dev.private.lun = IPMI_BMC_IPMB_LUN_BMC;
  local_dev.private.net_fn = IPMI_NET_FN_APP_RQ;
  ERR (ipmi_lan_cmd2 (&local_dev, 
		      obj_cmd_rq, 
		      tmpl_cmd_get_session_challenge_rq, 
		      obj_cmd_rs, 
		      tmpl_cmd_get_session_challenge_rs) != -1);
  
  /* Note: We need to copy local_dev.io.outofband.rs.obj_hdr_session
     content to dev->io.outofband.rs.obj_hdr_session here -- Bala.A */
  return (0);
}

int8_t 
fill_cmd_activate_session (u_int8_t auth_type, 
			   u_int8_t max_priv_level, 
			   u_int8_t *challenge_str, 
			   u_int32_t challenge_str_len, 
			   u_int32_t initial_outbound_seq_num, 
			   fiid_obj_t obj_cmd)
{
  if (!IPMI_SESSION_AUTH_TYPE_VALID(auth_type)
      || !IPMI_PRIV_LEVEL_VALID(max_priv_level)
      || challenge_str_len > IPMI_SESSION_CHALLENGE_STR_LEN
      || !obj_cmd)
    {
      errno = EINVAL;
      return (-1);
    }

  FIID_OBJ_SET (obj_cmd, tmpl_cmd_activate_session_rq, "cmd", 
		IPMI_CMD_ACTIVATE_SESSION);
  FIID_OBJ_SET (obj_cmd, tmpl_cmd_activate_session_rq, "auth_type", 
		auth_type);
  FIID_OBJ_SET (obj_cmd, tmpl_cmd_activate_session_rq, "max_priv_level", 
		max_priv_level);
  ERR_EXIT (fiid_obj_memset_field (obj_cmd, '\0', 
                                   tmpl_cmd_activate_session_rq,
                                   "challenge_str") == 0);
  if (challenge_str)
    fiid_obj_set_data (obj_cmd, tmpl_cmd_activate_session_rq, 
                       "challenge_str", challenge_str, 
                       challenge_str_len);
  FIID_OBJ_SET (obj_cmd, tmpl_cmd_activate_session_rq, "initial_outbound_seq_num", 
		initial_outbound_seq_num);
  return (0);
}

int8_t 
ipmi_lan_activate_session (int sockfd, 
			   struct sockaddr *hostaddr, 
			   size_t hostaddr_len, 
			   u_int8_t auth_type, 
			   u_int32_t tmp_session_id, 
			   u_int8_t *auth_code_data, 
			   u_int32_t auth_code_data_len, 
			   u_int8_t max_priv_level, 
			   u_int8_t *challenge_str, 
			   u_int32_t challenge_str_len, 
			   u_int32_t initial_outbound_seq_num, 
			   u_int8_t rq_seq, 
			   fiid_obj_t obj_cmd_rs)
{
  fiid_obj_t obj_cmd_rq;
  
  if (!(hostaddr && sockfd && hostaddr_len && tmp_session_id &&
	challenge_str && obj_cmd_rs))
    {
      errno = EINVAL;
      return (-1);
    }
  
  FIID_OBJ_ALLOCA (obj_cmd_rq, tmpl_cmd_activate_session_rq);

  ERR (fill_cmd_activate_session (auth_type, max_priv_level, challenge_str, 
				  challenge_str_len, initial_outbound_seq_num,
				  obj_cmd_rq) != -1);

  ERR (ipmi_lan_cmd (sockfd, hostaddr, hostaddr_len, auth_type, 
		     0, tmp_session_id, auth_code_data, auth_code_data_len,
		     IPMI_NET_FN_APP_RQ, IPMI_BMC_IPMB_LUN_BMC, rq_seq, 
		     obj_cmd_rq, tmpl_cmd_activate_session_rq,
		     obj_cmd_rs, tmpl_cmd_activate_session_rs) != -1);
  return (0);
}

int8_t 
ipmi_cmd_activate_session2 (ipmi_device_t *dev, 
			    u_int8_t *challenge_str, 
			    u_int32_t challenge_str_len, 
			    fiid_obj_t obj_cmd_rs)
{
  fiid_obj_t obj_cmd_rq;
  
  if (!(dev->private.local_sockfd && 
	dev->private.remote_host && 
	dev->private.remote_host_len && 
	dev->private.session_id &&
	challenge_str && 
	obj_cmd_rs))
    {
      errno = EINVAL;
      return (-1);
    }
  
  FIID_OBJ_ALLOCA (obj_cmd_rq, tmpl_cmd_activate_session_rq);
  
  ERR (fill_cmd_activate_session (dev->private.auth_type, 
				  dev->private.priv_level, 
				  challenge_str, 
				  challenge_str_len, 
				  dev->private.initial_outbound_seq_num, 
				  obj_cmd_rq) != -1);
  
  dev->private.lun = IPMI_BMC_IPMB_LUN_BMC;
  dev->private.net_fn = IPMI_NET_FN_APP_RQ;
  ERR (ipmi_lan_cmd2 (dev, 
		      obj_cmd_rq, 
		      tmpl_cmd_activate_session_rq,
		      obj_cmd_rs, 
		      tmpl_cmd_activate_session_rs) != -1);
  return (0);
}

int8_t 
fill_cmd_set_session_priv_level (u_int8_t priv_level, 
				 fiid_obj_t obj_cmd)
{
  if (!IPMI_PRIV_LEVEL_VALID(priv_level)
      || obj_cmd == NULL)
    {
      errno = EINVAL;
      return (-1);
    }

  FIID_OBJ_SET (obj_cmd, tmpl_cmd_set_session_priv_level_rq, "cmd",
		IPMI_CMD_SET_SESSION_PRIV_LEVEL);
  FIID_OBJ_SET (obj_cmd, tmpl_cmd_set_session_priv_level_rq, "priv_level",
		priv_level);
  return (0);
}  

int8_t 
ipmi_lan_set_session_priv_level (int sockfd, 
				 struct sockaddr *hostaddr, 
				 size_t hostaddr_len, 
				 u_int8_t auth_type, 
				 u_int32_t session_seq_num, 
				 u_int32_t session_id, 
				 u_int8_t *auth_code_data, 
				 u_int32_t auth_code_data_len, 
				 u_int8_t priv_level, 
				 u_int8_t rq_seq, 
				 fiid_obj_t obj_cmd_rs)
{
  fiid_obj_t obj_cmd_rq;
  
  if (!(hostaddr && sockfd && hostaddr_len && session_id && obj_cmd_rs))
    {
      errno = EINVAL;
      return (-1);
    }
  
  obj_cmd_rq = alloca (fiid_obj_len_bytes (tmpl_cmd_set_session_priv_level_rq));
  memset (obj_cmd_rq, 0, fiid_obj_len_bytes (tmpl_cmd_set_session_priv_level_rq));
  ERR (obj_cmd_rq);

  ERR (fill_cmd_set_session_priv_level (priv_level, obj_cmd_rq) != -1);

  ERR (ipmi_lan_cmd (sockfd, hostaddr, hostaddr_len, auth_type, session_seq_num, 
		     session_id, auth_code_data, auth_code_data_len,
		     IPMI_NET_FN_APP_RQ, IPMI_BMC_IPMB_LUN_BMC, rq_seq,
		     obj_cmd_rq, tmpl_cmd_set_session_priv_level_rq,
		     obj_cmd_rs, tmpl_cmd_set_session_priv_level_rs) != -1);
  return (0);
}

int8_t 
ipmi_cmd_set_session_priv_level2 (ipmi_device_t *dev, 
				  fiid_obj_t obj_cmd_rs)
{
  fiid_obj_t obj_cmd_rq;
  
  if (!(dev->private.local_sockfd && 
	dev->private.remote_host && 
	dev->private.remote_host_len && 
	dev->private.session_id && 
	obj_cmd_rs))
    {
      errno = EINVAL;
      return (-1);
    }
  
  obj_cmd_rq = alloca (fiid_obj_len_bytes (tmpl_cmd_set_session_priv_level_rq));
  memset (obj_cmd_rq, 0, fiid_obj_len_bytes (tmpl_cmd_set_session_priv_level_rq));
  ERR (obj_cmd_rq);
  
  ERR (fill_cmd_set_session_priv_level (dev->private.priv_level, obj_cmd_rq) != -1);
  
  dev->private.lun = IPMI_BMC_IPMB_LUN_BMC;
  dev->private.net_fn = IPMI_NET_FN_APP_RQ;
  ERR (ipmi_lan_cmd2 (dev, 
		      obj_cmd_rq, 
		      tmpl_cmd_set_session_priv_level_rq,
		      obj_cmd_rs, 
		      tmpl_cmd_set_session_priv_level_rs) != -1);
  return (0);
}

int8_t 
ipmi_lan_open_session (int sockfd, 
		       struct sockaddr *hostaddr, 
		       size_t hostaddr_len, 
		       u_int8_t auth_type, 
		       char *username, 
		       char *password, 
		       u_int32_t initial_outbound_seq_num, 
		       u_int8_t priv_level, 
		       u_int32_t *session_seq_num, 
		       u_int32_t *session_id, 
		       u_int8_t *rq_seq)
{
  fiid_obj_t obj_cmd_rs;
  u_int64_t temp_session_id, temp_session_seq_num;
  u_int8_t challenge_str[IPMI_CHALLENGE_STR_MAX];
  int password_len = strlen (password);

  if ((password_len > IPMI_USER_PASSWORD_MAX_LENGTH) || 
      (rq_seq == NULL))
    {
      errno = EINVAL;
      return (-1);
    }

  *rq_seq = 0;
  obj_cmd_rs = fiid_obj_alloc (tmpl_cmd_get_channel_auth_caps_rs);
  if (ipmi_lan_get_channel_auth_caps (sockfd, hostaddr, hostaddr_len, *rq_seq, obj_cmd_rs) == -1)
    goto error;
  if (!ipmi_comp_test (obj_cmd_rs))
    goto error;
  ipmi_xfree (obj_cmd_rs);
  IPMI_LAN_RQ_SEQ_INC (*rq_seq);

  obj_cmd_rs = fiid_obj_alloc (tmpl_cmd_get_session_challenge_rs);
  if (ipmi_lan_get_session_challenge (sockfd, hostaddr, hostaddr_len, 
				      auth_type, username, *rq_seq, obj_cmd_rs) == -1)
    goto error;
  if (!ipmi_comp_test (obj_cmd_rs))
    goto error;
  FIID_OBJ_GET (obj_cmd_rs, tmpl_cmd_get_session_challenge_rs, 
		"tmp_session_id", &temp_session_id);
  *session_id = temp_session_id;
  fiid_obj_get_data (obj_cmd_rs, tmpl_cmd_get_session_challenge_rs, "challenge_str", challenge_str);
  ipmi_xfree (obj_cmd_rs);
  IPMI_LAN_RQ_SEQ_INC (*rq_seq);

  obj_cmd_rs = fiid_obj_alloc (tmpl_cmd_activate_session_rs);
  if (ipmi_lan_activate_session (sockfd, hostaddr, hostaddr_len, 
				 auth_type, *session_id, password, password_len,
				 priv_level, challenge_str, IPMI_CHALLENGE_STR_MAX,
				 initial_outbound_seq_num, *rq_seq, obj_cmd_rs) == -1)
    goto error;
  if (!ipmi_comp_test (obj_cmd_rs))
    goto error;
  FIID_OBJ_GET (obj_cmd_rs, tmpl_cmd_activate_session_rs, 
		"session_id", &temp_session_id);
  *session_id = temp_session_id;
  FIID_OBJ_GET (obj_cmd_rs, tmpl_cmd_activate_session_rs, 
		"initial_inbound_seq_num", &temp_session_seq_num);
  *session_seq_num = temp_session_seq_num;
  ipmi_xfree (obj_cmd_rs);
  IPMI_LAN_RQ_SEQ_INC (*rq_seq);

  obj_cmd_rs = fiid_obj_alloc (tmpl_cmd_set_session_priv_level_rs);
  if (ipmi_lan_set_session_priv_level (sockfd, hostaddr, hostaddr_len, 
				       auth_type, *session_seq_num, *session_id, 
				       password, password_len, priv_level,
				       *rq_seq,  obj_cmd_rs) == -1)
    goto error;
  if (!ipmi_comp_test (obj_cmd_rs))
    goto error;
  ipmi_xfree (obj_cmd_rs);
  IPMI_LAN_RQ_SEQ_INC (*rq_seq);

  return (0);

 error:
  ipmi_xfree (obj_cmd_rs);
  return (-1);
}

int8_t 
ipmi_lan_open_session2 (ipmi_device_t *dev)
{
  fiid_obj_t obj_cmd_rs;
  u_int64_t temp_session_id, temp_session_seq_num;
  u_int8_t challenge_str[IPMI_CHALLENGE_STR_MAX];
  u_int64_t supported_auth_type = 0;
  
  dev->private.rq_seq = 0;
  
  FIID_OBJ_ALLOCA (obj_cmd_rs, tmpl_cmd_get_channel_auth_caps_rs);
  if (ipmi_cmd_get_channel_auth_caps2 (dev, obj_cmd_rs) != 0)
    {
      return (-1);
    }
  if (!ipmi_comp_test (obj_cmd_rs))
    {
      return (-1);
    }
  switch (dev->private.auth_type)
    {
    case IPMI_SESSION_AUTH_TYPE_NONE:
      FIID_OBJ_GET (obj_cmd_rs, tmpl_cmd_get_channel_auth_caps_rs, 
		    "auth_type.none", &supported_auth_type);
      break;
    case IPMI_SESSION_AUTH_TYPE_MD2:
      FIID_OBJ_GET (obj_cmd_rs, tmpl_cmd_get_channel_auth_caps_rs, 
		    "auth_type.md2", &supported_auth_type);
      break;
    case IPMI_SESSION_AUTH_TYPE_MD5:
      FIID_OBJ_GET (obj_cmd_rs, tmpl_cmd_get_channel_auth_caps_rs, 
		    "auth_type.md5", &supported_auth_type);
      break;
    case IPMI_SESSION_AUTH_TYPE_STRAIGHT_PASSWD_KEY:
      FIID_OBJ_GET (obj_cmd_rs, tmpl_cmd_get_channel_auth_caps_rs, 
		    "auth_type.straight_passwd_key", &supported_auth_type);
      break;
    case IPMI_SESSION_AUTH_TYPE_OEM_PROP:
      FIID_OBJ_GET (obj_cmd_rs, tmpl_cmd_get_channel_auth_caps_rs, 
		    "auth_type.oem_prop", &supported_auth_type);
      break;
    default:
      errno = EINVAL;
      return (-1);
    }
  if (supported_auth_type == 0)
    {
      errno = ENOTSUP;
      return (-1);
    }
  
  FIID_OBJ_ALLOCA (obj_cmd_rs, tmpl_cmd_get_session_challenge_rs);
  if (ipmi_cmd_get_session_challenge2 (dev, obj_cmd_rs) == -1)
    {
      return (-1);
    }
  if (!ipmi_comp_test (obj_cmd_rs))
    {
      return (-1);
    }
  FIID_OBJ_GET (obj_cmd_rs, tmpl_cmd_get_session_challenge_rs, 
		"tmp_session_id", &temp_session_id);
  dev->private.session_id = temp_session_id;
  fiid_obj_get_data (obj_cmd_rs, 
		     tmpl_cmd_get_session_challenge_rs, 
		     "challenge_str", 
		     challenge_str);
  
  FIID_OBJ_ALLOCA (obj_cmd_rs, tmpl_cmd_activate_session_rs);
  if (ipmi_cmd_activate_session2 (dev, 
				  challenge_str, 
				  IPMI_CHALLENGE_STR_MAX, 
				  obj_cmd_rs) == -1)
    {
      return (-1);
    }
  if (!ipmi_comp_test (obj_cmd_rs))
    {
      return (-1);
    }
  FIID_OBJ_GET (obj_cmd_rs, tmpl_cmd_activate_session_rs, 
		"session_id", &temp_session_id);
  dev->private.session_id = temp_session_id;
  FIID_OBJ_GET (obj_cmd_rs, tmpl_cmd_activate_session_rs, 
		"initial_inbound_seq_num", &temp_session_seq_num);
  dev->private.session_seq_num = temp_session_seq_num;
  
  FIID_OBJ_ALLOCA (obj_cmd_rs, tmpl_cmd_set_session_priv_level_rs);
  if (ipmi_cmd_set_session_priv_level2 (dev, obj_cmd_rs) == -1)
    {
      return (-1);
    }
  if (!ipmi_comp_test (obj_cmd_rs))
    {
      return (-1);
    }
  
  return (0);
}

int8_t 
fill_cmd_close_session (u_int32_t close_session_id, 
			fiid_obj_t obj_cmd)
{
  if (obj_cmd == NULL)
    {
      errno = EINVAL;
      return (-1);
    }

  FIID_OBJ_SET (obj_cmd, tmpl_cmd_close_session_rq, "cmd",
		IPMI_CMD_CLOSE_SESSION);
  FIID_OBJ_SET (obj_cmd, tmpl_cmd_close_session_rq, "session_id",
		close_session_id);
  return (0);
}  

int8_t 
ipmi_lan_close_session (int sockfd, 
			struct sockaddr *hostaddr, 
			size_t hostaddr_len, 
			u_int8_t auth_type, 
			u_int32_t session_seq_num, 
			u_int32_t session_id, 
			u_int8_t *auth_code_data, 
			u_int32_t auth_code_data_len, 
			u_int8_t rq_seq, 
			u_int32_t close_session_id, 
			fiid_obj_t obj_cmd_rs)
{
  fiid_obj_t obj_cmd_rq;
  
  if (!(hostaddr && sockfd && hostaddr_len && obj_cmd_rs))
    {
      errno = EINVAL;
      return (-1);
    }
  
  obj_cmd_rq = alloca (fiid_obj_len_bytes (tmpl_cmd_close_session_rq));
  memset (obj_cmd_rq, 0, fiid_obj_len_bytes (tmpl_cmd_close_session_rq));
  ERR (obj_cmd_rq);

  ERR (fill_cmd_close_session (close_session_id, obj_cmd_rq) != -1);
  ERR (ipmi_lan_cmd (sockfd, hostaddr, hostaddr_len, auth_type,
		     session_seq_num, session_id, auth_code_data, auth_code_data_len,
		     IPMI_NET_FN_APP_RQ, IPMI_BMC_IPMB_LUN_BMC, rq_seq,
		     obj_cmd_rq, tmpl_cmd_close_session_rq,
		     obj_cmd_rs, tmpl_cmd_close_session_rs) != -1);
  return (0);
}

int8_t 
ipmi_lan_close_session2 (ipmi_device_t *dev, 
			 fiid_obj_t obj_cmd_rs)
{
  fiid_obj_t obj_cmd_rq;
  
  if (!(dev->private.remote_host && 
	dev->private.local_sockfd && 
	dev->private.remote_host_len && 
	obj_cmd_rs))
    {
      errno = EINVAL;
      return (-1);
    }
  
  obj_cmd_rq = alloca (fiid_obj_len_bytes (tmpl_cmd_close_session_rq));
  memset (obj_cmd_rq, 0, fiid_obj_len_bytes (tmpl_cmd_close_session_rq));
  ERR (obj_cmd_rq);
  
  ERR (fill_cmd_close_session (dev->private.session_id, obj_cmd_rq) != -1);
  dev->private.lun = IPMI_BMC_IPMB_LUN_BMC;
  dev->private.net_fn = IPMI_NET_FN_APP_RQ;
  ERR (ipmi_lan_cmd2 (dev, 
		      obj_cmd_rq, 
		      tmpl_cmd_close_session_rq,
		      obj_cmd_rs, 
		      tmpl_cmd_close_session_rs) != -1);
  if (!ipmi_comp_test (obj_cmd_rs))
    {
      return (-1);
    }
  
  return (0);
}

int8_t 
fill_kcs_set_channel_access (fiid_obj_t obj_data_rq, 
			     u_int8_t channel_number, 
			     u_int8_t ipmi_messaging_access_mode, 
			     u_int8_t user_level_authentication, 
			     u_int8_t per_message_authentication, 
			     u_int8_t pef_alerting, 
			     u_int8_t channel_access_set_flag, 
			     u_int8_t channel_privilege_level_limit, 
			     u_int8_t channel_privilege_level_limit_set_flag)
{
  if (obj_data_rq == NULL
      || !IPMI_CHANNEL_NUMBER_VALID(channel_number))
    {
      errno = EINVAL;
      return (-1);
    }

  FIID_OBJ_SET (obj_data_rq, 
		tmpl_set_channel_access_rq, 
		"cmd", 
		IPMI_CMD_SET_CHANNEL_ACCESS);
  
  FIID_OBJ_SET (obj_data_rq, 
		tmpl_set_channel_access_rq, 
		"channel_number", 
		channel_number);
  
  FIID_OBJ_SET (obj_data_rq, 
		tmpl_set_channel_access_rq, 
		"ipmi_messaging_access_mode", 
		ipmi_messaging_access_mode);
  
  FIID_OBJ_SET (obj_data_rq, 
		tmpl_set_channel_access_rq, 
		"user_level_authentication", 
		user_level_authentication);
  
  FIID_OBJ_SET (obj_data_rq, 
		tmpl_set_channel_access_rq, 
		"per_message_authentication", 
		per_message_authentication);
  
  FIID_OBJ_SET (obj_data_rq, 
		tmpl_set_channel_access_rq, 
		"pef_alerting", 
		pef_alerting);
  
  FIID_OBJ_SET (obj_data_rq, 
		tmpl_set_channel_access_rq, 
		"channel_access_set_flag", 
		channel_access_set_flag);
  
  FIID_OBJ_SET (obj_data_rq, 
		tmpl_set_channel_access_rq, 
		"channel_privilege_level_limit", 
		channel_privilege_level_limit);
  
  FIID_OBJ_SET (obj_data_rq, 
		tmpl_set_channel_access_rq, 
		"channel_privilege_level_limit_set_flag", 
		channel_privilege_level_limit_set_flag);
  
  return 0;
}

int8_t 
ipmi_kcs_set_channel_access (u_int8_t channel_number, 
			     u_int8_t ipmi_messaging_access_mode, 
			     u_int8_t user_level_authentication, 
			     u_int8_t per_message_authentication, 
			     u_int8_t pef_alerting, 
			     u_int8_t channel_access_set_flag, 
			     u_int8_t channel_privilege_level_limit, 
			     u_int8_t channel_privilege_level_limit_set_flag, 
			     fiid_obj_t obj_data_rs)
{
  fiid_obj_t obj_data_rq; 
  int8_t status;
  
  obj_data_rq = fiid_obj_alloc (tmpl_set_channel_access_rq);
  ERR (fill_kcs_set_channel_access (obj_data_rq, 
				    channel_number, 
				    ipmi_messaging_access_mode, 
				    user_level_authentication, 
				    per_message_authentication, 
				    pef_alerting, 
				    channel_access_set_flag, 
				    channel_privilege_level_limit, 
				    channel_privilege_level_limit_set_flag) == 0); 
  status = ipmi_kcs_cmd (IPMI_BMC_IPMB_LUN_BMC, IPMI_NET_FN_APP_RQ, 
			 obj_data_rq, tmpl_set_channel_access_rq, 
			 obj_data_rs, tmpl_set_channel_access_rs);
  free (obj_data_rq);
  return status;
}

int8_t 
fill_kcs_set_user_name (fiid_obj_t obj_data_rq, 
			u_int8_t user_id, 
			char *user_name,
                        unsigned int user_name_len)
{
  /* achu: username can be IPMI_USER_NAME_MAX_LENGTH length.  Null
   * termination in IPMI packet not required
   */
  if (!obj_data_rq
      || (user_name && user_name_len > IPMI_USER_NAME_MAX_LENGTH))
    {
      errno = EINVAL;
      return -1;
    }

  FIID_OBJ_SET (obj_data_rq, 
		tmpl_set_user_name_rq, 
		"cmd", 
		IPMI_CMD_SET_USER_NAME);
  
  FIID_OBJ_SET (obj_data_rq, 
		tmpl_set_user_name_rq, 
		"user_id", 
		user_id);
  
  /* achu: The BMC may ignore any '\0' characters that indicate the
   * end of the string.  So we need to guarantee the buffer is
   * completely cleared before setting anything.
   */
  ERR_EXIT (fiid_obj_memset_field(obj_data_rq, '\0', 
                                  tmpl_set_user_name_rq,
                                  "user_name") == 0);

  if (user_name)
    ERR_EXIT (fiid_obj_set_data (obj_data_rq, 
                                 tmpl_set_user_name_rq, 
                                 "user_name", 
                                 user_name,
                                 user_name_len) == 0);
  
  return 0;
}

int8_t 
ipmi_kcs_set_user_name (u_int8_t user_id, 
			char *user_name, 
			fiid_obj_t obj_data_rs)
{
  fiid_obj_t obj_data_rq; 
  int8_t status;
  
  obj_data_rq = fiid_obj_alloc (tmpl_set_user_name_rq);
  ERR (fill_kcs_set_user_name (obj_data_rq, user_id, 
			       user_name, 
			       (user_name) ? strlen(user_name) : 0) == 0);
  status = ipmi_kcs_cmd (IPMI_BMC_IPMB_LUN_BMC, IPMI_NET_FN_APP_RQ, 
			 obj_data_rq, tmpl_set_user_name_rq, 
			 obj_data_rs, tmpl_set_user_name_rs);
  free (obj_data_rq);
  return status;
}

int8_t 
fill_kcs_get_user_name (fiid_obj_t obj_data_rq, u_int8_t user_id) 
{
  FIID_OBJ_SET (obj_data_rq, 
		tmpl_get_user_name_rq, 
		"cmd", 
		IPMI_CMD_GET_USER_NAME_CMD);
  
  FIID_OBJ_SET (obj_data_rq, 
		tmpl_get_user_name_rq, 
		"user_id", 
		user_id);
  
  return 0;
}

int8_t 
ipmi_kcs_get_user_name (u_int8_t user_id, 
			fiid_obj_t obj_data_rs)
{
  fiid_obj_t obj_data_rq; 
  int8_t status;
  
  obj_data_rq = fiid_obj_alloc (tmpl_get_user_name_rq);
  ERR (fill_kcs_get_user_name (obj_data_rq, user_id) == 0);
  status = ipmi_kcs_cmd (IPMI_BMC_IPMB_LUN_BMC, IPMI_NET_FN_APP_RQ, 
			 obj_data_rq, tmpl_get_user_name_rq, 
			 obj_data_rs, tmpl_get_user_name_rs);
  free (obj_data_rq);
  return status;
}

int8_t 
fill_kcs_set_user_password (fiid_obj_t obj_data_rq, 
			    u_int8_t user_id, 
			    u_int8_t operation, 
			    char *user_password,
                            unsigned int user_password_len)
{
  /* achu: password can be IPMI_USER_PASSWORD_MAX_LENGTH length.  Null
   * termination in IPMI packet not required
   */
  if (!obj_data_rq
      || (user_password 
          && user_password_len > IPMI_USER_PASSWORD_MAX_LENGTH))
    {
      errno = EINVAL;
      return -1;
    }

  FIID_OBJ_SET (obj_data_rq, 
		tmpl_set_user_password_rq, 
		"cmd", 
		IPMI_CMD_SET_USER_PASSWORD_CMD);
  
  FIID_OBJ_SET (obj_data_rq, 
		tmpl_set_user_password_rq, 
		"user_id", 
		user_id);
  
  FIID_OBJ_SET (obj_data_rq, 
		tmpl_set_user_password_rq, 
		"operation", 
		operation);
  
  /* achu: The BMC may ignore any '\0' characters that indicate the
   * end of the string.  So we need to guarantee the buffer is
   * completely cleared before setting anything.
   */
  ERR_EXIT (fiid_obj_memset_field(obj_data_rq, '\0', 
                                  tmpl_set_user_password_rq,
                                  "password") == 0);

  if (user_password)
    ERR_EXIT (fiid_obj_set_data (obj_data_rq,
                                 tmpl_set_user_password_rq,
                                 "password",
                                 user_password,
                                 user_password_len) == 0);

  return 0;
}

int8_t 
ipmi_kcs_set_user_password (u_int8_t user_id, 
			    u_int8_t operation, 
			    char *user_password,
			    fiid_obj_t obj_data_rs)
{
  fiid_obj_t obj_data_rq; 
  int8_t status;
  
  obj_data_rq = fiid_obj_alloc (tmpl_set_user_password_rq);
  ERR (fill_kcs_set_user_password (obj_data_rq, user_id, operation, 
				   user_password, 
				   (user_password) ? strlen(user_password) : 0) == 0);
  status = ipmi_kcs_cmd (IPMI_BMC_IPMB_LUN_BMC, IPMI_NET_FN_APP_RQ, 
			 obj_data_rq, tmpl_set_user_password_rq, 
			 obj_data_rs, tmpl_set_user_password_rs);
  free (obj_data_rq);
  return status;
}

int8_t 
fill_kcs_set_user_access (fiid_obj_t obj_data_rq, 
			  u_int8_t channel_number,
			  u_int8_t user_id,
			  u_int8_t restrict_to_callback,
			  u_int8_t enable_link_auth,
			  u_int8_t enable_ipmi_msgs,
			  u_int8_t user_privilege_level_limit,
			  u_int8_t user_session_number_limit)
{
  if (obj_data_rq == NULL
      || !IPMI_CHANNEL_NUMBER_VALID(channel_number))
    {
      errno = EINVAL;
      return (-1);
    }

  FIID_OBJ_SET (obj_data_rq, 
		tmpl_set_user_access_rq, 
		"cmd", 
		IPMI_CMD_SET_USER_ACCESS_CMD);
  
  FIID_OBJ_SET (obj_data_rq, 
		tmpl_set_user_access_rq, 
		"modify_flag", 
		1);
  
  FIID_OBJ_SET (obj_data_rq, 
		tmpl_set_user_access_rq, 
		"user_flags.restrict_to_callback", 
		restrict_to_callback);
  
  FIID_OBJ_SET (obj_data_rq, 
		tmpl_set_user_access_rq, 
		"user_flags.enable_link_auth", 
		enable_link_auth);
  
  FIID_OBJ_SET (obj_data_rq, 
		tmpl_set_user_access_rq, 
		"user_flags.enable_ipmi_msgs", 
		enable_ipmi_msgs);
  
  FIID_OBJ_SET (obj_data_rq, 
		tmpl_set_user_access_rq, 
		"channel_number", 
		channel_number);
  
  FIID_OBJ_SET (obj_data_rq, 
		tmpl_set_user_access_rq, 
		"user_id", 
		user_id);
  
  FIID_OBJ_SET (obj_data_rq,
		tmpl_set_user_access_rq,
		"user_privilege_level_limit",
		user_privilege_level_limit);
  
  FIID_OBJ_SET (obj_data_rq,
		tmpl_set_user_access_rq,
		"user_session_number_limit",
		user_session_number_limit);
  
  return 0;
}

int8_t
ipmi_kcs_set_user_access (u_int8_t channel_number,
			  u_int8_t user_id,
			  u_int8_t restrict_to_callback,
			  u_int8_t enable_link_auth,
			  u_int8_t enable_ipmi_msgs,
			  u_int8_t user_privilege_level_limit,
			  u_int8_t user_session_number_limit, 
			  fiid_obj_t obj_data_rs)
{
  fiid_obj_t obj_data_rq; 
  int8_t status;
  
  obj_data_rq = fiid_obj_alloc (tmpl_set_user_access_rq);
  ERR (fill_kcs_set_user_access (obj_data_rq, 
				 channel_number,
				 user_id,
				 restrict_to_callback,
				 enable_link_auth,
				 enable_ipmi_msgs,
				 user_privilege_level_limit,
				 user_session_number_limit) == 0);
  status = ipmi_kcs_cmd (IPMI_BMC_IPMB_LUN_BMC, IPMI_NET_FN_APP_RQ, 
			 obj_data_rq, tmpl_set_user_access_rq, 
			 obj_data_rs, tmpl_set_user_access_rs);
  free (obj_data_rq);
  return status;
}

int8_t
fill_kcs_get_user_access (fiid_obj_t obj_data_rq, 
			  u_int8_t channel_number,
			  u_int8_t user_id)
{
  if (obj_data_rq == NULL
      || !IPMI_CHANNEL_NUMBER_VALID(channel_number))
    {
      errno = EINVAL;
      return (-1);
    }

  FIID_OBJ_SET (obj_data_rq, 
		tmpl_get_user_access_rq, 
		"cmd", 
		IPMI_CMD_GET_USER_ACCESS_CMD);
  
  FIID_OBJ_SET (obj_data_rq, 
		tmpl_get_user_access_rq, 
		"user_id", 
		user_id);
  
  FIID_OBJ_SET (obj_data_rq,
		tmpl_get_user_access_rq,
		"channel_number",
		channel_number);
  
  return 0;
}

int8_t
ipmi_kcs_get_user_access (u_int8_t channel_number,
			  u_int8_t user_id,
			  fiid_obj_t obj_data_rs)
{
  fiid_obj_t obj_data_rq; 
  int8_t status;
  
  obj_data_rq = fiid_obj_alloc (tmpl_get_user_access_rq);
  ERR (fill_kcs_get_user_access (obj_data_rq, channel_number, user_id) == 0);
  status = ipmi_kcs_cmd (IPMI_BMC_IPMB_LUN_BMC, IPMI_NET_FN_APP_RQ, 
			 obj_data_rq, tmpl_get_user_access_rq, 
			 obj_data_rs, tmpl_get_user_access_rs);
  free (obj_data_rq);
  return status;
}


int8_t
fill_kcs_get_channel_access (fiid_obj_t obj_data_rq, 
			     u_int8_t channel_number,
			     u_int8_t channel_access_set_flag)
{
  if (obj_data_rq == NULL
      || !IPMI_CHANNEL_NUMBER_VALID(channel_number))
    {
      errno = EINVAL;
      return (-1);
    }

  FIID_OBJ_SET (obj_data_rq, 
		tmpl_get_channel_access_rq, 
		"cmd", 
		IPMI_CMD_GET_CHANNEL_ACCESS);
  
  FIID_OBJ_SET (obj_data_rq, 
		tmpl_get_channel_access_rq, 
		"channel_number", 
		channel_number);
  
  FIID_OBJ_SET (obj_data_rq,
		tmpl_get_channel_access_rq,
		"channel_access_set_flag",
		channel_access_set_flag);
  
  return 0;
}

int8_t
ipmi_kcs_get_channel_access (u_int8_t channel_number,
			     u_int8_t channel_access_set_flag,
			     fiid_obj_t obj_data_rs)
{
  fiid_obj_t obj_data_rq; 
  int8_t status;
  
  obj_data_rq = fiid_obj_alloc (tmpl_get_channel_access_rq);
  ERR (fill_kcs_get_channel_access (obj_data_rq, channel_number, channel_access_set_flag) == 0);
  status = ipmi_kcs_cmd (IPMI_BMC_IPMB_LUN_BMC, IPMI_NET_FN_APP_RQ, 
			 obj_data_rq, tmpl_get_channel_access_rq, 
			 obj_data_rs, tmpl_get_channel_access_rs);
  free (obj_data_rq);
  return status;
}

int8_t 
fill_kcs_get_channel_info (fiid_obj_t obj_data_rq, u_int8_t channel_number)
{
  if (obj_data_rq == NULL
      || !IPMI_CHANNEL_NUMBER_VALID(channel_number))
    {
      errno = EINVAL;
      return (-1);
    }

  FIID_OBJ_SET (obj_data_rq, 
		tmpl_get_channel_info_rq, 
		"cmd", 
		IPMI_CMD_GET_CHANNEL_INFO_CMD);
  
  FIID_OBJ_SET (obj_data_rq, 
		tmpl_get_channel_info_rq, 
		"channel_number", 
		channel_number);
  
  return 0;
}

int8_t 
ipmi_kcs_get_channel_info (u_int8_t channel_number,
			   fiid_obj_t obj_data_rs)
{
  fiid_obj_t obj_data_rq; 
  int8_t status;
  
  obj_data_rq = fiid_obj_alloc (tmpl_get_channel_info_rq);
  ERR (fill_kcs_get_channel_info (obj_data_rq, channel_number) == 0);
  status = ipmi_kcs_cmd (IPMI_BMC_IPMB_LUN_BMC, IPMI_NET_FN_APP_RQ, 
			 obj_data_rq, tmpl_get_channel_info_rq, 
			 obj_data_rs, tmpl_get_channel_info_rs);
  free (obj_data_rq);
  return status;
}

int8_t
ipmi_check_cmd(fiid_template_t tmpl_cmd, fiid_obj_t obj_cmd, u_int8_t cmd)
{
  u_int64_t cmd_recv;

  if (!(obj_cmd && tmpl_cmd))
    {
      errno = EINVAL;
      return (-1);
    }

  if (!fiid_obj_field_lookup (tmpl_cmd, "cmd"))
    {
      errno = EINVAL;
      return (-1);
    }

  FIID_OBJ_GET(obj_cmd, tmpl_cmd, "cmd", &cmd_recv);

  return ((((int8_t)cmd_recv) == cmd) ? 1 : 0);
}

int8_t
ipmi_check_comp_code(fiid_template_t tmpl_cmd, fiid_obj_t obj_cmd, u_int8_t comp_code)
{
  u_int64_t comp_code_recv;

  if (!(obj_cmd && tmpl_cmd))
    {
      errno = EINVAL;
      return (-1);
    }

  if (!fiid_obj_field_lookup (tmpl_cmd, "comp_code"))
    {
      errno = EINVAL;
      return (-1);
    }

  FIID_OBJ_GET(obj_cmd, tmpl_cmd, "comp_code", &comp_code_recv);

  return ((((int8_t)comp_code_recv) == comp_code) ? 1 : 0);
}

/* achu: return type int8_t is ok, b/c channel numbers only range from
 * 0h - Fh.
 */
static int8_t
_search_for_medium_channel_number (u_int8_t channel_medium_type)
{
  fiid_obj_t data_rs;
  u_int64_t val;
  int i;
  
  FIID_OBJ_ALLOCA (data_rs, tmpl_get_channel_info_rs);
  
  /* Channel numbers range from 0 - 7 */
  for (i = 0; i < 8; i++)
    {
      if (ipmi_kcs_get_channel_info (i, data_rs) != 0)
	continue;
      
      if (IPMI_COMP_CODE(data_rs) != IPMI_COMMAND_SUCCESS)
	continue;
      
      FIID_OBJ_GET (data_rs, 
		    tmpl_get_channel_info_rs, 
		    "channel_medium_type", 
		    &val);
      if ((u_int8_t)val == channel_medium_type)
	{
	  FIID_OBJ_GET (data_rs, 
			tmpl_get_channel_info_rs, 
			"actual_channel_number", 
			&val);
	  return (int8_t)val;
	}
   }
  
  return -1;
}

int8_t 
ipmi_get_channel_number (u_int8_t channel_medium_type)
{
  if (channel_medium_type == IPMI_CHANNEL_MEDIUM_TYPE_LAN_802_3)
    {
      fiid_obj_t obj_data_rs;
      u_int64_t manf_id, prod_id;
  
      FIID_OBJ_ALLOCA (obj_data_rs, tmpl_cmd_get_dev_id_rs);
  
      if (ipmi_kcs_get_dev_id (obj_data_rs) != 0)
	return (-1);
  
      FIID_OBJ_GET (obj_data_rs, tmpl_cmd_get_dev_id_rs, "manf_id.id", &manf_id);
      FIID_OBJ_GET (obj_data_rs, tmpl_cmd_get_dev_id_rs, "prod_id", &prod_id);
      
      switch (manf_id)
	{
	case IPMI_MANF_ID_INTEL:
	case 0xB000157: // Intel 
	  switch (prod_id)
	    {
	    case IPMI_PROD_ID_SE7501WV2:
	      return 7;
	    }
	}
    }
  return _search_for_medium_channel_number (channel_medium_type);
}
