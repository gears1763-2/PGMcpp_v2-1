/*
 *  Dispatchable <-- Hydro class implementation file
 */


#include "../../../header/includes.h"
#include "../../../header/assets/dispatchable/Dispatchable.h"
#include "../../../header/assets/dispatchable/Hydro.h"


Hydro :: Hydro(
    structDispatchable struct_disp,
    structHydro struct_Hydro
) : Dispatchable(struct_disp) {
    /*
     *  Hydro class constructor
     */
    
    this->struct_disp.disp_type = HYDRO;
    this->struct_hydro = struct_hydro;
    
    //...
    
    if (this->struct_disp.test_flag) {
        std::cout << "\tHydro object constructed at " << this
            << std::endl;
    }
    
    return;
}


double Hydro :: requestProductionkW(double requested_production_kW) {
    /*
     *  Method to handle production requests (subject to active
     *  operating constraints) and return provided production
     */
    
    //...
    
    return 0;
}


Hydro :: ~Hydro() {
    /*
     *  Hydro class destructor
     */
    
    if (this->struct_disp.test_flag) {
        std::cout << "\tHydro object at " << this
            << " destroyed" << std::endl;
    }
    
    return;
}
