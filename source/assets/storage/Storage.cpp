/*
 *  Storage class implementation file
 */


#include "../../../header/includes.h"
#include "../../../header/assets/storage/Storage.h"


Storage :: Storage(
    structStorage struct_storage
) {
    /*
     *  Storage class constructor
     */
    
    this->struct_storage = struct_storage;
    
    //...
    
    if (this->struct_storage.test_flag) {
        std::cout << "\tStorage object constructed at " << this
            << std::endl;
    }
    
    return;
}


//...


Storage :: ~Storage() {
    /*
     *  Storage class destructor
     */
    
    if (this->struct_storage.test_flag) {
        std::cout << "\tStorage object at " << this
            << " destroyed" << std::endl;
    }
    
    return;
}
