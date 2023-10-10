#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <sstream>

using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
    : GameWorld(assetPath), m_bank(0)
{
}

StudentWorld::~StudentWorld() {
    cleanUp();
}

PlayerAvatar* StudentWorld::getYoshiPointer() const {
    return m_yoshipointer;
}

PlayerAvatar* StudentWorld::getPeachPointer() const {
    return m_peachpointer;
}

int StudentWorld::getBank() const {
    return m_bank;
}

void StudentWorld::setBank(int set) {
    m_bank = set;
}

void StudentWorld::bowserdropping(int x, int y) {
    std::list<Actor*>::iterator it;
    std::list<Actor*>::iterator temp;
    it = m_actorlist.begin();
    while (it != m_actorlist.end()) {
        if ((*it)->getX() == x && (*it)->getY() == y && (*it)->isSquare()) {
            Actor* tempptr = *it;
            temp = it;
            it++;
            delete (tempptr);
            m_actorlist.erase(temp);
            playSound(SOUND_DROPPING_SQUARE_CREATED);
        }
        else it++;
    }
    m_actorlist.push_back(new DroppingSquare(x / SPRITE_WIDTH, y / SPRITE_HEIGHT, this));
}

void StudentWorld::addvortex(int x, int y, int direction) {
    if (direction == GraphObject::right) {
        m_actorlist.push_back(new Vortex((x + 16) / SPRITE_WIDTH, y / SPRITE_HEIGHT, direction, this));
    }
    else if (direction == GraphObject::left) {
        m_actorlist.push_back(new Vortex((x - 16) / SPRITE_WIDTH, y / SPRITE_HEIGHT, direction, this));
    }
    else if (direction == GraphObject::up) {
        m_actorlist.push_back(new Vortex(x / SPRITE_WIDTH, (y + 16) / SPRITE_HEIGHT, direction, this));
    }
    else if (direction == GraphObject::down) {
        m_actorlist.push_back(new Vortex(x / SPRITE_WIDTH, (y - 16) / SPRITE_HEIGHT, direction, this));
    }
    playSound(SOUND_PLAYER_FIRE);
}

void StudentWorld::teleportbaddie(Actor* baddie) {
    int newX = randInt(0, 15);
    int newY = randInt(0, 15);
    while (isEmptySpace(newX * 16, newY * 16)) {
        newX = randInt(0, 15);
        newY = randInt(0, 15);
    }
    baddie->moveTo(newX * 16, newY * 16);
    baddie->setImpacted(true);
    playSound(SOUND_HIT_BY_VORTEX);
}

void StudentWorld::vortexhit(int x, int y, Vortex* vortex) {
    std::list<Actor*>::iterator it;
    it = m_actorlist.begin();
    while (it != m_actorlist.end()) {
        if (vortex->overlaps(*it)) {
            teleportbaddie(*it);
            vortex->setStatus(false);
            playSound(SOUND_HIT_BY_VORTEX);
            return;
        }
        it++;
    }
}

std::list<Actor*> StudentWorld::getsList() {
    return m_actorlist;
}

