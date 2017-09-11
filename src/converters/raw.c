/*
 * Copyright (C) 2013  Roel Janssen <roel@moefel.org>
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

#include "raw.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <locale.h>
#include "../datatypes/configuration.h"
#include "../datatypes/element.h"
#include "../datatypes/clock.h"

#define SPIKE_THRESHOLD 25.0

extern dt_configuration settings;

/*----------------------------------------------------------------------------.
  | CO_WRITE_RAW_FILE                                                          |
  '----------------------------------------------------------------------------*/
int
co_raw_create_file (const char* filename, GSList* data)
{
  char* output = co_raw_create (data);
  if (data == NULL) return 1;

  FILE* file;
  file = fopen (filename, "w");
  if (file != NULL)
    fwrite (output, strlen (output), 1, file);
  else
  {
    printf ("%s: Couldn't write to '%s'.\r\n", __func__, filename);
    return 1;
  }

  free (output);
  fclose (file);
  return 0;
}

char*
co_raw_create (GSList* data)
{
  /* Floating-point numbers should be written with a dot instead of a comma.
   * To ensure that this happens, (temporarily) set the locale to the "C"
   * locale for this program. */
  setlocale (LC_NUMERIC, "C");

  if (g_slist_length (data) == 0)
  {
    printf ("%s: No useful data was found in the file.\r\n", __func__);
    return NULL;
  }

  int written = 0;

  size_t output_len = 64 * g_slist_length (data) + 1;
  char* output = malloc (output_len);
  if (output == NULL)
  {
    printf ("%s: Couldn't allocate enough memory.\r\n", __func__);
    return NULL;
  }

  /*--------------------------------------------------------------------------.
    | COUNTING VARIABLES                                                       |
    '--------------------------------------------------------------------------*/
  unsigned short time = 0;
  double subtime = 0;

  dt_coordinate prev;

  GSList* stroke_data = NULL; 

  /*--------------------------------------------------------------------------.
    | WRITE DATA POINTS                                                        |
    '--------------------------------------------------------------------------*/
  while (data != NULL)
  {
    written += sprintf (output + written, "%013.6f: ", time + subtime);
    dt_element* e = (dt_element *)data->data;

    switch (e->type)
    {
      /*------------------------------------------------------------------.
        | COORDINATE DESCRIPTOR                                            |
        '------------------------------------------------------------------*/
      case TYPE_COORDINATE:
        {
          dt_coordinate* c = (dt_coordinate *)e;

          written += sprintf (output + written, "COORDINATE %13.6f, %13.6f", c->x, c->y);

          float distance = sqrt(
              (c->x - prev.x) * (c->x - prev.x) +
              (c->y - prev.y) * (c->y - prev.y));

          if (distance > SPIKE_THRESHOLD)
            written += sprintf(output + written, " skip");

          if (c->x == prev.x && c->y == prev.y)
            written += sprintf(output + written, " dup");

          prev.x = c->x, prev.y = c->y;
          subtime += CLOCK_FREQUENCY;
        }
        break;
        /*------------------------------------------------------------------.
          | CLOCK PRESSURE                                                   |
          '------------------------------------------------------------------*/
      case TYPE_PRESSURE:
        {
          dt_pressure* p = (dt_pressure *)e;
          written += sprintf (output + written, "PRESSURE   %13.6f", p->pressure);
        }
        break;
        /*------------------------------------------------------------------.
          | CLOCK STROKE                                                     |
          '------------------------------------------------------------------*/
      case TYPE_STROKE:
        {
          dt_stroke* s = (dt_stroke *)e;
          if (s->value == 1)
            written += sprintf (output + written, "STROKE      START");
          else if (s->value == 0)
            written += sprintf (output + written, "STROKE       STOP");
          else
            written += sprintf (output + written, "STROKE     %6hu", s->value);
        }
        break;
        /*------------------------------------------------------------------.
          | CLOCK TILT                                                       |
          '------------------------------------------------------------------*/
      case TYPE_TILT:
        {
          dt_tilt* t = (dt_tilt *)e;
          written += sprintf (output + written, "TILT          %3hu, %3hu", t->x, t->y);
        }
        break;
        /*------------------------------------------------------------------.
          | CLOCK DESCRIPTOR                                                 |
          '------------------------------------------------------------------*/
      case TYPE_CLOCK:
        {
          dt_clock* c = (dt_clock *)e;
          written += sprintf (output + written, "CLOCK      %6hu", c->counter);
          time = c->counter;
          subtime = 0;
        }
        break;
    }

    written += sprintf(output + written, "\n");

    data = data->next;
  }

  written += sprintf(output + written, "\n");

  output_len = written + 1;
  output = realloc(output, output_len);
  output[written] = '\0';

  g_slist_free (data);
  data = NULL;

  /* Reset to default locale settings. */
  setlocale (LC_NUMERIC, "");

  return output;
}
