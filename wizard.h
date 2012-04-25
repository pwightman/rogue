#ifndef __WIZARD_H__
#define __WIZARD_H__

#include "game_object.h"
#include "game_board.h"

/** wizard **
 *
 * Represents a Wizard in the game Rogue. Characterized by the letter 'W'
 * and behaves by casting "spells" in the direction of the player.
 *
 * See abstract class game_object for details on the behavior of objects in general
 *
 **/
class wizard : public game_object
{
  public:
   wizard(int startX, int startY, player* _player);
   ~wizard();
   virtual bool update(game_board& board, int timestep);
   virtual void player_collision(game_board& board);
  private:
   int direction;
   player* _player;
};

#endif
