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

/**
 * @file   optimizers/point_reduction.h
 * @brief  A filter to reduce the amount of points in the drawing.
 * @author Roel Janssen
 */

#ifndef OPTIMIZERS_POINT_REDUCTION_H
#define OPTIMIZERS_POINT_REDUCTION_H

#include <glib.h>

int opt_point_reduction_apply (GSList* data);

#endif//OPTIMIZERS_POINT_REDUCTION_H
