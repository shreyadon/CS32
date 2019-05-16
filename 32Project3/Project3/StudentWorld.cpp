#include "StudentWorld.h"
#include "Compiler.h"
#include <string>
#include <iostream>
#include <sstream>  // defines the type std::ostringstream
#include <algorithm>
#include <iomanip>
using namespace std;
//static int ActCounter = 0;
GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

StudentWorld::StudentWorld(std::string assetDir) : GameWorld(assetDir)
{
    m_ticks = 2000;
    for(int i = 0; i < 4; i++)
        m_scores[i] = 0;
    for(int i = 0; i < 4; i++)
        m_numEntrants[i] = false;
    m_winner = -1;
    m_initialMax = 5;
}

StudentWorld::~StudentWorld()
{
    cleanUp();
}

int StudentWorld::init()
{
    // initialise the date structure
    
    
    resetTicks();
    
    Field field;
    string fieldFile = getFieldFilename();
    
    string error;
    // if level not found return
    if (field.loadField(fieldFile, error) != Field::LoadResult::load_success)
    {
        setError(fieldFile + " " + error);
        return GWSTATUS_LEVEL_ERROR;; // something bad happened!
    }
    //otherwise the load was successful and you can access the contents of the field
    
    //access each coordinate in the field and if the coord holds a particaular Actor then create one and push it onto the list
    
    Compiler *compilerForEntrant0, *compilerForEntrant1,
    *compilerForEntrant2, *compilerForEntrant3; AntHill *ah0, *ah1, *ah2, *ah3;
    // get the names of all of the ant program source files
    // we provide the getFilenamesOfAntPrograms() function for
    // you in our GameWorld class.
    vector<string> fileNames = getFilenamesOfAntPrograms();
    // compile the source file... If the compile function returns // false, there was a syntax error during compilation!

    switch(fileNames.size())
    {
        case 4: // if there are four files
            m_numEntrants[3] = true;
            compilerForEntrant3 = new Compiler;
            if (!compilerForEntrant3->compile(fileNames[3], error))
            {
                setError(fileNames[3] + " " + error);
                return GWSTATUS_LEVEL_ERROR;
            }
            m_names[3] = compilerForEntrant3->getColonyName();
            cerr << "name " << compilerForEntrant3->getColonyName() << endl;
        case 3: // if there are three files
            m_numEntrants[2] = true;
            compilerForEntrant2 = new Compiler;
            if (!compilerForEntrant2->compile(fileNames[2], error))
            {
                setError(fileNames[2] + " " + error);
                return GWSTATUS_LEVEL_ERROR;
            }
            m_names[2] = compilerForEntrant2->getColonyName();
        case 2: // if only two files
            m_numEntrants[1] = true;
            compilerForEntrant1 = new Compiler;
            if (!compilerForEntrant1->compile(fileNames[1], error))
            {
                setError(fileNames[1] + " " + error);
                return GWSTATUS_LEVEL_ERROR;
            }
            m_names[1] = compilerForEntrant1->getColonyName();
        case 1: // if only one file was given
            m_numEntrants[0] = true;
            compilerForEntrant0 = new Compiler;
            if (!compilerForEntrant0->compile(fileNames[0], error))
            {
                setError(fileNames[0] + " " + error);
                return GWSTATUS_LEVEL_ERROR;
            }
            m_names[0] = compilerForEntrant0->getColonyName();
    }
    
    // iterate through the data structure, and for every item on the field create a new object and push the object the the corresponding coordinates onto the data structure
    for (int x = 0; x < VIEW_WIDTH; x++)
    {
        for (int y = 0; y < VIEW_HEIGHT; y++)
        {
            Actor* a;
            Field::FieldItem ge = field.getContentsOf(x, y);
            switch (ge)
            {
                case Field::rock:
                {
                    a = new Pebble(this, x, y);
                    MapOfSpots[make_pair(x, y)].push_back(a);
                    break;
                }
                    
                case Field::food:
                {
                    a = new Food(this, x, y, 6000);
                    MapOfSpots[make_pair(x, y)].push_back(a);
                    break;
                }
                    
                case Field::water:
                {
                    a = new WaterPool(this, x, y);
                    MapOfSpots[make_pair(x, y)].push_back(a);
                    break;
                }
                    
                case Field::poison:
                {
                    a = new Poison(this, x, y);
                    MapOfSpots[make_pair(x, y)].push_back(a);
                    break;
                }
                case Field::grasshopper:
                {
                    a = new BabyGrasshopper(this, x, y, generateRandDir());
                    MapOfSpots[make_pair(x, y)].push_back(a);
                    break;
                }
                case Field::anthill0:
                {
                    if (m_numEntrants[0]) // if the first file was there
                    {
                        ah0 = new AntHill(this, x, y, 0, compilerForEntrant0);
                        MapOfSpots[make_pair(x, y)].push_back(ah0);;
                        cerr << "added anthill to vector" << endl;
                    }
                    
                    break;
                }
                case Field::anthill1:
                {
                    if (m_numEntrants[1]) // if the file exists
                    {
                        ah1 = new AntHill(this, x, y, 1, compilerForEntrant1);
                        MapOfSpots[make_pair(x, y)].push_back(ah1);
                    }
                    break;
                }
                case Field::anthill2:
                {
                    if (m_numEntrants[2]) // if the file exists
                    {
                        ah2 = new AntHill(this, x, y, 2, compilerForEntrant2);
                        MapOfSpots[make_pair(x, y)].push_back(ah2);
                    }
                    break;
                }
                case Field::anthill3:
                {
                    if (m_numEntrants[3]) // if the file exists
                    {
                        ah3 = new AntHill(this, x, y, 3, compilerForEntrant3);
                        MapOfSpots[make_pair(x, y)].push_back(ah3);
                    }
                    break;
                }
                case Field::empty:
                    break;
                default:
                    a = nullptr;
                    break;
            }
        }
    }
    
    
    return GWSTATUS_CONTINUE_GAME;
}
int StudentWorld::move()
{
    // update the current tick # in the simulation
    updateTickCount();
    
    // The term "actors" refers to all ants, anthills, poison, pebbles,
    // baby and adult Grasshoppers, food, pools of water, etc.
    // Give each actor a chance to do something
    //for (each actor q in the simulation)
    map<Pair, vector<Actor*>>::iterator it_map = MapOfSpots.begin();;
    vector<Actor*>::iterator it_actor;
    
    int old_x, old_y;
    int new_x, new_y;
    unsigned long old_size;
    
    //    if(it_map == MapOfSpots.end()) //end the game is it_map is empty
    //    {
    //        return GWSTATUS_LEVEL_ERROR;
    //    }
    
    //Run through the MapOfSpots to run the Actor's doSomething
    while (it_map != MapOfSpots.end())
    {
        it_actor = it_map->second.begin();
        while (it_actor != it_map->second.end())
        {
            //Get initial coordintaes
            old_x = (*it_actor)->getX();
            old_y = (*it_actor)->getY();
            old_size = it_map->second.size();
            Actor *temp = *it_actor;
            
            //Run doSomething which might change the coordinates
            if(!(*it_actor)->isDead())
            {
                (*it_actor)->doSomething();
                
                if (old_size == it_map->second.size())
                {
                    //Get new coordinates
                    new_x = (*it_actor)->getX();
                    new_y = (*it_actor)->getY();
                    
                    //If the actor has moved, change its position in the data structure by adding it at the new location and removing from the existing location
                    if(old_x != new_x || old_y != new_y)
                    {
                        MapOfSpots[(make_pair(new_x, new_y))].push_back(*it_actor);
                        it_actor = it_map->second.erase(it_actor);
                        continue; //after erasing the next item on the vector is returned, so we must not iterate or we might run into nullptr
                    }
                }
                else
                {
                    it_actor = find(it_map->second.begin(), it_map->second.end(), temp); //it_actor pointer correctly placed
                    continue;
                }
            }
            it_actor++;
        }
        if(it_map->second.size() == 0)
            it_map = MapOfSpots.erase(it_map);
        else
            it_map++;
    }
    
    // Remove newly-dead actors after each tick
    //
    removeDeadSimulationMapOfSpots(); // delete dead simulation MapOfSpots
    // Update the simulation Status Line
    updateDisplayText(); // update the ticks/ant stats text at screen top

    weHaveAWinningAnt(); // check if something is winning, use this to update the int m_winner to make updateDisplayText better at handling the asterisk '*'
    if (theSimulationIsOver())
    {
        if (weHaveAWinningAnt())
        {
            setWinner(getWinningAntsName());
            return GWSTATUS_PLAYER_WON;
        }
        else
            return GWSTATUS_NO_WINNER;
    }
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
     //the cleanUp() funtion traverses through the vector of Actors using an iterator and deletes each one, freeing up any allocated memory.
     //cleanUp() is used in the destructor for StudentWorld.
    
    map<pair<int,int>, vector<Actor*>>:: iterator it_map;
    vector<Actor*>::iterator it_actor;
    
    it_map = MapOfSpots.begin();
    while (it_map != MapOfSpots.end())
    {
        it_actor = it_map->second.begin();
        while (it_actor != it_map->second.end())
        {
            Actor *temp = *it_actor;
            delete temp;
            it_actor++;
        }
        it_map->second.clear();
        it_map++;
    }
    MapOfSpots.clear();
    return;

}

