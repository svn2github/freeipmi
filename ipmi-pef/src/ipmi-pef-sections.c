#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#if STDC_HEADERS
#include <string.h>
#endif /* STDC_HEADERS */
#include <errno.h>
#include <assert.h>

#include "ipmi-pef.h"
#include "ipmi-pef-common.h"
#include "ipmi-pef-sections.h"
#include "ipmi-pef-utils.h"

#include "ipmi-pef-community-string.h"
#include "ipmi-pef-alert-destination.h"
#include "ipmi-pef-alert-policy-table.h"
#include "ipmi-pef-event-filter-table.h"

static int
_add_section(struct section **sections, struct section *sect)
{
  if (!sections || !sect)
    return -1;
  
  if (*sections)
    {
      struct section *trav = *sections;
      while (trav->next)
	trav = trav->next;
      trav->next = sect;
    }
  else
    *sections = sect;

  return 0;
}

struct section *
ipmi_pef_sections_list_create (ipmi_pef_state_data_t *state_data)
{
  struct section *sections = NULL;
  struct section *sect = NULL;
  int8_t number_of_alert_destinations,
    number_of_alert_policy_entries,
    number_of_event_filters;
  int i;

  if (get_number_of_alert_destinations(state_data, 
                                       &number_of_alert_destinations) != PEF_ERR_SUCCESS)
    {
      if (state_data->prog_data->args->verbose_wanted)
        fprintf (stderr, "## FATAL: Unable to get Number of Alert Destinations\n");
      return NULL;
    }

  if (get_number_of_alert_policy_entries(state_data,
                                         &number_of_event_filters) != PEF_ERR_SUCCESS)
    {
      if (state_data->prog_data->args->verbose_wanted)
        fprintf (stderr, "## FATAL: Unable to get Number of Alert Policy Entries\n");
      return NULL;
    }
  
  if (get_number_of_event_filters(state_data, 
                                  &number_of_event_filters) != PEF_ERR_SUCCESS)
    {
      if (state_data->prog_data->args->verbose_wanted)
        fprintf (stderr, "## FATAL: Unable to get Number of Event Filters\n");
      return NULL;
    }

  if (!(sect = ipmi_pef_community_string_section_get (state_data)))
    goto cleanup;
  if (_add_section (&sections, sect) < 0)
    goto cleanup;

  for (i = 0; i < number_of_alert_destinations; i++)
    {
      if (!(sect = ipmi_pef_alert_destination_section_get(state_data, i+1)))
	goto cleanup;
      if (_add_section (&sections, sect) < 0)
	goto cleanup;
    }

  for (i = 0; i < number_of_alert_policy_entries; i++)
    {
      if (!(sect = ipmi_pef_alert_policy_table_section_get(state_data, i+1)))
	goto cleanup;
      if (_add_section (&sections, sect) < 0)
	goto cleanup;
    }

  for (i = 0; i < number_of_event_filters; i++)
    {
      if (!(sect = ipmi_pef_event_filter_table_section_get(state_data, i+1)))
	goto cleanup;
      if (_add_section (&sections, sect) < 0)
	goto cleanup;
    }
  
  return sections;

 cleanup:
  ipmi_pef_sections_list_destroy(state_data, sections);
  return NULL;
}

void 
ipmi_pef_sections_list_destroy(ipmi_pef_state_data_t *state_data,
                               struct section *sections)
{
  if (sections)
    {
      while (sections)
	{
	  struct section *sections_next = sections->next;
	  ipmi_pef_section_destroy(state_data, sections);
	  sections = sections_next;
	}
    }
}

struct section * 
ipmi_pef_section_create (ipmi_pef_state_data_t *state_data,
                         char *section_name)
{
  struct section *section = NULL;

  if (!section_name)
    return NULL;

  if (!(section = (struct section *) calloc (1, sizeof(*section))))
    {
      perror("calloc");
      goto cleanup;
    }

  if (!(section->section_name = strdup(section_name)))
    {
      perror("strdup");
      goto cleanup;
    }

  return section;
 cleanup:
  if (section)
    ipmi_pef_section_destroy (state_data, section);
  return NULL;
}

void 
ipmi_pef_section_destroy (ipmi_pef_state_data_t *state_data,
                          struct section *section)
{
  if (section)
    {
      if (section->section_name)
	free(section->section_name);
      
      while (section->keyvalues)
	{
	  struct keyvalue *keyvalue_next = section->keyvalues->next;

	  if (section->keyvalues->value)
	    free(section->keyvalues->value);

	  free(section->keyvalues);
	  section->keyvalues = keyvalue_next;
	}

      free(section);
    }
}

int 
ipmi_pef_section_add_keyvalue (ipmi_pef_state_data_t *state_data,
                               struct section *section,
                               const char *key,
                               const char *desc,
                               unsigned int flags,
                               Keyvalue_Checkout checkout,
                               Keyvalue_Commit commit,
                               Keyvalue_Diff diff,
                               Keyvalue_Validate validate)
{
  struct keyvalue *kv;

  if (!state_data
      || !section
      || !key
      || !desc
      || !checkout
      || !commit
      || !diff
      || !validate)
    return -1;

  if (!(kv = (struct keyvalue *) calloc (1, sizeof(*kv))))
    {
      perror ("calloc");
      goto cleanup;
    }

  kv->key = key;
  kv->desc = desc;
  kv->flags = flags;
  kv->checkout = checkout;
  kv->commit = commit;
  kv->diff = diff;
  kv->validate = validate;

  if (section->keyvalues)
    {
      struct keyvalue *trav = section->keyvalues;
      while (trav->next)
	trav = trav->next;
      trav->next = kv;
    }
  else
    section->keyvalues = kv;

  return 0;
 cleanup:
  if (kv)
    free(kv);
  return -1;
}

