#ifndef __BOULDER_H__
#define __BOULDER_H__

#include "game_object.h"
#include "game_board.h"

/** boulder **
 *
 * This class represents an object in the game Rogue. It is characterized by the letter 'B'
 * and it behaves by falling quickly down, and rising slowly.
 *
 **/
class boulder : public game_object
{
  public:
   boulder(int startX, int startY);
   ~boulder();
   virtual bool update(game_board& board, int timestep);
   virtual void player_collision(game_board& board);
  private:
   int direction;
};

#endif
