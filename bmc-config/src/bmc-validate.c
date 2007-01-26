#include "bmc-config.h"
#include "bmc-common.h"
#include "bmc-config-api.h"
#include "bmc-diff.h"
#include "bmc-map.h"
#include "bmc-sections.h"
#include "bmc-validate.h"

bmc_validate_t 
yes_no_validate (const struct bmc_config_arguments *args,
                 const struct section *sect,
                 const char *value)
{
  if (value && (same (value, "yes") || same (value, "no")))
    return BMC_VALIDATE_VALID_VALUE;
  return BMC_VALIDATE_INVALID_VALUE;
}

bmc_validate_t 
number_range_one_byte (const struct bmc_config_arguments *args,
                       const struct section *sect,
                       const char *value)
{
  long int conv;
  char *endptr;

  conv = strtol (value, &endptr, 0);

  if (*endptr)
    return BMC_VALIDATE_INVALID_VALUE;

  if (conv < 0 || conv > 255)
    return BMC_VALIDATE_INVALID_VALUE;

  return BMC_VALIDATE_VALID_VALUE;
}

bmc_validate_t
ip_address_validate (const struct bmc_config_arguments *args,
                     const struct section *sect,
                     const char *value)
{
  struct in_addr a;

  if (inet_aton (value, &a))
    return BMC_VALIDATE_VALID_VALUE;
  return BMC_VALIDATE_INVALID_VALUE;
}

bmc_validate_t
mac_address_validate (const struct bmc_config_arguments *args,
                      const struct section *sect,
                      const char *value)
{
  unsigned int foo;

  if (sscanf (value,
              "%02x:%02x:%02x:%02x:%02x:%02x",
              &foo,
              &foo,
              &foo,
              &foo,
              &foo,
              &foo) == 6)
    return BMC_VALIDATE_VALID_VALUE;
  return BMC_VALIDATE_INVALID_VALUE;
}

bmc_validate_t 
channel_access_mode_validate (const struct bmc_config_arguments *args,
                              const struct section *sect,
                              const char *value)
{
  if (channel_access_mode (value) >= 0)
    return BMC_VALIDATE_VALID_VALUE;
  return BMC_VALIDATE_INVALID_VALUE;
}

bmc_validate_t 
get_privilege_limit_number_validate (const struct bmc_config_arguments *args,
                                     const struct section *sect,
                                     const char *value)
{
  if (get_privilege_limit_number (value) > 0)
    return BMC_VALIDATE_VALID_VALUE;
  return BMC_VALIDATE_INVALID_VALUE;
}

bmc_validate_t 
privilege_level_number_validate (const struct bmc_config_arguments *args,
                                 const struct section *sect,
                                 const char *value)
{
  if (privilege_level_number (value) > 0)
    return BMC_VALIDATE_VALID_VALUE;
  return BMC_VALIDATE_INVALID_VALUE;
}

bmc_validate_t 
rmcpplus_priv_number_validate (const struct bmc_config_arguments *args,
                               const struct section *sect,
                               const char *value)
{
  if (rmcpplus_priv_number (value) >= 0)
    return BMC_VALIDATE_VALID_VALUE;
  return BMC_VALIDATE_INVALID_VALUE;
}

bmc_validate_t 
ip_address_source_number_validate (const struct bmc_config_arguments *args,
                                   const struct section *sect,
                                   const char *value)
{
  if (ip_address_source_number (value) >= 0)
    return BMC_VALIDATE_VALID_VALUE;
  return BMC_VALIDATE_INVALID_VALUE;
}

bmc_validate_t 
power_restore_policy_number_validate (const struct bmc_config_arguments *args,
                                      const struct section *sect,
                                      const char *value)
{
  if (power_restore_policy_number (value) != -1)
    return BMC_VALIDATE_VALID_VALUE;
  return BMC_VALIDATE_INVALID_VALUE;
}

bmc_validate_t 
connect_mode_number_validate (const struct bmc_config_arguments *args,
                              const struct section *sect,
                              const char *value)
{
  if (connect_mode_number (value) != -1)
    return BMC_VALIDATE_VALID_VALUE;
  return BMC_VALIDATE_INVALID_VALUE;
}

bmc_validate_t 
flow_control_number_validate (const struct bmc_config_arguments *args,
                              const struct section *sect,
                              const char *value)
{
  if (flow_control_number (value) > -1)
    return BMC_VALIDATE_VALID_VALUE;
  return BMC_VALIDATE_INVALID_VALUE;
}

bmc_validate_t 
bit_rate_number_validate (const struct bmc_config_arguments *args,
                          const struct section *sect,
                          const char *value)
{
  if (bit_rate_number (value) > -1)
    return BMC_VALIDATE_VALID_VALUE;
  return BMC_VALIDATE_INVALID_VALUE;
}

bmc_validate_t 
sol_bit_rate_number_validate (const struct bmc_config_arguments *args,
                              const struct section *sect,
                              const char *value)
{
  if (sol_bit_rate_number (value) != -1)
    return BMC_VALIDATE_VALID_VALUE;
  return BMC_VALIDATE_INVALID_VALUE;
}

