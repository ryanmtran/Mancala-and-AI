#include "Board.h"
#include "Game.h"
#include <iostream>

//Constructor implementation for game
Game::Game(const Board& b, Player* south, Player* north): m_board(b), m_south(south), m_north(north), m_turn(SOUTH) //implementation of game constructor and initialization of meber variables
{
}

void Game::status(bool& over, bool& hasWinner, Side& winner) const //implementation that figures out the current status of the game
{
	int countNorth = 0; //we will track the number of 0's we find for north
	int countSouth = 0; //we will track the number of 0's we find for south
	for (int i = 1; i <= m_board.holes(); i++) //iterates through board to see if we still have moves
	{
		if (m_board.beans(NORTH, i) == 0) //we are checking for the entirty of the North side for 0 bead holes
		{
			countNorth++;
		}
	}
	for (int i = 1; i <= m_board.holes(); i++) //iterates to see if we still have moves
	{
			if (m_board.beans(SOUTH, i) == 0) // we are checking for the entirty of the North side for 0 bead holes
			{
				countSouth++;
			}
	}
	if (countNorth != m_board.holes() && countSouth != m_board.holes()) //if both sides are not empty, then it is not over
	{
		over = false;
	}
	else
	{
		over = true;
		if (m_board.beans(SOUTH, 0) + m_board.beansInPlay(SOUTH) == m_board.beans(NORTH, 0) + m_board.beansInPlay(NORTH) ) //we add the remaining items on the board to the pots and compare them
		{
			hasWinner = false; //if equal there is no winner
		}
		else
		{
			hasWinner = true;
			if (m_board.beans(SOUTH, 0) + m_board.beansInPlay(SOUTH) > m_board.beans(NORTH, 0) + m_board.beansInPlay(NORTH))
			{
				winner = SOUTH; //if south has more winner is south
			}
			else
			{
				winner = NORTH; //if north has more winner is north
			}
		}
	}
}

void Game::display() const //implementation of display function that handles interface of showing current state of game
{
	std::cout << "               ";
	for (int i = 0;  i != m_board.holes()/2; i++)
	{
		std::cout << "   ";
	}
	std::cout << "NORTH" << endl;
	std::cout << "\t\t";
	for (int j = 1; j <= m_board.holes(); j++)
	{
		std::cout << j << "   ";
	}
	std::cout << endl;
	std::cout << "                ";
	for (int j = 1; j <= m_board.holes(); j++) //iterates to see if we still have moves
	{
		std::cout << "---";
	}
	for (int j = 3; j <= m_board.holes(); j++) //iterates to see if we still have moves
	{
		std::cout << "-";
	}

	std::cout <<endl << "\t\t";
	;

	for (int j = 1; j <= m_board.holes(); j++) //iterates to see if we still have moves
	{
		std::cout << m_board.beans(NORTH, j) << "   ";
	}
	std::cout << endl << m_north->name() << "'s Pot " << m_board.beans(NORTH, 0);
	
	std::cout << "   ";
	for (int i = 0; i < m_board.holes(); i++)
	{
		std::cout << "    ";
	}

	std::cout<< m_south->name() <<"'s Pot " << m_board.beans(SOUTH, 0)<< endl << "\t\t";
	;

	for (int i = 1; i <= m_board.holes(); i++) //iterates through board to see if we still have moves
	{
		std::cout << m_board.beans(SOUTH, i) << "   ";
	}
	
	std::cout << endl;

	std::cout << "                ";
	for (int j = 1; j <= m_board.holes(); j++) //iterates to see if we still have moves
	{
		std::cout << "---";
	}
	for (int j = 3; j <= m_board.holes(); j++) //iterates to see if we still have moves
	{
		std::cout << "-";
	}

	std::cout << endl << "\t\t";
	for (int j = 1; j <= m_board.holes(); j++)
	{
		std::cout << j << "   ";
	}
	std::cout << endl << "               ";
	for (int i = 0; i != m_board.holes() / 2; i++)
	{
		std::cout << "   " ;
	}
	std::cout << "SOUTH" << endl << endl <<endl;
}

