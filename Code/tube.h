// MSc C++ Programming - Assessed Exercise No. 1
// Author: William Moyle
// Last modified: 27/10/14
// Function header file

////////////////////////////////////////////////////////////////////////////////////
// PRE-SUPPLIED FUNCTIONS
////////////////////////////////////////////////////////////////////////////////////

enum Direction {N, S, W, E, NE, NW, SE, SW, INVALID_DIRECTION};

/* error codes for Question 3 */
#define ERROR_START_STATION_INVALID -1 
#define ERROR_ROUTE_ENDPOINT_IS_NOT_STATION -2
#define ERROR_LINE_HOPPING_BETWEEN_STATIONS -3
#define ERROR_BACKTRACKING_BETWEEN_STATIONS -4
#define ERROR_INVALID_DIRECTION -5
#define ERROR_OFF_TRACK -6
#define ERROR_OUT_OF_BOUNDS -7

/* pre-supplied function to load a tube map from a file*/
char **load_map(const char *filename, int &height, int &width);

/* pre-supplied function to print the tube map */
void print_map(char **m, int height, int width);

/* pre-supplied helper function to describe errors for Question 3 */
const char *error_description(int code);

/* presupplied helper function for converting string to Direction enum */
Direction string_to_direction(const char *token);

////////////////////////////////////////////////////////////////////////////////////
// FUNCTIONS WRITTEN BY WILL MOYLE
////////////////////////////////////////////////////////////////////////////////////


/* function locates the symbol 'target' in 'height' x 'width' map 'm' and 
   either returns 'true' if the symbol is found and sets r and c to the 
   symbol's coordinates, or returns 'false' and sets r and c to -1 */
bool get_symbol_position(char **m, int height, int width, char target, int &r, int &c);

/* returns the character symbol for the station or line given in 'name' by
   searching through the files "stations.txt" and "lines.txt" */
char get_symbol_for_station_or_line(const char *name);


/* returns the number of steps required to complete the journey 'route' 
   from 'start_station' within map 'm' of dimensions 'height' x 'width'
   and sets 'destination' to the final station, or returns an appropriate 
   error message */
int validate_route(char **m, int height, int width, const char *start_station, char *route, char *destination);






