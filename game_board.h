#ifndef __GAME_BOARD_H__
#define __GAME_BOARD_H__

#include <list>
#include <QChar>
#include <QTimer>
#include <string>
#include "game_object.h"
class Window;
#include "window.h"
class player;
#include "player.h"
#include "dragon.h"
#include "zombie.h"
#include "boulder.h"
#include "spell.h"
#include "wizard.h"

using namespace std;

class game_board : public QObject
{
 public:
  /** Contructor **
   *
   * Builds the game state based on a given type
   * defined in Window. Window::DUNGEON and Window::GAUNTLET
   *
   * Inputs: [Window] window : Window that this gameboard is part of
   *         [int] type : Type of game to play
   *
   */
  game_board(Window* window, int type);
  
  /** get_char **
   *
   * Get the character from the board at the given position
   *
   * Inputs: [int] col : column of the board to check
   *         [int] row : row of the board to check
   *
   * Output: [QChar] Character at the specified col and row
   *
   ***/
  QChar get_char(int col, int row);

  /** set_char **
   *
   * Set the character in the board to a new character
   *
   * Inputs: [int] col : column of the board to set
   *         [int] row : row of the board to set
   *
   * Output: none.
   *
   ****/
  void set_char(int col, int row, QChar newChar);
  /** update **
   *
   * Asks all the dynamically moving objects to update themselves
   * to their new position. Has no knowledge of time, just moves
   * one position based on their own internal logic
   *
   * Inputs: none
   * Output: none
   *
   **/
  void update();

  /** print **
   *
   * Print the board to the console
   *
   **/
  void print();

  /** get_timer  **
   *
   * Returns a pointer to the timer controlling the board updates
   *
   * Inputs: none
   * Output: [QTimer*] : Timer controlling game updates
   *
   **/
  QTimer* get_timer();
  /** kill_game **
   *
   * Stops the game from running and displays a message to the screen
   *
   * Inputs: none
   * Output: none
   *
   **/
  void kill_game(string message);

  /** reset_player **
   *
   * Resets the player (@) to its starting position of the current level
   *
   * Inputs: none
   * Output: none
   *
   **/
  void reset_player();

  /** reset_game_state **
   *
   * Resets all lists of objects and the game board
   * to be new, untouched list/arrays
   *
   * Inputs: [int] width  : width of the new screen
   *         [int] height : height of the new screen
   * Output: none
   *
   **/

  void reset_game_state(int width, int height);

  /** next_wave **
   *
   * Used in the gauntlet version of the game. Randomly produces a new map
   * and decreases the time interval by 50ms.
   *
   * Inputs: none
   * Output: none
   *
   **/
  void next_wave();

  /** get_width **
   *
   * Returns the width of the board
   *
   * Inputs: none
   * Output: none
   *
   **/
  int get_width();

  /** get_height **
   *
   * Returns the height of the board
   *
   * Inputs: none
   * Output: none
   *
   **/

  int get_height();

  /** register_game_object **
   *
   * This function is used to register new game objects in the event loop.
   * The next update after an object is submitted, it will be part of the
   * list of updated values
   *
   * Inputs: [game_object*] object : object to be added to the update list
   * Output: none
   *
   **/
  void register_game_object(game_object* object);

  /** delete_board **
   *
   * Deletes the game_board and ensures all vector pointers are deleted
   *
   * Inputs: none
   * Output: none
   *
   **/
  void delete_board();

  /** setup_gauntlet **
   *
   * This sets up the gauntlet version of the game. This function is completely
   * self contained and is the only thing that needs to be called to set up the
   * gauntlet. The preferred method of setting up the gauntlet is through the
   * constructor, however.
   *
   * Inputs: none
   * Output: none
   *
   **/
  void setup_gauntlet();

  /** read_game_board **
   *
   * This sets up the dungeon part of the game by reading in
   * a text file containing the layout of a dungeon. The text file
   * need only have the appearance of a map and will interpret all the
   * characters as its in-game counterpart. Makes for easy level editing!
   *
   * Inputs: [string] filename : name of the file containing the dungeon
   * Output: none
   *
   **/
  void read_game_board(string filename);

  /** delete_game_objects **
   *
   * Ensures that all lists containing game objects are deleted
   *
   * Inputs: none
   * Output: none
   *
   **/
  void delete_game_objects();

  /** set_timer_interval **
   * (Deprecated)
   * Sets the interval at which the timer will repeat at
   *
   * Inputs: [int] interval : time interval to repeat
   * Output: none
   *
   **/
  void set_timer_interval(int interval);

  /** start_game **
   *
   * This will set the timer in motion and start up the event loop
   *
   * Inputs: none
   * Output: none
   *
   **/
  void start_game();

  /** get_lives **
   *
   * Returns the number of lives the player has left. 0 means gameover
   *
   * Inputs: none
   * Output: [int] : number of lives remaining for the player
   *
   **/
  int get_lives();

  /** player_collided **
   *
   * The method determines the course of action when the player
   * approaches a spot on the board that is not a '.' or '#'. It finds
   * the type of square it encountered and puts the proper consequences in motion
   *
   * Inputs: [QChar] newChar : the type of character the player collided with
   *         [game_board&] board : the board on which the collision happened
   * Output: none
   *
   **/
  void player_collided(QChar newChar, game_board& board);

  /** ~game_board **
   *
   * Destructs the game
   *
   * Inputs: none
   * Output: none
   *
   **/
  ~game_board();

  public slots:

  /** move_player **
   *
   * If the key passed in is up/down/left/right then the player
   * will move one space in that direction
   *
   * Inputs: none
   * Output: none
   *
   **/
  void move_player( Qt::Key key );

 private:
  Window* window;  // Reference back to the containing windown
  vector<vector<QChar>*>* board; // Represents the board it's in
  int interval; // Time interval between board updates
  int type;     // Type of game (dungeon/gauntlet)
  int dungeonLevel; // Keeps track of what level of the dungeon the player is on
  player* _player;  // the player on the screen
  int timestep;     // Keeps track of how many updates have happen from (used to control speed)
  bool running;     // Whether the game is running or not
  int lives;        // How many lives the player has left
  QTimer* timer;    // Timer that submits update events
  int width;        // width of the board
  int height;       // height of the board
  list<game_object*>* _objects;     // game_objects that will be updated by the timer callback
  list<game_object*>* temp_objects; // Used so _object can replenish on its own time
};

#endif
