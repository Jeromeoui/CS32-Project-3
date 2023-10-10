#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

//ACTOR
Actor::Actor(int ImageID, int startX, int startY, int dir, int depth, double size, StudentWorld* worldlinker)
	: GraphObject(ImageID, startX* SPRITE_WIDTH, startY* SPRITE_HEIGHT, dir, depth, size), m_getworld(worldlinker)
{
	m_walkingstate = false;
	m_impactable = false;
	m_impacted = false;
	m_square = false;
	m_isvortex = false;
	m_isdirectional = false;
}

Actor::~Actor() {}

bool Actor::getStatus() const {
	return m_walkingstate;
}

void Actor::setStatus(bool activity) {
	m_walkingstate = activity;
}

bool Actor::isImpactable() const {
	return m_impactable;
}

void Actor::setImpactable(bool impact) {
	m_impactable = impact;
}

bool Actor::isSquare() const {
	return m_square;
}

void Actor::setSquare(bool square) {
	m_square = square;
}

bool Actor::isImpacted() const {
	return m_impacted;
}

void Actor::setImpacted(bool impact) {
	m_impacted = impact;
}

bool Actor::isVortex() const {
	return m_isvortex;
}

void Actor::setisVortex(bool vortex) {
	m_isvortex = vortex;
}

bool Actor::isDirectionalSq() const {
	return m_isdirectional;
}

void Actor::setisDirectional(bool directional) {
	m_isdirectional = directional;
}

StudentWorld* Actor::getWorld() const {
	return m_getworld;
}

//AGENTS
Agents::Agents(int ImageID, int startX, int startY, StudentWorld* worldlinker):
	Actor(ImageID, startX, startY, right, 0, 1, worldlinker), m_ticks_to_move(0), m_walkingdirection(right) {}
Agents:: ~Agents() {}

//TICKS TO MOVE
int Agents::getTickstoMove() const {
	return m_ticks_to_move;
}

void Agents::setTickstoMove(int ticks) {
	m_ticks_to_move = ticks;
}

void Agents::doSomething() {}

//DIRECTION
void Agents::setWalkingDirection(int wd) {
	m_walkingdirection = wd;
}

int Agents::getWalkingDirection() const {
	return m_walkingdirection;
}

//OUTOFBOUNDS
bool Agents::isOutofBounds(int i, int j) const {
	return (i < 0 || i > 255 || j < 0 || j > 255);
}

//FORK
bool Agents::isAtFork() const {
	int countpathways = 0;
	if (getWalkingDirection() == right && getX()%16==0 && getY()%16==0 && !(getWorld()->isDirectional(getX(), getY()) ) ) {
		if (!isOutofBounds(getX() + 16, getY()) && !(getWorld()->isEmptySpace(getX() + 16, getY()))) countpathways++; //CHECK RIGHT
		if (!isOutofBounds(getX(), getY() + 16) && !(getWorld()->isEmptySpace(getX(), getY() + 16))) countpathways++; //CHECK UP
		if (!isOutofBounds(getX(), getY() - 1) && !(getWorld()->isEmptySpace(getX(), getY() - 1))) countpathways++; //CHECK DOWN
	}
	else if (getWalkingDirection() == left && getX() % 16 == 0 && getY() % 16 == 0 && !(getWorld()->isDirectional(getX(), getY())) ) {
		if (!isOutofBounds(getX() - 1, getY()) && !(getWorld()->isEmptySpace(getX() - 1, getY()))) countpathways++; //CHECK LEFT
		if (!isOutofBounds(getX(), getY() + 16) && !(getWorld()->isEmptySpace(getX(), getY() + 16))) countpathways++; //CHECK UP
		if (!isOutofBounds(getX(), getY() - 1) && !(getWorld()->isEmptySpace(getX(), getY() - 1))) countpathways++; //CHECK DOWN
	}
	else if (getWalkingDirection() == up && getX() % 16 == 0 && getY() % 16 == 0 && !(getWorld()->isDirectional(getX(), getY())) ) {
		if (!isOutofBounds(getX(), getY() + 16) && !(getWorld()->isEmptySpace(getX(), getY() + 16))) countpathways++; //CHECK UP
		if (!isOutofBounds(getX() - 1, getY()) && !(getWorld()->isEmptySpace(getX() - 1, getY()))) countpathways++; //CHECK LEFT
		if (!isOutofBounds(getX() + 16, getY()) && !(getWorld()->isEmptySpace(getX() + 16, getY()))) countpathways++; //CHECK RIGHT
	}
	else if (getWalkingDirection() == down && getX() % 16 == 0 && getY() % 16 == 0 && !(getWorld()->isDirectional(getX(), getY())) ) {
		if (!isOutofBounds(getX(), getY() - 1) && !(getWorld()->isEmptySpace(getX(), getY() - 1))) countpathways++; //CHECK DOWN
		if (!isOutofBounds(getX() - 1, getY()) && !(getWorld()->isEmptySpace(getX() - 1, getY()))) countpathways++; //CHECK LEFT
		if (!isOutofBounds(getX() + 16, getY()) && !(getWorld()->isEmptySpace(getX() + 16, getY()))) countpathways++; //CHECK RIGHT
	}
	return (countpathways >= 2);
}

