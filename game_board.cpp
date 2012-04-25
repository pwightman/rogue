#include <iostream>
#include <QTime>
#include <fstream>
#include <sstream>
#include "game_board.h"

using namespace std;

/** game_board class **
 *
 * This class represents the internal logic and layout of a game of
 * Rogue, adapted to the demo project in the Qt 4.7 examples. Uses game_objects
 * which update themselves to the board at a specified and changeable time interval.
 * It also takes care of all collision detection. See header file for function contracts
 * 
 *
 **/
game_board::game_board(Window* window, int type)
{
   this->window = window;
   this->type = type;
   _objects = NULL;
   temp_objects = NULL;
   timer = NULL;
   board = NULL;
   lives = 3;
   dungeonLevel = 1;

   if( type == 0 )
      setup_gauntlet();
   else if( type == 1 )
      read_game_board("dungeon_1.txt");
}

void game_board::read_game_board(string filename)
{
   fstream in(filename.c_str(), fstream::in);
   int x = 0;
   int y = 0;

   string line;

   while(in >> line)
   {
      y++;
      x = line.length();
   }


   this->width = x;
   this->height = y;
   window->setFSize(x, y);

   in.close();
   in.open(filename.c_str(), fstream::in);

   cout << x << endl;
   cout << y << endl;

   reset_game_state(x, y);

   y = 0;
   x = 0;
   while(in >> line)
   {
      for(x = 0; x < line.length(); x++)
      {
	 QChar newChar = line[x];
	 this->set_char(x, y, newChar);
	 if( newChar == 'B')
	    _objects->push_back(new boulder(x, y));
	 else if( newChar == 'D')
	    _objects->push_back(new dragon(x, y));
	 else if( newChar == 'Z')
	    _objects->push_back(new zombie(x, y, _player));
	 // COULD POSSIBLE CAUSE BAD THINGS TO HAPPEN IF PLAYER
	 // IS NOT ENCOUNTERED BEFORE SPELLS!
	 else if( newChar == '+')
	    _objects->push_back(new spell(x, y, _player->getX(), _player->getY()));
	 else if( newChar == 'W')
	    _objects->push_back(new wizard(x, y, _player));
	 else if( newChar == 'S')
	 {
	    _player->set_position(x + 1, y);
	    _player->set_start_position(x + 1, y);
	 }
      }
      y++;
   }

   set_char(_player->getX(), _player->getY(), '@');
   interval = 50;
   start_game();
}

void game_board::reset_game_state(int width, int height)
{
   if( _objects != NULL  || temp_objects != NULL)
      delete_game_objects();
   if( timer != NULL )
      delete timer;
   if( board != NULL )
      delete_board();
   this->width = width;
   this->height = height;
   _objects = new list<game_object*>();
   temp_objects = new list<game_object*>();
   timer = new QTimer();
   board = new vector<vector<QChar>*>(width);

   timestep = 0;
   _player = new player(this);

   for(unsigned int i = 0; i < board->size(); i++)
      board->at(i) = new vector<QChar>(height);
   
}


void game_board::update()
{
   ++timestep;
   if(timestep > 100)
      timestep = 0;

   list<game_object*>::iterator i = _objects->begin();
   while (i != _objects->end())
   {
      if (!(*i)->update(*this, timestep))
      {
	 i = _objects->erase(i);  // alternatively, i = items.erase(i);

      }
      else
	 ++i;
   }

   i = temp_objects->begin();
   while (i != temp_objects->end())
   {
      _objects->push_back(*i);
  
      i = temp_objects->erase(i);
   }
}

void game_board::next_wave()
{
   // First check endgame
   if(interval == 50)
   {
      this->kill_game("You won!");
      return;
   }
   interval -= 50;
   setup_gauntlet();
}

int game_board::get_lives()
{
   return lives;
}

void game_board::setup_gauntlet()
{
   reset_game_state(70, 20);
   qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));  
   window->setFSize(70, 20);
   for ( int x = 0; x < width; ++x)
      for ( int y = 0; y < height; ++y) 
      {

	 if( x == width - 1 )
	    this->set_char(x, y, 'E');
	 else if (x == 0 || x == width - 1 || y == 0 || y == height - 1 || qrand() % 10 == 0)
	    this->set_char(x, y, '#');
	 else
	    this->set_char(x, y, '.');
      }



   for( int i = 2; i < width - 2; i++)
   {
      _objects->push_back(new dragon(i, qrand() % (height - 2) + 1));
   }

   this->set_char(_player->getY(), _player->getX(), _player->getChar());  
   interval = 300;

   start_game();
}

