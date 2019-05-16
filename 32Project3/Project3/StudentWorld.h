#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include <string>
#include <iostream>
#include <vector>
#include "GameWorld.h"
#include "GameConstants.h"
#include "Field.h"
#include "Actor.h"
#include "Compiler.h"
using namespace std;

class Actor;
class Compiler;
typedef std::pair<int, int> Pair;
typedef std::map<int, std::pair<int, int>> VectorCoords;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir);
    virtual ~StudentWorld();
    
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    
    // Can an insect move to x,y?
    bool canMoveTo(int x, int y) const;
    
    // If an item that can be picked up to be eaten is at x,y, return a
    // pointer to it; otherwise, return a null pointer.  (Edible items are
    // only ever going be food.)
    Actor* getEdibleAt(int x, int y) const;
    
    // If a pheromone of the indicated colony is at x,y, return a pointer
    // to it; otherwise, return a null pointer.
    Actor* getPheromoneAt(int x, int y, int colony) const;
    
    // Is an enemy of an ant of the indicated colony at x,y?
    bool isEnemyAt(int x, int y, int colony) const;
    
    // Is something dangerous to an ant of the indicated colony at x,y?
    bool isDangerAt(int x, int y, int colony) const;

    // Bite an enemy of an ant of the indicated colony at me's location
    // (other than me; insects don't bite themselves).  Return true if an
    // enemy was bitten.
    bool biteEnemyAt(Actor* me, int colony, int biteDamage);
    
    // Poison all poisonable actors at x,y.
    bool poisonAllPoisonableAt(int x, int y);
    
    // Stun all stunnable actors at x,y.
    bool stunAllStunnableAt(int x, int y);
    
    // Record another ant birth for the indicated colony.
    void increaseScore(int colony);
    
    // This function deletes dead simulation MapOfSpots
    void removeDeadSimulationMapOfSpots();
    
    // This function updates the simulation Status Line
    void updateDisplayText();
    
    // Returns true if the sim is over
    bool theSimulationIsOver() const;
    
    // Changes the ticks everytime move() is called
    void updateTickCount();
    
    // Returns true if there is a winning ant
    bool weHaveAWinningAnt();
    
    // Return the name of the winning ant
    string getWinningAntsName() const;
    
    // returns a random direction
    Actor::Direction generateRandDir() const;
    
    // adds an Ant at the location of the anthill
    void addAnt(Actor* anthill);
    
    // adds a pheromone at the location of ant
    void addPheromone(Actor* ant);
    
    // adds a AdultGrasshopper at the location of the babyGrasshopper
    void addAdultGrasshopper(Actor* BabyGrasshopper);
    
    // adds food if an ant drops of food or insect dies
    void addFoodSW(Actor* actor, int amt);
    
    // functions formats things for updateDisplayText
    string someFunctionToFormatThingsNicely(int ticks, int antsAnt0, int antsAnt1, int antsAnt2, int antsAnt3, int winningAntNumber);
    // Sets ticks back to 2000
    void resetTicks();
    
private:
    std::vector<std::string> ActorList;
    std::map<Pair, std::vector<Actor*>> MapOfSpots;
    int m_ticks;
    bool m_numEntrants[4];
    int m_scores[4];
    string m_names[4];
    int m_winner;
    int m_initialMax;
};

#endif // STUDENTWORLD_H_
