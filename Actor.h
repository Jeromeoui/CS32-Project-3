#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
//BASE CLASS
class Actor : public GraphObject
{
public:
	Actor(int ImageID, int startX, int startY, int dir, int depth, double size, StudentWorld* worldlinker);
	virtual ~Actor();

	virtual void doSomething() = 0;
	//INACTIVE OR ACTIVE
	virtual bool getStatus() const;
	virtual void setStatus(bool activity);

	virtual bool isImpactable() const;
	virtual void setImpactable(bool impact);

	bool isImpacted() const;
	void setImpacted(bool impact);

	virtual bool isSquare() const;
	virtual void setSquare(bool square);
	//virtual bool amIaMonster() const;

	bool isVortex() const;
	void setisVortex(bool vortex);

	bool isDirectionalSq() const;
	void setisDirectional(bool directional);

	//POINTER TO STUDENTWORLD
	StudentWorld* getWorld() const;


private:
	bool m_walkingstate;
	StudentWorld* m_getworld;

	bool m_impactable;
	bool m_impacted;
	bool m_square;
	bool m_isvortex;
	bool m_isdirectional;
};

class Agents :public Actor
{
public:
	Agents(int ImageID, int startX, int startY, StudentWorld* worldlinker);
	virtual ~Agents();

	//TICKS TO MOVE
	int getTickstoMove() const;
	void setTickstoMove(int ticks);
	

	//DIRECTION
	void setWalkingDirection(int wd);
	int getWalkingDirection() const;

	//OUTOFBOUNDS
	bool isOutofBounds(int i, int j) const;

	//FORK
	bool isAtFork() const;

	//TURNS
	bool isBlocked();
	void move();
	void navigateTurns();

	virtual void doSomething();
private:
	int m_ticks_to_move;
	int m_walkingdirection;
};

//CHARACTERS
class PlayerAvatar : public Agents
{
public:
	PlayerAvatar(int ImageID, int startX, int startY, int playernumber, StudentWorld* worldlinker);
	virtual ~PlayerAvatar();


	//COINS
	int getCoins() const;
	void setCoins(int newcoins);
	
	//STARS
	int getStars() const;
	void setStars(int newstars);
	
	//VORTEX
	bool hasVortex() const;
	void setVortex(bool status);
	
	virtual void doSomething();

	//DIEROLL
	int getDieRoll() const;
	void setDieRoll(int dieroll);

	//DIRECTION
	/*void setWalkingDirection(int wd);
	int getWalkingDirection() const;*/

	//OUTOFBOUNDS
	//bool isOutofBounds(int i, int j) const;

	//FORK
	//bool isAtFork() const;

	//NEW
	bool isNew() const;
	void setNew(bool setnew);
	bool isEventNew() const;
	void setEventNew(bool setnew);
	bool isStarNew() const;
	void setStarNew(bool setnew);
	bool isBankNew() const;
	void setBankNew(bool setnew);
	bool isBowserNew() const;
	void setBowserNew(bool setnew);
	bool isBooNew() const;
	void setBooNew(bool setnew);

	bool startisnotafork() const;
	void setstartisnotafork(bool setnew);

	bool hasteleported() const;
	void sethasteleported(bool tele);

private:
	int m_coins;
	int m_stars;
	bool m_vortex;

	//int m_ticks_to_move;
	int m_playernumber;
	int m_dieroll;
	//int m_walkingdirection;

	bool m_new;
	bool m_eventnew;
	bool m_starnew;
	bool m_banknew;
	bool m_bowsernew;
	bool m_boonew;
	bool m_isstart;
	bool m_hasteleported;
};

//BADDIES
class Baddie :public Agents
{
public:
	Baddie(int ImageID, int startX, int startY, StudentWorld* worldlinker);
	virtual ~Baddie();

	//virtual bool amIaMonster();
	int getPause() const;
	void setPause(int pause);

