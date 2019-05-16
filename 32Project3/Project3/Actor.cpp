#include "Actor.h"
#include "StudentWorld.h"


// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

// ACTOR //

Actor::Actor(StudentWorld* world, int startX, int startY, Direction startDir, int imageID, int depth):GraphObject(imageID, startX, startY, startDir, depth)
{
    cerr << "Actor Constrcutor" << endl;
    m_sw = world;
    m_state = false;
    m_isStunned = false;
}

bool Actor::isDead() const
{
    return m_state;
}

bool Actor::blocksMovement() const
{
    return false; // all Actors other than the Pebble return false for this
}

void Actor::getBitten(int amt)
{ }

 void Actor::getPoisoned()
{ }

bool Actor::isEdible() const
{
    return false; // this return false for everything other than the Food object
}

bool Actor::isPheromone(int colony) const
{
    return false; // this return false for everything other than the Pheromone object
}

bool Actor::isEnemy(int colony) const
{
    return false; // false for everthing other than those Insects
}

bool Actor::isDangerous(int colony) const
{
    return false; // false for all Actors except TriggerableActors, ants and adult Grasshoppers
}

StudentWorld* Actor::getWorld() const
{
    return m_sw; // returns the StudentWorld that the actor belongs to
}

void Actor::setDead()
{
    m_state = true;
}

bool Actor::canBeTriggered() const
{
    return false; // returns false for all except Ant and BabyGrasshoppers
}

bool Actor::isStunned() const
{
    return m_isStunned;
}

void Actor::setStunned()
{
    m_isStunned = true;
}

void Actor::setUnstunned()
{
    cerr << "setting unstunned" << endl;
    m_isStunned = false;
}

int Actor::getEnergy() const
{
    return 0;
}

void Actor::updateEnergy(int amt)
{ }

int Actor::getColony() const
{
    return -1; // not actual colonies, only used with Anthill or Ants
}

Compiler* Actor::getProgram() const
{
    return nullptr; // not actual program, only used for Anthill or ants
}

void Actor::increaseSleepTicks(int amt)
{
    
}

// PEBBLE //

Pebble::Pebble(StudentWorld* sw, int startX, int startY):Actor(sw, startX, startY, right, IID_ROCK, 1)
{
    cerr << "Pebble Constrcutor" << endl;
}


void Pebble::doSomething()
{
    // a Pebble does nothing
}

bool Pebble::blocksMovement() const
{
    return true; // the pebble does Block Movement
}

// ENERGYHOLDERS //

EnergyHolder::EnergyHolder(StudentWorld* sw, int startX, int startY, Direction startDir, int energy, int imageID, int depth):Actor (sw, startX, startY, startDir, imageID, depth)
{
    cerr << "EnergyHolder Constrcutor" << endl;
    m_energy = energy;
}

int EnergyHolder::getEnergy() const
{
    return m_energy;
}

void EnergyHolder::updateEnergy(int amt)
{
    m_energy += amt;
}

void EnergyHolder::addFood(int amt)
{
    if (amt != 0)
    {
        Actor* f = getWorld()->getEdibleAt(getX(), getY());
        if (f != nullptr) // food already exists at this location
        {
            // so just date more food units to the existing food object
            f->updateEnergy(amt);
        }
        else
        {
            // Create a new Food object
            getWorld()->addFoodSW(this, amt);
        }
    }
}

int EnergyHolder::pickupAndEatFood(int amt)
{
    
    int food = 0;
    Actor* f = getWorld()->getEdibleAt(getX(), getY());
    if (f != nullptr)// check if there is food
    {
        //It will eat up to amt units of food from the square, and this amount will directly increase its hit points.
        food = f->getEnergy();
        if (food >= amt)
            food = amt;
        updateEnergy(food);
        f->updateEnergy(-food);
        if (f->getEnergy() <= 0) // if the food object not longer has food units set dead
        {
            f->setDead();
        }
    }
    return food;
}

