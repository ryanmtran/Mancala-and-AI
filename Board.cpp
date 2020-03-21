#include "Board.h"
#include <iostream>
//implementation of Board's constructors
Board::Board(int nHoles, int nInitialBeansPerHole) : m_holes(nHoles) //member initialization
{
	if (nHoles <= 0) //if individual enters non positve holes, we make holes equal to 1
	{
		nHoles = 1;
		m_holes = 1;
	}
	if (nInitialBeansPerHole < 0) //if individual enteres negative beans, we make beans equal to 0
	{
		nInitialBeansPerHole = 0;
	}

	for (int i = 0; i < nHoles; i++) //adds the number of marbles to south vector's holes
	{
		south.push_back(nInitialBeansPerHole);
	}
	south.push_back(0);//creates the pot for south
	for (int j = nHoles; j > 0; j--) //adds the number of marbles to north vector's holes
	{
		north.push_back(nInitialBeansPerHole);
	}
	north.push_back(0); //creates the pot for north
}


Board::Board(const Board &other): m_holes(other.m_holes) //implementation of copy constructor
{
	for (int i = 1; i <= m_holes; i++) //adds the number of marbles to south vector's holes
	{
		south.push_back(other.beans(SOUTH, i)); //pushing other south's beans onto new vertex
	}
	south.push_back(other.beans(SOUTH, 0));//copies the pot for south
	for (int j = m_holes; j > 0; j--) //adds the number of marbles to north vector's holes
	{
		north.push_back(other.beans(NORTH, j)); //pushing other norths's beans onto new vertex
	}
	north.push_back(other.beans(NORTH, 0)); //copies the pot for north
}

int Board::holes() const //implementation of Board's holes() function that returns the number of holes we have
{
	return m_holes;
}

int Board::beans(Side s, int hole) const  //implementation of beans function that returns how many beans we have in a hole on a certain side
{
	if (hole > holes() || hole < 0) //invalid case when we have negative hole or hole that exceed number
	{
		return -1;
	}
	else if (s == NORTH && hole == 0) //north pot
	{
			return north[holes()];
	}
	else if (s == SOUTH && hole == 0) //south pot
	{
		return south[holes()];
	}

	else if (s == SOUTH) //south hole
	{
		return south[hole - 1];
	}
	else //north hole
	{
		return north[holes()-hole];
	}
}

int Board::beansInPlay(Side s) const //returns the beans in play in north side or south side
{
	int count = 0;
	if (s == SOUTH)
	{
		for (int i = 0; i < holes(); i++) //loop that iterates through vector to add up all the beans in play on south side
		{
			count = count + south[i];
		}
	}
	else
	{
		for (int j = 0; j < holes(); j++) //loop that iterates through vector to add up all the beans in play
		{
			count = count + north[j];
		}
	}
	return count;
}

int Board::totalBeans() const //implementation of totalBeans function of Board class
{
	return beansInPlay(NORTH) + beansInPlay(SOUTH) + north[holes()] + south[holes()]; //returns total number of beans on board
}

