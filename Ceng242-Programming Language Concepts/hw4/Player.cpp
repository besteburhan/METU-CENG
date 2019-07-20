#include "Player.h"
#include <iostream>
#include <string>

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE 
*/



    uint Player::getID() const{
    	return this->id;
    }
    const Coordinate& Player::getCoord() const{
    	return this->coordinate;
    }

    int Player::getHP() const{
    	return this->HP;
    }

    /**
     * Board ID is two-decimal ID for the Player.
     *
     * Player ID = 0, 91
     * Board ID = "00", "91"
     *
     * @return BoardID of the user.
     */
    std::string Player::getBoardID() const{
    	std::string res ;
    	if(this->id < 10) res= "0" + std::to_string(this->id);
    	else res =  std::to_string(this->id);
    	
    	return res;
    }

    /**
     * Decide whether the player is dead.
     *
     * @return true if the player's hp <= 0, false otherwise.
     */
    bool Player::isDead() const{
    	if((this->getHP()) <= 0 ) return true;
    	else false;
    }
    /**
     * Execute the given move for the player's coordinates.
     *
     * Important note: Priority list does NOT matter here.
     *
     * NOOP and ATTACK are no-op.
     *
     * Do not forget to print the move.
     * "-playerFullName(playerHP)- moved UP/DOWN/LEFT/RIGHT."
     *
     * "Tracer00(100) moved UP."
     *
     * @param move Move to make.
     */
    void Player::executeMove(Move move){
    	std::string m;
    	std::string res;

    	switch(move)
		{
    		case UP  : m = "UP";	  	break;
    		case DOWN: m = "DOWN";  	break;
    		case LEFT : m = "LEFT";   break;
    		case RIGHT : m = "RIGHT"; break;
		}
		if(m=="UP" || m=="DOWN" || m=="LEFT" || m=="RIGHT"){
			res = this->getFullName() + "(" + std::to_string(this->HP) + ") moved " + m + ".";
		    std::cout << res << std::endl;
		    this->coordinate =this->coordinate + move;
		  }

    }

    /**
     * Attack the given player, and decide whether the attacked player is dead.
     *
     * Important note: Priority list does NOT matter here.
     *
     * Formulae : RHS's HP -= max((LHS's damage - RHS's armor), 0)
     *
     * Do not forget to print the attack.
     *
     * "-lhsFullName(lhsHP)- ATTACKED -rhsFullName(rhsHP)-! (-damageDone-)
     *
     * "Tracer00(100) ATTACKED Tracer01(100)! (-10)"
     *
     * @param player Player to be attacked.
     * @return true if attacked player is dead, false otherwise.
     */
    bool Player::attackTo(Player *player){
    	bool dam = (Entity::damageForWeapon(this->getWeapon())- Entity::damageReductionForArmor(player->getArmor()) < 0);
    	int damagedone = dam ? 0 : (Entity::damageForWeapon(this->getWeapon())- Entity::damageReductionForArmor(player->getArmor())) ;
    	if(this->getID()!=player->getID()){std::cout << this->getFullName()  <<"(" << this->HP << ") ATTACKED " 
    	    			  << player->getFullName()  << "(" << player->HP << ")! ("<< -damagedone <<")" << std::endl;
    	    	player->HP -= damagedone;}
    	return player->isDead();
    	
    }
    void Player::setCoord(Coordinate & coord){
    	this->coordinate = coord;
    }
     void Player::setHP(int HP){
     	this->HP = HP;
     }


    // DO NOT MODIFY THE UPPER PART
    // ADD OWN PUBLIC METHODS/PROPERTIES BELOW