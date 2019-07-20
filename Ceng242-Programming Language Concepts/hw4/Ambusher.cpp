#include "Ambusher.h"

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE 
*/
Ambusher::~Ambusher(){
}
Armor Ambusher::getArmor() const{
	return armor;
}
Weapon Ambusher::getWeapon() const {
	return weapon;
}
/**
* Every player has a different priority move list.
* It's explained in the Players' header.
*
* @return The move priority list for the player.
*/
std::vector<Move> Ambusher::getPriorityList() const {
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
const std::string Ambusher::getFullName() const{
	return Name+ this->getBoardID();
}