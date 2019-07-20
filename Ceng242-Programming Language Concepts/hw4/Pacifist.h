#ifndef HW4_PACIFIST_H
#define HW4_PACIFIST_H


#include "Player.h"

class Pacifist : public Player {
public:
    Pacifist(uint id, int x, int y): Player(id,x,y){
    
    this->Name= "Pacifist";
    this->Priority = { UP, LEFT, DOWN, RIGHT };
    armor = METAL;
    weapon =NOWEAPON;
    this->HP=100;
    }

    // Name     : Pacifist
    // Priority : { UP, LEFT, DOWN, RIGHT }

    // Armor    : METAL
    // Weapon   : NOWEAPON
    // HP       : 100

    // DO NOT MODIFY THE UPPER PART
    // ADD OWN PUBLIC METHODS/PROPERTIES/OVERRIDES BELOW
    
	~Pacifist();
	Armor getArmor() const;
	Weapon 	getWeapon() const ;
	std::vector<Move> 	getPriorityList() const ;
	const std::string 	getFullName() const;
    
    
};


#endif //HW4_PACIFIST_H
