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

/**
 * @file   converters/raw.h
 * @brief  List read raw data as readable list
 * @author Benjamin Peter
 */

#ifndef CONVERTERS_RAW_H
#define CONVERTERS_RAW_H

#include <glib.h>

int co_raw_create_file (const char* filename, GSList* data);
char* co_raw_create (GSList* data);

#endif//CONVERTERS_RAW_H
