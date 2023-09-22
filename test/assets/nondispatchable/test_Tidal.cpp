/*
 *  PGMcpp : PRIMED Grid Modelling Code (in C++) - v2.1
 *
 *  Anthony Truelove MASc, P.Eng.
 *  email:  gears1763@tutanota.com
 *  github: gears1763-2
 *
 *  See license terms
 *
 *  Testing suite for the Nondispatchable <-- Tidal class.
 */

std::cout << std::endl;
printGold("Testing Nondispatchable <-- Tidal ... ");
std::cout << std::endl;

try {
    //  construction
    structNondispatchable struct_nondisp;
    struct_nondisp.test_flag = true;
    
    structTidal struct_tidal;
    
    Tidal test_tidal(struct_nondisp, struct_tidal, 16);
    
    
    //...
    
} catch (...) {
    printRed("\n\t\t\t\tNondispatchable <-- Tidal Tests:  FAIL\n");
    std::cout << std::endl;
    
    throw;
}

printGreen("\n\t\t\t\tNondispatchable <-- Tidal Tests:  PASS");
std::cout << std::endl;
