/*
 * Copyright (C) 2017  Benjamin Peter <benjaminpeter@arcor.de>
 *
 * This file is part of InklingReader
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
 */

#include "merge-clock-entries.h"

#include "../datatypes/element.h"
#include "../datatypes/clock.h"

#include <stdio.h>
#include <stdlib.h>

void merge_clock_entries(GSList** data, GSList** clock_list_entries, unsigned int* consecutive_clocks)
{
  if (*consecutive_clocks > 2) {

    // Keep the first and the last clock entry
    GSList* clock_list_it = (*clock_list_entries)->next;
    while (clock_list_it != NULL)
    {
      if (--(*consecutive_clocks) > 1) {
        GSList* link_to_be_removed = clock_list_it->data;
        g_free(link_to_be_removed->data);
        *data = g_slist_delete_link(*data, link_to_be_removed);
      }
      clock_list_it = clock_list_it->next;
    }
  }

  g_slist_free(*clock_list_entries);
  *clock_list_entries = NULL;
  *consecutive_clocks = 0;
}

GSList* opt_merge_clock_entries (GSList* data)
{
  GSList* it = data;
  unsigned int consecutive_clocks = 0;
  GSList* clock_list_entries = NULL;

  while (it != NULL)
  {
    dt_element* e = (dt_element *)it->data;
    if (e->type == TYPE_CLOCK) {
      consecutive_clocks++;
      clock_list_entries = g_slist_append(clock_list_entries, it);
    }
    else {
      merge_clock_entries(&data, &clock_list_entries, &consecutive_clocks);
    }

    it = it->next;
  }

  merge_clock_entries(&data, &clock_list_entries, &consecutive_clocks);

  return data;
}