//NAVIGATE TURNS
bool Agents::isBlocked() {
	int xnew, ynew;
	if (getWalkingDirection() == right) {
		getPositionInThisDirection(getWalkingDirection(), 16, xnew, ynew);
		if (getWorld()->isEmptySpace(xnew, ynew) || isOutofBounds(xnew, ynew)) return true;
	}
	else if (getWalkingDirection() == left){
		getPositionInThisDirection(getWalkingDirection(), 1, xnew, ynew);
		if (getWorld()->isEmptySpace(xnew, ynew) || isOutofBounds(xnew, ynew)) return true;
	}
	else if (getWalkingDirection() == up){
		getPositionInThisDirection(getWalkingDirection(), 16, xnew, ynew);
		if (getWorld()->isEmptySpace(xnew, ynew) || isOutofBounds(xnew, ynew)) return true;
	}
	else if (getWalkingDirection() == down) {
		getPositionInThisDirection(getWalkingDirection(), 1, xnew, ynew);
		if (getWorld()->isEmptySpace(xnew, ynew) || isOutofBounds(xnew, ynew)) return true;
	}
	return false;
}

void Agents::move() {
	if (getWalkingDirection() == right) {
		moveTo(getX() + 2, getY());
	}
	else if (getWalkingDirection() == left) {
		moveTo(getX() - 2, getY());
	}
	else if (getWalkingDirection() == up) {
		moveTo(getX(), getY() + 2);
	}
	else if (getWalkingDirection() == down) {
		moveTo(getX(), getY() - 2);
	}
}

void Agents::navigateTurns() { //change direction
	int xnew, ynew;
	if (getWalkingDirection() == right) {
		getPositionInThisDirection(getWalkingDirection(), 16, xnew, ynew);
		if (getWorld()->isEmptySpace(xnew, ynew) || isOutofBounds(xnew, ynew)) {
			getPositionInThisDirection(up, 16, xnew, ynew);
			if (!(getWorld()->isEmptySpace(xnew, ynew)) && !isOutofBounds(xnew, ynew)) {
				setWalkingDirection(up);
				setDirection(right);
				return;
			}
			else {
				setWalkingDirection(down);
				setDirection(right);
				return;
			}
		}
		else return;
		
	}

	else if (getWalkingDirection() == left) {
		getPositionInThisDirection(getWalkingDirection(), 1, xnew, ynew);
		if (getWorld()->isEmptySpace(xnew, ynew) || isOutofBounds(xnew, ynew)) {
			getPositionInThisDirection(up, 16, xnew, ynew);
			if (!(getWorld()->isEmptySpace(xnew, ynew)) && !isOutofBounds(xnew, ynew)) {
				setWalkingDirection(up);
				setDirection(right);
				return;
			}
			else {
				setWalkingDirection(down);
				setDirection(right);
				return;
			}
		}
		else return;
	}

	else if (getWalkingDirection() == up) {
		getPositionInThisDirection(getWalkingDirection(), 16, xnew, ynew);
		if (getWorld()->isEmptySpace(xnew, ynew) || isOutofBounds(xnew, ynew)) {
			getPositionInThisDirection(right, 16, xnew, ynew);
			if (!(getWorld()->isEmptySpace(xnew, ynew)) && !isOutofBounds(xnew, ynew)) {
				setWalkingDirection(right);
				setDirection(right);
				return;
			}
			else {
				setWalkingDirection(left);
				setDirection(left);
				return;
			}
		}
		else return;
	}

	else if (getWalkingDirection() == down) {
		getPositionInThisDirection(getWalkingDirection(), 1, xnew, ynew);
		if (getWorld()->isEmptySpace(xnew, ynew) || isOutofBounds(xnew, ynew)) {
			getPositionInThisDirection(right, 16, xnew, ynew);
			if (!(getWorld()->isEmptySpace(xnew, ynew)) && !isOutofBounds(xnew, ynew)) {
				setWalkingDirection(right);
				setDirection(right);
				return;
			}
			else {
				setWalkingDirection(left);
				setDirection(left);
				return;
			}
		}
		else return;
	}
}

