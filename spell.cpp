#include "spell.h"
#include <iostream>
using namespace std;

/** spell **
 *
 * See header file for comments
 *
 **/
spell::spell(int startX, int startY, int playerX, int playerY)
{
   character = '+';
   x = startX;
   y = startY;
   this->playerX = playerX;
   this->playerY = playerY;
   px = startX;
   py = startY;
   dx = qAbs(playerX - startX);
   dy = qAbs(playerY - startY);
   if( startX < playerX )
      sx = 1;
   else
      sx = -1;
   if( startY < playerY )
      sy = 1;
   else
      sy = -1;
   err = dx - dy;
}

spell::~spell()
{

}

bool spell::update(game_board& board, int timestep)
{
   if( (timestep % 1) != 0 )
      return true;


   //  if x0 = x1 and y0 = y1 exit loop
   int e2 = 2*err;
   if( e2 > -dy )
   {
      err -= dy;
      x += sx;
   }
   if( e2 < dx )
   {
      err = err + dx;
      y = y + sy;
   }

   board.set_char(px, py, '.');
   px = x;
   py = y;

   QChar comp = board.get_char(x,y);
   if( comp == '@' )
   {
      player_collision(board);
      return false;
   }
   else if( comp != '.' )
      return false;
   

   board.set_char(x, y, character);


   return true;
}

void spell::player_collision(game_board& board)
{
   
   board.reset_player();
}
