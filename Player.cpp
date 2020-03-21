#include "Player.h" 
#include <string>
#include <iostream>

//Implmentations of Abstract Class Player
Player::Player(std::string name) //implementation of player constructor
{
	m_name = name;
}

std::string Player::name() const //implementation of name getter
{
	return m_name;
}

bool Player::isInteractive() const //implementation of isInteractive--returns false because this is the virtual and will be derived often
{
	return false;
}
Player::~Player() //Player destructor necessary because of abstract class
{	
}


//Implementaions of Human Player Class
HumanPlayer::HumanPlayer(std::string name): Player(name) //implementation of humanplayer constructor
{
}

bool HumanPlayer::isInteractive() const //returns true because human is interactive
{
	return true;
}

int HumanPlayer::chooseMove(const Board& b, Side s) const //implementation of function that determines how human chooses move
{
	if (b.beansInPlay(SOUTH) == 0 || b.beansInPlay(NORTH) == 0) //if we can't make a move, we need to return -1
	{
		return -1;
	}
	std::cout << "Enter a hole value: " ;  //Prompts person to put in a hole number
	int val;
	cin >> val;
	while (val <= 0 || val > b.holes() || b.beans(s, val) == 0) //Ensures that the user eventually inputs a valid hole number
	{
		std::cout << "Please enter a valid hole number with beans: ";
		cin >> val;
	}
	return val;
}
HumanPlayer::~HumanPlayer()//destructor for human player
{
}

//implementation of Bad Player functions
BadPlayer::BadPlayer(std::string name) : Player(name)//constructor for bad Player
{
}
int BadPlayer::chooseMove(const Board& b, Side s) const//badPlayer arbitrarily always picks hole 1
{
	if (b.beansInPlay(SOUTH) == 0 || b.beansInPlay(NORTH) == 0) //if we can't make a move, we need to return -1
	{
		return -1;
	}
	for (int i = 1; i <= b.holes(); i++)//iterates through holes until we find the first one that is not empty
	{
		if (b.beans(s, i) != 0)
		{
			return i;
		}
	}
	return 2; //this will never happen, but here so g++ is not throwing errors
}

BadPlayer::~BadPlayer() //BadPlayer's destructor
{

}