//PLAYERAVATAR

PlayerAvatar::PlayerAvatar(int ImageID, int startX, int startY, int playernumber, StudentWorld* worldlinker)
	: Agents(ImageID, startX, startY, worldlinker), m_playernumber(playernumber), m_dieroll(0)
{
	m_coins = 0;
	m_stars = 0;
	m_vortex = false;

	m_new = true;
	m_eventnew = true;
	m_starnew = true;
	m_banknew = true;
	m_bowsernew = true;
	m_boonew = true;
	m_isstart = true;
	m_hasteleported = false;
}
PlayerAvatar:: ~PlayerAvatar() {}

//PLAYER'S COINS
int PlayerAvatar::getCoins() const {
	return m_coins;
}
void PlayerAvatar::setCoins(int newcoins) {
	m_coins = newcoins;
}
//PLAYER'S STARS
int PlayerAvatar::getStars() const {
	return m_stars;
}
void PlayerAvatar::setStars(int newstars) {
	m_stars = newstars;
}
//PLAYER'S VORTEX
bool PlayerAvatar::hasVortex() const {
	return m_vortex;
}
void PlayerAvatar::setVortex(bool status) {
	m_vortex = status;
}

void PlayerAvatar::setDieRoll(int dieroll) {
	m_dieroll = dieroll;
}

int PlayerAvatar::getDieRoll() const {
	return m_dieroll;
};

//New

bool PlayerAvatar::isNew() const {
	return m_new;
};

void PlayerAvatar::setNew(bool setnew) {
	m_new = setnew;
}

bool PlayerAvatar::isEventNew() const {
	return m_eventnew;
}
void PlayerAvatar::setEventNew(bool setnew) {
	m_eventnew = setnew;
}

bool PlayerAvatar::isStarNew() const {
	return m_starnew;
}

void PlayerAvatar::setStarNew(bool setnew) {
	m_starnew = setnew;
}

bool PlayerAvatar::isBankNew() const {
	return m_banknew;
}

void PlayerAvatar::setBankNew(bool setnew) {
	m_banknew = setnew;
}

bool PlayerAvatar::isBowserNew() const {
	return m_bowsernew;
}

void PlayerAvatar::setBowserNew(bool setnew) {
	m_bowsernew = setnew;
}

bool PlayerAvatar::isBooNew() const {
	return m_boonew;
}

void PlayerAvatar::setBooNew(bool setnew) {
	m_boonew = setnew;
}

bool PlayerAvatar::startisnotafork() const {
	return m_isstart;
}

void PlayerAvatar::setstartisnotafork(bool setnew) {
	m_isstart = setnew;
}

bool PlayerAvatar::hasteleported() const {
	return m_hasteleported;
}

void PlayerAvatar::sethasteleported(bool tele) {
	m_hasteleported = tele;
}

//DOSOMETHING

void PlayerAvatar::doSomething() {
	
	if (!getStatus()) {
		//1a
		if (isBlocked()) { //Reorientate
			navigateTurns();
		}
		int move = getWorld()->getAction(m_playernumber);
		if (move == ACTION_ROLL) {
			int die_roll = randInt(1, 10);
			setDieRoll(die_roll); // verify
			setTickstoMove(die_roll * 8);

			setNew(true);
			setEventNew(true);
			setStatus(true);
			setStarNew(true);
			setBankNew(true);
			setBowserNew(true);
			setBooNew(true);
			
		}
		else if (move == ACTION_FIRE) {//VORTEX
			if (hasVortex()) {
				getWorld()->addvortex(getX(), getY(), getWalkingDirection());
				setVortex(false);
			}
			else return;
		}
		else return;
	}

	if (getStatus()) {
			if (isAtFork() && !startisnotafork()) { //Fork
			int move = getWorld()->getAction(m_playernumber);
			if (move == ACTION_NONE) return;
			if (getWalkingDirection() == left && move == ACTION_RIGHT
				|| getWalkingDirection() == right && move == ACTION_LEFT
				|| getWalkingDirection() == up && move == ACTION_DOWN
				|| getWalkingDirection() == down && move == ACTION_UP) return;
			if (move == ACTION_LEFT) {
				if (!(getWorld()->isEmptySpace(getX() - 1, getY())) && !isOutofBounds(getX() - 1, getY())) {
					setWalkingDirection(left);
					setDirection(left);
				}
				else return;
			}
			else if (move == ACTION_RIGHT) {
				if (!(getWorld()->isEmptySpace(getX() + 16, getY())) && !isOutofBounds(getX() + 16, getY())) {
					setWalkingDirection(right);
					setDirection(right);
				}
				else return;
			}
			else if (move == ACTION_UP) {
				if (!(getWorld()->isEmptySpace(getX(), getY() + 16)) && !isOutofBounds(getX(), getY() + 16)) {
					setWalkingDirection(up);
					setDirection(right);
				}
				else return;
			}
			else if (move == ACTION_DOWN) {
				if (!(getWorld()->isEmptySpace(getX(), getY() - 1)) && !isOutofBounds(getX(), getY() - 1)) {
					setWalkingDirection(down);
					setDirection(right);
				}
				else return;
			}
			else return;
		}
		else if (isBlocked()) { //Turn
			navigateTurns();
		}
		move();
		setTickstoMove(getTickstoMove() - 1);
		setDieRoll(ceil(getTickstoMove() / 8.00)); //verify
		setstartisnotafork(false);
		if (getTickstoMove() == 0) {
			setStatus(false);
		}
		return;
	}
	
}

