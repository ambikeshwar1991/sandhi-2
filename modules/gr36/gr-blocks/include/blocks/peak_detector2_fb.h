/* -*- c++ -*- */
/*
 * Copyright 2007,2013 Free Software Foundation, Inc.
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

#ifndef INCLUDED_GR_PEAK_DETECTOR2_FB_H
#define INCLUDED_GR_PEAK_DETECTOR2_FB_H

#include <blocks/api.h>
#include <gr_sync_block.h>

namespace gr {
  namespace blocks {

    /*!
     * \brief Detect the peak of a signal
     * \ingroup level_blk
     *
     * If a peak is detected, this block outputs a 1, or it outputs
     * 0's. A separate debug output may be connected, to view the
     * internal EWMA described below.
     *
     * \param threshold_factor_rise The threshold factor determins
     *        when a peak is present. An EWMA average of the signal is
     *        calculated and when the value of the signal goes over
     *        threshold_factor_rise*average, we call the peak.
     * \param look_ahead The look-ahead value is used when the
     *        threshold is found to locate the peak within this range.
     * \param alpha The gain value of a single-pole moving average filter.
     */
    class BLOCKS_API peak_detector2_fb : virtual public gr_sync_block
    {
    public:
      // gr::blocks::peak_detector2_fb::sptr
      typedef boost::shared_ptr<peak_detector2_fb> sptr;

      static sptr make(float threshold_factor_rise=7,
                       int look_ahead=1000, float alpha=0.001);

      /*! \brief Set the threshold factor value for the rise time
       *  \param thr new threshold factor
       */
      virtual void set_threshold_factor_rise(float thr) = 0;

      /*! \brief Set the look-ahead factor
       *  \param look new look-ahead factor
       */
      virtual void set_look_ahead(int look) = 0;

      /*! \brief Set the running average alpha
       *  \param alpha new alpha for running average
       */
      virtual void set_alpha(int alpha) = 0;

      /*! \brief Get the threshold factor value for the rise time
       *  \return threshold factor
       */
      virtual float threshold_factor_rise() = 0;

      /*! \brief Get the look-ahead factor value
       *  \return look-ahead factor
       */
      virtual int look_ahead() = 0;

      /*! \brief Get the alpha value of the running average
       *  \return alpha
       */
      virtual float alpha() = 0;
    };

  } /* namespace blocks */
} /* namespace gr */

#endif /* INCLUDED_GR_PEAK_DETECTOR2_FB_H */
