/*
 *  Storage <-- HydroStorage class implementation file
 */


#include "../../../header/includes.h"
#include "../../../header/assets/storage/Storage.h"
#include "../../../header/assets/storage/HydroStorage.h"


HydroStorage :: HydroStorage(
    structStorage struct_storage,
    structHydroStorage struct_hydro_storage
) : Storage(struct_storage) {
    /*
     *  HydroStorage class constructor
     */
    
    this->struct_storage.storage_type = STORAGE_HYDRO;
    this->struct_hydro_storage = struct_hydro_storage;
    
    //...
    
    if (this->struct_storage.test_flag) {
        std::cout << "\tHydroStorage object constructed at " << this
            << std::endl;
    }
    
    return;
}


//...


HydroStorage :: ~HydroStorage() {
    /*
     *  HydroStorage class destructor
     */
    
    if (this->struct_storage.test_flag) {
        std::cout << "\tHydroStorage object at " << this
            << " destroyed" << std::endl;
    }
    
    return;
}