//////////////BADDIES
Baddie::Baddie(int ImageID, int startX, int startY, StudentWorld* worldlinker)
	: Agents(ImageID, startX, startY, worldlinker), m_pausecounter(180), m_traveldist(0) {
	setImpactable(true);
}

Baddie::~Baddie() {}

int Baddie::getPause() const {
	return m_pausecounter;
}
void Baddie::setPause(int pause) {
	m_pausecounter = pause;
}



bool Baddie::isInContact(PlayerAvatar* player) const {
	return (player->getX() == getX() && player->getY() == getY() && player->getStatus());
}

void Baddie::handleforks() {
	int move = randInt(1, 4);//left == 1, right == 2, up == 3, down == 4
	while ((move == 1 && (getWorld()->isEmptySpace(getX() - 1, getY()) || isOutofBounds(getX() - 1, getY())))
		|| (move == 2 && (getWorld()->isEmptySpace(getX() + 16, getY()) || isOutofBounds(getX() + 16, getY())))
		|| (move == 3 && (getWorld()->isEmptySpace(getX(), getY() + 16) || isOutofBounds(getX(), getY() + 16)))
		|| (move == 4 && (getWorld()->isEmptySpace(getX(), getY() - 1) || isOutofBounds(getX() - 1, getY() - 1)))
		)
	{
		move = randInt(1, 4);
	}
	switch (move) {
	case 1:
		setWalkingDirection(left);
		setDirection(left);
		break;
	case 2:
		setWalkingDirection(right);
		setDirection(right);
		break;
	case 3:
		setWalkingDirection(up);
		setDirection(right);
		break;
	case 4:
		setWalkingDirection(down);
		setDirection(right);
		break;
	}
}

void Baddie::handleimpacted() {
	setWalkingDirection(right);
	setDirection(right);
	setStatus(false);
	setPause(180);
	setImpacted(false);
}

//bool Baddie::isImpacted() const {
//	return m_impacted;
//}
//
//void Baddie::setImpacted(bool impact) {
//	m_impacted = impact;
//}

//////BOWSER
Bowser::Bowser(int startX, int startY, StudentWorld* worldlinker)
	: Baddie(IID_BOWSER, startX, startY, worldlinker) {}

Bowser::~Bowser() {
}



void Bowser::actOnPlayer(PlayerAvatar* player) {
	if (isInContact(player) && player->isBowserNew()) {
		int chance = randInt(1, 2);
		switch (chance) {
		case 1:
			break;
		case 2:
			player->setCoins(0);
			player->setStars(0);
			getWorld()->playSound(SOUND_BOWSER_ACTIVATE);
		}
		player->setBowserNew(false);
	}
}

void Bowser::doSomething() {
	if (!getStatus()) {
		actOnPlayer(getWorld()->getPeachPointer());
		actOnPlayer(getWorld()->getYoshiPointer());
		setPause(getPause() - 1);
		if (getPause() == 0) {
			int squares_to_move = randInt(1, 10);
			setTickstoMove(squares_to_move * 8);
			//pick new random direction
			//update direction and sprite
			if (isBlocked()) { //Reorientate
				navigateTurns();
			}
			setStatus(true);
		}
	}
	if (getStatus()) {
		//forks
		if (isAtFork()) { //Fork 
			handleforks();
		}
		//turns
		else if (isBlocked()) {
			navigateTurns();
		}
		move();
		setTickstoMove(getTickstoMove() - 1);
		if (getTickstoMove() == 0) {
			setStatus(false);
			setPause(180);
			int chance = randInt(1, 4);
			switch (chance) {
			case 1:
			case 2:
			case 3:
				break;
			case 4:
				getWorld()->bowserdropping(getX(), getY());
				return; //droppings
			}
		}
	}
	if (isImpacted()) {
		handleimpacted();
	}
}

