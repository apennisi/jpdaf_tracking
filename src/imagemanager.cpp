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

ImageManager::ImageManager(const std::string &d)
{
    const auto& dir_name = d;
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

    assert(!fileNames.empty());
    sorting(fileNames);
    currentFrameIndex = 0;
}

void ImageManager::sorting(std::vector<std::string>& data) const
{
    std::sort(data.begin(), data.end(), natural_sort);
}

std::string ImageManager::getNext(const int &speed) {
    auto currentFile = fileNames[currentFrameIndex];
    if(currentFrameIndex + speed > fileNames.size())
    {
        currentFrameIndex = fileNames.size();
    }
    else
    {
        currentFrameIndex += speed;
    }
    return currentFile;
}