bool StudentWorld::canMoveTo(int x, int y) const
{
    // this function traverses through the vector of Actors using an iterator and checks if a pebble exits at that particular spot
    cerr << MapOfSpots.size() << endl;
    map<pair<int,int>, vector<Actor*>>::const_iterator it = MapOfSpots.begin();
    for (it = MapOfSpots.begin(); it != MapOfSpots.end(); it++)
    {
        for (vector<Actor*>::const_iterator p = it->second.begin(); p != it->second.end(); p++)
        {
            // if a pebble with the same coordinates is found then return true
            if ((*p)->getX() == x && (*p)->getY() == y && (*p)->blocksMovement())
                return false;
        }
    }
    return true;
}

Actor* StudentWorld::getEdibleAt(int x, int y) const
{
    map<pair<int,int>, vector<Actor*>>:: const_iterator it;
    for (it = MapOfSpots.begin(); it != MapOfSpots.end(); it++)
    {
        for (vector<Actor*>::const_iterator p = it->second.begin(); p != it->second.end(); p++)
        {
            // if a Food object with the same coordinates is found then return true
            if ((*p)->getX() == x && (*p)->getY() == y && (*p)->isEdible())
                return *p;
        }
    }
    return nullptr;
}

Actor* StudentWorld::getPheromoneAt(int x, int y, int colony) const
{
    map<pair<int,int>, vector<Actor*>>:: const_iterator it;
    for (it = MapOfSpots.begin(); it != MapOfSpots.end(); it++)
    {
        for (vector<Actor*>::const_iterator p = it->second.begin(); p != it->second.end(); p++)
        {
            // if a Phermone object with the same coordinates is found then return true
            if ((*p)->getX() == x && (*p)->getY() == y && (*p)->isPheromone(colony))
                return *p;
        }
    }
    return nullptr;
}