//implementation of Smart Player functions
SmartPlayer::SmartPlayer(std::string name) : Player(name) //constructor for smart Player
{
}
//implementation of minimax function derived from player.h
void SmartPlayer::minimax(int height, const Board &b, Side s, int &bestHole, int &value, AlarmClock& ac) const
{
	int const maxHeight = 4; //the number of iterations we want
	value = -999; 

	 if (b.beansInPlay(SOUTH) == 0 || b.beansInPlay(NORTH) == 0) //no moves exist
	{
		bestHole = -1;
		int score1 = b.beans(s, 0);//this takes our pot
		int score2 = b.totalBeans() - score1; //this gives us the other player's pot
		if (score1 == score2)
		{
			value = 0;
		}
		else if (score1 > score2) //This is the best case scenario, we won
		{
			value = 999;
		}
		else
		{
			value = -999; //Worst case scenario, we lose
		}
	
		/*if (height % 2 != 0) //this is if it is the opponent's turn, we multiply by -1
		{
			value = -1 * returnValue;

		}
		*/
		
		return;
	}
	 else if (ac.timedOut()) //finish what we are doing if ac times out
	 {
		 if (s == SOUTH) ///if we are the south side
		 {
			 value = b.beans(SOUTH, 0) - b.beans(NORTH, 0); ///we return diffrence between our pots
		 }
		 else if (s == NORTH)
		 {
			 value = b.beans(NORTH, 0) - b.beans(SOUTH, 0); ///we return diffrence between our pots
		 }
		 return;
	 }



	else if (height == maxHeight) //when we reach maximum depth
	{
		bestHole = -1;
		if (s == SOUTH) ///if we are the south side
		{
			value = b.beans(SOUTH, 0) - b.beans(NORTH, 0); ///we return diffrence between our pots
		}
		else if (s == NORTH)
		{
			value = b.beans(NORTH, 0) - b.beans(SOUTH, 0); ///we return diffrence between our pots
		}
		return;
	}


	for (int i = 1; i <= b.holes(); i++) //iterate through our holes
	{

		if (b.beans(s, i) == 0)
			continue;
		Board c = b; //create a board to which we will make moves for

		int tempHole; //this is where we end up after the sow is complete
		int valAfterSow;
		Side tempSide;
		c.sow(s, i, tempSide, tempHole);//we will actually make a move
		if (tempSide == s && c.beans(s, tempHole) == 1 && s == SOUTH && c.beans(NORTH, tempHole) != 0) //we sow for south
		{
			c.moveToPot(SOUTH, tempHole, SOUTH);//transfer south's hole to south's pot
			c.moveToPot(NORTH, tempHole, SOUTH); //transfer north's hole to south's pot
		}
		else if (tempSide == s && c.beans(s, tempHole) == 1 && s == NORTH && c.beans(SOUTH, tempHole) != 0) //we sow for south
		{
			c.moveToPot(SOUTH, tempHole, NORTH);//transfer south's hole to south's pot
			c.moveToPot(NORTH, tempHole, NORTH); //transfer north's hole to south's pot
		}

		//find value after sowing
		//valAfterSow = mypotval - opspotval
		if (s == SOUTH)
		{
			valAfterSow = c.beans(SOUTH, 0) - c.beans(NORTH, 0);
		}
		else if (s == NORTH)
		{
			valAfterSow = c.beans(NORTH, 0) - c.beans(SOUTH, 0);
		}

		int currValue; //this value will be compared to valDif to see which move we want to do
		int currHole; //this value will be used to see if the player gets a second turn
		int valDif; //accounts for how much the opponent's move hurt us (or when we get a second turn how much it helps us)

		if (s == NORTH) //so we can put in opposite side in recursive call
		{		
			if (tempHole == 0 && tempSide == s) //We want to take into account the instance where we get another turn
			{
				minimax(height, c, s, currHole, currValue, ac); //keep the side the same and don't increment height
				valDif = valAfterSow + currValue; //valDif is an addition because being on the same side is a good thing
				if (valDif > value)
				{
					bestHole = i;
					value = valDif;
				}
			}
			else
			{
				minimax(height + 1, c, SOUTH, currHole, currValue, ac);
				valDif = valAfterSow - currValue; //ValDif is diffrence because we're seeing how much they're choice hurt us
				if (valDif > value)
				{
					bestHole = i;
					value = valDif;
				}
			}
		}
		else if (s == SOUTH) //so we can put in the opposite side in recursive call
		{
			if (tempHole == 0 && tempSide == s) //We want to take into account the instance where we get another turn
			{
				minimax(height, c, s, currHole, currValue, ac);
				valDif = valAfterSow + currValue; //ValDif is diffrence because we're seeing how much they're choice hurt us
				if (valDif > value)
				{
					bestHole = i;
					value = valDif;
				}
			}
			else
			{
				minimax(height + 1, c, NORTH, currHole, currValue, ac);
				valDif = valAfterSow - currValue; //ValDif is diffrence because we're seeing how much they're choice hurt us
				if (valDif > value)
				{
					bestHole = i; 
					value = valDif;
				}
			}
		}

	}
	return;

}




int SmartPlayer::chooseMove(const Board& b, Side s) const //implementation of SmartPlayer's chooseMove, which returns the hole we will be using
{
	if (b.beansInPlay(SOUTH) == 0 || b.beansInPlay(NORTH) == 0)
	{
		return -1;
	}
	int hole;
	for (int i = 1; i <= b.holes(); i++) //by default, we set hole to random valid hole
	{
		if (b.beans(s, i) != 0)
		{
			hole = i;
			break;
		}
	}
	int value = -9999999;
	AlarmClock ac(4900);  // will time out after 4.9 seconds
	minimax(0, b, s, hole, value, ac); //call minimax helper function to do the work
	return hole;
	
}

SmartPlayer::~SmartPlayer() //destructor for Smart Player
{
}


