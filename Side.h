#ifndef SIDE_H
#define SIDE_H

enum Side { NORTH, SOUTH }; //enumeration where side can only be north or south

const int NSIDES = 2;
const int POT = 0;

inline
Side opponent(Side s) //returns the opposite of north and south
{
	return Side(NSIDES - 1 - s);
}
#endif