#include "player.h"
#include <iostream>
using namespace std;

player::player(game_board* board)
{
   x = 1;
   y = 1;
   startX = 1;
   startY = 1;
   character = '@';
   this->board = board;
}

player::~player()
{

}

void player::move( Qt::Key direction, game_board& board )
{
   int newX = 0, newY = 0;
   switch (direction) {
      case Qt::Key_A:
	 if (board.get_char(x - 1, y    ) != '#')
	    --newX;   
	 break;
      case Qt::Key_D:
	 if (board.get_char(x + 1, y    ) != '#')
	    ++newX;
	 break;
      case Qt::Key_W:
	 if (board.get_char(x    , y - 1) != '#')
	    --newY;
	 break;
      case Qt::Key_S:
	 if (board.get_char(x    , y + 1) != '#')
	    ++newY;
	 break;    
      default:
	 break;
   }


   board.set_char(x, y, '.');
   QChar newChar = board.get_char(x + newX, y + newY);

   // If it's not one of these two, then notify the board
   // Board will take care of what happens when you collide with
   // that object
   if(newChar != '.' && newChar != '@')
   {
      board.player_collided(newChar, board);
      return;
   }
   board.set_char(x + newX, y + newY, character);
   x += newX;
   y += newY;

}

void player::set_position(int x, int y)
{
   this->x = x;
   this->y = y;
}

void player::set_start_position(int x, int y)
{
   this->startX = x;
   this->startY = y;
}

void player::goto_start()
{
   board->set_char(x, y, '.');
   x = startX;
   y = startY;
   board->set_char(startX, startY, character);
}

bool player::update(game_board& board, int timestep)
{
    return true;
}