void game_board::reset_player()
{
   lives--;
   if(lives == 0)
      kill_game("You used all your lives!");
   _player->goto_start();
}

void game_board::register_game_object(game_object* object)
{
   temp_objects->push_back(object);
}

QTimer* game_board::get_timer()
{
   return timer;
}

void game_board::delete_game_objects()
{
   list<game_object*>::iterator it;
   if(_objects != NULL )
   {
      for( it = _objects->begin(); it != _objects->end(); it++ )
	 delete (*it);
      delete _objects;
      _objects = NULL;
   }
   if(temp_objects != NULL)
   {
      for( it = temp_objects->begin(); it != temp_objects->end(); it++ )
	 delete (*it);
      delete temp_objects;
      temp_objects = NULL;
   }

   
}

void game_board::delete_board()
{
   for( unsigned int i = 0; i < board->size(); i++ )
      delete board->at(i);
   delete board;
   board = NULL;
}

int game_board::get_width()
{
   return width;
}

int game_board::get_height()
{
   return height;
}

void game_board::start_game()
{
   QPoint topLeft(0, 0);
   QPoint bottomRight(width, height);
   QRect frameSize(topLeft, bottomRight);
   window->setFrameRect(frameSize);
   timer->start(interval);
   connect(timer, SIGNAL(timeout()), 
	   window, SLOT(update()));
   running = true;
}

void game_board::set_timer_interval(int interval)
{
   this->interval = interval;
}

QChar game_board::get_char(int col, int row)
{
  return board->at(col)->at(row);
}

void game_board::set_char(int col, int row, QChar newChar)
{
   board->at(col)->at(row) = newChar;
}

void game_board::move_player( Qt::Key key)
{
   if(running)
      _player->move( key, *this );
}

game_board::~game_board()
{
   delete_game_objects();
   delete_board();
   delete timer;
   delete _player;
}

void game_board::print()
{
   for(unsigned int i = 0; i < board->size(); i++)
   {
      for(unsigned int j = 0; j < board->at(0)->size(); j++)
	 cout << board->at(i)->at(j).toAscii();
      cout << endl;
   }
}

void game_board::kill_game(string message)
{
   timer->stop();
   running = false;
   cout << message << endl;

   int startingX = (width/2) - (message.length() / 2) - 1;
   int startingY = (height/2);
   string empty = " ";
   message = empty + message + empty;

   for(int i = 0; i < message.length(); i++)
   {
      QChar newChar = message[i];
      set_char(i + startingX, startingY - 1, ' ');
      set_char(i + startingX, startingY    , newChar );
      set_char(i + startingX, startingY + 1, ' ');
   }
}

void game_board::player_collided(QChar newChar, game_board& board)
{
   if(newChar == 'E')
   {
      if(type == Window::GAUNTLET)
	 this->next_wave();
      if(type == Window::DUNGEON)
      {
	 if(dungeonLevel == 5)
	 {
	    kill_game("You won!");
	    return;
	 }
	 dungeonLevel++;
	 string dung = "dungeon_";
	 std::stringstream ss;
	 ss << dungeonLevel;
	 dung += ss.str();
	 dung += ".txt";
	 read_game_board(dung);
      }
   }
   
   list<game_object*>::iterator it;
   for( it = _objects->begin(); it != _objects->end(); it++)
      if((*it)->getChar() == newChar)
      {
	 (*it)->player_collision(board);
	 break;
      }

/*   cout << "Player x and y: " << _player->getX() << " " <<  _player->getY() << endl;
   for( it = _objects->begin(); it != _objects->end(); it++)
   {
      cout << "Object x and y: " << (*it)->getX() << " " << (*it)->getY() << endl;
      if((*it)->getX() == _player->getX() && (*it)->getY() == _player->getY())
      {
	 (*it)->player_collision(board);
	 break;
      }
   }
*/
}
