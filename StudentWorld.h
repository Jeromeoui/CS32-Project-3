#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Board.h"
#include <string>
#include "Actor.h"
#include <list>
#include <iostream>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetPath);
	~StudentWorld();
	virtual int init();
	virtual int move();
	virtual void cleanUp();
	bool isEmptySpace(int i, int j);
	bool isDirectional(int i, int j);
	int getDirectional(int i, int j);
	PlayerAvatar* getYoshiPointer() const;
	PlayerAvatar* getPeachPointer() const;

	int getBank() const;
	void setBank(int set);

	void bowserdropping(int x, int y);
	void addvortex(int x, int y, int direction);
	void vortexhit(int x, int y, Vortex* vortex);
	void teleportbaddie(Actor* baddie);

	/*Actor* getList();
	Actor* getListEnd();*/

	std::list<Actor*> getsList();

private:
	std::list<Actor*> m_actorlist;
	Board bd;
	PlayerAvatar* m_yoshipointer;
	PlayerAvatar* m_peachpointer;
	int m_bank;
};

#endif // STUDENTWORLD_H_