#ifndef __ZOMBIE_H__
#define __ZOMBIE_H__

#include "game_object.h"
#include "game_board.h"
#include "player.h"

/** zombie **
 *
 * Represents a Zombie in the game Rogue. Characterized by the letter 'Z' and it
 * behaves by always moving in the direction of the player
 *
 **/
class zombie : public game_object
{
  public:
   zombie(int startX, int startY, player* _player);
   ~zombie();
   virtual bool update(game_board& board, int timestep);
   virtual void player_collision(game_board& board);
  private:
   int direction;
   player* _player;
};

#endif
