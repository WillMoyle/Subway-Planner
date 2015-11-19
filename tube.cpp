// MSc C++ Programming - Assessed Exercise No. 1
// Author: William Moyle
// Last modified: 27/10/14
// Function definition file

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <cstring>
#include <cctype>
#include <cstdlib>

using namespace std;

#include "tube.h"

////////////////////////////////////////////////////////////////////////////////////
// PRE-SUPPLIED FUNCTIONS
////////////////////////////////////////////////////////////////////////////////////

// FUNCTION DECLARATIONS THAT DO NOT APPEAR IN THE HEADER FILE:
/* returns the symbol of the given station or line 'name' by searching through
   the file called 'file_name', or returns ' ' if not found */
char get_symbol_from_file(const char *name, const char *file_name);
/* returns true if compass1 is the reverse direction of compass2 */
bool backtracking(Direction compass1, Direction compass2);
/* check that supplied route is valid. returns 1 if valid, 
   or appropriate error code */
int route_valid(char *route);
/* given a station's symbol, returns the station name */
char *get_station_for_symbol(char symbol);
/* moves the position in the direction stated */
int move_position(char ** m, int height, int width, Direction compass, int &row, int &column);
/* gives a direction based upon two characters (e.g. N, goes to N) */
Direction next_direction(char a, char b);



// START OF FUNCTION allocate_2D_array
char **allocate_2D_array(int rows, int columns) {
  char **m = new char *[rows];
  assert(m);
  for (int r=0; r<rows; r++) {
    m[r] = new char[columns];
    assert(m[r]);
  }
  return m;
}
// END OF FUNCTION



// START OF FUNCTION deallocate_2D_array
void deallocate_2D_array(char **m, int rows) {
  for (int r=0; r<rows; r++)
    delete [] m[r];
  delete [] m;
}
// END OF FUNCTION



// START OF FUNCTION get_map_dimensions
bool get_map_dimensions(const char *filename, int &height, int &width) {
  char line[512];
  
  ifstream input(filename);

  height = width = 0;

  input.getline(line,512);  
  while (input) {
    if ( (int) strlen(line) > width)
      width = strlen(line);
    height++;
    input.getline(line,512);  
  }

  if (height > 0)
    return true;
  return false;
}
// END OF FUNCTION



// START OF FUNCTION load_map
char **load_map(const char *filename, int &height, int &width) {

  bool success = get_map_dimensions(filename, height, width);
  
  if (!success)
    return NULL;

  char **m = allocate_2D_array(height, width);
  
  ifstream input(filename);

  char line[512];
  char space[] = " ";

  for (int r = 0; r<height; r++) {
    input.getline(line, 512);
    strcpy(m[r], line);
    while ( (int) strlen(m[r]) < width )
      strcat(m[r], space);
  }
  
  return m;
}
// END OF FUNCTION



// START OF FUNCTION print_map
void print_map(char **m, int height, int width) {
  cout << setw(2) << " " << " ";
  for (int c=0; c<width; c++)
    if (c && (c % 10) == 0) 
      cout << c/10;
    else
      cout << " ";
  cout << endl;

  cout << setw(2) << " " << " ";
  for (int c=0; c<width; c++)
    cout << (c % 10);
  cout << endl;

  for (int r=0; r<height; r++) {
    cout << setw(2) << r << " ";    
    for (int c=0; c<width; c++) 
      cout << m[r][c];
    cout << endl;
  }
}
// END OF FUNCTION



// START OF FUNCTION error_distcription
const char *error_description(int code) {
  switch(code) {
  case ERROR_START_STATION_INVALID: 
    return "Start station invalid";
  case ERROR_ROUTE_ENDPOINT_IS_NOT_STATION:
    return "Route endpoint is not a station";
  case ERROR_LINE_HOPPING_BETWEEN_STATIONS:
    return "Line hopping between stations not possible";
  case ERROR_BACKTRACKING_BETWEEN_STATIONS:
    return "Backtracking along line between stations not possible";
  case ERROR_INVALID_DIRECTION:
    return "Invalid direction";
  case ERROR_OFF_TRACK:
    return "Route goes off track";
  case ERROR_OUT_OF_BOUNDS:
    return "Route goes off map";
  }
  return "Unknown error";
}
// END OF FUNCTION



// START OF FUNCTION string_to_direction
Direction string_to_direction(const char *token) {
  const char *strings[] = {"N", "S", "W", "E", "NE", "NW", "SE", "SW"};
  for (int n=0; n<8; n++) {
    if (!strcmp(token, strings[n])) 
      return (Direction) n;
  }
  return INVALID_DIRECTION;
}
// END OF FUNCTION


////////////////////////////////////////////////////////////////////////////////////
// FUNCTIONS WRITTEN BY WILL MOYLE
////////////////////////////////////////////////////////////////////////////////////



