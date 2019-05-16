#include "provided.h"
#include <string>
#include "MyMap.h"
using namespace std;

class AttractionMapperImpl
{
public:
    AttractionMapperImpl();
    ~AttractionMapperImpl();
    void init(const MapLoader& ml);
    bool getGeoCoord(string attraction, GeoCoord& gc) const;
private:
    string changeToLower(string input) const;
    MyMap<string, GeoCoord>* mapOfAttractions;
};

AttractionMapperImpl::AttractionMapperImpl()
{
    mapOfAttractions = new MyMap<string, GeoCoord>;
}

AttractionMapperImpl::~AttractionMapperImpl()
{
    //delete all new objects
    delete mapOfAttractions;
}

void AttractionMapperImpl::init(const MapLoader& ml)
{
    StreetSegment stSegment;
    for (int i = 0; i < ml.getNumSegments(); i++) // for each segment of the street
    {
        ml.getSegment(i, stSegment); 
        for (int j = 0; j < stSegment.attractions.size(); j++)
        {
            string nameOfAttraction = changeToLower(stSegment.attractions[j].name); // get the name of the attraction after changing it to lower case
            GeoCoord location = stSegment.attractions[j].geocoordinates; // get the geoCoords
            mapOfAttractions->associate(nameOfAttraction, location); // link them
        }
    }
}

bool AttractionMapperImpl::getGeoCoord(string attraction, GeoCoord& gc) const
{
    string att = attraction;
    string lowerAtt = changeToLower(att);
    if (mapOfAttractions->find(lowerAtt) != nullptr)
    {
        gc = *(mapOfAttractions->find(lowerAtt));
        return true;
    }
    return false;
}

string AttractionMapperImpl::changeToLower(string input) const
{
    string output; // change all input chars to lower case and store it in output
    for (int i = 0; i < input.length(); i++)
    {
        output += tolower(input[i]);
    }
    return output;
}


//******************** AttractionMapper functions *****************************

// These functions simply delegate to AttractionMapperImpl's functions.
// You probably don't want to change any of this code.

AttractionMapper::AttractionMapper()
{
    m_impl = new AttractionMapperImpl;
}

AttractionMapper::~AttractionMapper()
{
    delete m_impl;
}

void AttractionMapper::init(const MapLoader& ml)
{
    m_impl->init(ml);
}

bool AttractionMapper::getGeoCoord(string attraction, GeoCoord& gc) const
{
    return m_impl->getGeoCoord(attraction, gc);
}
