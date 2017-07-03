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

#ifndef _TRACK_H_
#define _TRACK_H_

#include <iostream>
#include <memory>
#include <Eigen/Dense>
#include <opencv2/core/eigen.hpp>
#include <opencv2/opencv.hpp>

#include "kalman.h"

namespace JPDAFTracker
{
  class Track
  {
    public:
      Track() { ; }
      Track(const float& dt, const cv::Point2f& target_delta, const float& x, const float& y, 
	    const float& vx, const float& vy, const float& g_sigma, const float& gamma, const Eigen::Matrix2f& _R);
      cv::Point2f predict();
      void setColor(const cv::Scalar& _color) 
      {
	color_ = _color;
      }
      const cv::Scalar getColor() const
      {
	return color_;
      }
      void setId(const int& _id)
      {
	id = _id;
      }
      const int getId() const
      {
	return id;
      }
      const cv::Point2f getLastPrediction() const
      {
	return KF->getLastPrediction();
      }
      const Eigen::Vector2f getLastPredictionEigen() 
      {
	return KF->getLastPredictionEigen();
      }
      const cv::Mat S_cv() 
      {
	const Eigen::Matrix2f& S = KF->getS();
	cv::Mat Scv;
	cv::eigen2cv(S, Scv);
	return Scv;
      }
      const Eigen::Matrix2f S() const
      {
	return KF->getS();
      }
      const float getEllipseVolume() const
      {
	return ellipse_volume;
      }
      const void gainUpdate(const float& beta)
      {
	KF->gainUpdate(beta);
      }
      const Eigen::Vector4f update(const std::vector< Eigen::Vector2f >& selected_detections, const Eigen::VectorXf& beta, const float& last_beta)
      {
	life_time++;
	nodetections = 0;
	return KF->update(selected_detections, beta, last_beta);
      }
      void increaseLifetime()
      {
	life_time++;
      }
      void notDetected()
      {
	nodetections++;
      }
      bool isAlive()
      {
	return nodetections < maxNotDetection;
      }
      enum TrackState {NONE, ACCEPT, DISCARD};
      const TrackState getEntropy() const
      {
	return entropy_sentinel;
      }
      void setDt(const double& _dt)
      {
	KF->setDt(_dt);
      }
      const Eigen::Vector4f getUpdate()
      {
	return KF->getUpdate();
      }
    private:
      int id;
      int maxNotDetection;
      std::shared_ptr<Kalman> KF;
      float ellipse_volume;
      int number_returns;
      float side;
      float g_sigma;
      float gamma;
      int life_time;
      int nodetections;
      float initial_entropy;
      TrackState entropy_sentinel;
      cv::Point2f last_prediction;
      cv::Scalar color_;
  };
}

#endif