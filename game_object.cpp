/** game_object **
 * See header file for comments regarding this class
 *
 **/

#include "game_object.h"

bool game_object::update(game_board& board, int timestep) { return true; }
int game_object::getX() { return x; }
int game_object::getY() { return y; }
QChar game_object::getChar() { return character; }
void game_object::player_collision(game_board& board) { }