// START OF FUNCTION get_symbol_position
bool get_symbol_position(char **m, int height, int width, char target, int &r, int &c) {
    bool success(false);
    for (int row = 0; row < height; row++)
    {
       for (int column = 0; column < width; column++)
       {
           if (m[row][column] == target)
           {
               r = row;
               c = column;
               success = true;
               return success;
           }
       }
    }
    r = -1;
    c = -1;
    return success;
}
// END OF FUNCTION



// START OF FUNCTION get_symbol_for_station_or_line
char get_symbol_for_station_or_line(const char *name)
{
  char symbol = ' ';

  // search within "stations.txt":
  symbol = get_symbol_from_file(name,"stations.txt");

  // if symbol not in "stations.txt", search "lines.txt":
  if (symbol == ' ')
    {
      symbol = get_symbol_from_file(name,"lines.txt");
    }

  return symbol;
}
// END OF FUNCTION



// START OF FUNCTION get_symbol_from_file
char get_symbol_from_file(const char *name, const char *file_name)
{
  ifstream input_file;
  char character = '\n', current_symbol;
  int position = 0, length = strlen(name);
  bool found = false;  

  // Open the input file:
  input_file.open(file_name);
  if (input_file.fail())
    {
      cout << "Failed to open " << file_name << " file.";
      exit(1);
    }
    
  while (!input_file.eof())
    {
      if ((character == '\n') && !input_file.eof())
        {
	  // set the first character of the line to 'current_symbol':
	  input_file.get(current_symbol);
	  input_file.get(character);
	  input_file.get(character);
	  while ((character == name[position]) 
		 && (character != '\n') 
		 && (position < length) 
		 && !input_file.eof())
	    {
	      // runs through characters after symbol to match the rest of the line to the input
	      found = true;
	      input_file.get(character);
	      position++;
	      if ((character != name[position]) && (position < length))
		{
		  found = false;
		  position = 0;
		}
	    }
	  if (found)
	    {
	      input_file.close();  
	      return current_symbol; // leaves function if there is a match to 'name'
	    }
        }
      input_file.get(character);
    }
  
  input_file.close();
  return ' ';
}
// END OF FUNCTION



// START OF FUNCTION route_valid
int route_valid(char *route)
{
  int route_length = strlen(route);
  for (int position = 0; position < route_length; position++)
    {
      while (route[position] == ',')
	{
	  position++;
	}
      // case of one letter compass direction:
      if ((route[position+1] == ',')||(position==route_length-1))
	{
	  if ((route[position] != 'N') 
	      && (route[position] != 'E') 
	      && (route[position] != 'S') 
	      && (route[position] != 'W'))
	    {
	      return ERROR_INVALID_DIRECTION;
	    }
	}
      
      // case of two letter compass direction:
      else if ((route[position+2] == ',')||(position==route_length-2))
	{
	  if ((route[position] != 'N') 
	      && (route[position] != 'S') 
	      && (route[position+1] != 'E') 
	      && (route[position+1] != 'W'))
	    {
	      return ERROR_INVALID_DIRECTION;
	    }
	}
    }
  return 1;
}
// END OF FUNCTION


// START OF FUNCTION validate_route
int validate_route(char **m, int height, int width, const char *start_station, char *route, char *destination)
{
  int current_row(0), current_column(0);
  
  // check that start_station is valid
  char start_symbol = get_symbol_for_station_or_line(start_station);
  bool found_start = get_symbol_position(m, height, width, start_symbol, current_row, current_column);
  if (start_symbol == ' ' || found_start == false)
    {
      return ERROR_START_STATION_INVALID;
    }
  
  // check that supplied route is valid
  if (route_valid(route)!=1)
    {
      return ERROR_INVALID_DIRECTION;
    }
  
  int route_length = strlen(route), direction_changes(0);
  Direction compass1=next_direction(route[0],route[1]), compass2;
  char previous_symbol(start_symbol), current_symbol(start_symbol), next_symbol(start_symbol);
  
  // run though the route, moving the current position on the map until the route ends
  for (int position = 0; position < route_length; position++)
    {
      //check route[position] refers to the first character of a direction (not ',')
      if (route[position] == ',')
	{
	  position++;
	}

      // set compass2 to the next_direction
      if (position == route_length-1)
	{
	  compass2 = next_direction(route[position], ',');
	}
      else
	{
	  compass2 = next_direction(route[position], route[position+1]);
	}
      
      if (compass2 == NW || compass2 == NE || compass2 == SW || compass2 == SE)
	{
	  position++;
	}
      
      // move the current position along compass 2 direction
      int movement_error = move_position(m, height, width, compass2, current_row, current_column);
      next_symbol = m[current_row][current_column];
      
      // check if the position movement returned an out of bounds or off track error
      if(movement_error < 0)
	{
	  return movement_error;
	}

      // if the direction has changed:
      if (compass2 != compass1)
	{
	  // if the change doesn't take place at a station:
	  if (!strcmp(get_station_for_symbol(current_symbol),""))
	    {
	      if (current_symbol != next_symbol) // check for line hopping between stations
		{
		  return ERROR_LINE_HOPPING_BETWEEN_STATIONS; 
		}
	      else if (backtracking(compass1,compass2)) // check for backtracking between stations
		{
		  return  ERROR_BACKTRACKING_BETWEEN_STATIONS;
		}
	    }
	  // increase counter if changing lines or backtracking at a station
	  else if ((next_symbol != previous_symbol) || (backtracking(compass1,compass2)))
	    {
	      direction_changes++;
	    }
	}

      compass1 = compass2;
      previous_symbol = current_symbol;
      current_symbol = next_symbol;
    }
  
  char *station = get_station_for_symbol(current_symbol);
  
  strcpy(destination, station);

  delete station;

  // checks that the route ends in a station
  if (!strcmp(destination,""))
    {
      return ERROR_ROUTE_ENDPOINT_IS_NOT_STATION;
      cout << endl;
    }

  return direction_changes;
  
}
// END OF FUNCTION