// FOOD //

Food::Food(StudentWorld* sw, int startX, int startY, int energy):EnergyHolder(sw, startX, startY, right, energy, IID_FOOD, 2)
{
    cerr << "Food Constructor" << endl;
}

void Food::doSomething()
{
    // do nothing
}

bool Food::isEdible() const
{
    return true; // A Food object is edible
}

// ANTHILL //

AntHill::AntHill(StudentWorld* sw, int startX, int startY, int colony, Compiler* program):EnergyHolder(sw, startX, startY, right, 8999, IID_ANT_HILL, 2)
{
    m_program = program;
    m_colony = colony;
    cerr << "AntHill Constructor" << endl;
}

void AntHill::doSomething()
{
    cerr << "in anthill doSomething." << endl;
    //It decreases its (queen’s) hit points by 1 unit.
    updateEnergy(-1);
    // If the anthill’s (queen’s) hit points reaches zero units, then:
    if (getEnergy() <= 0)
    {
        //The anthill must set its status to dead so it can be removed from the simulation after the current tick.
        setDead();
        //The anthill must immediately return.
        return;
    }
    
    //The anthill checks to see if there is any food on its square. If so:
    //It will eat up to 10,000 units of food from the square, and this amount will directly increase its hit points.
    cerr << "Anthill " << m_colony << " energy before food " << getEnergy() << endl;
    int food = pickupAndEatFood(10000);
    cerr << "debug" << m_colony << " " << getEnergy() << endl;
        
    if (food > 0) // AntHill picked up food
    {
        
        cerr << "anthill ate " << food << endl;
        return;
    }
    //The anthill checks to see if it has enough energy - at least 2,000 hit points - to produce a new ant. If so:
    if (getEnergy() >= 2000)
    {
        cerr << "need to add ant" << endl;
        
        getWorld()->addAnt(this);
        
        // It reduces its own hit points by 1,500.
        updateEnergy(-1500);
        
        // It asks StudentWorld to increase the count of the total number of ants that this colony has produced. This needs to be tracked in order to determine the winner ant colony.
        getWorld()->increaseScore(m_colony);
    }
}

int AntHill::getColony() const
{
    // returns the colony of the Anthill, helpful while creating new ants
    return m_colony;
}

Compiler* AntHill::getProgram() const
{
    // returns the program of the Anthill, helpful while creating new ants
    return m_program;
}

// PHEROMONE //

Pheromone::Pheromone(StudentWorld* sw, int startX, int startY, int colony, int imageID):EnergyHolder(sw, startX, startY, right, 256, imageID, 2)
{
    cerr << "Pheromone Constrcutor" << endl;
    m_colony = colony;
}

void Pheromone::doSomething()
{
    // They decrease their strength by 1 unit.
    updateEnergy(-1);
    // If their strength reaches zero units, then they must set their status to dead so they can be removed from the simulation after the current tick.
    if (getEnergy() <= 0)
        setDead();
}

bool Pheromone::isPheromone(int colony) const
{
    if (colony == m_colony) // a Pheromone can only be detected by it's own ants
        return true;
    return false;
}

// TRIGGERABLEACTOR //

TriggerableActor::TriggerableActor(StudentWorld* sw, int x, int y, int imageID):Actor(sw, x, y, right, imageID, 2)
{
    cerr << "TriggerableActor Constrcutor" << endl;
}

bool TriggerableActor::isDangerous(int colony) const
{
    return true; // all TriggerableActors are dangerous
}

// WATERPOOL //
WaterPool::WaterPool(StudentWorld* sw, int x, int y): TriggerableActor (sw, x, y, IID_WATER_POOL)
{
    cerr << "WaterPool Constrcutor" << endl;
}

void WaterPool::doSomething()
{
    getWorld()->stunAllStunnableAt(getX(), getY()); // Stuns all actors that can be stunned at it's block
};

// POISON //

