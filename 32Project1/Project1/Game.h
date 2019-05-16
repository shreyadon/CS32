#ifndef Game_h
#define Game_h
///////////////////////////////////////////////////////////////////////////
// Arena definition
///////////////////////////////////////////////////////////////////////////
#include "globals.h"
#include "Player.h"

class Game
{
public:
    // Constructor/destructor
    Game(int rows, int cols, int nRats);
    ~Game();
    
    // Mutators
    void play();
    
private:
    Arena* m_arena;
    
    // Helper functions
    string takePlayerTurn();
};

#endif /* Game_h */
