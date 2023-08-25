/*
 *  Nondispatchable <-- Tidal class implementation file
 */


#include "../../../header/includes.h"
#include "../../../header/assets/nondispatchable/Nondispatchable.h"
#include "../../../header/assets/nondispatchable/Tidal.h"


Tidal :: Tidal(
    structNondispatchable struct_nondisp,
    structTidal struct_tidal
) : Nondispatchable(struct_nondisp) {
    /*
     *  Tidal class constructor
     */
    
    this->struct_tidal = struct_tidal;
    
    //...
    
    if (this->struct_nondisp.test_flag) {
        std::cout << "\tTidal object constructed at " << this
            << std::endl;
    }
    
    return;
}


//...


Tidal :: ~Tidal() {
    /*
     *  Tidal class destructor
     */
    
    if (this->struct_nondisp.test_flag) {
        std::cout << "\tTidal object at " << this
            << " destroyed" << std::endl;
    }
    
    return;
}
