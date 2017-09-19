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

#include "remove-coords-without-stroke.h"

#include <stdio.h>
#include "../datatypes/element.h"
#include "../datatypes/coordinate.h"

void delete_entries(GSList** data, GSList** coord_list_entries, unsigned int inside_stroke)
{
  if (*coord_list_entries == NULL) {
    return;
  }

  if (! inside_stroke) { 
    unsigned int removed = 0;
    GSList* coord_list_it = (*coord_list_entries)->next;
    while (coord_list_it != NULL)
    {
      *data = g_slist_delete_link(*data, coord_list_it->data);
      coord_list_it = coord_list_it->next;
      removed++;
    }

    if (removed)
      printf("  deleted %hd elements out of strokes\n", removed);
  }

  g_slist_free(*coord_list_entries);
  *coord_list_entries = NULL;
}

GSList* pro_remove_coords_without_stroke (GSList* data)
{
  GSList* it = data;
  GSList* coord_list_entries = NULL;
  unsigned char inside_stroke = 0;

  while (it != NULL)
  {
    dt_element* e = (dt_element *)it->data;

    if (e->type == TYPE_COORDINATE) {
      if (! inside_stroke) {
        coord_list_entries = g_slist_append(coord_list_entries, it);
      }
    }
    if (e->type == TYPE_PRESSURE) {
      if (! inside_stroke) {
        coord_list_entries = g_slist_append(coord_list_entries, it);
      }
    }
    if (e->type == TYPE_TILT) {
      if (! inside_stroke) {
        coord_list_entries = g_slist_append(coord_list_entries, it);
      }
    }
    else if (e->type == TYPE_STROKE) {
      dt_stroke* s = (dt_stroke *)e;

      delete_entries(&data, &coord_list_entries, inside_stroke);

      inside_stroke = s->value;
    }

    it = it->next;
  }

  delete_entries(&data, &coord_list_entries, inside_stroke);

  return data;
}

