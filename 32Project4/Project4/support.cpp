//
//  support.cpp
//  BruinNav
//
//  Created by Shreya Donepudi on 16/03/17.
//  Copyright © 2017 Brute. All rights reserved.
//

#include "support.h"

/*
 0 degrees <= travelAngle <= 22.5 degrees: east
 ● 22.5 degrees < travelAngle <= 67.5 degrees: northeast
 ● 67.5 degrees < travelAngle <= 112.5 degrees: north
 ● 112.5 degrees < travelAngle <= 157.5 degrees: northwest
 ● 157.5 degrees < travelAngle <= 202.5 degrees: west
 ● 202.5 degrees < travelAngle <= 247.5 degrees: southwest
 ● 247.5 degrees < travelAngle <= 292.5 degrees: south
 ● 292.5 degrees < travelAngle <= 337.5 degrees: southeast
 ● 337.5 degrees < travelAngle < 360 degrees: east
 */

std::string directionOfLine(const GeoSegment &geoSeg)
{
    if(angleOfLine(geoSeg) > 22.5 && angleOfLine(geoSeg) <= 67.5)
        return "northeast";
    if(angleOfLine(geoSeg) > 67.5 && angleOfLine(geoSeg) <= 112.5)
        return "north";
    if(angleOfLine(geoSeg) > 112.5 && angleOfLine(geoSeg) <= 157.5)
        return "northwest";
    if(angleOfLine(geoSeg) > 157.5 && angleOfLine(geoSeg) <= 202.5)
        return "west";
    if(angleOfLine(geoSeg) > 202.5 && angleOfLine(geoSeg) <= 247.5)
        return "southwest";
    if(angleOfLine(geoSeg) > 247.5 && angleOfLine(geoSeg) <= 292.5)
        return "south";
    if(angleOfLine(geoSeg) > 292.5 && angleOfLine(geoSeg) <= 337.5)
        return "southeast";
    else
        return "east";
}
