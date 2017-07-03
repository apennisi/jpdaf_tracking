#include "global_tracker.h"

using namespace JPDAFTracker;

GlobalTracker::GlobalTracker(const TrackerParam& _param)
  : Tracker(_param)
{
  trackID_ = 0;
  init_ = true;
  startTracking_ = false;
  rng_ = cv::RNG(12345);
}

void GlobalTracker::track(const GlobalTracker::Detections& _detections)
{
  if(init_)
  {
    prev_detections_.clear();
    for(const auto& det : _detections)
    {
      prev_detections_.push_back(Eigen::Vector2f(det.x(), det.y()));
    }
    init_ = false;
  }
  else if(!init_ && !startTracking_)
  {
    not_associated_.clear();
    for(const auto& det : _detections)
    {
      not_associated_.push_back(Eigen::Vector2f(det.x(), det.y()));
    }
    manage_new_tracks();
    if(localTrackers_.size() > 0)
    {
      tracks_.clear();
      tracks_ = localTrackers_.at(0)->tracks();
      startTracking_ = true;
    }
    else
    {
      prev_detections_ = not_associated_;
    }
  }
  else
  {
    not_associated_.clear();
    VecBool isAssoc(_detections.size(), false); //all the detections are not associated
    uint i;
    for(const auto& tracker : localTrackers_)
    {
      tracker->track(_detections, isAssoc, trackID_);
    }
    i = 0;
    for(const auto& ass : isAssoc)
    {
      if(!ass)
      {
	not_associated_.push_back(Eigen::Vector2f(_detections.at(i).x(), _detections.at(i).y()));
      }
      ++i;
    }
    
    delete_tracks();
    
    tracks_.clear();
    for(const auto& tracker : localTrackers_)
    {
      Tracks tr = tracker->tracks();
      for(const auto& t : tr)
      {
	tracks_.push_back(t);
      }
    }
    
    cv::Mat_<int> q(cv::Size(tracks_.size(), _detections.size()), int(0));
    
    std::vector<Eigen::Vector2f> selected_detections;
  
    //ASSOCIATION
    std::vector<bool> not_associate;
    associate(selected_detections, q, _detections);
    
    if(q.total() > 0)
    {
      not_associate = analyze_tracks(q, _detections); //ASSIGN ALL THE NOT ASSOCIATED TRACKS
      //HYPOTHESIS
      const Matrices& association_matrices = generate_hypothesis(selected_detections, q);
      
      //COMPUTE JOINT PROBABILITY
      beta_ = joint_probability(association_matrices, selected_detections);
      last_beta_ = beta_.row(beta_.rows() - 1);
	
      //KALMAN PREDICT STEP
      uint i = 0, j = 0;
      
      for(const auto& track : tracks_)
      {
	if(not_associate.at(j))
	{
	  track->gainUpdate(last_beta_(i));
	}
	j++;
	i++;
      }
      
      //UPDATE AND CORRECT
      i = 0, j = 0;
      for(const auto& track : tracks_)
      {
	if(not_associate.at(j))
	{
	  track->update(selected_detections, beta_.col(i), beta_(beta_.rows() - 1, i) );
	}
	++j;
	++i;
      }
    }
    
    manage_new_tracks(); 
  }
}

void GlobalTracker::delete_tracks()
{
  for(int i = localTrackers_.size() - 1; i >= 0; --i)
  {
    if(localTrackers_.at(i)->size() == 0)
    {
      localTrackers_.erase(localTrackers_.begin() + i);
    }
  }
}