static struct section *
ipmi_pef_section_find_section (ipmi_pef_state_data_t *state_data,
                               const char *section_name)
{
  const struct section *sect;

  sect = state_data->sections;

  while (sect) 
    {
      if (same (section_name, sect->section_name))
        break;
      sect = sect->next;
    }

  return (struct section *)sect;
}

struct keyvalue *
ipmi_pef_section_find_keyvalue (ipmi_pef_state_data_t *state_data,
                                const char *section_name,
                                const char *key_name)
{

  const struct section *sect;
  struct keyvalue *kv = NULL;

  if (!(sect = ipmi_pef_section_find_section (state_data,
                                              section_name)))
    {
      fprintf (stderr, "Unknown section `%s'\n", section_name);
      return NULL;
    }

  kv = sect->keyvalues;

  while (kv) 
    {
      if (same (key_name, kv->key))
        break;
      kv = kv->next;
    }

  if (!kv) 
    {
      fprintf (stderr, "Unknown key `%s' in section `%s'\n",
               key_name, section_name);
      return NULL;
    }

  return kv;
}

int
ipmi_pef_section_set_value (ipmi_pef_state_data_t *state_data,
                            const char *section_name,
                            const char *key_name,
                            const char *value)
{
  struct section *sect;
  struct keyvalue *kv;

  if (!(sect = ipmi_pef_section_find_section (state_data, section_name)))
    return -1;

  if (!(kv = ipmi_pef_section_find_keyvalue (state_data, section_name, key_name)))
    return -1;

  if (kv->validate)
    {
      pef_validate_t v;

      if ((v = kv->validate (state_data, sect, value)) == PEF_VALIDATE_FATAL_ERROR)
        return -1;
      
      if (v == PEF_VALIDATE_INVALID_VALUE)
        {
          fprintf (stderr, "Invalid value `%s' for key `%s'\n",
                   value, key_name);
          return -1;
        }
    }

  if (kv->value)
    free (kv->value);
  
  if (!(kv->value = strdup (value)))
    {
      perror("strdup");
      return -1;
    }

  return 0;
}

pef_err_t
ipmi_pef_section_commit_value (ipmi_pef_state_data_t *state_data,
                               const char *section_name,
                               const char *key_name,
                               const char *value)
{
  struct section *sect;
  struct keyvalue *kv;

  if (!(sect = ipmi_pef_section_find_section (state_data, section_name)))
    return PEF_ERR_FATAL_ERROR;

  if (!(kv = ipmi_pef_section_find_keyvalue (state_data, section_name, key_name)))
    return PEF_ERR_FATAL_ERROR;

  if (kv->validate)
    {
      pef_validate_t v;

      if ((v = kv->validate (state_data, sect, value)) == PEF_VALIDATE_FATAL_ERROR)
        return PEF_ERR_FATAL_ERROR;
      
      if (v == PEF_VALIDATE_INVALID_VALUE)
        {
          fprintf (stderr, "Invalid value `%s' for key `%s'\n",
                   value, key_name);
          return PEF_ERR_NON_FATAL_ERROR;
        }
    }

  if (kv->value)
    free (kv->value);

  if (!(kv->value = strdup (value)))
    {
      perror("strdup");
      return PEF_ERR_FATAL_ERROR;
    }

  return kv->commit (state_data, sect, kv);
}

int
ipmi_pef_section_diff_value (ipmi_pef_state_data_t *state_data,
                             const char *section_name,
                             const char *key_name,
                             const char *value)
{
  struct section *sect;
  struct keyvalue *kv;

  if (!(sect = ipmi_pef_section_find_section (state_data, section_name)))
    return PEF_ERR_FATAL_ERROR;

  if (!(kv = ipmi_pef_section_find_keyvalue (state_data, section_name, key_name)))
    return PEF_ERR_FATAL_ERROR;

  if (kv->validate)
    {
      pef_validate_t v;

      if ((v = kv->validate (state_data, sect, value)) == PEF_VALIDATE_FATAL_ERROR)
        return PEF_ERR_FATAL_ERROR;
      
      if (v == PEF_VALIDATE_INVALID_VALUE)
        {
          fprintf (stderr, "Invalid value `%s' for key `%s'\n",
                   value, key_name);
          return PEF_ERR_NON_FATAL_ERROR;
        }
    }

  if (kv->value)
    free (kv->value);

  if (!(kv->value = strdup (value)))
    {
      perror("strdup");
      return PEF_ERR_FATAL_ERROR;
    }

  return kv->diff (state_data, sect, kv);
}

pef_err_t
ipmi_pef_sections_list (ipmi_pef_state_data_t *state_data)
{
  struct section *sect;

  sect = state_data->sections;

  while (sect)
    {
      printf("%s\n", sect->section_name); 
      sect = sect->next;
    }

  return PEF_ERR_SUCCESS;
}