	/*int getTicks() const;
	void setTicks(int ticks);*/
	
	bool isInContact(PlayerAvatar* player) const;
	virtual void actOnPlayer(PlayerAvatar* player) = 0;
	
	void handleforks();
	void handleimpacted();

	//bool isImpacted() const;
	//void setImpacted(bool impact);

private:
	int m_pausecounter;
	int m_traveldist;
	/*int m_ticks_to_move;*/
};

class Bowser : public Baddie
{
public:
	Bowser(int startX, int startY, StudentWorld* worldlinker);
	virtual ~Bowser();

	virtual void actOnPlayer(PlayerAvatar* player);
	virtual void doSomething();
	
private:

};

class Boo : public Baddie
{
public:
	Boo(int startX, int startY, StudentWorld* worldlinker);
	virtual ~Boo();

	void swapInventory(PlayerAvatar* player1, PlayerAvatar* player2);
	virtual void actOnPlayer(PlayerAvatar* player);
	virtual void doSomething();
	
private:
	
};

//SQUARES
class Square : public Actor
{
public:
	Square(int ImageID, int startX, int startY, int dir, StudentWorld* worldlinker);
	virtual ~Square();

private:

};

class CoinSquare : public Square
{
public:
	CoinSquare(int ImageID, int startX, int startY, StudentWorld* worldlinker);
	~CoinSquare();

	virtual void editInventory(PlayerAvatar& character) = 0;
	virtual void doSomething();

	//ALIVE FLAG
	bool isAlive();
	void setDead();

private:
	bool m_alive;
};

//COINSQUARES
class BlueCoinSquare : public CoinSquare
{
public:
	BlueCoinSquare(int startX, int startY, StudentWorld* worldlinker);
	~BlueCoinSquare();

	virtual void editInventory(PlayerAvatar& character);
	virtual void doSomething();

private:
};

class RedCoinSquare : public CoinSquare
{
public:
	RedCoinSquare(int startX, int startY, StudentWorld* worldlinker);
	~RedCoinSquare();

	virtual void editInventory(PlayerAvatar& character);
	virtual void doSomething();

private:
};

class StarSquare : public CoinSquare
{
public:
	StarSquare(int startX, int startY, StudentWorld* worldlinker);
	~StarSquare();

	virtual void editInventory(PlayerAvatar& character);
	virtual void doSomething();
	
private:
};

class BankSquare : public Square
{
public:
	BankSquare(int startX, int startY, StudentWorld* worldlinker);
	~BankSquare();

	virtual void doSomething();
	void withdraw(PlayerAvatar& character);
	void deposit(PlayerAvatar& character);

private:
};

class DroppingSquare : public CoinSquare
{
public:
	DroppingSquare(int startX, int startY, StudentWorld* worldlinker);
	~DroppingSquare();

	virtual void editInventory(PlayerAvatar& character);
	virtual void doSomething();

private:
};

//EVENT SQUARES
class EventSquare : public Square
{
public:
	EventSquare(int startX, int startY, StudentWorld* worldlinker);
	~EventSquare();

	virtual void doSomething(); 

	void doEvent(PlayerAvatar* player1, PlayerAvatar* player2);
	void randomTeleport(PlayerAvatar* player);
	void swapPositions(PlayerAvatar* player1, PlayerAvatar* player2);
	void giveVortex(PlayerAvatar* player);


private:
};

//DIRECTIONSQUARES
class DirectionSquare : public Square
{
public:
	DirectionSquare(int startX, int startY, int dir, StudentWorld* worldlinker);
	~DirectionSquare();

	virtual void doSomething();

private:
};

//PROJECTILES
class Vortex :public Agents
{
public:
	Vortex(int startX, int startY, int dir, StudentWorld* worldlinker);
	~Vortex();

	bool overlaps(Actor* baddie);
	virtual void doSomething();
private:

};


#endif // ACTOR_H_