#include "zombie.h"
#include <iostream>
using namespace std;

zombie::zombie(int startX, int startY, player* _player)
{
   this->_player = _player;
   character = 'Z';
   x = startX;
   y = startY;
   direction = 1;
}

zombie::~zombie()
{

}

bool zombie::update(game_board& board, int timestep)
{
   if( (timestep % 10) != 0 )
      return true;;

   int xDirection;
   int yDirection;

   if (x > _player->getX())
      xDirection = -1;
   else if (x < _player->getX())
      xDirection = 1;
   else
      xDirection = 0;

   if (y > _player->getY())
      yDirection = -1;
   else if (y < _player->getY())
      yDirection = 1;
   else
      yDirection = 0;


   QChar newChar = board.get_char(x + xDirection, y + yDirection);
   if(newChar == '.' || newChar == '@')
   {
      board.set_char(x + xDirection, y + yDirection, character);
      board.set_char(x, y, '.');
      y += yDirection;
      x += xDirection;
      if(newChar == '@')
	 player_collision(board);
   }
   return true;
}

void zombie::player_collision(game_board& board)
{
   
   board.reset_player();
}
