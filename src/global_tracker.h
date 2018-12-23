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

#ifndef _GLOBAL_TRACKER_H_
#define _GLOBAL_TRACKER_H_

#include "tracker.h"

namespace JPDAFTracker
{
  class GlobalTracker : public Tracker
  {
  private:
    typedef std::shared_ptr<Tracker> LocalTracker_ptr;
    typedef std::vector<LocalTracker_ptr> LocalTrackers;
  public:
    explicit GlobalTracker(const TrackerParam& _param);
    void track(const Detections& _detections) override;
  private:
    LocalTrackers localTrackers_;
  private:
    void delete_tracks() override;
    void manage_new_tracks() override;
  private:
    void associate(Vectors2f& _selected_detections, cv::Mat& _q, const Detections& _detections) override;
  };
}

#endif
