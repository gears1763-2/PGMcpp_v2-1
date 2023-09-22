/*
 *  PGMcpp : PRIMED Grid Modelling Code (in C++) - v2.1
 *
 *  Anthony Truelove MASc, P.Eng.
 *  email:  gears1763@tutanota.com
 *  github: gears1763-2
 *
 *  See license terms
 *
 *  Testing suite for the Dispatchable <-- Hydro class.
 *
 *  *** WORK IN PROGRESS - NOT YET FULLY IMPLEMENTED ***
 */

std::cout << std::endl;
printGold("Testing Dispatchable <-- Hydro ... ");
std::cout << std::endl;

try {
    //  construction
    structDispatchable struct_disp;
    struct_disp.test_flag = true;
    
    structHydro struct_hydro;
    
    Hydro test_hydro(struct_disp, struct_hydro, 16);
    
    
    //...
    
} catch (...) {
    printRed("\n\t\t\t\tDispatchable <-- Hydro Tests:  FAIL\n");
    std::cout << std::endl;
    
    throw;
}

printGreen("\n\t\t\t\tDispatchable <-- Hydro Tests:  PASS");
std::cout << std::endl;
