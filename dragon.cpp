#include "dragon.h"
#include <iostream>
using namespace std;

/** dragon **
 *
 * See header file comments
 *
 **/

dragon::dragon(int startX, int startY)
{
   character = 'D';
   x = startX;
   y = startY;
   direction = 1;
}

dragon::~dragon()
{

}

bool dragon::update(game_board& board, int timestep)
{
   
   if(board.get_char(x, y + direction) == '#')
      direction *= -1;

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

void dragon::player_collision(game_board& board)
{
   
   board.reset_player();
}
