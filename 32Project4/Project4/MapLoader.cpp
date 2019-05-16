#include <stdio.h>
#include <iostream>
#include <sstream>
#include "provided.h"
#include <vector>
#include <fstream>
using namespace std;

class MapLoaderImpl
{
public:
    MapLoaderImpl();
    ~MapLoaderImpl();
    bool load(string mapFile);
    size_t getNumSegments() const;
    bool getSegment(size_t segNum, StreetSegment& seg) const;
private:
    vector<string> getCoords(string input);
    GeoSegment getGeoSegment(string input);
    Attraction getAttraction(string input);
    vector <StreetSegment*> m_streetSegs;
    string trim(string const& str);

    
};

MapLoaderImpl::MapLoaderImpl()
{
    
}

MapLoaderImpl::~MapLoaderImpl()
{
    //  delete all new objects
    vector<StreetSegment*>::iterator it_vector;
    StreetSegment* temp = nullptr;
    
    it_vector = m_streetSegs.begin();
    while (it_vector != m_streetSegs.end())
    {
        temp = *it_vector;
        it_vector++;
        delete temp;
    }
    
}

bool MapLoaderImpl::load(string mapFile)
{
    ifstream infile(mapFile);    // infile is a name of our choosing
    if ( ! infile )		        // Did opening the file fail?
    {
        cerr << "Error: Cannot open mapdata.txt!" << endl;
        return false;
    }
    string input;
    string geoCoords;
    int numOfAttractions;
    while(getline(infile, input))
    {
        StreetSegment* tempStSeg = new StreetSegment; // get the name of the street
        tempStSeg->streetName = input;
        
        getline(infile, input);
        tempStSeg->segment = getGeoSegment(input); // get the GeoSegment
        infile >> numOfAttractions; // get all the attractions of that segment
        
        getline(infile, input);
        if (numOfAttractions > 0)
        {
            for (int i = 0; i < numOfAttractions; i++) // for each attraction push onto the vector
            {
                getline(infile, input);
                tempStSeg->attractions.push_back(getAttraction(input));
            }
        }
        m_streetSegs.push_back(tempStSeg); //  push new street segment on to the vector of st segments
    }
    return true;
}

size_t MapLoaderImpl::getNumSegments() const
{
    return m_streetSegs.size();
}

bool MapLoaderImpl::getSegment(size_t segNum, StreetSegment &seg) const
{
    if (segNum >= getNumSegments())
        return false;
    seg = *m_streetSegs[segNum];
    return true;
}

vector<string> MapLoaderImpl::getCoords(string input)
{
    vector<string> coordinates;
    istringstream ss1(input);
    int count = 0;
    string token1;
    string token2;
    
    while(getline(ss1, token1, ',')) // get line till the ','
    {
        if (count == 1)
        {
            istringstream ss2(trim(token1));
            
            while(getline(ss2, token2, ' ')) // get line till the ' '
            {
                coordinates.push_back(trim(token2));
            }
        }
        else
        {
            coordinates.push_back(trim(token1));
        }
        count++;
    }
    return coordinates;
}

GeoSegment MapLoaderImpl::getGeoSegment(string input)
{
    vector<string> coords = getCoords(input); // create a new vector of coordinates
    GeoCoord start(coords[0], coords[1]);
    GeoCoord end(coords[2], coords[3]);
    GeoSegment segment(start, end); // use these to create a new GeoSement
    return segment;
}

Attraction MapLoaderImpl::getAttraction(string input)
{
    Attraction attraction;
    string token, longitude, latitude;
    istringstream ss(input);
    int count = 0;
    while(getline(ss, token, '|')) // get line till '|'
    {
        if (count == 0)
        {
            attraction.name = token;
        }
        else if(count == 1)
        {
            istringstream ss2(token);
            getline(ss2, longitude, ','); // get line till ','
            getline(ss2, latitude, ','); // get line till ','
            GeoCoord attCoord(trim(longitude), trim(latitude));
            attraction.geocoordinates = attCoord;
        }
        count++;
    }
    return attraction;
}

string MapLoaderImpl::trim(string const& str)
{
    // this function removes all spaces from the string
    if(str.empty())
        return str;
    
    std::size_t firstScan = str.find_first_not_of(' ');
    std::size_t first     = firstScan == std::string::npos ? str.length() : firstScan;
    std::size_t last      = str.find_last_not_of(' ');
    return str.substr(first, last-first+1);
}
//******************** MapLoader functions ************************************

// These functions simply delegate to MapLoaderImpl's functions.
// You probably don't want to change any of this code.

MapLoader::MapLoader()
{
    m_impl = new MapLoaderImpl;
}

MapLoader::~MapLoader()
{
    delete m_impl;
}

bool MapLoader::load(string mapFile)
{
    return m_impl->load(mapFile);
}

size_t MapLoader::getNumSegments() const
{
    return m_impl->getNumSegments();
}

bool MapLoader::getSegment(size_t segNum, StreetSegment& seg) const
{
    return m_impl->getSegment(segNum, seg);
}
