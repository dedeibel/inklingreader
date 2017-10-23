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

#include "remove-spike-coords.h"

#include <stdio.h>
#include <math.h>

#include "../datatypes/element.h"
#include "../datatypes/coordinate.h"
#include "../datatypes/clock.h"

/* Taken from svg.c as a starting point, THRESHOLD * SHRINK */
const static float OPT_SPIKE_THRESHOLD = 440.;

static float distance(dt_coordinate* a, dt_coordinate* b) {
  return sqrt(
      (a->x - b->x) * (a->x - b->x) +
      (a->y - b->y) * (a->y - b->y));
}

static int is_spike_coordinate(dt_coordinate* prev, dt_coordinate* current) {
  return distance(prev, current) > OPT_SPIKE_THRESHOLD;
}

GSList* opt_remove_spike_coords (GSList* data)
{
  GSList* it = data;
  GSList* delete_link;
  dt_coordinate* prev_coordinate = NULL;

  while (it != NULL)
  {
    dt_element* e = (dt_element *)it->data;

    if (e->type == TYPE_COORDINATE) {
      if (prev_coordinate != NULL) {
        //
        // TODO now working yet - see output
        //
        //printf("REMOVE COORD %.0fx%.0f – %.0fx%.0f - distance %f\n", ((dt_coordinate*)e)->x, ((dt_coordinate*)e)->y, prev_coordinate->x, prev_coordinate->y, distance(prev_coordinate, (dt_coordinate*)e));
        if (is_spike_coordinate(prev_coordinate, (dt_coordinate*)e)) {

          // remove coord and tilt and pressure
          delete_link = it;
          g_free(delete_link->data);
          if (it->next) {
            it = it->next;
          }
          data = g_slist_delete_link(data, delete_link);

          if (it != NULL) {
            dt_element* elem = (dt_element*)it->data;
            if (elem->type == TYPE_PRESSURE) {
              delete_link = it;
              g_free(delete_link->data);
              if (it->next) {
                it = it->next;
              }
              data = g_slist_delete_link(data, delete_link);
            }
          }

          if (it != NULL) {
            dt_element* elem = (dt_element*)it->data;
            if (elem->type == TYPE_TILT) {
              delete_link = it;
              g_free(delete_link->data);
              if (it->next) {
                it = it->next;
              }
              data = g_slist_delete_link(data, delete_link);
            }
          }
        }
      }

      prev_coordinate = (dt_coordinate*) e;
    }
    else if (e->type == TYPE_STROKE) {
      prev_coordinate = NULL;
    }

    it = it->next;
  }

  return data;
}