Poison::Poison(StudentWorld* sw, int x, int y): TriggerableActor (sw, x, y, IID_POISON)
{
    cerr << "Poison Constrcutor" << endl;
}

void Poison::doSomething()
{
    getWorld()->poisonAllPoisonableAt(getX(), getY()); // Poisons all actors that can be poisoned at it's block
}

// INSECT //

Insect::Insect(StudentWorld* world, int startX, int startY, Direction dir,  int energy, int imageID):EnergyHolder(world, startX, startY, dir, energy, imageID, 1)
{
    cerr << "Insect Constrcutor" << endl;
    updateEnergy(energy);
    m_sleepTicks = 0;
}

void Insect::getBitten(int amt)
{
    // deducts the bite damage from the energy
    updateEnergy(-amt);
}

void Insect::getPoisoned()
{
    // deducts the energy when poisoned
    updateEnergy(-150);

}

bool Insect::isEnemy(int colony)
{
    return true; // this is just a general response, all insects are enemies, only ants have a specific function for this
}

void Insect::getXYInFrontOfMe(int& x, int& y)
{
    // changes the coordinates of x and y dependign on the direction
    Direction dir = getDirection();
    switch (dir) {
        case right:
            x++;
            break;
        case left:
            x--;
            break;
        case up:
            y++;
            break;
        case down:
            y--;
            break;
        default:
            break;
    }
}

bool Insect::moveForwardIfPossible()
{
    // use temp variables to get new coordinates
    int newX = getX(), newY = getY();
    getXYInFrontOfMe(newX, newY);
    // if you can move then move and return true else return false
    if ((getWorld()->canMoveTo(newX, newY)))
    {
        cerr << "Insect Moved" << endl;
        moveTo(newX, newY);
        return true;
    }
    return false;
}

void Insect::increaseSleepTicks(int amt)
{
    m_sleepTicks += amt;
}

int Insect::getSleepingTicks() const
{
    return m_sleepTicks;
}

void Insect::decreaseSleepingTicks()
{
    m_sleepTicks--;
}


// ANT //

Ant::Ant(StudentWorld* sw, int startX, int startY, Direction dir, int colony, Compiler* program, int imageID, Actor* myAntHill):Insect(sw, startX, startY, dir, 1500, imageID)
{
    cerr << "Ant Constrcutor" << endl;
    m_colony = colony;
    m_food = 0;
    m_blocked = false;
    m_randVal = 0;
    m_counter = 0;
    m_bitten = false;
    m_program = program;
    m_anthill = myAntHill;
}

