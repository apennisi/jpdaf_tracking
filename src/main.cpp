#include <fstream>
#include "tracker.h"
#include "global_tracker.h"
#include "imagemanager.h"
#include "detection.h"

using namespace JPDAFTracker;

std::map<int, std::vector<std::vector<std::string>>> petsReading(const std::string& _gt)
{
  std::ifstream file(_gt);
  if (!file.is_open())
  {
    std::cerr << "Error: cannot read the file: " << _gt << std::endl;
    exit(-1);
  }


  std::string line;
  std::string delimiter(",");
  std::map<int, std::vector<std::vector<std::string>>> pets;


  while (std::getline(file, line))
  {
    if (line.size() == 0)
      continue;
    size_t start = 0;
    auto end = line.find(delimiter);
    std::vector<std::string> row;
    while (end != std::string::npos)
    {
      row.push_back(line.substr(start, end - start));
      start = end + delimiter.length();
      end = line.find(delimiter, start);
    }
    row.push_back(line.substr(start, end - start));

    const uint& n = atoi(row[1].c_str());
    std::vector<std::vector<std::string>> detections;
    uint j = 2;
    for (uint i = 0; i < n; ++i)
    {
      std::vector<std::string> currDetections;
      try
      {
        currDetections.push_back(row[j]);
        currDetections.push_back(row[++j]);
        currDetections.push_back(row[++j]);
        currDetections.push_back(row[++j]);
      }
      catch (...)
      {
        std::cerr << "Error: cannot read parse:\n " << line << std::endl;
        exit(-1);
      }
      ++j;
      detections.push_back(currDetections);
    }

    pets.insert(std::make_pair(atoi(row[0].c_str()), detections));
  }

  return pets;
}

cv::Mat H = (cv::Mat_<double>(3, 3) << -2.24449, -8.3239, 1146.4,
             -0.913708, -10.0553, 1058.36,
             -0.00251937, -0.0164418, 1);

cv::Mat H_inv = H.inv();

void calcProjection(const cv::Point2f& point, double& x, double& y)
{
  x = (point.x * H.at<double>(0, 0) + point.y * H.at<double>(0, 1) + H.at<double>(0, 2)) /
      (point.x * H.at<double>(2, 0) + point.y * H.at<double>(2, 1) + H.at<double>(2, 2));
  y = (point.x * H.at<double>(1, 0) + point.y * H.at<double>(1, 1) + H.at<double>(1, 2)) /
      (point.x * H.at<double>(2, 0) + point.y * H.at<double>(2, 1) + H.at<double>(2, 2));
}

void calcInvProjection(const cv::Point2f& point, double& x, double& y)
{
  x = (point.x * H_inv.at<double>(0, 0) + point.y * H_inv.at<double>(0, 1) + H_inv.at<double>(0, 2)) /
      (point.x * H_inv.at<double>(2, 0) + point.y * H_inv.at<double>(2, 1) + H_inv.at<double>(2, 2));
  y = (point.x * H_inv.at<double>(1, 0) + point.y * H_inv.at<double>(1, 1) + H_inv.at<double>(1, 2)) /
      (point.x * H_inv.at<double>(2, 0) + point.y * H_inv.at<double>(2, 1) + H_inv.at<double>(2, 2));
}

cv::Mat mosaic(const cv::Mat& img1, const cv::Mat& img2)
{
  cv::Mat _mosaic(cv::Size(img1.cols * 2, img1.rows), CV_8UC3);
  img1.copyTo(_mosaic(cv::Rect(0, 0, img1.cols, img1.rows)));
  img2.copyTo(_mosaic(cv::Rect(img1.cols, 0, img2.cols, img2.rows)));
  return _mosaic;
}

int main(int argc, char** argv)
{
  if (argc != 4)
  {
    return -1;
  }
  try
  {
    TrackerParam params;
    params.read(std::string(argv[1]));

    auto detections = petsReading(argv[2]);
    auto planView = cv::imread("PlanView.png");
    ImageManager img(argv[3]);
    std::shared_ptr<Tracker> tracker(new GlobalTracker(params));
    std::vector<std::vector<std::string>> curr;
    std::vector<Detection> dets;

    cv::Mat image, trackingImg, plan;
    cv::Rect rect;
    std::vector<cv::Rect> rects;
    std::vector<cv::Point2f> points;

    const auto& milliseconds = 1000.0 / 7.0;

    // Setup output video
    cv::VideoWriter output_cap(
      "output.avi",
      cv::VideoWriter::fourcc('D', 'I', 'V', 'X'),
      7,
      cv::Size(1536, 576));

    for (uint i = 0; i < detections.size(); ++i)
    {
      rects.clear();
      points.clear();
      dets.clear();
      image = cv::imread(img.getNext(1));
      trackingImg = image.clone();
      //plan = planView.clone();

      curr = detections[i + 1];
      std::stringstream ss;
      auto j = 0;
      double x, y;
      for (const auto& c : curr)
      {
        rect = cv::Rect(
          cvRound(strtod(c[0].c_str(), nullptr)),
          cvRound(strtod(c[1].c_str(), nullptr)),
          cvRound(strtod(c[2].c_str(), nullptr)),
          cvRound(strtod(c[3].c_str(), nullptr)));

        rects.push_back(rect);

        points.push_back(cv::Point2f(rect.x + (rect.width >> 1), rect.y + rect.height));
        calcProjection(cv::Point2f(rect.x + (rect.width >> 1), rect.y + rect.height), x, y);
        Detection d(rect.x + (rect.width >> 1), rect.y + (rect.height >> 1), rect.width, rect.height);
        dets.push_back(d);

        cv::rectangle(image, rect, cv::Scalar(0, 0, 255), 3);

        ss.str("");
        ss << j;

        cv::putText(
          image,
          ss.str(),
          cv::Point(cvRound(atof(c[0].c_str())), cvRound(atof(c[1].c_str()))),
          cv::FONT_HERSHEY_SIMPLEX,
          0.55,
          cv::Scalar(0, 255, 0),
          2,
          cv::LineTypes::LINE_AA);

        ++j;
      }

      tracker->track(dets);
      tracker->drawTracks(trackingImg);
      const auto& m = mosaic(image, trackingImg);
      cv::imshow("JPDAFTracker", m);
      output_cap << m;
      cv::waitKey(cvRound(milliseconds));
      dets.clear();
    }
  }
  catch (...)
  {
    return -1;
  }

  return 0;
}
