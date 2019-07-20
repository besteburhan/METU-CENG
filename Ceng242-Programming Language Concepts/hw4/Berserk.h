#ifndef HW4_BERSERK_H
#define HW4_BERSERK_H


#include "Player.h"
#include <string>
#include <vector>
class Berserk : public Player {
public:
    Berserk(uint id, int x, int y): Player(id,x,y){
    
    this->Name= "Berserk";
    this->Priority = { ATTACK, UP, LEFT, DOWN, RIGHT };
    armor = WOODEN;
    weapon =PISTOL;
    this->HP=100;
    }

    // Name     : Berserk
    // Priority : { ATTACK, UP, LEFT, DOWN, RIGHT }

    // Armor    : WOODEN
    // Weapon   : PISTOL
    // HP       : 100

    // DO NOT MODIFY THE UPPER PART
    // ADD OWN PUBLIC METHODS/PROPERTIES/OVERRIDES BELOW
	~Berserk();
	Armor getArmor() const;
	Weapon 	getWeapon() const ;
	std::vector<Move> 	getPriorityList() const ;
	const std::string 	getFullName() const;
    
    
};


#endif //HW4_BERSERK_H