bool StudentWorld::isEnemyAt(int x, int y, int colony) const
{
    map<pair<int,int>, vector<Actor*>>:: const_iterator it;
    for (it = MapOfSpots.begin(); it != MapOfSpots.end(); it++)
    {
        for (vector<Actor*>::const_iterator p = it->second.begin(); p != it->second.end(); p++)
        {
            // if an Enemy object with the same coordinates is found then return true
            if ((*p)->getX() == x && (*p)->getY() == y && (*p)->isEnemy(colony))
            {
                return true;
            }
        }
    }
    return false;
}

bool StudentWorld::isDangerAt(int x, int y, int colony) const
{
    map<pair<int,int>, vector<Actor*>>:: const_iterator it;
    for (it = MapOfSpots.begin(); it != MapOfSpots.end(); it++)
    {
        for (vector<Actor*>::const_iterator p = it->second.begin(); p != it->second.end(); p++)
        {
            // if a Dangerous object with the same coordinates is found then return true
            if ((*p)->getX() == x && (*p)->getY() == y && (*p)->isDangerous(colony))
            {
                return true;
            }
        }
    }
    return false;
}


bool StudentWorld::biteEnemyAt(Actor* me, int colony, int biteDamage)
{
    map<pair<int,int>, vector<Actor*>>:: const_iterator it;
    vector<Actor*> Actor_temp; // create a vector to store all the enemy actors
    int count = 0;
    for (it = MapOfSpots.begin(); it != MapOfSpots.end(); it++)
    {
        for (vector<Actor*>::const_iterator p = it->second.begin(); p != it->second.end(); p++)
        {
            // if a Food object with the same coordinates is found then return true
            if ((*p)->getX() == (*me).getX() && (*p)->getY() == (*me).getY() && (*p)->isEnemy(colony))
            {
                Actor_temp.push_back(*p);
                count++;
            }
        }
    }
    if (count == 0)
        return false;
    count = randInt(0, count-1); // randomly pick an enemy and bite
    Actor_temp[count]->getBitten(biteDamage);
    return true;
}

