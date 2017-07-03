#ifndef _LOCAL_TRACKER_
#define _LOCAL_TRACKER_

#include "tracker.h"

namespace JPDAFTracker
{
  class LocalTracker : public Tracker
  {
    public:
      LocalTracker(const TrackerParam& _param);
      void track(const Detections& _detections, VecBool& _isAssoc, uint& _trackID);
    public:
      inline void push_back(const Track_ptr& _track)
      {
	tracks_.push_back(_track);
      }
      inline Vectors2f getNotAssociated() const
      {
	return not_associated_;
      }
      const Tracks getTracks() const
      {
	return tracks_;
      }
    private:
      double time;
    private:
     void associate(Vectors2f& _selected_detections, cv::Mat& _q, const Detections& _detections, VecBool& _isAssoc);
     void delete_tracks();
  };
}

#endif