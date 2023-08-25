/*
 *  Nondispatchable <-- Wave class implementation file
 */


#include "../../../header/includes.h"
#include "../../../header/assets/nondispatchable/Nondispatchable.h"
#include "../../../header/assets/nondispatchable/Wave.h"


Wave :: Wave(
    structNondispatchable struct_nondisp,
    structWave struct_wave
) : Nondispatchable(struct_nondisp) {
    /*
     *  Wave class constructor
     */
    
    this->struct_wave = struct_wave;
    
    //...
    
    if (this->struct_nondisp.test_flag) {
        std::cout << "\tWave object constructed at " << this
            << std::endl;
    }
    
    return;
}


//...


Wave :: ~Wave() {
    /*
     *  Wave class destructor
     */
    
    if (this->struct_nondisp.test_flag) {
        std::cout << "\tWave object at " << this
            << " destroyed" << std::endl;
    }
    
    return;
}
