/*
 *  Storage <-- BatteryStorage class header file
 */


#ifndef BATTERYSTORAGE_H
#define BATTERYSTORAGE_H


#include "../../../includes.h"
#include "../Storage.h"


struct structBatteryStorage {
    //...
};


class BatteryStorage : public Storage {
    public:
        //  1. attributes
        structBatteryStorage struct_battery_storage;
        
        
        //  2. methods
        BatteryStorage(structStorage, structBatteryStorage);

        //...
        
        ~BatteryStorage(void);
};


#endif  /* BATTERYSTORAGE_H */