int StudentWorld::init()
{
    //Board bd;
    string board_file;
    int choice = getBoardNumber();
    switch (choice) {
    case 1:
        board_file = assetPath() + "board01.txt";
        break;
    case 2:
        board_file = assetPath() + "board02.txt";
        break;
    case 3:
        board_file = assetPath() + "board03.txt";
        break;
    case 4:
        board_file = assetPath() + "board04.txt";
        break;
    case 5:
        board_file = assetPath() + "board05.txt";
        break;
    case 6:
        board_file = assetPath() + "board06.txt";
        break;
    case 7: 
        board_file = assetPath() + "board07.txt";
        break;
    case 8:
        board_file = assetPath() + "board08.txt";
        break;
    case 9:
        board_file = assetPath() + "board09.txt";
        break;
    }
    Board::LoadResult result = bd.loadBoard(board_file);
    if (result == Board::load_fail_file_not_found) {
        cerr << "Could not find board01.txt data file\n";
        return  GWSTATUS_BOARD_ERROR;
    }
    else if (result == Board::load_fail_bad_format) {
        cerr << "Your board was improperly formatted\n";
        return  GWSTATUS_BOARD_ERROR;
    }
    else if (result == Board::load_success) {
        cerr << "Successfully loaded board\n";

        for (int i = 0; i < BOARD_HEIGHT; i++) {
            for (int j = 0; j < BOARD_WIDTH; j++) {
                Board::GridEntry ge = bd.getContentsOf(i, j);
                switch (ge) {
                case Board::empty:
                    cout << "Location " << i << ", " << j << " is empty\n";
                    break;
                case Board::boo:
                    cout << "Location " << i << ", " << j << " has a Boo and a blue coin square\n";
                    m_actorlist.push_back(new BlueCoinSquare(i, j, this));
                    m_actorlist.push_back(new Boo(i, j, this));
                    break;
                case Board::bowser:
                    cout << "Location " << i << ", " << j << " has a Bowser and a blue coin square\n";
                    m_actorlist.push_back(new BlueCoinSquare(i, j, this));
                    m_actorlist.push_back(new Bowser(i, j, this));
                    break;
                case Board::player:
                    cout << "Location " << i << ", " << j << " has Peach & Yoshi and a blue coin square\n";
                    m_actorlist.push_back(new BlueCoinSquare(i, j, this));
                    m_yoshipointer = new PlayerAvatar(IID_YOSHI, i, j, 2, this);
                    m_actorlist.push_back(m_yoshipointer);
                    m_peachpointer = new PlayerAvatar(IID_PEACH, i, j, 1, this);
                    m_actorlist.push_back(m_peachpointer);
                    break;
                case Board::red_coin_square:
                    cout << "Location " << i << ", " << j << " has a red coin square\n";
                    m_actorlist.push_back(new RedCoinSquare(i, j, this));
                    break;
                case Board::blue_coin_square:
                    cout << "Location " << i << ", " << j << " has a blue coin square\n";
                    m_actorlist.push_back(new BlueCoinSquare(i, j, this));
                    break;
                case Board::star_square:
                    cout << "Location " << i << ", " << j << " has a star square\n";
                    m_actorlist.push_back(new StarSquare(i, j, this));
                    break;
                case Board::event_square:
                    cout << "Location " << i << ", " << j << " has an event square\n";
                    m_actorlist.push_back(new EventSquare(i, j, this));
                    break;
                case Board::bank_square:
                    cout << "Location " << i << ", " << j << " has a bank square\n";
                    m_actorlist.push_back(new BankSquare(i, j, this));
                    break;
                case Board::up_dir_square:
                    cout << "Location " << i << ", " << j << " has an up direction square\n";
                    m_actorlist.push_back(new DirectionSquare(i, j, GraphObject::up, this));
                    break;
                case Board::down_dir_square:
                    cout << "Location " << i << ", " << j << " has a down direction square\n";
                    m_actorlist.push_back(new DirectionSquare(i, j, GraphObject::down, this));
                    break;
                case Board::left_dir_square:
                    cout << "Location " << i << ", " << j << " has a left direction square\n";
                    m_actorlist.push_back(new DirectionSquare(i, j, GraphObject::left, this));
                    break;
                case Board::right_dir_square:
                    cout << "Location " << i << ", " << j << " has a right direction square\n";
                    m_actorlist.push_back(new DirectionSquare(i, j, GraphObject::right, this));
                    break;
                }
            }
        }
    }

    startCountdownTimer(99);  // this placeholder causes timeout after 5 seconds
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit ESC.
    // Notice that the return value GWSTATUS_NOT_IMPLEMENTED will cause our framework to end the game.
    
    int peachstars = getPeachPointer()->getStars();
    int peachcoins = getPeachPointer()->getCoins();
    int yoshistars = getYoshiPointer()->getStars();
    int yoshicoins = getYoshiPointer()->getCoins();

    //SCOREBOARD
    ostringstream ps;
    ps << peachstars;
    string pstars = ps.str();

    ostringstream pc;
    pc << peachcoins;
    string pcoins = pc.str();

    ostringstream ys;
    ys << yoshistars;
    string ystars = ys.str();

    ostringstream yc;
    yc << yoshicoins;
    string ycoins = yc.str();

    ostringstream tr;
    tr << timeRemaining();
    string timeremaining = tr.str();

    string pvortex;
    if (getPeachPointer()->hasVortex()) { pvortex = " VOR"; }
    string yvortex;
    if (getYoshiPointer()->hasVortex()) { yvortex = "VOR"; }

    ostringstream pdr;
    pdr << getPeachPointer()->getDieRoll();
    string proll = pdr.str();

    ostringstream ydr;
    ydr << getYoshiPointer()->getDieRoll();
    string yroll = ydr.str();

    ostringstream bank;
    bank << m_bank;
    string bankcount = bank.str();
 
    string scoreboard = "P1 Roll : " + proll + " Stars : " + pstars + " $$ : " + pcoins + pvortex 
        + " | Time : " + timeremaining + " | Bank : " + bankcount + " | P2 Roll : " + yroll + " Stars : " 
        + ystars + " $$ : " + ycoins + " " + yvortex;
    setGameStatText(scoreboard);
    
    list<Actor*>::iterator it;
    list<Actor*>::iterator temp;
    it = m_actorlist.begin();
    while (it != m_actorlist.end()) {
        (*it)->doSomething();
        if ((*it)->isVortex() && !((*it)->getStatus())) {
            Actor* tempptr = *it;
            temp = it;
            it++;
            delete tempptr;
            m_actorlist.erase(temp);
        }
        else it++;
    }



    if (timeRemaining() <= 0) {
        playSound(SOUND_GAME_FINISHED);
        
        if (getYoshiPointer()->getStars() > getPeachPointer()->getStars()) {
            setFinalScore(getYoshiPointer()->getStars(), getYoshiPointer()->getCoins());
            return GWSTATUS_YOSHI_WON;
        }
        if (getPeachPointer()->getStars() > getYoshiPointer()->getStars()) {
            setFinalScore(getPeachPointer()->getStars(), getPeachPointer()->getCoins());
            return GWSTATUS_PEACH_WON;
        }
        if (getYoshiPointer()->getCoins() > getPeachPointer()->getCoins()) {
            setFinalScore(getYoshiPointer()->getStars(), getYoshiPointer()->getCoins());
            return GWSTATUS_YOSHI_WON;
        }
        if (getPeachPointer()->getCoins() > getYoshiPointer()->getCoins()) {
            setFinalScore(getPeachPointer()->getStars(), getPeachPointer()->getCoins());
            return GWSTATUS_PEACH_WON;
        }
        int randwinner = randInt(1, 2);
        switch (randwinner) {
        case 1:
            setFinalScore(getPeachPointer()->getStars(), getPeachPointer()->getCoins());
            return GWSTATUS_PEACH_WON;
        case 2:
            setFinalScore(getYoshiPointer()->getStars(), getYoshiPointer()->getCoins());
            return GWSTATUS_YOSHI_WON;
        }
        //return GWSTATUS_NOT_IMPLEMENTED;
    }

    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    list<Actor*>::iterator it;
    list<Actor*>::iterator temp;
    it = m_actorlist.begin();
    while (it != m_actorlist.end()) {
        temp = it;
        it++;
        delete (*temp);
        m_actorlist.erase(temp);
    }
}

bool StudentWorld::isEmptySpace(int i, int j) {
    Board::GridEntry ge = bd.getContentsOf(i/16, j/16);
    if (ge == Board::empty) return true;
    else return false;
}

bool StudentWorld::isDirectional(int i, int j) {
    list<Actor*>::iterator it;
    it = m_actorlist.begin();
    while (it != m_actorlist.end()) {
        if ((*it)->getX() == i && (*it)->getY() == j && (*it)->isDirectionalSq()) {
            return true;
        }
        it++;
    }
    return false;

    /*Board::GridEntry ge = bd.getContentsOf(i / 16, j / 16);
    if (ge == Board::up_dir_square || ge == Board::down_dir_square || ge == Board::left_dir_square || ge == Board::right_dir_square) return true;
    else return false;*/
}

int StudentWorld::getDirectional(int i, int j) {
    Board::GridEntry ge = bd.getContentsOf(i / 16, j / 16);
    if (ge == Board::up_dir_square) return GraphObject::up;
    if (ge == Board::down_dir_square) return GraphObject::down;
    if (ge == Board::left_dir_square) return GraphObject::left;
    if (ge == Board::right_dir_square) return GraphObject::right;
    else return 1;
}