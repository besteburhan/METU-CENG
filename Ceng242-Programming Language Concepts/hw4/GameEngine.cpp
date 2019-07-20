#include "GameEngine.h"
#include <iostream>

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE 
*/
//  uint currentRound;
//  Board board;


    GameEngine::~GameEngine(){

    	for ( std::vector<Player*>::iterator i = this->players->begin(); i != this->players->end(); ++i )
    		delete *i;
    	delete players;
    
}
    const Board& GameEngine::getBoard() const{
    	return this->board;
    }

    /**
     * Indexing.
     *
     * Find the player with given ID.
     *
     * nullptr if not exists.
     *
     * @param id ID of the player.
     * @return The player with given ID.
     */
    Player* GameEngine::operator[](uint id) const{
    	
    	for(std::vector<Player*>::iterator i= this->players->begin(); i!= this->players->end(); i++){
    		if((*i)->getID()==id)
    			return *i;
    	}
    	return nullptr;
    }

    /**
     * Decide whether the game is finished.
     *
     * @return true if there is only 1 player (alive), on top of the hill; or there are 0 players. False otherwise.
     */
    bool GameEngine::isFinished() const{
    	int countAlive = 0;
    	bool res = false;
    	if(this->players->empty()) return true;
    	for(std::vector<Player*>::iterator i= this->players->begin(); i!= this->players->end(); i++){
    		if(!(*i)->isDead()){
    		    countAlive++;
    		    if( this->board.isCoordHill((*i)->getCoord()) ) res =true; 
    		}
    	}
    	if((countAlive==1 && res) || countAlive == 0) return true;
    	return false;
    }

    /**
     * Take turn for every player.
     *
     * How-to:
     * - Announce turn start (cout).
     * Example: -- START ROUND 1  --
     * - board.updateStorm(currentRound)
     * - For every player (sorted according to their IDs) that isn't dead (HP <= 0):
     *      - takeTurnForPlayer(player).
     * - Announce turn end (cout).
     * Example: -- END ROUND 1 --
     */
    void GameEngine::takeTurn(){
    	std::cout << "-- START ROUND " << currentRound <<" --" << std::endl;

    	board.updateStorm(currentRound);
    	int index = 0;
    	while(index < this->players->size()) {
    		Player *player = (*this->players)[index];
    		takeTurnForPlayer(player->getID());
    		if(player == (*this->players)[index])index++;
    		

    	}
    	std::cout << "-- END ROUND " << currentRound <<" --" << std::endl;
    	currentRound++;
    }

    /**
     * The most important (algorithm-wise) method.
     *
     * How-to:
     * - Get player with ID. Return NOOP if not exists.
     * - Get player's priority list.
     * - Get player's visibility from the board (visibleCoordsFromCoord).
     *
     * - If the player is in the storm (isStormInCoord), announce the damage and give player stormDamage.
     * - Example: Tracer01(10) is STORMED! (-10)
     *
     * - If dead, announce the death, remove player from the board/list/anywhere, and return NOOP.
     * - Example: Tracer01(0) DIED.
     *
     * - For MOVE in player's priority list:
     *          - If the MOVE is NOOP:
     *              - return NOOP.
     *          - Else if the MOVE is ATTACK:
     *              - Get all players that this player can attack (board[coord] for each in visibilityCoords).
     *              - If none, continue.
     *              - Else:
     *                  - Pick the one with most priority (lowest ID).
     *                  - isPlayerDead = player.attackTo(thatPlayer).
     *                  - if isPlayerIsDead:
     *                      - announce the death.
     *                      - remove thatPlayer from the board/list/anywhere.
     *                  - return ATTACK.
     *          - Else (UP/DOWN/LEFT/RIGHT):
     *              - calculateCoordWithMove(move).
     *              - If the new coordinate is different than the player's (meaning it's able to do that move)
     *              AND the player is getting closer to the hill;
     *                  - player.executeMove(MOVE).
     *                  - return MOVE.
     *              - Else:
     *                  - continue.
     *
     * // If the priority list is exhausted;
     * return NOOP.
     *
     * @param player Player ID to move.
     * @return move Decided move.
     */
    Move GameEngine::takeTurnForPlayer(uint playerID){
    	Player *p = nullptr;
    	for(std::vector<Player*>::iterator i= this->players->begin(); i!= this->players->end(); i++) {
    		if((*i)->getID()==playerID) p = (*i);
    	}

    	if(p==nullptr) return NOOP;

    	std::vector<Move> priorityList = p->getPriorityList();
    	std::vector<Coordinate> visibilityCoords = this->board.visibleCoordsFromCoord(p->getCoord());
    	if(this->board.isStormInCoord(p->getCoord())) {
    		int storm = Entity::stormDamageForRound(currentRound);
    		std::cout << p->getFullName()  << "(" << p->getHP() << ") is STORMED! (-" << storm << ")" << std::endl;
    		p->setHP(p->getHP()-storm);

    	}
    	if(p->isDead()){
    		std::cout << p->getFullName()  << "(" << p->getHP() << ") DIED." << std::endl;
    		this->board.removePlayer(p);
    		
    		return NOOP;

    	}
    	
    			
    	for(std::vector<Move>::iterator moveIt= priorityList.begin(); moveIt!= priorityList.end(); moveIt++) {

    		if((*moveIt) == NOOP){
    			return NOOP;
    		}

    		else if((*moveIt)== ATTACK){
    			Player *attackPlayer =nullptr;
    			for(std::vector<Coordinate>::iterator coordIt= visibilityCoords.begin(); coordIt!= visibilityCoords.end(); coordIt++){

    				if(this->board[(*coordIt)])
    					attackPlayer = (this->board[(*coordIt)]);
    			}

    			if(attackPlayer==nullptr) continue;
    			else{
    				
    					bool isPlayerDead= p->attackTo(attackPlayer);
    					if(isPlayerDead){
    						std::cout << attackPlayer->getFullName()  << "(" << attackPlayer->getHP() << ") DIED." << std::endl;
    						this->board.removePlayer(attackPlayer);
    					}
    					return ATTACK;
    				}
    			
    		}
    		else{
    			Coordinate c = this->board.calculateCoordWithMove(*(moveIt),p->getCoord());
    			
    			if(c!= p->getCoord() && c -(this->board.gethCoord() ) < (p->getCoord())-(this->board.gethCoord())){
    				p->executeMove((*moveIt));
    				return (*moveIt);
    			}
    			else continue;
    		}
    	}
    	return NOOP;
    }

    /**
     * Find the winner player.
     *
     * nullptr if there are 0 players left, or the game isn't finished yet.
     *
     * @return The winner player.
     */
    Player *GameEngine::getWinnerPlayer() const{
    	int countAlive=0;
    	Player *p;
    	if(!isFinished()) return nullptr;
    	for(std::vector<Player*>::iterator i= this->players->begin(); i!= this->players->end(); i++){
    		if(!(*i)->isDead()) {
    			countAlive++;
    			p = (*i);}
    	}
    	if( countAlive ==0) return nullptr;
    	return p;

    }

