/*
 *  Nondispatchable class implementation file
 */


#include "../../../header/includes.h"
#include "../../../header/assets/nondispatchable/Nondispatchable.h"


Nondispatchable :: Nondispatchable(
    structNondispatchable struct_nondisp
) {
    /*
     *  NonDispatchable class constructor
     */
    
    this->struct_nondisp = struct_nondisp;
    
    this->production_vec_kW.resize(this->struct_nondisp.n_timesteps, 0);
    //...
    
    if (this->struct_nondisp.test_flag) {
        std::cout << "\tNondispatchable object constructed at " << this
            << std::endl;
    }
    
    return;
}


//...


Nondispatchable :: ~Nondispatchable() {
    /*
     *  Nondispatchable class destructor
     */
    
    if (this->struct_nondisp.test_flag) {
        std::cout << "\tNondispatchable object at " << this
            << " destroyed" << std::endl;
    }
    
    return;
}
