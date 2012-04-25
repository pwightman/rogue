#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "game_object.h"
#include "game_board.h"


class player : public game_object
{
  public:
   player(game_board* board);
   ~player();
   enum Direction { Up, Down, Left, Right };
   void move( Qt::Key direction, game_board& board );
   void set_position(int x, int y);
   void set_start_position(int x, int y);
   void goto_start();
   virtual bool update(game_board& board, int timestep);
  private:
   game_board* board;
   int startX;
   int startY;
};

#endif
