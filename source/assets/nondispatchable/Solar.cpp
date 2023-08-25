/*
 *  Nondispatchable <-- Solar class implementation file
 */


#include "../../../header/includes.h"
#include "../../../header/assets/nondispatchable/Nondispatchable.h"
#include "../../../header/assets/nondispatchable/Solar.h"


Solar :: Solar(
    structNondispatchable struct_nondisp,
    structSolar struct_solar
) : Nondispatchable(struct_nondisp) {
    /*
     *  Solar class constructor
     */
    
    this->struct_solar = struct_solar;
    
    //...
    
    if (this->struct_nondisp.test_flag) {
        std::cout << "\tSolar object constructed at " << this
            << std::endl;
    }
    
    return;
}


//...


Solar :: ~Solar() {
    /*
     *  Solar class destructor
     */
    
    if (this->struct_nondisp.test_flag) {
        std::cout << "\tSolar object at " << this
            << " destroyed" << std::endl;
    }
    
    return;
}
