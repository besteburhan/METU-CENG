#ifndef HW4_AMBUSHER_H
#define HW4_AMBUSHER_H


#include "Player.h"

class Ambusher : public Player {
public:
    Ambusher(uint id, int x, int y): Player(id,x,y){
    
    this->Name= "Ambusher";
    this->Priority = { ATTACK };
    armor = NOARMOR;
    weapon =SEMIAUTO;
    this->HP=100;
    }

    // Name     : Ambusher
    // Priority : { ATTACK }

    // Armor    : NOARMOR
    // Weapon   : SEMIAUTO
    // HP       : 100

    // DO NOT MODIFY THE UPPER PART
    // ADD OWN PUBLIC METHODS/PROPERTIES/OVERRIDES BELOW

    ~Ambusher();
    Armor getArmor() const;
    Weapon  getWeapon() const ;
    std::vector<Move>   getPriorityList() const ;
    const std::string   getFullName() const;
    
    
};


#endif //HW4_AMBUSHER_H
