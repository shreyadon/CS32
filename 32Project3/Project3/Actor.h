#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "Compiler.h"
#include <cmath>

class StudentWorld;
class Compiler;

class Actor : public GraphObject
{
public:
    Actor(StudentWorld* world, int startX, int startY, Direction startDir, int imageID, int depth);
    
    // Action to perform each tick.
    virtual void doSomething() = 0;
    
    // Is this actor dead?
    bool isDead() const;
    
    // Does this actor block movement?
    virtual bool blocksMovement() const;
    
    // Cause this actor to be be bitten, suffering an amount of damage.
    virtual void getBitten(int amt);
    
    // Cause this actor to be be poisoned.
    virtual void getPoisoned();
    
    // Can this actor be picked up to be eaten?
    virtual bool isEdible() const;
    
    // Is this actor detected by an ant as a pheromone?
    virtual bool isPheromone(int colony) const;
    
    // Is this actor an enemy of an ant of the indicated colony?
    virtual bool isEnemy(int colony) const;
    
    // Is this actor detected as dangerous by an ant of the indicated colony?
    virtual bool isDangerous(int colony) const;
    
    // Get this actor's world.
    StudentWorld* getWorld() const;
    
    // Set the m_state to true => making an Actor dead
    void setDead();
    
    // Return true if the object is an ant or a babyGrasshopper
    virtual bool canBeTriggered() const;
    
    //Returns true if Stunned
    bool isStunned() const;
    
    // Set as Stunned
    void setStunned();
    
    // Set as not Stunned
    void setUnstunned();
    
    // Gets Energy for eneryy holders, return 0 for all else
    virtual int getEnergy() const;
    
    // Updates Energy for energy holders, does nothing for the rest
    virtual void updateEnergy(int amt);
    
    // Returns the colony that the anthill or ant belongs to
    virtual int getColony() const;
    
    // Returns the program that runs the ants of certain type
    virtual Compiler* getProgram() const;
    
    // Increases the sleeping ticks, only for insect
    virtual void increaseSleepTicks(int amt);
    
private:
    StudentWorld* m_sw;
    bool m_state;
    bool m_isStunned;
};

class Pebble : public Actor
{
public:
    Pebble(StudentWorld* sw, int startX, int startY);
    virtual void doSomething();
    virtual bool blocksMovement() const;
};

class EnergyHolder : public Actor
{
public:
    EnergyHolder(StudentWorld* sw, int startX, int startY, Direction startDir, int energy, int imageID, int depth);
    
    // Get this actor's amount of energy (for a Pheromone, same as strength).
    virtual int getEnergy() const;
    
    // Adjust  actor's amount of energy upward or downward.
    virtual void updateEnergy(int amt);
    
    // Add an amount of food to this actor's location.
    void addFood(int amt);
    
    // Have this actor pick up an amount of food and eat it.
    int pickupAndEatFood(int amt);
    
private:
    int m_energy;
};

class Food : public EnergyHolder
{
public:
    Food(StudentWorld* sw, int startX, int startY, int energy);
    virtual void doSomething();
    virtual bool isEdible() const;
};

class AntHill : public EnergyHolder
{
public:
    AntHill(StudentWorld* sw, int startX, int startY, int colony, Compiler* program);
    virtual void doSomething();
    virtual int getColony() const;
    virtual Compiler* getProgram() const;
private:
    int m_colony;
    Compiler* m_program;
};

class Pheromone : public EnergyHolder
{
public:
    Pheromone(StudentWorld* sw, int startX, int startY, int colony, int imageID);
    virtual void doSomething();
    virtual bool isPheromone(int colony) const;
private:
    int m_colony;
};

class TriggerableActor : public Actor
{
public:
    TriggerableActor(StudentWorld* sw, int x, int y, int imageID);
    virtual bool isDangerous(int colony) const;
};

class WaterPool : public TriggerableActor
{
public:
    WaterPool(StudentWorld* sw, int x, int y);
    virtual void doSomething();
};

class Poison : public TriggerableActor
{
public:
    Poison(StudentWorld* sw, int x, int y);
    virtual void doSomething();
};

class Insect : public EnergyHolder
{
public:
    Insect(StudentWorld* world, int startX, int startY, Direction dir, int energy, int imageID);
    virtual void doSomething() = 0;
    virtual void getBitten(int amt);
    virtual void getPoisoned();
    virtual bool isEnemy(int colony);
    
    // Set x,y to the coordinates of the spot one step in front of this insect.
    void getXYInFrontOfMe(int& x, int& y);
    
    // Move this insect one step forward if possible, and return true;
    // otherwise, return false without moving.
    bool moveForwardIfPossible();
    
    int getSleepingTicks() const;
    void decreaseSleepingTicks();
    void increaseSleepTicks(int amt);
private:
    int m_sleepTicks;

};

class Ant : public Insect
{
public:
    Ant(StudentWorld* sw, int startX, int startY, Direction dir, int colony, Compiler* program, int imageID, Actor* myAntHill);
    virtual void doSomething();
    virtual bool isEnemy(int colony) const;
    virtual void getBitten(int amt);
    virtual int getColony() const;
    virtual bool isDangerous(int colony) const;
    virtual bool canBeTriggered() const;
    void setBitten();
    
private:
    int m_colony;
    int m_food;
    bool m_blocked;
    int m_randVal;
    int m_counter;
    bool m_bitten;
    Compiler* m_program;
    Actor* m_anthill;
};

class Grasshopper : public Insect
{
public:
    Grasshopper(StudentWorld* sw, int startX, int startY, Direction dir, int energy, int imageID);
    virtual void doSomething() const;
    int getDistance() const;
    void resetDistance();
    void decreaseDistance();
    // Increase the number of ticks this insect will sleep by the indicated amount.
    int getTicks() const;
    void decreaseTicks();
    void resetTicks();
    void setDistanceZero();
private:
    int m_distance;
    int m_ticks;
    
};

class BabyGrasshopper : public Grasshopper
{
public:
    BabyGrasshopper(StudentWorld* sw, int startX, int startY, Direction dir);
    virtual void doSomething();
    virtual bool canBeTriggered() const;
};

class AdultGrasshopper : public Grasshopper
{
public:
    AdultGrasshopper(StudentWorld* sw, int startX, int startY, Direction dir);
    virtual bool isDangerous(int colony) const;
    virtual void doSomething();
    virtual void getBitten(int amt);
};

#endif // ACTOR_H_
