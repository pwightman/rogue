#include "boulder.h"
#include <iostream>
using namespace std;

/** boulder **
 *
 * See header file for comments
 *
 **/
boulder::boulder(int startX, int startY)
{
   character = 'B';
   x = startX;
   y = startY;
   direction = 1;
}

boulder::~boulder()
{

}

bool boulder::update(game_board& board, int timestep)
{

   if(board.get_char(x, y + direction) == '#')
      direction *= -1;

   if( direction == -1 && (timestep % 4) != 0)
      return true;
   else if( direction == 1 && (timestep % 2) != 0)
      return true;


   QChar newChar = board.get_char(x, y + direction);

   if(newChar == '.' || newChar == '@')
   {
      board.set_char(x, y + direction, character);
      board.set_char(x, y, '.');
      y += direction;
      if(newChar == '@')
	 player_collision(board);
   }
   return true;
}

void boulder::player_collision(game_board& board)
{
   
   board.reset_player();
}
