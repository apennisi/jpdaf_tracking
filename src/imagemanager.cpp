/*
 *  EAT Event Annotation Tool
 *  Copyright 2015 Andrea Pennisi
 *
 *  This file is part of AT and it is distributed under the terms of the
 *  GNU Lesser General Public License (Lesser GPL)
 *
 *
 *
 *  EAT is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  EAT is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with EAT.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 *  EAT has been written by Andrea Pennisi
 *
 *  Please, report suggestions/comments/bugs to
 *  andrea.pennisi@gmail.com
 *
 */

#include <imagemanager.h>
#include <natural_less.h>
#include <string>
#include <algorithm>
#include <cassert>
#ifdef _WINDOWS
#include <Windows.h>
#elif
#include <dirent.h>
#endif


std::vector<std::string> ImageManager::getAllFilesInFolder(const std::string& path) const
{
#ifdef _WINDOWS
  std::vector<std::string> names;
  auto search_path = path + "/*.*";
  WIN32_FIND_DATA fd;
  const auto hFind = ::FindFirstFile(search_path.c_str(), &fd);
  if (hFind != INVALID_HANDLE_VALUE)
  {
    do
    {
      // read all (real) files in current folder
      // , delete '!' read other 2 default folder . and ..
      if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
      {
        names.emplace_back(path + fd.cFileName);
      }
    }
    while (::FindNextFile(hFind, &fd));
    ::FindClose(hFind);
  }
  return names;
#elif
     DIR *dir;
     try
     {
         dir = opendir(d.c_str());
     } catch (std::exception& e)
     {
         std::cout << "Directory does not exist " << e.what() << std::endl;
     }

     struct dirent *dp;
     while ((dp=readdir(dir)) != NULL) {
         if(strcmp(dp->d_name, "..") != 0  &&  strcmp(dp->d_name, ".") != 0 && dp->d_name[0] != '.'
                 && dp->d_name[0] != '~') {
             fileNames.push_back(dir_name + "/" + std::string(dp->d_name));
         }
     }
#endif
}

ImageManager::ImageManager(const std::string& dir)
{
  fileNames = getAllFilesInFolder(dir);

  assert(!fileNames.empty());
  sorting(fileNames);
  currentFrameIndex = 0;
}

void ImageManager::sorting(std::vector<std::string>& data) const
{
  std::sort(data.begin(), data.end(), natural_sort);
}

std::string ImageManager::getNext(const int& speed)
{
  auto currentFile = fileNames[currentFrameIndex];
  if (currentFrameIndex + speed > fileNames.size())
  {
    currentFrameIndex = fileNames.size();
  }
  else
  {
    currentFrameIndex += speed;
  }
  return currentFile;
}
