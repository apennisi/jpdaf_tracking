#include "track.h"

using namespace JPDAFTracker;

Track::Track(const float& dt, const cv::Point2f& target_delta, const float& x, const float& y, const float& vx, const float& vy,
    const float& g_sigma, const float& gamma, const Eigen::Matrix2f& _R)
  : g_sigma(g_sigma), gamma(gamma)
{
  KF = std::shared_ptr<Kalman>(new Kalman(dt, target_delta, x, y, vx, vy, _R));
  life_time = 0;
  nodetections = 0;
  maxNotDetection = 10;
  entropy_sentinel = TrackState::NONE;
  id = -1;
}


cv::Point2f Track::predict()
{
  last_prediction = KF->predict();
  const Eigen::Matrix2f& S = KF->getS();
  if(life_time == 0) 
  {
    initial_entropy = KF->getEntropy();
  }
  else if(nodetections >= maxNotDetection)
  {
    entropy_sentinel = TrackState::DISCARD;
  }
  else if(life_time >= 10)
  {
    entropy_sentinel = TrackState::ACCEPT;
  }
  
  //Compute the volume VG
  ellipse_volume = CV_PI * g_sigma * sqrt(S.determinant());
  const float& param = ellipse_volume*gamma+1;
  number_returns = std::floor(param);
  side = sqrt(param / gamma) * .5; 
  return last_prediction;
}


