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
    
    this->replaced_vec.resize(this->struct_storage.n_timesteps, false);
    
    this->charge_vec_kWh.resize(this->struct_storage.n_timesteps, 0);
    this->charging_vec_kW.resize(this->struct_storage.n_timesteps, 0);
    this->discharging_vec_kW.resize(this->struct_storage.n_timesteps, 0);
    
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
