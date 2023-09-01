/*
 *  Storage <-- BatteryStorage <-- LiIon class header file
 */


#ifndef LiIon_H
#define LiIon_H


#include "../../../includes.h"
#include "../Storage.h"
#include "BatteryStorage.h"


struct structLiIon {
    //...
};


class LiIon : public BatteryStorage {
    public:
        //  1. attributes
        structLiIon struct_liion;
        
        
        //  2. methods
        LiIon(structStorage, structBatteryStorage, structLiIon);
        
        //...
        
        ~LiIon(void);
};


#endif  /* LiIon_H */