void GlobalTracker::manage_new_tracks()
{
  const uint& prevDetSize = prev_detections_.size();
  const uint& deteSize = not_associated_.size();
  if(prevDetSize == 0)
  {
    prev_detections_ = not_associated_;
  }
  else if (deteSize == 0)
  {
    prev_detections_.clear();
  }
  else
  {
    cv::Mat assigmentsBin = cv::Mat::zeros(cv::Size(deteSize, prevDetSize), CV_32SC1);
    cv::Mat costMat = cv::Mat(cv::Size(deteSize, prevDetSize), CV_32FC1);
    
    auto euclideanDist = [](const Eigen::Vector2f& p1, const Eigen::Vector2f& p2)
			    { 
			      const Eigen::Vector2f& tmp = p1 - p2; 
			      return sqrt(tmp(0) * tmp(0) + tmp(1) * tmp(1));
			    };

    assignments_t assignments;
    distMatrix_t costs(deteSize * prevDetSize);

    for(uint i = 0; i < prevDetSize; ++i)
    {
      for(uint j = 0; j < deteSize; ++j)
      {
	costs.at(i + j * prevDetSize ) = euclideanDist(not_associated_.at(j), prev_detections_.at(i));
	costMat.at<float>(i, j) = costs.at(i + j * prevDetSize );
      }
    }
    
    	  
    AssignmentProblemSolver APS;
    APS.Solve(costs, prevDetSize, deteSize, assignments, AssignmentProblemSolver::optimal);

    const uint& assSize = assignments.size();
    
    for(uint i = 0; i < assSize; ++i)
    {
      if( assignments[i] != -1 && costMat.at<float>(i, assignments[i]) < param_.assocCost)
      {
	assigmentsBin.at<int>(i, assignments[i]) = 1;
      }
    }
    
    const uint& rows = assigmentsBin.rows;
    const uint& cols = assigmentsBin.cols;
    
    LocalTracker_ptr tracker = LocalTracker_ptr(new LocalTracker(param_));
        
    
    for(uint i = 0; i < rows; ++i)
    {
      for(uint j = 0; j < cols; ++j)
      {
	if(assigmentsBin.at<int>(i, j))
	{
	  const float& vx = not_associated_.at(j).x() - prev_detections_.at(i).x();
	  const float& vy = not_associated_.at(j).y() - prev_detections_.at(i).y();
	  std::shared_ptr<Track> tr(new Track(param_.dt, param_.target_delta, not_associated_.at(j).x(), not_associated_.at(j).y(), 
						vx, vy, param_.g_sigma, param_.gamma, param_.R ));
	  tracker->push_back(tr);
	}
      }
    }
    if(tracker->size() > 0)
    {
      localTrackers_.push_back(tracker);
    }
      
    
    cv::Mat notAssignedDet(cv::Size(assigmentsBin.cols, 1), CV_32SC1, cv::Scalar(0));
    for(uint i = 0; i < assigmentsBin.rows; ++i)
    {
      notAssignedDet += assigmentsBin.row(i);
    }
    
    notAssignedDet.convertTo(notAssignedDet, CV_8UC1);
    notAssignedDet = notAssignedDet == 0;
    
    cv::Mat dets;
    cv::findNonZero(notAssignedDet, dets);
    prev_detections_.clear();
    for(uint i = 0; i < dets.total(); ++i)
    {
      const uint& idx = dets.at<cv::Point>(i).x;
      prev_detections_.push_back(not_associated_.at(i));
    }
  }
}


void GlobalTracker::associate(std::vector< Eigen::Vector2f >& _selected_detections, cv::Mat& _q, 
			const std::vector< Detection >& _detections)
{
  //Extracting the measurements inside the validation gate for all the tracks
  //Create a q matrix with a width = clutter + number of tracks
  _q = cv::Mat_<int>(cv::Size(tracks_.size() + 1, _detections.size()), int(0));
  uint validationIdx = 0;
  uint j = 0;
  
  auto euclideanDist = [](const Eigen::Vector2f& _p1, const Eigen::Vector2f& _p2)
			    { 
			      const Eigen::Vector2f& tmp = _p1 - _p2; 
			      return sqrt(tmp(0) * tmp(0) + tmp(1) * tmp(1));
			    };
  
  for(const auto& detection : _detections)
  {
    Eigen::Vector2f det;
    det << detection.x(), detection.y();
    uint i = 1;
    bool found = false;
    cv::Mat det_cv(cv::Size(2, 1), CV_32FC1);
    det_cv.at<float>(0) = det(0);
    det_cv.at<float>(1) = det(1);
    for(auto& track : tracks_)
    {
      const Eigen::Vector2f& tr = track->getLastPredictionEigen();
      cv::Mat tr_cv(cv::Size(2, 1), CV_32FC1);
      tr_cv.at<float>(0) = tr(0);
      tr_cv.at<float>(1) = tr(1);
      const int& id = track->getId();
      const Eigen::Matrix2f& S = track->S().inverse();
      cv::Mat S_cv;
      cv::eigen2cv(S, S_cv);
      const float& mah = cv::Mahalanobis(tr_cv, det_cv, S_cv);
      const float& eucl = euclideanDist(det, tr);
      if(mah <= param_.global_g_sigma && eucl <= param_.global_assocCost)
      {
	_q.at<int>(validationIdx, 0) = 1;
	_q.at<int>(validationIdx, i) = 1;
	found = true;
      }
      ++i;
    }
    if(found)
    {
      _selected_detections.push_back(det);
      validationIdx++;
    }
    ++j;
  }
  _q = _q(cv::Rect(0, 0, tracks_.size() + 1, validationIdx));
  ////////////////////////////////////////////////////////////////////////////
}