bool StudentWorld::poisonAllPoisonableAt(int x, int y)
{
    map<pair<int,int>, vector<Actor*>>:: const_iterator it;
    for (it = MapOfSpots.begin(); it != MapOfSpots.end(); it++)
    {
        for (vector<Actor*>::const_iterator p = it->second.begin(); p != it->second.end(); p++)
        {
            if ((*p)->getX() == x && (*p)->getY() == y && (*p)->canBeTriggered()) // poison only if can be poisoned

            {
                (*p)->getPoisoned();
                 if ((*p)->getEnergy() <= 0)
                 {
                     (*p)->setDead();
                 }
                
            }
        }
    }
    return false;
}

bool StudentWorld::stunAllStunnableAt(int x, int y)
{
    map<pair<int,int>, vector<Actor*>>:: const_iterator it;
    for (it = MapOfSpots.begin(); it != MapOfSpots.end(); it++)
    {
        for (vector<Actor*>::const_iterator p = it->second.begin(); p != it->second.end(); p++)
        {
            if ((*p)->getX() == x && (*p)->getY() == y && (*p)->canBeTriggered())
            {
                if (!(*p)->isStunned()) // stun only if you haven't already been stunned by the same water pool
                {
                    (*p)->setStunned();
                    (*p)->increaseSleepTicks(2);
                }
            }
        }
    }
    return false;
}

void StudentWorld::increaseScore(int colony)
{
    m_scores[colony] += 1;
}

void StudentWorld::updateDisplayText()
{
    int ticks = m_ticks;
    
    int winningAntNumber;
    
    winningAntNumber = m_winner;
    // Create a string from your statistics, of the form:
    // Ticks: 1134 - AmyAnt: 32 BillyAnt: 33 SuzieAnt*: 77 IgorAnt: 05
    string s = someFunctionToFormatThingsNicely(ticks, m_scores[0], m_scores[1], m_scores[2], m_scores[3], winningAntNumber);
    // Finally, update the display text at the top of the screen with your
    // newly created stats
    setGameStatText(s); // calls our provided GameWorld::setGameStatText
}

