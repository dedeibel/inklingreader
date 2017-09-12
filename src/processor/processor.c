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

#include "processor.h"

#include "../datatypes/element.h"
#include "../datatypes/clock.h"

#include "merge-clock-entries.h"
#include "remove-coords-without-stroke.h"
#include "remove-duplicate-coords.h"
#include "remove-spike-coords.h"
#include "remove-empty-strokes.h"
#include "enrich-coords.h"

GSList* pro_process (GSList* data)
{
  GSList* head = data;
  head = pro_merge_clock_entries(data);
  head = pro_remove_coords_without_stroke(data);
  head = pro_remove_duplicate_coords(data);
  head = pro_remove_spike_coords(data);
  head = pro_remove_empty_strokes(data);
  head = pro_enrich_coords(data);
  return head;
}
