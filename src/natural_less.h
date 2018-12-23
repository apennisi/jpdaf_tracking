/*
 *  natural_less
 *
 *  Created by Kenneth Laskoski on 16/12/12.
 *  Copyright (c) 2012 Kenneth Laskoski. All rights reserved.
*/

#ifndef NATURAL_LESS_H
#define NATURAL_LESS_H

#include <iostream>
#include <cstdlib>     /* strtol */

inline int strcmp_natural(const char* a, const char* b)
{
  if (!a || !b) // if one doesn't exist, it comes first
    return a ? 1 : b ? -1 : 0;

  if (isdigit(*a) && isdigit(*b)) // both start with numbers
  {
    char* remainderA;
    char* remainderB;
    const auto valA = strtol(a, &remainderA, 10);
    const auto valB = strtol(b, &remainderB, 10);
    if (valA != valB)
    {
      return valA - valB; // smaller comes first
    }
    else
    {
      const auto lengthA = remainderA - a;
      const auto lengthB = remainderB - b;

      if (lengthA != lengthB)
      {
        if (lengthA - lengthB >= std::numeric_limits<int>::max())
          throw std::runtime_error("The strings are too long!");
        return static_cast<int>(lengthA - lengthB); // shorter comes first
      }
      else // all being equal, recurse
        return strcmp_natural(remainderA, remainderB);
    }
  }

  if (isdigit(*a) || isdigit(*b)) // if just one is a number
    return isdigit(*a) ? -1 : 1; // numbers always come first

  while (*a && *b) // non-numeric characters
  {
    if (isdigit(*a) || isdigit(*b))
      return strcmp_natural(a, b); // recurse
    if (*a != *b)
      return *a - *b;
    a++;
    b++;
  }
  return *a ? 1 : *b ? -1 : 0;
}

inline bool natural_sort(const std::string& lhs, const std::string& rhs)
{
  return strcmp_natural(lhs.c_str(), rhs.c_str()) < 0;
}


#endif // NATURAL_LESS_H
