/*
 *  Storage <-- BatteryStorage <-- LiIon class implementation file
 */


#include "../../../../header/includes.h"
#include "../../../../header/assets/storage/Storage.h"
#include "../../../../header/assets/storage/batterystorage/BatteryStorage.h"
#include "../../../../header/assets/storage/batterystorage/LiIon.h"


LiIon :: LiIon(
    structStorage struct_storage,
    structBatteryStorage struct_battery_storage,
    structLiIon struct_liion
) : BatteryStorage(struct_storage, struct_battery_storage) {
    /*
     *  LiIon class constructor
     */
    
    this->struct_storage.storage_type = STORAGE_LIION;
    this->struct_liion = struct_liion;
    
    //...
    
    if (this->struct_storage.test_flag) {
        std::cout << "\tLiIon object constructed at " << this
            << std::endl;
    }
    
    return;
}


//...


LiIon :: ~LiIon() {
    /*
     *  LiIon class destructor
     */
    
    if (this->struct_storage.test_flag) {
        std::cout << "\tLiIon object at " << this
            << " destroyed" << std::endl;
    }
    
    return;
}
