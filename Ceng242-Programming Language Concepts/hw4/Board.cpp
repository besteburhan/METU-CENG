#include "Board.h"


/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE 
*/

	
    Board::~Board(){
    }

    uint Board::getSize() const{
    	return this->boardSize;
    }
    /**
     * Decide whether the coordinate is in the board limits.
     *
     * @param coord Coordinate to search.
     * @return true if coord is in limits, false otherwise.
     */
    bool Board::isCoordInBoard(const Coordinate& coord) const{
    	if(coord.x >=0 && coord.x < this->getSize() && coord.y >=0 && coord.y < getSize())
    		return true;
    	return false;
    }
    /**
     * Decide whether the given coordinate is in storm.
     *
     * @param coord Coordinate to search.
     * @return true if covered in storm, false otherwise.
     */
    bool Board::isStormInCoord(const Coordinate &coord) const{
    	if(!isCoordInBoard(coord)) return false; 
    	if(coord==hCoord) return false;
    	for(int i=0; i< storm;i++){
    		if(coord.x == i || coord.x == boardSize-1-i || coord.y == i || coord.y == boardSize-1-i)
    			return true;
    	}
    	return false;
    }

    /**
     * Decide whether the given coordinate is the hill.
     *
     * @param coord Coordinate to search.
     * @return true if the coord is at the very center of the board, false otherwise.
     */
    bool Board::isCoordHill(const Coordinate& coord) const{
    	Coordinate hillCoord( (int(boardSize)-1)/2,(int(boardSize)-1)/2);
    	if(hillCoord == coord) return true;
    	return false;
    }
    /**
     * Indexing.
     *
     * Find the player in coordinate.
     *
     * nullptr if player does not exists in given coordinates, or !isCoordInBoard
     *
     * @param coord  The coordinate to search.
     * @return The player in coordinate.
     */
    Player * Board::operator[](const Coordinate& coord) const{
    	if(!this->isCoordInBoard(coord)) return nullptr;
    	for(std::vector<Player*>::iterator i= players->begin(); i!= players->end(); i++){
    		if((*i)->getCoord()==coord)
    			return *i;
    	}
    	return nullptr;
    }
    /**
     * Calculate the new coordinate with the given move and coordinate.
     *
     * NOOP and ATTACK are no-op, return coord.
     *
     * The new coordinate cannot be outside of the borders.
     * If it's the case, return coord.
     *
     * Also, if there's another player in the new coordinate,
     * return coord.
     *
     * @param move Move to be made.
     * @param coord The coordinate to be moved.
     * @return Calculated coordinate after the move.
     */
    Coordinate Board::calculateCoordWithMove(Move move, const Coordinate &coord) const{
    	Coordinate c = coord;
        Player* p = (*this)[coord];
        c = c+move;
        if(!isCoordInBoard(c)) return coord;
        for(std::vector<Player*>::iterator i= players->begin(); i!= players->end(); i++){
    		if((*i)->getCoord()==c)
    			return coord;
    		}
    	return c;
    }

    /**
     * Find the visible coordinates from given coordinate.
     *
     * Explanation: The immediate UP/DOWN/LEFT/RIGHT tiles must be calculated.
     *
     * There could be max of 4 tiles, and min of 2 tiles (on corners).
     * Every found coordinate must be in the board limits.
     *
     * If the given coordinate is not in board, return a vector with size = 0. Order does NOT matter.
     *
     * Example:
     *
     * 01----
     * 02HH--
     * ------
     *
     * visibleCoordsFromCoord(Coordinate(0, 0)) == { (1, 0), (0, 1) }
     * visibleCoordsFromCoord(Coordinate(1, 1)) == { (1, 0), (2, 1), (1, 2), (0, 1) }
     * visibleCoordsFromCoord(Coordinate(-1, 0)) == { }
     *
     * @param coord The coordinate to search.
     * @return All coordinates visible.
     */
    std::vector<Coordinate> Board::visibleCoordsFromCoord(const Coordinate &coord) const{

    	Coordinate c=coord;

    	std::vector<Coordinate> visibleCoordVector;
    	if(!(isCoordInBoard(c))) return visibleCoordVector;
    	
    	if(isCoordInBoard(Coordinate(c.x,c.y -1))) visibleCoordVector.push_back(Coordinate(c.x,c.y -1));
    	if(isCoordInBoard(Coordinate(c.x +1,c.y))) visibleCoordVector.push_back(Coordinate(c.x +1,c.y));
    	if(isCoordInBoard(Coordinate(c.x,c.y +1))) visibleCoordVector.push_back(Coordinate(c.x,c.y +1));
    	if(isCoordInBoard(Coordinate(c.x -1,c.y))) visibleCoordVector.push_back(Coordinate(c.x -1,c.y));
    	

    }

    /**
     * Calculate the storm according to the currentRound.
     *
     * @param currentRound The current round being played.
     */
    void Board::updateStorm(uint currentRound){
    	storm =  Entity::stormWidthForRound(currentRound);

    }
    void Board::removePlayer(Player* player){

    	for(std::vector<Player*>::iterator i= this->players->begin(); i!= this->players->end(); i++){
    		if((*i)->getID()==player->getID()){
    		    this->players->erase(i);
    		    //delete player;
    		    return;}

    	}
    }
    Coordinate Board::gethCoord(){
    	return this->hCoord;
    }