void Ant::doSomething()
{
    //The ant must lose one hit point (because it burns energy during each tick and gets hungrier).
    updateEnergy(-1);
    
    // The ant must check to see if its hit points have reached zero.
    if(getEnergy() <= 0)
    {
        //If so, it must: Add 100 units of food to the simulation world in its current x,y location, Set its state to dead.
        
        addFood(100);
        setDead();
        return;
    }
    
    if (getSleepingTicks() != 0) // if Ant is stunned
    {
        cerr << "sleeping ants" << getSleepingTicks() << endl;
        decreaseSleepingTicks();
        return;
    }
    
    setUnstunned();
    Compiler::Command cmd;
    
 
    for (int i = 0; i < 10; i++) // keep running for a max of ten commands
    {
        // get the command from element m_counter of the vector
        if (! m_program->getCommand(m_counter , cmd) )
        {
            cerr << "setting dead no command" << endl;
            setDead();
            return;
        } // error - no such instruction!
        switch (cmd.opcode)
        {
            case Compiler::moveForward:
            {
                // cause the ant to move forward by // updating its x,y coordinates
                cerr << "cmd : moveForward" << endl;
                if (moveForwardIfPossible())
                {
                    cerr << "Ant moved" << endl;
                    m_blocked = false;
                    m_bitten = false;
                }
                else // couldn't move to
                {
                    cerr << "ant couldn't move it was blocked" << endl;
                    m_blocked = true;
                }
                m_counter++;
                return;
            }
            case Compiler::eatFood:
            {
                cerr << "cmd : eatFood" << endl;
                if (m_food >= 100) // if it carries more than 100 units eat 100
                {
                    cerr << "Ant ate 100" << endl;
                    updateEnergy(100);
                    m_food -= 100;
                }
                else // other wise eat whatever it has
                {
                    cerr << "Ant ate " << m_food << endl;
                    updateEnergy(m_food);
                    m_food = 0;
                }
                m_counter++;
                return;
            }
            case Compiler::dropFood:
            {
                cerr << "cmd : dropFood" << endl;
                //getWorld()->addFoodSW(m_anthill, m_food);
                addFood(m_food);
                m_food = 0;
                m_counter++;
                return;
            }
            case Compiler::bite:
            {
                cerr << "cmd : bite" << endl;
                if (getWorld()->isEnemyAt(getX(), getY(), m_colony))
                {
                    cerr << "found an enemy" << endl;
                    getWorld()->biteEnemyAt(this, m_colony, 15);
                }
                m_counter++;
                return;
            }
           
            case Compiler::pickupFood:
            {
                cerr << "cmd : pickupFood" << endl;
                if (m_food < 1800) // while ant has capacity to hold food
                {
                    Actor* a = getWorld()->getEdibleAt(getX(), getY());
                    int amount = a->getEnergy();
                    amount = 1800 - m_food + amount;
                    if (amount >= 400) //if ant has capacity more than 400 then only pick up 400
                        amount = 400;
                    // otherwise pick up how much ever you can
                    // check if current location has food
                    if (a != nullptr)
                    {
                        m_food += amount;
                        a->updateEnergy(-amount);
                    }
                }
                m_counter++;
                return;
            }
            case Compiler::emitPheromone:
            {
                cerr << "cmd : emitpheromone" << endl;
                Actor* a = getWorld()->getPheromoneAt(getX(), getY(), m_colony);
                if (a != nullptr) // if ant can find it's own pheromone
                {
                    
                    int total = a->getEnergy();
                    if (total < 768) // if pheromone can hold more scent add more scent
                    {
                        cerr << "added scent" << endl;
                        int amount = 768 - total;
                        if (amount > 256)
                            amount = 256;
                        a->updateEnergy(amount);
                    }
                }
                else // otherwise create a new pheromone object
                {
                    cerr << "create a new pheromone" << endl;
                    getWorld()->addPheromone(this);
                }
                m_counter++;
                return;
            }
            case Compiler::faceRandomDirection:
            {
                // set new random direction
                cerr << "cmd : faceRandomDirection" << endl;
                setDirection(getWorld()->generateRandDir());
                m_counter++;
                return;
            }
            case Compiler::rotateClockwise:
            {
                cerr << "cmd : rotateClockwise" << endl;
                switch (getDirection())
                {
                    case right:
                        setDirection(down);
                        break;
                    case down:
                        setDirection(left);
                    case left:
                        setDirection(up);
                    case up:
                        setDirection(right);
                    default:
                        break;
                }
                m_counter++;
                return;
            }
            case Compiler::rotateCounterClockwise:
            {
                cerr << "cmd : rotateCounterClockwise" << endl;
                switch (getDirection())
                {
                    case right:
                        setDirection(up);
                        break;
                    case down:
                        setDirection(right);
                    case left:
                        setDirection(down);
                    case up:
                        setDirection(left);
                    default:
                        break;
                }
                m_counter++;
                return;
            }
            case Compiler::generateRandomNumber:
            {
                cerr << "cmd : generateRandomNumber" << endl;
                if (stoi(cmd.operand1) == 0)
                    m_randVal = 0;
                else
                    m_randVal = randInt(0, stoi(cmd.operand1) - 1);
                m_counter++;
                break;
            }
            case Compiler::goto_command:
            {
                cerr << "cmd : goto_command" << endl;
                m_counter = stoi(cmd.operand1);
                break;
            }
            case Compiler::if_command:
            {
                cerr << "cmd : if_command" << endl;
                switch(stoi(cmd.operand1))
                {
                    case Compiler::last_random_number_was_zero:
                    {
                        cerr << "cmd : last_last_random_number_was_zero" << endl;
                        if (m_randVal == 0)
                        {
                            cerr << "the number was zero" << endl;
                            m_counter= stoi(cmd.operand2);
                            //break;
                        }
                        else
                            m_counter++;
                        break;
                    }
                    case Compiler::i_am_carrying_food:
                    {
                        // if you are carrying food then
                        cerr << "cmd : i_am_carrying_food" << endl;
                        if (m_food > 0)
                        {
                            cerr << "I am carrying food" << endl;
                            m_counter= stoi(cmd.operand2);
                            //break;
                        }
                        else
                            m_counter++;
                        break;
                    }
                        case Compiler::i_am_hungry:
                    {
                        // if you don't have energy
                        cerr << "cmd : i_am_hungry" << endl;
                        if(getEnergy() <= 25)
                        {
                            cerr << "i am hungry" << endl;
                            m_counter= stoi(cmd.operand2);
                            //break;
                        }
                        else
                            m_counter++;
                        break;
                    }
                    case Compiler::i_am_standing_with_an_enemy:
                    {
                        cerr << "cmd : i_am_standing_with_an_enemy" << endl;
                        // if enemy is at you location
                        if(getWorld()->isEnemyAt(getX(), getY(), m_colony))
                        {
                            cerr << "I am standing with an enemy" << endl;
                            m_counter= stoi(cmd.operand2);
                            //break;
                        }
                        else
                            m_counter++;
                        break;
                    }
                    case Compiler::i_am_standing_on_food:
                    {
                        cerr << "cmd : i_am_standing_with_on_food" << endl;
                        if(getWorld()->getEdibleAt(getX(), getY()) != nullptr)
                        {
                            cerr << "I am standing on food" << endl;
                            m_counter = stoi(cmd.operand2);
                        }
                        else
                            m_counter++;
                        break;
                        
                    }
                    case Compiler::i_am_standing_on_my_anthill:
                    {
                        // if you are standing on your own anthill
                        cerr << "cmd : i_am_standing_on_my_anthill" << endl;
                        if(getX() == m_anthill->getX() && getY() == m_anthill->getY() && !(m_anthill->isDead()))
                        {
                            cerr << "i am standing on my anthill" << endl;
                            m_counter= stoi(cmd.operand2);
                            //break;
                        }
                        else
                            m_counter++;
                        break;
                    }
                    case Compiler::i_smell_pheromone_in_front_of_me:
                    {
                        cerr << "cmd : i_smell_pheromone" << endl;
                        int x = getX(), y = getY();
                        getXYInFrontOfMe(x, y);
                        // if the pheromone if in front of you
                        if(getWorld()->getPheromoneAt(x, y, m_colony) != nullptr)
                        {
                            cerr << "i do smell pheromone" << endl;
                            m_counter= stoi(cmd.operand2);
                            //break;
                        }
                        else
                            m_counter++;
                        break;
                    }
                    case Compiler::i_smell_danger_in_front_of_me:
                    {
                        cerr << "cmd : i_smell_danger_in_front_of_me" << endl;
                        int x = getX(), y = getY();
                        getXYInFrontOfMe(x, y);
                        // if danger is in front of the ant
                        if(getWorld()->isDangerAt(x, y, m_colony))
                        {
                            cerr << "i do smell danger" << endl;
                            m_counter= stoi(cmd.operand2);
                            
                        }
                        else
                            m_counter++;
                        break;
                    }
                    case Compiler::i_was_bit:
                    {
                        cerr << "cmd : i_was_bit" << endl;
                        if(m_bitten == true)
                        {
                            cerr << "I was bitten" << endl;
                            m_counter= stoi(cmd.operand2);
                            
                        }
                        else
                            m_counter++;
                        break;
                    }
                    case Compiler::i_was_blocked_from_moving:
                    {
                        cerr << "cmd : i_was_blocked_from_moving" << endl;
                        if(m_blocked == true)
                        {
                            cerr << "i was blocked from moving " << endl;
                            m_counter= stoi(cmd.operand2);
                            
                        }
                        else
                            m_counter++;
                        break;
                    }
                        
                }
            }
            default:
                break;
        }
    }

}