bool Game::move() //implemenetation of move function in class game
{
	bool t_over = false; //initailized to see if our game is over or not
	bool t_hasWinner;
	Side t_winner;
	status(t_over, t_hasWinner, t_winner); //call status to find out if the game is over
	if (t_over == true) //we don't want to move if the game has been completed, so we return false if so
	{
		return false;
	}
	else
	{
		if (m_turn ==SOUTH && !m_south->isInteractive()) //condition for when a player is interactive, we want to stop every now and then so that we can keep up with the score
		{
			cout << "Press enter to continue." << endl;
			cin.ignore(10000, '\n');
		}
		else if (m_turn == NORTH && !m_north->isInteractive())  //condition for when a player is interactive, we want to stop every now and then so that we can keep up with the score
		{
			cout << "Press enter to continue." << endl;
			cin.ignore(10000, '\n');
		}
		int hole_num = 1; //we will use this to track what number we picked to sow from

		if (m_turn == SOUTH) //we will call south's player's choose move if the current turn is south
		{
			hole_num = m_south->chooseMove(m_board, m_turn);
			cout << m_south->name() << " chose hole: " << hole_num << endl;
		}
		else  //we will call north's player's choose move if the current turn is north
		{
			hole_num = m_north->chooseMove(m_board, m_turn);
			cout << m_north->name() << " chose hole: " << hole_num << endl;

		}

		Side endSide; //keep track of what side we end at
		int endHole; //keep track of what hole we end at
		
		m_board.sow(m_turn, hole_num, endSide, endHole);
		if (endHole == 0) //if we ended at a pot
		{
			display();
			move();

		}

		else if (endSide == m_turn && m_board.beans(m_turn, endHole) == 1 && m_turn == SOUTH && m_board.beans(NORTH, endHole) != 0) //capture when we are on the south side
		{
			m_board.moveToPot(SOUTH, endHole, SOUTH);//transfer south's hole to south's pot
			m_board.moveToPot(NORTH, endHole, SOUTH); //transfer north's hole to south's pot
			m_turn = NORTH; //turn changes
		}
		else if (endSide == m_turn && m_board.beans(m_turn, endHole) == 1 && m_turn == NORTH && m_board.beans(SOUTH, endHole) != 0) //capture when we are on the north side
		{
			m_board.moveToPot(SOUTH, endHole, NORTH);//transfer south's hole to south's pot
			m_board.moveToPot(NORTH, endHole, NORTH); //transfer north's hole to south's pot
			m_turn = SOUTH; //turn changes
		}
		else if (m_turn == SOUTH) //we got to a hole that is not special on the south side and we end turn
		{
			m_turn = NORTH;
		}
		else  //we got to a hole that is not special on the north side and we end turn
		{
			m_turn = SOUTH;
		}
		status(t_over, t_hasWinner, t_winner);
		if (t_over == true) //we need to check if after we made a move if our board is empty so we can move everything to the pot
		{
			for (int i = 1; i <= m_board.holes(); i++) //the following code moves all the beans to the pot using iteration
			{
				m_board.moveToPot(SOUTH, i, SOUTH); //moves to south pot
				m_board.moveToPot(NORTH, i, NORTH); //moves to north pot

			}
		}
		return true;
	}
}

void Game::play() //implementation of game class's play function
{
	if ( m_board.beansInPlay(SOUTH) == 0 ||  m_board.beansInPlay(NORTH) == 0) //If either sides has the conditions in which the game is over, we want to sweep into the pot
	{
		for (int i = 1; i <= m_board.holes(); i++)
		{
			m_board.moveToPot(NORTH, i, NORTH); //we want to sweep all of north's beans into north pot
			m_board.moveToPot(SOUTH, i, SOUTH); //we want to sweep all of south's beans into south pot
		}
	}

	bool t_over = false; //initailized to see if our game is over or not
	bool t_hasWinner;
	Side t_winner;
	display(); //iniial display that shows what the board looks like before any moves
	status(t_over, t_hasWinner, t_winner); //call status to find out if the game is over
	while (t_over != true)
	{
		move();
		display();
		status(t_over, t_hasWinner, t_winner); //call status to find out if the game is over
	}

	if (!t_hasWinner) //if the game was a tie
	{
		cout << "There was no winner";
	}
	else //print out the winner 
	{
		if (t_winner == SOUTH) 
		{
			cout << "The Winner Was " << m_south->name();
		}
		else
		{
			cout << "The Winner Was " << m_north->name();

		}
	}
}

int Game::beans(Side s, int hole) const //implementation of function that returns the number of beans we have in a hole
{
	if (hole < 0 || hole > m_board.holes()) //invalid cases
	{
		return -1;
	}
	else
	{
		return m_board.beans(s, hole); //we get back the number of beans back
	}
}


