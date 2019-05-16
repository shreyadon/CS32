#include "support.h"
#include "provided.h"
#include <string>
#include <vector>
#include <list>
#include <iostream>
using namespace std;

class NavigatorImpl
{
public:
    NavigatorImpl();
    ~NavigatorImpl();
    bool loadMapData(string mapFile);
    NavResult navigate(string start, string end, vector<NavSegment>& directions) const;
private:
    MapLoader mapData;
    AttractionMapper attMap;
    SegmentMapper segMap;
    struct Node
    {
        Node(Node* parent, GeoCoord geoCoords, StreetSegment stSeg, double cost, double guess)
        {
            m_parent = parent;
            m_geoCoords = geoCoords;
            m_stSeg = stSeg;
            m_cost = cost;
            m_guess = guess;
            m_total = m_cost + m_guess;
        }
        Node()
        {
            m_parent = nullptr;
            m_total = m_cost = m_guess = 0;
        }
        Node *m_parent;
        GeoCoord m_geoCoords;
        StreetSegment m_stSeg;
        double m_total, m_cost, m_guess;
    };
    
    list <Node*> *open; //open list
    list <Node*> *closed; //closed list
    bool checkSuccessor(list<Node*>* listNode, Node* successor) const;
    void checkSuccessorAndPushBack(list<Node*>* openList, list<Node*>* closedList, Node* successor) const;
    void getDirections(Node *destination, vector<NavSegment>& directions) const;
};

NavigatorImpl::NavigatorImpl()
{
    open = new list<Node*>;
    closed = new list<Node*>;
}

NavigatorImpl::~NavigatorImpl()
{
    list<Node*>::iterator it_list;
    Node* temp = nullptr;
    
    it_list = open->begin();
    while (it_list != open->end())
    {
        temp = *it_list;
        it_list++;
        delete temp;
    }
    delete open;
    
    it_list = closed->begin();
    while (it_list != closed->end())
    {
        temp = *it_list;
        it_list++;
        delete temp;
    }
    delete closed;
}

bool NavigatorImpl::loadMapData(string mapFile)
{
    if(!mapData.load(mapFile))
    {
        return false;
    }
    attMap.init(mapData);
    segMap.init(mapData);
    return true;  
}

NavResult NavigatorImpl::navigate(string start, string end, vector<NavSegment> &directions) const
{
    GeoCoord fillMe;
    bool found = attMap.getGeoCoord(start, fillMe);
    if (!found) // couldn't find a valid starting location
    {
        cerr << "No geolocation found for " << start << endl;
        return NAV_BAD_SOURCE;
    }
    found = attMap.getGeoCoord(end, fillMe);
    if (!found) // couldn't find a valid destination
    {
        cerr << "No geolocation found for " << end << endl;
        return NAV_BAD_DESTINATION;
    }

    GeoCoord startAttCoords, endAttCoords;
    attMap.getGeoCoord(start, startAttCoords); //initialize start pos
    attMap.getGeoCoord(end, endAttCoords); //initialize end pos
    //distance from beginning to end
    double totalDist = distanceEarthMiles(startAttCoords, endAttCoords);

    vector<StreetSegment> firstSegment = segMap.getSegments(startAttCoords); //
    //node first;
    // should be 1 becuase startattcoords are in the middle of the stseg
    Node* first = new Node(nullptr, startAttCoords, firstSegment[0], 0, totalDist);
    open->push_front(first); //put starting node on open list
    
    list<Node*>::iterator p;
    Node* cur = nullptr;
    while(!open->empty()) // following A* algorithm
    {
        

        p = open->begin();
        double minTotal = (*p)->m_total; // start with the first element of the list
        cur = *p;
        while  (p != open->end())
        {
            if ((*p)->m_total < minTotal) // if a better total cost is found change the cur node
            {
                minTotal = (*p)->m_total;
                cur = *p;
            }
            p++;
        }
        open->remove(cur); // remove the current node from the list
        
        vector<StreetSegment> stSuccessors = segMap.getSegments((*cur).m_geoCoords); // creating new vector of street segements that successors to the current node
        
        Node* successor = nullptr;
        for(size_t i = 0; i < stSuccessors.size(); i++) // for each successor
        {
            for (size_t j = 0; j < stSuccessors[i].attractions.size(); j++)  // for each attraction on the st segment
            {
                if (stSuccessors[i].attractions[j].geocoordinates == endAttCoords) // if the coordinates of the attraction match those of the end location. create a new with the st segment's data
                {
                    successor = new Node(cur, endAttCoords, stSuccessors[i], cur->m_cost + distanceEarthMiles(cur->m_geoCoords, endAttCoords), 0);
                    //checkSuccessorAndPushBack(openList, closedList, successor);
                    
                    closed->push_back(cur); // push back the start point on to the closed list
                    //this statement is skipped, and would cause a memory leak because cur would never be deleted?
                    
                    closed->push_back(successor); // push the successor on to the closed list
                    
                    getDirections(successor, directions); // get direction to the destination
                    // found a route.
                    return NAV_SUCCESS;
                }
            }
            if (stSuccessors[i].segment.start == cur->m_geoCoords)
            {
                //if your current node geocoord matches with your st segments start geocoord
                // create a new successor node with the geocoords of the st segment end
                successor = new Node(cur, stSuccessors[i].segment.end, stSuccessors[i], cur->m_cost + distanceEarthMiles(cur->m_geoCoords, stSuccessors[i].segment.end), distanceEarthMiles(stSuccessors[i].segment.end, endAttCoords));
                checkSuccessorAndPushBack(open, closed, successor);
                
            }
            else if (stSuccessors[i].segment.end == cur->m_geoCoords)
            {
                //if your current node geocoord matches with your st segments start geocoord
                // create a new successor node with the geocoords of the st segment start
                successor = new Node(cur, stSuccessors[i].segment.start, stSuccessors[i], cur->m_cost + distanceEarthMiles(cur->m_geoCoords, stSuccessors[i].segment.start), distanceEarthMiles(stSuccessors[i].segment.start, endAttCoords));
                checkSuccessorAndPushBack(open, closed, successor);
            }
            else
            {
                //add both the start and end node when we have the start location  and attraction but not the streetSegment's start or end) at that GeoCoord
                // create a new successor node with the geocoords of the st segment start
                successor = new Node(cur, stSuccessors[i].segment.start, stSuccessors[i], cur->m_cost + distanceEarthMiles(cur->m_geoCoords, stSuccessors[i].segment.start), distanceEarthMiles(stSuccessors[i].segment.start, endAttCoords));
                checkSuccessorAndPushBack(open, closed, successor);
                
                // create a new successor node with the geocoords of the st segment end
                successor = new Node(cur, stSuccessors[i].segment.end, stSuccessors[i], cur->m_cost + distanceEarthMiles(cur->m_geoCoords, stSuccessors[i].segment.end), distanceEarthMiles(stSuccessors[i].segment.end, endAttCoords));
                checkSuccessorAndPushBack(open, closed, successor);
            }
        }
        // push the cur node on the closed list
        closed->push_back(cur);
    }
    
    // we've gone through the algorithm but couldn't find a valid route
    return NAV_NO_ROUTE;

}

