/*
 * Copyright 2012 Free Software Foundation, Inc.
 *
 * This file is part of GNU Radio
 *
 * GNU Radio is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * GNU Radio is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNU Radio; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef INCLUDED_RUNTIME_API_H
#define INCLUDED_RUNTIME_API_H

//this is part of core for now, treat it as such
#include <gr_core_api.h>
#define GR_RUNTIME_API GR_CORE_API

#include <boost/shared_ptr.hpp>

namespace gnuradio
{

    struct ElementImpl;

}//namespace gnuradio

#endif /* INCLUDED_RUNTIME_API_H */