///////BOO

Boo::Boo(int startX, int startY, StudentWorld* worldlinker)
	: Baddie(IID_BOO, startX, startY, worldlinker) {}

Boo::~Boo() {}

void Boo::swapInventory(PlayerAvatar* player1, PlayerAvatar* player2) {
	int chance = randInt(1, 2);
	int tempcoins = 0;
	int tempstars = 0;
	switch (chance) {
	case 1:
		tempcoins = player1->getCoins();
		player1->setCoins(player2->getCoins());
		player2->setCoins(tempcoins);
		break;
	case 2:
		tempstars = player1->getStars();
		player1->setStars(player2->getStars());
		player2->setStars(tempstars);
	}
}

void Boo::actOnPlayer(PlayerAvatar* player) {
	if (isInContact(player) && player->isBooNew()) {
		int chance = randInt(1, 2);
		int tempcoins = 0;
		int tempstars = 0;
		switch (chance) {
		case 1:
			tempcoins = getWorld()->getPeachPointer()->getCoins();
			getWorld()->getPeachPointer()->setCoins(getWorld()->getYoshiPointer()->getCoins());
			getWorld()->getYoshiPointer()->setCoins(tempcoins);
			break;
		case 2:
			tempstars = getWorld()->getPeachPointer()->getStars();
			getWorld()->getPeachPointer()->setStars(getWorld()->getYoshiPointer()->getStars());
			getWorld()->getYoshiPointer()->setStars(tempstars);
		}
		getWorld()->playSound(SOUND_BOO_ACTIVATE);
		player->setBooNew(false);
	}
}

void Boo::doSomething() {
	if (!getStatus()) {
		actOnPlayer(getWorld()->getPeachPointer());
		actOnPlayer(getWorld()->getYoshiPointer());
		setPause(getPause() - 1);
		if (getPause() == 0) {
			int squares_to_move = randInt(1, 3);
			setTickstoMove(squares_to_move * 8);
			//pick new random direction
			//update direction and sprite
			if (isBlocked()) { //Reorientate
				navigateTurns();
			}
			setStatus(true);
		}
	}
	else if (getStatus()) {
		//forks
		if (isAtFork()) { //Fork 
			handleforks();
		}
		//turns
		else if (isBlocked()) {
			navigateTurns();
		}
		move();
		setTickstoMove(getTickstoMove() - 1);
		if (getTickstoMove() == 0) {
			setStatus(false);
			setPause(180);
		}
	}
	if (isImpacted()) {
		handleimpacted();
	}
}

////////SQUARES
Square::Square(int ImageID, int startX, int startY, int dir, StudentWorld* worldlinker)
	: Actor(ImageID, startX, startY, dir, 1, 1, worldlinker) {
	setSquare(true);
}
Square::~Square() {}

////////COINSQUARES
CoinSquare::CoinSquare(int ImageID, int startX, int startY, StudentWorld* worldlinker)
	: Square(ImageID, startX, startY, right, worldlinker), m_alive(true) {}
CoinSquare::~CoinSquare() {}


//ALIVE FLAG
bool CoinSquare::isAlive() {
	return m_alive;
}
void CoinSquare::setDead() {
	m_alive = false;
}

void CoinSquare::doSomething() {
	if (!isAlive()) return;
	if (getWorld()->getPeachPointer()->getX() == getX() && getWorld()->getPeachPointer()->getY() == getY() && !(getWorld()->getPeachPointer()->getStatus()) && getWorld()->getPeachPointer()->isNew()) {
		editInventory(*(getWorld()->getPeachPointer()));
		getWorld()->getPeachPointer()->setNew(false);
		getWorld()->playSound(SOUND_GIVE_COIN);
	}
	if (getWorld()->getYoshiPointer()->getX() == getX() && getWorld()->getYoshiPointer()->getY() == getY() && !(getWorld()->getYoshiPointer()->getStatus()) && getWorld()->getYoshiPointer()->isNew()) {
		editInventory(*(getWorld()->getYoshiPointer()));
		getWorld()->getYoshiPointer()->setNew(false);
		getWorld()->playSound(SOUND_GIVE_COIN);
	}
}

////////////BLUECOINSQUARES
BlueCoinSquare::BlueCoinSquare(int startX, int startY, StudentWorld* worldlinker)
	: CoinSquare(IID_BLUE_COIN_SQUARE, startX, startY, worldlinker) {}

BlueCoinSquare::~BlueCoinSquare() {}

void BlueCoinSquare::editInventory(PlayerAvatar& character) {
	character.setCoins(character.getCoins() + 3);
}