bool Board::sow(Side s, int hole, Side& endSide, int& endHole) //implementation of sow function that moves beans
{
	//condition for invalid s or hole entered
	if ((s != NORTH && s != SOUTH) || hole <= 0 || hole > holes())
	{
		return false;
	}
	else if (beans(s, hole) == 0)
	{
		return false;
	}
	else
	{
		Side flag = s; //tracks what side we are on
		int bean_hand = beans(s, hole); //denotes the number of beans we had in the very beginning
		int index; //used for indexing in the vertex

		if (s == SOUTH) //puts the inital hole value into terms of indices for the vectors
		{
			index = hole - 1;
			south[index] = 0; //removes bean from pot because we put them in our hand
		}
		else if (s == NORTH) //puts the inital hole value into terms of indices for the vectos
		{
			index = holes() - hole;
			north[index] = 0; //removes bean from pot because we put then in our hand
		}

		while (bean_hand != 0)
		{
			//when the sowing started on the south side
			if (s == SOUTH && index != holes() && flag == SOUTH) //when we are still on our own south side
			{
				flag = SOUTH;
				bean_hand--; //decrement hand
				index++; //increment index
				south[index]++;
			}
			else if (s == SOUTH && index == holes() && flag == SOUTH) //when we are on our own south side and hit our pot
			{
				flag = NORTH;
				bean_hand--; 
				index = 0; //we change flag and reset index
				north[index] ++;
			}

			else if (s == SOUTH && index != holes() - 1 && flag == NORTH) //when the south player is disbursing on the north side
			{
				flag = NORTH;
				bean_hand--;
				index++;
				north[index] ++;
			}

			else if (s == SOUTH && index == holes() - 1 && flag == NORTH) //when the south player is disbursing on the north side and hits their pot
			{
				flag = SOUTH;
				index = 0; //we change flag and reset index
				bean_hand--;
				south[index] ++;

			}

			//when the sowing started on the norht side
			else if (s == NORTH && index != holes() && flag == NORTH) //when we are still on the north side
			{
				flag = NORTH;
				bean_hand--;
				index++;
				north[index]++;
			}
			else if (s == NORTH && index == holes() && flag == NORTH) //when we are on the north side and we hit our own pot
			{
				flag = SOUTH;
				bean_hand--;
				index = 0; //we change flag and reset index
				south[index]++;
			}

			else if (s == NORTH && index != holes() - 1 && flag == SOUTH) //when the north player is disbursing on the south side
			{
				flag = SOUTH;
				bean_hand--;
				index++;
				south[index] ++;
			}

			else if (s == NORTH && index == holes() - 1 && flag == SOUTH) //when the north player is disbursing on the south side and hits their pot
			{
				flag = NORTH;
				index = 0; // we change flag and reset index
				bean_hand--; 
				north[index]++;
			}

		}
		endSide = flag; //we set the endflag to the flag we were left with upon leaving the while loop
		if (index == holes()) //for the instance where we land at the pot
		{
			endHole = 0; //convert index back to positions
		}
		else if (endSide == SOUTH)
		{
			endHole = index + 1; //convert index back to positions
		}
		else if (endSide == NORTH)
		{
			endHole = holes() - index; //convert index bac to positions
		}

	}
	return true;
}

bool Board::moveToPot(Side s, int hole, Side potOwner) //implementation of movetoPot function that takes beans from hole and moves to pot
{
	if (hole <= 0 || hole > holes()) //invalid cases of pot, hole that exceeds numbering, or negative hol
	{
		return false;
	}
	else if (potOwner==NORTH) //when the potOwner we want to transfer to is North
	{
		if (s == NORTH) //when the side we are taking from us nother
		{
			north[holes()] = north[holes()] + north[holes() - hole];
			north[holes() - hole] = 0;
		}
		else if (s == SOUTH) //when the side we are transfering from is south
		{
			north[holes()] = north[holes()] + south[hole - 1];
			south[hole - 1] = 0;
		}
	}

	else if (potOwner == SOUTH) //when the pot we want to move beans to are south
	{
		if (s == NORTH) //when the side we are transfering from is north
		{
			south[holes()] = south[holes()] + north[holes() - hole];
			north[holes() - hole] = 0;
		}
		else if (s == SOUTH) //when the side we are transfering from is south
		{
			south[holes()] = south[holes()] + south[hole - 1];
			south[hole - 1] = 0;
		}
	}
	return true;

}

bool Board::setBeans(Side s, int hole, int beans) //implementation of setBeans function that sets beans on a certain side
{
	if (beans < 0 || hole > holes() || hole < 0) //invalid cases: when beans is negative or when hole is not on the board
	{
		return false;
	}
	else
	{
		if (s == SOUTH) //when the side we are working with is south
		{
			if (hole == 0) //when we are messing with the pot
			{
				south[holes()] = beans;
			}
			else
			{
				south[hole - 1] = beans;
			}
		}
		else if (s == NORTH) //when the side we are working with is north
		{
			if (hole == 0) //when we are messing with the pot
			{
				north[holes()] = beans;
			}
			else
			{
				north[holes() - hole] = beans;
			}
		}
		return true;
	}
}