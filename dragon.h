#ifndef __DRAGON_H__
#define __DRAGON_H__

#include "game_object.h"
#include "game_board.h"

/**
 * Represents a dragon in the game Rogue. It is characterized by the letter 'D'
 * behaves by moving up and down, changing directions when it hits walls
 *
 **/
class dragon : public game_object
{
  public:
   dragon(int startX, int startY);
   ~dragon();
   virtual bool update(game_board& board, int timestep);
   virtual void player_collision(game_board& board);
  private:
   int direction;
};

#endif