void BlueCoinSquare::doSomething() {
	if (!isAlive()) return;
	if (getWorld()->getPeachPointer()->getX() == getX() && getWorld()->getPeachPointer()->getY() == getY() && !(getWorld()->getPeachPointer()->getStatus()) && getWorld()->getPeachPointer()->isNew()) {
		editInventory(*(getWorld()->getPeachPointer()));
		getWorld()->getPeachPointer()->setNew(false);
		getWorld()->playSound(SOUND_GIVE_COIN);
	}
	if (getWorld()->getYoshiPointer()->getX() == getX() && getWorld()->getYoshiPointer()->getY() == getY() && !(getWorld()->getYoshiPointer()->getStatus()) && getWorld()->getYoshiPointer()->isNew()) {
		editInventory(*(getWorld()->getYoshiPointer()));
		getWorld()->getYoshiPointer()->setNew(false);
		getWorld()->playSound(SOUND_GIVE_COIN);
	}
}

//////////////REDCOINSQUARES
RedCoinSquare::RedCoinSquare(int startX, int startY, StudentWorld* worldlinker)
	: CoinSquare(IID_RED_COIN_SQUARE, startX, startY, worldlinker) {}
RedCoinSquare::~RedCoinSquare() {}

void RedCoinSquare::editInventory(PlayerAvatar& character) {
	int coincount = character.getCoins();
	if (coincount <= 3) {
		character.setCoins(0);
	}
	else
		character.setCoins(character.getCoins() - 3);
}
void RedCoinSquare::doSomething() {
	if (!isAlive()) return;
	if (getWorld()->getPeachPointer()->getX() == getX() && getWorld()->getPeachPointer()->getY() == getY() && !(getWorld()->getPeachPointer()->getStatus()) && getWorld()->getPeachPointer()->isNew()) {
		editInventory(*(getWorld()->getPeachPointer()));
		getWorld()->getPeachPointer()->setNew(false);
		getWorld()->playSound(SOUND_TAKE_COIN);
	}
	if (getWorld()->getYoshiPointer()->getX() == getX() && getWorld()->getYoshiPointer()->getY() == getY() && !(getWorld()->getYoshiPointer()->getStatus()) && getWorld()->getYoshiPointer()->isNew()) {
		editInventory(*(getWorld()->getYoshiPointer()));
		getWorld()->getYoshiPointer()->setNew(false);
		getWorld()->playSound(SOUND_TAKE_COIN);
	}
}

//////////STARSQUARE (UNFINISHED)
StarSquare::StarSquare(int startX, int startY, StudentWorld* worldlinker)
	: CoinSquare(IID_STAR_SQUARE, startX, startY, worldlinker) {}
StarSquare::~StarSquare() {}

void StarSquare::editInventory(PlayerAvatar& character) {
	int coincount = character.getCoins();
	int starcount = character.getStars();
	if (coincount < 20) {
		return;
	}
	else {
		character.setCoins(coincount - 20);
		character.setStars(starcount + 1);
		getWorld()->playSound(SOUND_GIVE_STAR);
	}
}

void StarSquare::doSomething() {
	if (getWorld()->getPeachPointer()->getX() == getX() && getWorld()->getPeachPointer()->getY() == getY() && 
		getWorld()->getPeachPointer()->getStatus()) {
		editInventory(*(getWorld()->getPeachPointer()));
	}
	else if (getWorld()->getPeachPointer()->getX() == getX() && getWorld()->getPeachPointer()->getY() == getY() &&
		!(getWorld()->getPeachPointer()->getStatus()) && getWorld()->getPeachPointer()->isStarNew()) {
		editInventory(*(getWorld()->getPeachPointer()));
		getWorld()->getPeachPointer()->setStarNew(false);
	}

	if (getWorld()->getYoshiPointer()->getX() == getX() && getWorld()->getYoshiPointer()->getY() == getY() &&
		getWorld()->getYoshiPointer()->getStatus()) {
		editInventory(*(getWorld()->getYoshiPointer()));
	}
	else if (getWorld()->getYoshiPointer()->getX() == getX() && getWorld()->getYoshiPointer()->getY() == getY() &&
		!(getWorld()->getYoshiPointer()->getStatus()) && getWorld()->getPeachPointer()->isStarNew()) {
		editInventory(*(getWorld()->getYoshiPointer()));
		getWorld()->getYoshiPointer()->setStarNew(false);
	}
}

/////BANK SQUARE
BankSquare::BankSquare(int startX, int startY, StudentWorld* worldlinker)
	: Square(IID_BANK_SQUARE, startX, startY, right, worldlinker) {}
BankSquare::~BankSquare() {}

