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


#ifndef _TRACKER_PARAM_H_
#define _TRACKER_PARAM_H_

#include <iostream>
#include <fstream>
#include <Eigen/Dense>
#include <opencv2/opencv.hpp>

namespace JPDAFTracker
{
  class TrackerParam
  {
    public:
      float pd;
      float pg;
      float g_sigma;
      float lambda;
      float gamma;
      float assocCost;
      float global_assocCost;
      float global_g_sigma;
      int max_missed_rate;
      int min_acceptance_rate;
      cv::Point2f target_delta;
      Eigen::Matrix2f R;
      float dt;
    public:
      TrackerParam() { ; }
      void read(const std::string& filename);
      TrackerParam& operator=(const TrackerParam& param_copy)
      {
	this->pd = param_copy.pd;
	this->pg = param_copy.pg;
	this->g_sigma = param_copy.g_sigma;
	this->lambda = param_copy.lambda;
	this->gamma = param_copy.gamma;
	this->target_delta = param_copy.target_delta;
	this->assocCost = param_copy.assocCost;
	this->dt = param_copy.dt;
	return *this;
      }
  };
}


#endif