bool Ant::isEnemy(int colony) const
{
    if(m_colony == colony) // only colonies that aren't yours are enemy
        return false;
    return true;
}

void Ant::getBitten(int amt)
{
    updateEnergy(-amt);
    setBitten(); // set m_bitten to true
    if (getEnergy() <= 0)
    {
        addFood(100);
        setDead();
    }
}

bool Ant::isDangerous(int colony) const
{
    if (colony == m_colony) // everything other than your colony is danger
        return false;
    return true;
}

bool Ant::canBeTriggered() const
{
    return true;
}

void Ant::setBitten()
{
    m_bitten = true;
}

int Ant::getColony() const
{
    return m_colony;
}

// Grasshopper //

Grasshopper::Grasshopper(StudentWorld* sw, int startX, int startY, Direction dir, int energy, int imageID):Insect(sw, startX, startY, dir, energy, imageID)
{
    cerr << "Grasshopper Constrcutor" << endl;
    m_ticks = 0;
    m_distance = randInt(2, 10);
}

void Grasshopper::doSomething() const
{ }

int Grasshopper::getDistance() const
{
    return m_distance;
}

void Grasshopper::resetDistance()
{
    m_distance = randInt(2, 10);
}

void Grasshopper::decreaseDistance()
{
    m_distance--;
}

