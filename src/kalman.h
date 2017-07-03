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


#ifndef _KALMAN_H_
#define _KALMAN_H_

#include <iostream>
#include <Eigen/Dense>
#include <opencv2/opencv.hpp>

namespace JPDAFTracker
{
  class Kalman
  {
    public:
      Kalman() { ; }
      Kalman(const float& dt, const cv::Point2f& target_delta, const float& x, const float& y, const float& vx, const float& vy, const Eigen::Matrix2f& _R);
      cv::Point2f predict();
      void gainUpdate(const float& beta);
      Eigen::Vector4f update(const std::vector< Eigen::Vector2f >& selected_detections, const Eigen::VectorXf& beta, const float& last_beta);
    public:
      inline const Eigen::Matrix2f getS() const
      {
	return S;
      }
      const cv::Point getLastPrediction() const
      {
	return last_prediction;
      }
      inline const Eigen::Vector2f getLastPredictionEigen() const
      {
	return last_prediction_eigen;
      }
      inline const float getEntropy() const
      {
	return entropy;
      }
      void setDt(const double& dt)
      {
	A(4) = A(14) = dt;
      }
      const Eigen::Vector4f getUpdate()
      {
	return x_filter;
      }
    private:
      Eigen::Matrix4f A; //Evolution state matrix
      Eigen::Matrix2f Q; //Covariance Matrix associated to the evolution process
      Eigen::MatrixXf G; //Evolution Noise Matrix
      Eigen::Matrix4f P; //Covariance Matrix
      Eigen::MatrixXf C;
      Eigen::Matrix2f R; //Proces measurement Covariance matrix
      Eigen::Matrix2f S;
      Eigen::MatrixXf K; //Gain
      Eigen::Matrix4f P_predict; //Covariance Matrix predicted error
      Eigen::Vector4f x_predict;
      Eigen::Vector4f x_filter;
      Eigen::Vector2f z_predict;
      cv::Point2f last_prediction;
      Eigen::Vector2f last_prediction_eigen;
      cv::Point2f last_speed;
      bool first;
      float entropy;
    private:
      static constexpr float k = 5.0620; // n/2 * log(4*PI) where n is the state dimention (x, y, x', y')
      //static constexpr float k = 2.1984;
  };
}

#endif