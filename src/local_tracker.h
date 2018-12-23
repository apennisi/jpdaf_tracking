#ifndef _LOCAL_TRACKER_
#define _LOCAL_TRACKER_

#include "tracker.h"

namespace JPDAFTracker
{
  class LocalTracker : public Tracker
  {
  public:
    explicit LocalTracker(const TrackerParam& _param);
    void track(const Detections& _detections, VecBool& _isAssoc, uint& _trackID) override;
  public:
    void push_back(const Track_ptr& _track) override
    {
      tracks_.push_back(_track);
    }

    Vectors2f getNotAssociated() const
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
    void associate(Vectors2f& _selected_detections, cv::Mat& _q, const Detections& _detections, VecBool& _isAssoc) override;
    void delete_tracks() override;
  };
}

#endif
