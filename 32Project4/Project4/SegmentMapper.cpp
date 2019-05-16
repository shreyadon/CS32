#include "provided.h"
#include "support.h"
#include <vector>
#include "MyMap.h"


using namespace std;

class SegmentMapperImpl
{
public:
    SegmentMapperImpl();
    ~SegmentMapperImpl();
    void init(const MapLoader& ml);
    vector<StreetSegment> getSegments(const GeoCoord& gc) const;
private:
    MyMap<GeoCoord, vector<StreetSegment>>* mapOfGeoCoords;

};

SegmentMapperImpl::SegmentMapperImpl()
{
    mapOfGeoCoords = new MyMap<GeoCoord, vector<StreetSegment>>;
}

SegmentMapperImpl::~SegmentMapperImpl()
{
    delete mapOfGeoCoords;
}

void SegmentMapperImpl::init(const MapLoader& ml)
{
    StreetSegment stSeg;
    vector<StreetSegment> *stSegVec;
    size_t numberOfAttractions;
    for (size_t i = 0; i < ml.getNumSegments(); i++)
    {
        ml.getSegment(i, stSeg);
        
        // The StreetSegment starts at that geocoordinate
        stSegVec = mapOfGeoCoords->find(stSeg.segment.start);
        if (stSegVec == nullptr) // if no vector exists then create a new vector and associate in the map
        {
            vector<StreetSegment> tempVector;
            tempVector.push_back(stSeg);
            mapOfGeoCoords->associate(stSeg.segment.start, tempVector);
        }
        else // if vector already exists with that geocoord just add the st. segment to the vector
            (*stSegVec).push_back(stSeg);
        
        // The StreetSegment ends at that geocoordinate
        stSegVec = mapOfGeoCoords->find(stSeg.segment.end);
        if (stSegVec == nullptr) // if no vector exists then create a new vector and associate in the map
        {
            vector<StreetSegment> tempVector;
            tempVector.push_back(stSeg);
            mapOfGeoCoords->associate(stSeg.segment.end, tempVector);
        }
        else// if vector already exists with that geocoord just add the st. segment to the vector
            (*stSegVec).push_back(stSeg);
        
        // The StreetSegment has an attraction on it with that geocoordinate
        numberOfAttractions = stSeg.attractions.size();
        for (int i = 0; i < numberOfAttractions; i++)
        {
            stSegVec = mapOfGeoCoords->find(stSeg.attractions[i].geocoordinates);
            if (stSegVec == nullptr) // if no vector exists then create a new vector and associate in the map

            {
                vector<StreetSegment> tempVector;
                tempVector.push_back(stSeg);
                mapOfGeoCoords->associate(stSeg.attractions[i].geocoordinates, tempVector);
            }
            else// if vector already exists with that attraction just add the st. segment to the vector
                (*stSegVec).push_back(stSeg);
        }
    }
}

vector<StreetSegment> SegmentMapperImpl::getSegments(const GeoCoord& gc) const
{
    vector<StreetSegment> segments;
    if (mapOfGeoCoords->find(gc) != nullptr)
    {
        segments = *(mapOfGeoCoords->find(gc));
    }
    return segments;
}

//******************** SegmentMapper functions ********************************

// These functions simply delegate to SegmentMapperImpl's functions.
// You probably don't want to change any of this code.

SegmentMapper::SegmentMapper()
{
    m_impl = new SegmentMapperImpl;
}

SegmentMapper::~SegmentMapper()
{
    delete m_impl;
}

void SegmentMapper::init(const MapLoader& ml)
{
    m_impl->init(ml);
}

vector<StreetSegment> SegmentMapper::getSegments(const GeoCoord& gc) const
{
    return m_impl->getSegments(gc);
}
