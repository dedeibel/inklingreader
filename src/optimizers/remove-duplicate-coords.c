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

#include "remove-duplicate-coords.h"

#include <stdio.h>
#include "../datatypes/element.h"
#include "../datatypes/coordinate.h"

static void remove_list_entries(GSList** data, GSList** coord_list_entries)
{
  GSList* coord_list_it = (*coord_list_entries);
  while (coord_list_it != NULL)
  {
    GSList* link_to_be_removed = coord_list_it->data;
    g_free(link_to_be_removed->data);
    *data = g_slist_delete_link(*data, link_to_be_removed);
    coord_list_it = coord_list_it->next;
  }

  g_slist_free(*coord_list_entries);
  *coord_list_entries = NULL;
}

static int is_equal_coordinate(dt_coordinate* a, dt_coordinate* b) {
  return
    a->x == b->x &&
    a->y == b->y;
}

GSList* opt_remove_duplicate_coords (GSList* data)
{
  GSList* it = data;
  int found_duplicate = 0;
  GSList* coord_list_entries = NULL;
  dt_coordinate* prev_coordinate = NULL;

  while (it != NULL)
  {
    dt_element* e = (dt_element *)it->data;
    if (prev_coordinate == NULL) {
      if (e->type == TYPE_COORDINATE) {
        prev_coordinate = (dt_coordinate*) e;
      }
    }
    else {
      if (e->type == TYPE_COORDINATE) {
        if (is_equal_coordinate(prev_coordinate, (dt_coordinate*)e)) {
          found_duplicate = 1;
          coord_list_entries = g_slist_append(coord_list_entries, it);
        }
        else {
          found_duplicate = 0;
          prev_coordinate = (dt_coordinate*)e;
          remove_list_entries(&data, &coord_list_entries);
        }
      }
      else if (found_duplicate && (e->type == TYPE_TILT || e->type == TYPE_PRESSURE)) {
        coord_list_entries = g_slist_append(coord_list_entries, it);
      }
      else if (e->type == TYPE_STROKE) {
        dt_stroke* s = (dt_stroke *)e;
        if (! s->value) {
          found_duplicate = 0;
          prev_coordinate = NULL;
          remove_list_entries(&data, &coord_list_entries);
        }
      }
    }

    it = it->next;
  }

  remove_list_entries(&data, &coord_list_entries);
  return data;
}