void BankSquare::doSomething() {
	//WALKING
	if (getWorld()->getPeachPointer()->getX() == getX() && getWorld()->getPeachPointer()->getY() == getY() &&
		getWorld()->getPeachPointer()->getStatus()) {
		deposit(*(getWorld()->getPeachPointer()));
	}
	//LANDING
	else if (getWorld()->getPeachPointer()->getX() == getX() && getWorld()->getPeachPointer()->getY() == getY() &&
		!(getWorld()->getPeachPointer()->getStatus()) && (getWorld()->getPeachPointer()->isBankNew())) {
		withdraw(*(getWorld()->getPeachPointer()));
		getWorld()->getPeachPointer()->setBankNew(false);
	}
	//WALKING
	if (getWorld()->getYoshiPointer()->getX() == getX() && getWorld()->getYoshiPointer()->getY() == getY() &&
		getWorld()->getYoshiPointer()->getStatus()) {
		deposit(*(getWorld()->getYoshiPointer()));
	}
	//LANDING
	else if (getWorld()->getYoshiPointer()->getX() == getX() && getWorld()->getYoshiPointer()->getY() == getY() &&
		!(getWorld()->getYoshiPointer()->getStatus()) && (getWorld()->getYoshiPointer()->isBankNew())) {
		withdraw(*(getWorld()->getYoshiPointer()));
		getWorld()->getYoshiPointer()->setBankNew(false);
	}
}

void BankSquare::withdraw(PlayerAvatar& character) {
	character.setCoins(character.getCoins() + getWorld()->getBank());
	getWorld()->setBank(0);
	getWorld()->playSound(SOUND_WITHDRAW_BANK);
}
	
void BankSquare::deposit(PlayerAvatar& character) {
	if (character.getCoins() < 5) {
		getWorld()->setBank(getWorld()->getBank() + character.getCoins());
		character.setCoins(0);
	}
	else {
		getWorld()->setBank(getWorld()->getBank() + 5);
		character.setCoins(character.getCoins() - 5);
	}
	getWorld()->playSound(SOUND_DEPOSIT_BANK);
}


////////DROPPING SQUARE
DroppingSquare::DroppingSquare(int startX, int startY, StudentWorld* worldlinker)
	: CoinSquare(IID_DROPPING_SQUARE, startX, startY, worldlinker) {}
DroppingSquare::~DroppingSquare() {}

void DroppingSquare::editInventory(PlayerAvatar& character) {
	int scenario = randInt(1, 2);
	int coincount = character.getCoins();
	int starcount = character.getStars();
	switch (scenario)
	{
	case 1:
		if (coincount >= 10) {
			character.setCoins(coincount - 10);
		}
		else
			character.setCoins(0);
		break;
	case 2:
		if (starcount >= 1) {
			character.setStars(starcount - 1);
		}
		else
			character.setStars(0);
		break;
	}
}
void DroppingSquare::doSomething() {
	if (getWorld()->getPeachPointer()->getX() == getX() && getWorld()->getPeachPointer()->getY() == getY() && !(getWorld()->getPeachPointer()->getStatus()) && getWorld()->getPeachPointer()->isNew()) {
		editInventory(*(getWorld()->getPeachPointer()));
		getWorld()->getPeachPointer()->setNew(false);
		getWorld()->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);
	}
	if (getWorld()->getYoshiPointer()->getX() == getX() && getWorld()->getYoshiPointer()->getY() == getY() && !(getWorld()->getYoshiPointer()->getStatus()) && getWorld()->getYoshiPointer()->isNew()) {
		editInventory(*(getWorld()->getYoshiPointer()));
		getWorld()->getYoshiPointer()->setNew(false);
		getWorld()->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);
	}
}

//////////EVENT SQUARE
EventSquare::EventSquare(int startX, int startY, StudentWorld* worldlinker)
	: Square(IID_EVENT_SQUARE, startX, startY, right, worldlinker) {}
EventSquare::~EventSquare() {}

void EventSquare::randomTeleport(PlayerAvatar* player){
	int newX = randInt(0, 15);
	int newY = randInt(0, 15);
	while (getWorld()->isEmptySpace(newX * 16, newY * 16)) {
		newX = randInt(0, 15);
		newY = randInt(0, 15);
	}
	player->moveTo(newX * 16, newY * 16);
	getWorld()->playSound(SOUND_PLAYER_TELEPORT);

}