void StudentWorld::removeDeadSimulationMapOfSpots()
{
    // this function traverses through the vector of Actors using an iterator and deletes one when it is dead, freeing up any allocated memory.
    
    map<pair<int,int>, vector<Actor*>>:: iterator it;
    vector<Actor*>::iterator p;
    
    for (it = MapOfSpots.begin(); it != MapOfSpots.end(); it++)
    {
        for (p = it->second.begin(); p != it->second.end();)
        {
            Actor* tmpActor = *p;
            if (tmpActor->isDead())
            {
                p = it->second.erase(p);
                delete tmpActor;
            }
            else
            {
                ++p;
            }
        }
    }
    
}

// returns true if the sim is over
bool StudentWorld::theSimulationIsOver() const
{
    if (m_ticks == 0)
        return true;
    else
        return false;
}

void StudentWorld::updateTickCount()
{
    m_ticks--;
}

bool StudentWorld::weHaveAWinningAnt()
{
    
    for(int i = 0; i < 4; i++)
    {
        if (m_scores[i] > m_initialMax)
        {
            m_initialMax = m_scores[i];
            m_winner = i;
        }
    }

    if(m_initialMax == 5)
        return false;
    return true;
}

string StudentWorld::getWinningAntsName() const
{
    return m_names[m_winner]; // returns the name of the winning ant
}

void StudentWorld::addAnt(Actor* anthill)
{
    // add an ant depending on the anthill colony
    Actor* a;
    switch (anthill->getColony()) {
        case 0:
            a = new Ant(this, anthill->getX(), anthill->getY(), generateRandDir(), anthill->getColony(), anthill->getProgram(), IID_ANT_TYPE0, anthill);
            break;
        case 1:
            a = new Ant(this, anthill->getX(), anthill->getY(), generateRandDir(), anthill->getColony(), anthill->getProgram(), IID_ANT_TYPE1, anthill);
            break;
        case 2:
            a = new Ant(this, anthill->getX(), anthill->getY(), generateRandDir(), anthill->getColony(), anthill->getProgram(), IID_ANT_TYPE2, anthill);
            break;
        case 3:
            a = new Ant(this, anthill->getX(), anthill->getY(), generateRandDir(), anthill->getColony(), anthill->getProgram(), IID_ANT_TYPE3, anthill);
            break;
        default:
            break;
    }
    
    // add the ant to the data structure, at the same position as the anthill
    map<pair<int,int>, vector<Actor*>>:: iterator it;
    for (it = MapOfSpots.begin(); it != MapOfSpots.end(); it++)
    {
        for (vector<Actor*>::iterator p = it->second.begin(); p != it->second.end(); p++)
        {
            if ((*p)->getX() == anthill->getX() && (*p)->getY() == anthill->getY())
            {
                it->second.push_back(a);
                return;
            }
        }
    }
    
}

void StudentWorld::addPheromone(Actor* ant)
{
    // add an pheromone depending on the ant colony
    Actor* a;
    switch (ant->getColony()) {
        case 0:
            a = new Pheromone(this, ant->getX(), ant->getY(), ant->getColony(), IID_PHEROMONE_TYPE0);
            break;
        case 1:
            a = new Pheromone(this, ant->getX(), ant->getY(), ant->getColony(), IID_PHEROMONE_TYPE1);
            break;
        case 2:
            a = new Pheromone(this, ant->getX(), ant->getY(), ant->getColony(), IID_PHEROMONE_TYPE2);
            break;
        case 3:
            a = new Pheromone(this, ant->getX(), ant->getY(), ant->getColony(), IID_PHEROMONE_TYPE3);
            break;
        default:
            break;
    }
   
    // add the pheromone to the data structure, at the same position as the ant
    map<pair<int,int>, vector<Actor*>>:: iterator it;
    for (it = MapOfSpots.begin(); it != MapOfSpots.end(); it++)
    {
        for (vector<Actor*>::iterator p = it->second.begin(); p != it->second.end(); p++)
        {
            if ((*p)->getX() == ant->getX() && (*p)->getY() == ant->getY())
            {
                it->second.push_back(a);
                return;
            }
            
        }
    }
}