int Grasshopper::getTicks() const
{
    return m_ticks;
}

void Grasshopper::decreaseTicks()
{
    m_ticks--;
}

void Grasshopper::resetTicks()
{
    m_ticks = 2;
}

void Grasshopper::setDistanceZero()
{
    m_distance = 0;
}

// BABYGrasshopper //

BabyGrasshopper::BabyGrasshopper(StudentWorld* sw, int startX, int startY, Direction dir):Grasshopper(sw, startX, startY, dir, 500, IID_BABY_GRASSHOPPER)
{
    cerr << "BabyGrasshopper Constrcutor" << endl;
}

void BabyGrasshopper::doSomething()
{
    updateEnergy(-1);
    
    //The baby Grasshopper must check to see if its hit points have reached zero.
    if (getEnergy() <= 0)
    {
        //If so, it must:
        // Add 100 units of food to the simulation world in its current x,y location
        cerr << "BabyGrasshopper died" << endl;
        addFood(100);
        setDead();
        return;
    }
    
    if (getTicks() > 0) // if BabyGrasshopper is sleeping
    {
        decreaseTicks();
        return;
    }
    
    if (getSleepingTicks() > 0)
    {
        cerr << "baby is stunned" << endl;
        decreaseTicks();
        return;
    }
    
    
    setUnstunned();
    
    resetTicks(); // start ticks over
    
    // The baby Grasshopper checks its hit points. If its hit points are greater than or equal to 1,600, then it will moult and turn into an adult Grasshopper.
    if (getEnergy() >= 1600)
    {
        //Create and add a new adult Grasshopper object to the simulation in the same square as the baby.
        getWorld()->addAdultGrasshopper(this);
        
        //Set the baby’s status status to dead, resulting in a pile of 100 units of food being
        cerr << "BabyGrasshopper died by moulting" << endl;
        addFood(100);
        setDead();
        
        return;
    }
    
    if (pickupAndEatFood(200) > 0) // BabyGrasshopper picked up any food
    {
        cerr << "BabyGrasshopper ate" << endl;
        int chance = randInt(0, 1); // 50% chance of sleeping after eating
        if (chance == 0)
            return;
    }
    
    if (getDistance() == 0)
        // change the direction and pick another random distance
    {
        setDirection(getWorld()->generateRandDir());
        resetDistance();
    }
    
    if (!moveForwardIfPossible())// if it didn't move, reset ditance needed to travel in current direction to 0
    {
        setDistanceZero();
        return;
    }
    else
    {
        cerr << "BabyGrasshopper moved" << endl;
        decreaseDistance();
    }
}

