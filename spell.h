#ifndef __SPELL_H__
#define __SPELL_H__

#include "game_object.h"
#include "game_board.h"
#include "player.h"

/** spell **
 *
 * Represents a spell in the game Rogue. Characterized by the letter '+' and
 * behaves by heading in the direction of some object's position received through
 * its constructor
 *
 **/
class spell : public game_object
{
  public:
   spell(int startX, int startY, int playerX, int playerY);
   ~spell();
   virtual bool update(game_board& board, int timestep);
   virtual void player_collision(game_board& board);
  private:
   int direction;
   int playerX;
   int playerY;
   int dx;
   int dy;
   int sx;
   int sy;
   int px;
   int py;
   int err;
};

#endif