// START OF FUNCTION next_direction
Direction next_direction(char a, char b)
{
  Direction compass;

  if (b == ',')
    {
      if (a == 'N')
	{
	  compass = N;
	}
      else if (a == 'E')
	{
	  compass = E;
	}
      else if (a == 'S')
	{
	  compass = S;
	}
      else if (a == 'W')
	{
	  compass = W;
	}
    }
  else
    {
      if (a == 'N' && b == 'E')
	{
	  compass = NE;
	}
      else if (a == 'S' && b == 'E')
	{
	  compass = SE;
	}
      else if (a == 'S' && b == 'W')
	{
	  compass = SW;
	}
      else if (a == 'N' && b == 'W')
	{
	  compass = NW;
	}
    }

  return compass;
}
// END OF FUNCTION



// START OF FUNCTION move_position
int move_position(char ** m, int height, int width, Direction compass, int &row, int &column)
{
  int new_row(row), new_column(column);  

  // changes current row and column depending on direction of compass
  switch (compass)
    {
    case 0: // N
      new_row--;
      break;
    case 1: // S
      new_row++;
      break;
    case 2: // W
      new_column--;
      break;
    case 3: // E
      new_column++;
      break;
    case 4: // NE
      new_row--;
      new_column++;
      break;
    case 5: // NW
      new_column--;
      new_row--;
      break;
    case 6: // SE
      new_column++;
      new_row++;
      break;
    case 7: // SW
      new_row++;
      new_column--;
      break;
    default:
      return ERROR_INVALID_DIRECTION;
    }

  // checks if out of bounds
  if ((new_row < 0) || (new_row > height) || (new_column < 0) || (new_column > width))
    {
      return ERROR_OUT_OF_BOUNDS;
    }

  // checks if off track
  if (m[new_row][new_column] == ' ')
    {
      return ERROR_OFF_TRACK;
    }

  row = new_row;
  column = new_column;
  return 1;
}
// END OF FUNCTION



// START OF FUNCTION get_station_for_symbol
char *get_station_for_symbol(char symbol)
{
  ifstream stations;

  stations.open("stations.txt");

  if (stations.fail())
    {
      cout << "\nError opening stations file.\n";
      exit(1);
    }

  char character = 10, current_symbol;
  char *station = new char[512];
  station[0] = '\0';
  int position(0);

  while (!stations.eof())
    {
      if (character == 10)
	{
	  stations.get(current_symbol); // sets current_symbol to be the first symbol on the line
	  if (current_symbol == symbol) // if the current_symbol is the one being searched for
	    {
	      stations.get(character);
	      stations.get(character);
	      while(character != 10)
		{
		  station[position] = character;
		  position++;
		  stations.get(character);
		}
	      stations.close();
	      return station; // allows function to leave once the station is found
	    }
	}
      stations.get(character);
    }
  return station; // returns blank string if station not found
}
// END OF FUNCTION



// START OF FUNCTION backtracking
bool backtracking(Direction compass1, Direction compass2)
{
  bool isBacktrack(false);
  
  switch (compass1)
    {
    case 0: // N
      if (compass2 == 1) {isBacktrack = true;} break;
    case 1: // S
      if (compass2 == 0) {isBacktrack = true;} break;
    case 2: // W
      if (compass2 == 3) {isBacktrack = true;} break;
    case 3: // E
      if (compass2 == 2) {isBacktrack = true;} break;
    case 4: // NE
      if (compass2 == 7) {isBacktrack = true;} break;
    case 5: // NW
      if (compass2 == 6) {isBacktrack = true;} break;
    case 6: // SE
      if (compass2 == 5) {isBacktrack = true;} break;
    case 7: // SW
      if (compass2 == 4) {isBacktrack = true;} break;
    default:
      break;
    }

  return isBacktrack;
}
// END OF FUNCTION
