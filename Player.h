
#include <string>
#include "Board.h"
#include <iostream>
#include <stdio.h>
#include <chrono>
#include <future>
#include <atomic>

#ifndef PLAYER_H
#define PLAYER_H

//The following code is the alarm clock functionality is given by Smallberg
//==========================================================================
// AlarmClock ac(numMilliseconds);  // Set an alarm clock that will time out
//                                  // after the indicated number of ms
// if (ac.timedOut())  // Will be false until the alarm clock times out; after
//                     // that, always returns true.
//==========================================================================

class AlarmClock
{
public:
	AlarmClock(int ms)
	{
		m_timedOut = false;
		m_isRunning = true;
		m_alarmClockFuture = std::async([=]() {
			for (int k = 0; k < ms && m_isRunning; k++)
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
			if (m_isRunning)
				m_timedOut = true;
		});
	}

	~AlarmClock()
	{
		m_isRunning = false;
		m_alarmClockFuture.get();
	}

	bool timedOut() const
	{
		return m_timedOut;
	}

	AlarmClock(const AlarmClock&) = delete;
	AlarmClock& operator=(const AlarmClock&) = delete;
private:
	std::atomic<bool> m_isRunning;
	std::atomic<bool> m_timedOut;
	std::future<void> m_alarmClockFuture;
};

class Player
{
public:
	Player(std::string name);
	//Create a Player with the indicated name.
	std::string name() const;
	//Return the name of the player.
	virtual bool isInteractive() const;
	//Return false if the player is a computer player.Return true if the player is human.Most kinds of players will be computer players.
	virtual int chooseMove(const Board& b, Side s) const = 0;
	//Every concrete class derived from this class must implement this function so that if the player were to be playing side s and had to make a move given board b, the function returns the move the player would choose.If no move is possible, return −1.
	virtual ~Player();
	//Since this class is designed as a base class, it should have a virtual destructor.
private:
	std::string m_name; //keeps track of name of player


};



class HumanPlayer : public Player //prompts individual for input
{
public:
	HumanPlayer(std::string name); //constructor for human player
	virtual bool isInteractive() const;
	//Return false if the player is a computer player.Return true if the player is human.Most kinds of players will be computer players.
	virtual int chooseMove(const Board& b, Side s) const;
	//Every concrete class derived from this class must implement this function so that if the player were to be playing side s and had to make a move given board b, the function returns the move the player would choose.If no move is possible, return ?1.
	virtual ~HumanPlayer();
	//Since this class is designed as a base class, it should have a virtual destructor.
};


class BadPlayer: public Player
{
public:
	BadPlayer(std::string name); //constructor for bad Player
	virtual int chooseMove(const Board& b, Side s) const;
	//Every concrete class derived from this class must implement this function so that if the player were to be playing side s and had to make a move given board b, the function returns the move the player would choose.If no move is possible, return ?1.
	virtual ~BadPlayer();
	//Since this class is designed as a base class, it should have a virtual destructor.
};


class SmartPlayer :public Player
{
public:
	SmartPlayer(std::string name); //constructor for bad Player
	virtual int chooseMove(const Board& b, Side s) const;
	//Every concrete class derived from this class must implement this function so that if the player were to be playing side s and had to make a move given board b, the function returns the move the player would choose.If no move is possible, return ?1.
	virtual ~SmartPlayer();
	//Since this class is designed as a base class, it should have a virtual destructor.

private:
	//minimax function is a helper function for smartPlayer's chooseMove, which determines the best hole to pick
	void minimax(int height, const Board &b, Side s, int &bestHole, int &value, AlarmClock& ac) const;


};


#endif