bool BabyGrasshopper::canBeTriggered() const
{
    return true;
}

// ADULTGrasshopper //

AdultGrasshopper::AdultGrasshopper(StudentWorld* sw, int startX, int startY, Direction dir):Grasshopper(sw, startX, startY, dir, 1600, IID_ADULT_GRASSHOPPER)
{
    cerr << "AdultGrasshopper Constrcutor" << endl;
}

bool AdultGrasshopper::isDangerous(int colony) const
{
    return true; // all Adult Grashopppers are dangerous
}

void AdultGrasshopper::doSomething()
{
    // The adult Grasshopper loses one hit point (as it gets hungrier).
    updateEnergy(-1);
    
    //The adult Grasshopper must check to see if its hit points have reached zero.
    if (getEnergy() <= 0)
    {
        //If so, it must:
        // Add 100 units of food to the simulation world in its current x,y location
        cerr << "AdultGrasshopper died" << endl;
        addFood(100);
        setDead();
        return;
    }
    
    if (getTicks() != 0) // if AdultGrasshopper is sleeping
    {
        decreaseTicks();
        return;
    }
    
    resetTicks();

    
    int chance = randInt(1, 3); //one in 3 change that it will bite if enemy exists on the same location
    if (getWorld()->isEnemyAt(getX(), getY(), -1))
    {
        // here using -1 because all ants are enemies
        if (chance == 3)
        {
            cerr << "AdultGrasshopper wants to bite" << endl;
            if (getWorld()->biteEnemyAt(this, -1, 50)) // if the Grasshopper bits
            {
                cerr << "AdultGrasshopper bit" << endl;
                return;
            }
        }
        
    }
    
    chance = randInt(1, 10); // 1 in 10 chance to jump
    if (chance == 10)
    {
        int jumpX, jumpY, theta;
        while(true)
        {
            theta = randInt(0, 360);
            jumpX = getX() + 10*cos(theta);
            jumpY = getY() + 10*sin(theta);
            if (jumpX >= 0 && jumpX < VIEW_WIDTH && jumpY >= 0 && jumpY < VIEW_HEIGHT)
            {
                if(getWorld()->canMoveTo(jumpX, jumpY))
                {
                    moveTo(jumpX, jumpY);
                    return;
                }
            }
        }
    }
    
    if (pickupAndEatFood(200) > 0) // AdultGrasshopper picked up food
    {
        // If the adult Grasshopper did eat, then there is a 50% chance it will want to immediately rest
        cout << "AdultGrasshopper ate" << endl;
        int chance = randInt(0, 1);
        if (chance == 0)
            return;
    }

    if (getDistance() == 0)
        // change the direction and pick another random distance
    {
        setDirection(getWorld()->generateRandDir());
        resetDistance();
    }
    
    if (!moveForwardIfPossible())// if it didn't move, switch to the opposite direction
    {
        setDistanceZero();
        return;
    }
    else
    {
        cerr << "AdultGrasshopper moved" << endl;
        decreaseDistance();
    }
}

void AdultGrasshopper::getBitten(int amt)
{
    // deducts the bite damage from the energy
    updateEnergy(-amt);
    if (getEnergy() <= 0)
    {
        addFood(100);
        setDead();
        return;
    }
    // if a grasshopper is bitten 50% chance it will bite back in the same tick
    int chance = randInt(1, 2);
    if (chance == 2)
    {
        getWorld()->biteEnemyAt(this, -1, 50);
    }
    
}