void EventSquare::swapPositions(PlayerAvatar* player1, PlayerAvatar* player2){
	int tempX = player1->getX();
	int tempY = player1->getY();
	int tempticks = player1->getTickstoMove();
	int walkingdir = player1->getWalkingDirection();
	int imagedir = player1->getDirection();
	bool state = player1->getStatus();

	player1->moveTo(player2->getX(), player2->getY());
	player1->setTickstoMove(player2->getTickstoMove());
	player1->setWalkingDirection(player2->getWalkingDirection());
	player1->setDirection(player2->getDirection());
	player1->setStatus(player2->getStatus());

	player2->moveTo(tempX, tempY);
	player2->setTickstoMove(tempticks);
	player2->setWalkingDirection(walkingdir);
	player2->setDirection(imagedir);
	player2->setStatus(state);

	getWorld()->playSound(SOUND_PLAYER_TELEPORT);
}

void EventSquare::giveVortex(PlayerAvatar* player){
	player->setVortex(true);
	getWorld()->playSound(SOUND_GIVE_VORTEX);

}

void EventSquare::doEvent(PlayerAvatar* player1, PlayerAvatar* player2) {
	int choice = randInt(1, 3);
	switch (choice) {
	case 1:
		randomTeleport(player1);
		break;
	case 2:
		swapPositions(player1, player2);
		player1->setEventNew(false);
		player2->setEventNew(false);
		break;
	case 3:
		if (!(player1->hasVortex())) 
		{
			giveVortex(player1);
		}
		player1->setEventNew(false);
		break;
	}
}

void EventSquare::doSomething() {
	;
	if (getWorld()->getPeachPointer()->getX() == getX() && getWorld()->getPeachPointer()->getY() == getY() && !(getWorld()->getPeachPointer()->getStatus()) && getWorld()->getPeachPointer()->isEventNew()) {
		doEvent(getWorld()->getPeachPointer(), getWorld()->getYoshiPointer());
		/*getWorld()->getPeachPointer()->setEventNew(false);*/
	}
		
	if (getWorld()->getYoshiPointer()->getX() == getX() && getWorld()->getYoshiPointer()->getY() == getY() && !(getWorld()->getYoshiPointer()->getStatus()) && getWorld()->getYoshiPointer()->isEventNew()) {
		doEvent(getWorld()->getYoshiPointer(), getWorld()->getPeachPointer());
		/*getWorld()->getYoshiPointer()->setEventNew(false);*/
	}
	
}

/////////////////DIRECTION SQUARE
DirectionSquare::DirectionSquare(int startX, int startY, int dir, StudentWorld* worldlinker)
	: Square(IID_DIR_SQUARE, startX, startY, dir, worldlinker) {
	setisDirectional(true);
}
DirectionSquare::~DirectionSquare() {}

void DirectionSquare::doSomething() {
	if (getWorld()->getPeachPointer()->getX() == getX() && getWorld()->getPeachPointer()->getY() == getY()) {
		if (getDirection() == left) {
			getWorld()->getPeachPointer()->setWalkingDirection(left);
			getWorld()->getPeachPointer()->setDirection(left);
		}
		else
		{
			getWorld()->getPeachPointer()->setWalkingDirection(getDirection());
			getWorld()->getPeachPointer()->setDirection(right);
		}
	}
	if (getWorld()->getYoshiPointer()->getX() == getX() && getWorld()->getYoshiPointer()->getY() == getY()) {
		if (getDirection() == left) {
			getWorld()->getYoshiPointer()->setWalkingDirection(left);
			getWorld()->getYoshiPointer()->setDirection(left);
		}
		else
		{
			getWorld()->getYoshiPointer()->setWalkingDirection(getDirection());
			getWorld()->getYoshiPointer()->setDirection(right);
		}
	}
}

////////////VORTEX

Vortex::Vortex(int startX, int startY, int dir, StudentWorld* worldlinker)
	:Agents(IID_VORTEX, startX, startY, worldlinker) {
	setStatus(true);
	setWalkingDirection(dir);
	setisVortex(true);
}

Vortex::~Vortex() {}

bool Vortex::overlaps(Actor* baddie) {
	if (baddie->isImpactable()) {
		int X = baddie->getX();
		int Y = baddie->getY();
		if (getDirection() == right) {
			return ((X - getX()) <= 15 && X >= getX() && Y == getY());
		}
		if (getDirection() == left) {
			return ((getX() - X) <= 15 && getX() >= X && Y == getY());
		}
		if (getDirection() == up) {
			return (X == getX() && Y - getY() <= 15 && Y >= getY());
		}
		if (getDirection() == down) {
			return (X == getX() && getY() - Y <= 15 && getY() >= Y);
		}
	}
	return false;
}

void Vortex::doSomething() {
	if (!getStatus()) return;
	move();
	if (getX() > 255 || getX() < 0 || getY() > 255 || getY() < 0) setStatus(false);
	getWorld()->vortexhit(getX(), getY(), this);
}