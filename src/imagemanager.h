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

/**
 * \file imagemanager.h
 *
 * \class ImageManager
 *
 * \brief Class for managing a set of images
 *
 **/

#ifndef IMAGEMANAGER_H
#define IMAGEMANAGER_H

#include <vector>

class ImageManager
{
public:
  /**
  * \brief Create a new object ImageManager
  *
  * \param dir: directory path
  *
  */
  explicit ImageManager(const std::string& dir);

  /**
   * \brief Return the next image
   *
   * \return return the next image
   *
   */
  std::string getNext(const int& speed);
private:
  /**
   * \brief Sort the file names according to the natural sort algorithm
   *
   * \param data: the vector containing the names of the files
   *
   */
  void sorting(std::vector<std::string>& data) const;
  std::vector<std::string> getAllFilesInFolder(const std::string& path) const;
  size_t currentFrameIndex;
  std::vector<std::string> fileNames;
};

#endif // IMAGEMANAGER_H