void StudentWorld::addAdultGrasshopper(Actor* BabyGrasshopper)
{
    // add the adultGrasshopper to the data structure, at the same position as the babyGrasshopper
    Actor* a;
    a = new AdultGrasshopper(this, BabyGrasshopper->getX(), BabyGrasshopper->getY(), generateRandDir());
    
    map<pair<int,int>, vector<Actor*>>:: iterator it;
    for (it = MapOfSpots.begin(); it != MapOfSpots.end(); it++)
    {
        for (vector<Actor*>::iterator p = it->second.begin(); p != it->second.end(); p++)
        {
            if ((*p)->getX() == BabyGrasshopper->getX() && (*p)->getY() == BabyGrasshopper->getY())
            {
                it->second.push_back(a);
                return;
            }
        }
    }
}

void StudentWorld::addFoodSW(Actor* actor, int amt)
{
    // add the food to the data structure, at the same position as the dead actor with the specified amount
    Actor* a;
    a = new Food(this, actor->getX(), actor->getY(), amt);
    
    map<pair<int,int>, vector<Actor*>>:: iterator it;
    for (it = MapOfSpots.begin(); it != MapOfSpots.end(); it++)
    {
        for (vector<Actor*>::iterator p = it->second.begin(); p != it->second.end(); p++)
        {
            if ((*p)->getX() == actor->getX() && (*p)->getY() == actor->getY())
            {
                it->second.push_back(a);
                return;
            }
        }
    }
}

Actor::Direction StudentWorld::generateRandDir() const
{
    // gets a random integer and return correcsponding direction
    Actor::Direction dir;
    int randint = randInt(1, 4);
    switch(randint)
    {
        case 1:
            dir = Actor::right;
            break;
        case 2:
            dir = Actor::left;
            break;
        case 3:
            dir = Actor::up;
            break;
        case 4:
            dir = Actor::down;
            break;
        default:
            exit(1);
            //should never get here
    }
    return dir;
}

string StudentWorld::someFunctionToFormatThingsNicely(int ticks, int antsAnt0, int antsAnt1, int antsAnt2, int antsAnt3, int winningAntNumber)
{
    // Create a string from your statistics, of the form:
    // Ticks: 1134 - AmyAnt: 32 BillyAnt: 33 SuzieAnt*: 77 IgorAnt: 05
    
    ostringstream oss;
    
    // adds spaces dependind on the the digits
    oss << "Ticks:";
    if (ticks < 10)
        oss << "    ";
    else if (ticks < 100)
        oss << "   ";
    else if (ticks < 1000)
        oss << "  ";
    else if (ticks < 10000)
        oss << " ";
    
    oss << ticks << " - ";
    
    // depending on whether or not the colonies exist, append name and num of ants of anthill to the string
    // if the winning ant exists, add asterisk.
    if (m_numEntrants[0])
    {
        oss << m_names[0];
        if (winningAntNumber == 0)
            oss << "*";
        oss.fill('0');
        oss << ": " << setw(2) << antsAnt0 << " ants ";
    }
    if (m_numEntrants[1])
    {
        oss << m_names[1];
        if (winningAntNumber == 1)
            oss << "*";
        oss.fill('0');
        oss << ": "<< setw(2) << antsAnt1 << " ants ";
    }
    if (m_numEntrants[2])
    {
        oss << m_names[2];
        if (winningAntNumber == 2)
            oss << "*";
        oss.fill('0');
        oss << ": "<< setw(2) << antsAnt2 << " ants ";
    }
    if (m_numEntrants[3])
    {
        oss << m_names[3];
        if (winningAntNumber == 3)
            oss << "*";
        oss.fill('0');
        oss << ": "<< setw(2) << antsAnt3 << " ants ";
    }
    
    string s = oss.str();//
    return s;
}

void StudentWorld::resetTicks()
{
    m_ticks = 2000;
}



// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
