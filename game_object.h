#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__

#include <QChar>
#include <QObject>

class game_board;

/** game_object **
 *
 * This class represents and object in the command-line-based game Rogue.
 * It contains its visual representation as a character, its x and y location
 * on the board, and the ability to update itself to a new position on the game
 * board based on its own internal logic. This is meant to be an abstract class
 * and not to be created directly
 *
 **/

class game_object : QObject
{
  public:
   virtual bool update(game_board& board, int timestep);
   virtual int getX();
   virtual int getY();
   virtual QChar getChar();
   virtual void player_collision(game_board& board);
   

  protected:
   QChar character;
   int x, y;
};



#endif


