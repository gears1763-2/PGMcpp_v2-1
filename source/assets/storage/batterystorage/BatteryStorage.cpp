/*
 *  Storage <-- BatteryStorage class implementation file
 */


#include "../../../../header/includes.h"
#include "../../../../header/assets/storage/Storage.h"
#include "../../../../header/assets/storage/batterystorage/BatteryStorage.h"


BatteryStorage :: BatteryStorage(
    structStorage struct_storage,
    structBatteryStorage struct_battery_storage
) : Storage(struct_storage) {
    /*
     *  BatteryStorage class constructor
     */
    
    this->struct_battery_storage = struct_battery_storage;

    //...
    
    if (this->struct_storage.test_flag) {
        std::cout << "\tBatteryStorage object constructed at " << this
            << std::endl;
    }
    
    return;
}


//...


BatteryStorage :: ~BatteryStorage() {
    /*
     *  BatteryStorage class destructor
     */
    
    if (this->struct_storage.test_flag) {
        std::cout << "\tBatteryStorage object at " << this
            << " destroyed" << std::endl;
    }
    
    return;
}
