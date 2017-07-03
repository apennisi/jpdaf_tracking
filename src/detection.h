/*
 *  JPDAFTracker 
 *  Copyright 2017 Andrea Pennisi
 *
 *  This file is part of JPDAFTracker and it is distributed under the terms of the
 *  GNU Lesser General Public License (Lesser GPL)
 *
 *
 *
 *  JPDAFTracker is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  JPDAFTracker is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with JPDAFTracker.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 *  JPDAFTracker has been written by Andrea Pennisi
 *
 *  Please, report suggestions/comments/bugs to
 *  andrea.pennisi@gmail.com
 *
 */

#ifndef _DETECTION_H_
#define _DETECTION_H_

#include <iostream>
#include <opencv2/opencv.hpp>

namespace JPDAFTracker
{
  class Detection
  {
    public:
      Detection(const float& _x, const float& _y, const int& _w, const int& _h)
	: m_x(_x), m_y(_y), m_w(_w), m_h(_h) 
      { 
	point = cv::Point2f(m_x, m_y);
	bbox = cv::Rect(m_x - (m_w >> 1), m_y - m_h, m_w, m_h);
      }
      const float x() const { return m_x; }
      const float y() const { return m_y; }
      const float w() const { return m_w; }
      const float h() const { return m_h; }
      const cv::Rect getRect() const { return bbox; }
      const cv::Point2f operator()() const
      {
	return point;
      }
      Detection& operator=(const Detection& d_copy)
      {
	this->m_x = d_copy.x();
	this->m_y = d_copy.y();
	this->m_w = d_copy.w();
	this->m_h = d_copy.h();
	return *this;
      }
    private:
      float m_x, m_y;
      int m_w, m_h;
      cv::Point2f point;
      cv::Rect bbox;
  };
}

#endif