void NavigatorImpl::getDirections(Node *destination, vector<NavSegment>& directions) const
{
    if (directions.size() != 0)
        directions.clear();
    
    //copying the route into a list in the correct order
    list<Node*> route;
    Node *current = destination; // should be the successor st segment node with the attraction on it
    while (current != nullptr)
    {
        route.push_front(current); // fill the list of routes with each node leading to the node of the start location
        current = current->m_parent;
    }
    
    list<Node*>::iterator ir_temp;
    for (list<Node*>::iterator ir = route.begin(); ir != route.end(); ir++) // transverse through the list of routes
    {
        ir_temp = ir;
        if ((++ir_temp) != route.end()) // if you haven't already reached the destination
        {
            GeoSegment tempGeoSeg((*ir)->m_geoCoords, (*ir_temp)->m_geoCoords); // create a temp geosegment with the coords of the first (first to last) street segment that you have to travel toward the destination
            double travelAngle = angleOfLine(tempGeoSeg); // get the angle of the tempgeoseg to create navsegments
            string tempDir;
            
            tempDir= directionOfLine(tempGeoSeg); // get the direction of your line from support.h
            
            if ((*ir)->m_stSeg.streetName != (*ir_temp)->m_stSeg.streetName) // if you change streets in the next geoseg, then say you have to turn i.e. turn-style create a navseg with the direction it needs to turn and push it on to the directions vector
            {
                //  A turn of less than 180 degrees from one segment onto another indicates a left turn. A turn of 180 degrees or more indicates a right turn.
                if (0 <= travelAngle && travelAngle < 180)
                    tempDir = "left";
                else
                    tempDir = "right";
    
                NavSegment tempNavSegment1(tempDir, (*ir_temp)->m_stSeg.streetName);
                directions.push_back(tempNavSegment1);
                
                tempDir = directionOfLine(tempGeoSeg);
                NavSegment tempNavSegment(tempDir, (*ir_temp)->m_stSeg.streetName, (*ir_temp)->m_cost - (*ir)->m_cost, tempGeoSeg); // create a proceed style navSeg with the dir, st name, distance, and geoseg and push it on to the directions vector
                directions.push_back(tempNavSegment);
                
            }
            else
            {
                NavSegment tempNavSegment(tempDir, (*ir)->m_stSeg.streetName, (*ir_temp)->m_cost - (*ir)->m_cost, tempGeoSeg); // create a proceed style navSeg with the dir, st name, distance, and geoseg and push it on to the directions vector
                directions.push_back(tempNavSegment);
            }
            
        }
    }
}

void NavigatorImpl::checkSuccessorAndPushBack(list<Node*>* openList, list<Node*>* closedList, Node* successor) const
{
    
    // following the A* algorithm
    // -----------------------------
    //if a node with the same position as successor is in the OPEN list \
    which has a lower f than successor, skip this successor
    //if a node with the same position as successor is in the CLOSED list \
    which has a lower f than successor, skip this successor
    //otherwise, add the node to the open list

    if (checkSuccessor(openList, successor))
        delete successor;
    else if (checkSuccessor(closedList, successor))
        delete successor;
    else
    {
        openList->push_back(successor);
    }
}

bool NavigatorImpl::checkSuccessor(list<Node*>* listNode, Node* successor) const
{
    
    // simple function to ( to help checkSuccessorAndPushBack) check if a node with the same position as successor is in a particular list \ which has a lower f than successor, skip this successor
    for (list<Node*>::iterator it_list = listNode->begin(); it_list != listNode->end(); it_list++)
    {
        if ((*it_list)->m_geoCoords == successor->m_geoCoords && (*it_list)->m_total < successor->m_total)
                return true;
    }
    return false;
}

//******************** Navigator functions ************************************

// These functions simply delegate to NavigatorImpl's functions.
// You probably don't want to change any of this code.

Navigator::Navigator()
{
    m_impl = new NavigatorImpl;
}

Navigator::~Navigator()
{
    delete m_impl;
}

bool Navigator::loadMapData(string mapFile)
{
    return m_impl->loadMapData(mapFile);
}

NavResult Navigator::navigate(string start, string end, vector<NavSegment>& directions) const
{
    return m_impl->navigate(start, end, directions);
}

