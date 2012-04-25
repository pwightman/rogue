#include "wizard.h"
#include <iostream>
using namespace std;


/** wizard **
 *
 * See header file for comments
 *
 **/

wizard::wizard(int startX, int startY, player* _player)
{
   character = 'W';
   x = startX;
   y = startY;
   direction = 1;
   this->_player = _player;
}

wizard::~wizard()
{

}

bool wizard::update(game_board& board, int timestep)
{
   if( timestep % 20 != 0)
      return true;
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
   if( newChar == '.'  || newChar == '@' )
   {
      board.register_game_object(new spell(x + xDirection, y + yDirection, _player->getX(), _player->getY()));
   }
   return true;
}

void wizard::player_collision(game_board& board)
{
   
   board.reset_player();
}
