//
//  support.h
//  BruinNav
//
//  Created by Shreya Donepudi on 14/03/17.
//  Copyright © 2017 Brute. All rights reserved.
//

#ifndef support_h
#define support_h
#include "provided.h"

inline
bool operator== (const GeoCoord &a, const GeoCoord &b)
{
    return (a.latitude == b.latitude && a.longitude == b.longitude);
}

inline
bool operator< (const GeoCoord &a, const GeoCoord &b)
{
    if (a.latitude != b.latitude)
        return (a.latitude < b.latitude);
    else
        return (a.longitude < b.longitude);
}

inline
bool operator> (const GeoCoord &a, const GeoCoord &b)
{
    if (a.latitude != b.latitude)
        return (a.latitude > b.latitude);
    else
        return (a.longitude > b.longitude);
}

std::string directionOfLine(const GeoSegment &geoSeg);

#endif /* support_h */
