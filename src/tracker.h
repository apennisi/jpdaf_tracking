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

#ifndef _TRACKER_H_
#define _TRACKER_H_

#include <iostream>
#include <Eigen/Dense>
#include <opencv2/opencv.hpp>
#include <opencv2/core/eigen.hpp>
#include <numeric>

#include "track.h"
#include "tracker_param.h"
#include "detection.h"
#include "hungarianAlg.h"

namespace JPDAFTracker
{
  class Tracker
  {
    protected:
      typedef std::vector<Eigen::Vector2f> Vectors2f;
      typedef std::vector<Eigen::MatrixXf> Matrices;
      typedef std::vector<Detection> Detections;
      typedef std::vector<bool> VecBool;
      typedef std::shared_ptr<Track> Track_ptr;
      typedef std::vector< Track_ptr > Tracks;
      typedef std::vector<Eigen::Vector2f> Vec2f;
    public:
      Tracker(const TrackerParam& _param) : param_(_param) { ; }
      void drawTracks(cv::Mat &_img) const;
    public:
      virtual void track(const Detections& _detections) { ; }
      virtual void track(const Detections& _detections, VecBool& _isAssoc, uint& _trackID) { ; }
      virtual inline void push_back(const Track_ptr& _track) { ; }
    public:
      inline const uint size() const
      {
	return tracks_.size();
      }
    public:
      Tracks tracks()
      {
	return tracks_;
      }
      const Tracks tracks() const
      {
	return tracks_;
      }
    protected:
      uint trackID_;
      bool init_;
      bool startTracking_;
      TrackerParam param_;
      Tracks tracks_;
      Vec2f prev_detections_;
      Vectors2f not_associated_;
      cv::RNG rng_;
      Eigen::MatrixXf beta_;
      Eigen::VectorXf last_beta_;
    private:
     virtual void delete_tracks() = 0;
     virtual void manage_new_tracks() { ; }
    protected:
      constexpr static uint MAX_ASSOC = 10000;
    protected:
     VecBool analyze_tracks(const cv::Mat& _q, const Detections& _detections);
     Matrices generate_hypothesis(const Vectors2f& _selected_detections, const cv::Mat& _q);
     Eigen::MatrixXf joint_probability(const Matrices& _association_matrices, const Vectors2f& _selected_detections);
    private:
     virtual void associate(Vectors2f& _selected_detections, cv::Mat& _q, const Detections& _detections) { ; }
     virtual void associate(Vectors2f& _selected_detections, cv::Mat& _q, const Detections& _detections, VecBool& _isAssoc) { ; }
  };
}

#endif