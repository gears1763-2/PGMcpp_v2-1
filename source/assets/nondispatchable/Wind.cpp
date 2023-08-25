/*
 *  Nondispatchable <-- Wind class implementation file
 */


#include "../../../header/includes.h"
#include "../../../header/assets/nondispatchable/Nondispatchable.h"
#include "../../../header/assets/nondispatchable/Wind.h"


Wind :: Wind(
    structNondispatchable struct_nondisp,
    structWind struct_wind
) : Nondispatchable(struct_nondisp) {
    /*
     *  Wind class constructor
     */
    
    this->struct_wind = struct_wind;
    
    //...
    
    if (this->struct_nondisp.test_flag) {
        std::cout << "\tWind object constructed at " << this
            << std::endl;
    }
    
    return;
}


//...


Wind :: ~Wind() {
    /*
     *  Wind class destructor
     */
    
    if (this->struct_nondisp.test_flag) {
        std::cout << "\tWind object at " << this
            << " destroyed" << std::endl;
    }
    
    return;
}
