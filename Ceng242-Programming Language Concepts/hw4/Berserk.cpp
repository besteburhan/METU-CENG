#include "Berserk.h"

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE 
*/
/*Berserk::Berserk(uint id, int x, int y){
	// Name     : Berserk
    // Priority : { ATTACK, UP, LEFT, DOWN, RIGHT }

    // Armor    : WOODEN
    // Weapon   : PISTOL
    // HP       : 100
    
    this->Name= "Berserk";
    this->Priority = { ATTACK, UP, LEFT, DOWN, RIGHT };
    armor = WOODEN;
    weapon =PISTOL;
    this->HP=100;

}*/
Berserk::~Berserk(){}
Armor Berserk::getArmor() const{
	return armor;
}
Weapon Berserk::getWeapon() const {
	return weapon;
}
/**
* Every player has a different priority move list.
* It's explained in the Players' header.
*
* @return The move priority list for the player.
*/
std::vector<Move> Berserk::getPriorityList() const {
	return Priority;
}
/**
* Get the full name of the player.
*
* Example (Tracer with ID 92) = "Tracer92"
* Example (Tracer with ID 1)  = "Tracer01"
*
* @return Full name of the player.
*/
const std::string Berserk::getFullName() const{
	return Name + this->getBoardID();
}