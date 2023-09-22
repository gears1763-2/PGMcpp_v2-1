/*
 *  PGMcpp : PRIMED Grid Modelling Code (in C++) - v2.1
 *
 *  Anthony Truelove MASc, P.Eng.
 *  email:  gears1763@tutanota.com
 *  github: gears1763-2
 *
 *  See license terms
 *
 *  Testing suite for the Nondispatchable <-- Wind class.
 */

std::cout << std::endl;
printGold("Testing Nondispatchable <-- Wind ... ");
std::cout << std::endl;

try {
    //  construction
    structNondispatchable struct_nondisp;
    struct_nondisp.test_flag = true;
    
    structWind struct_wind;
    
    Wind test_wind(struct_nondisp, struct_wind, 16);
    
    
    //...
    
} catch (...) {
    printRed("\n\t\t\t\tNondispatchable <-- Wind Tests:  FAIL\n");
    std::cout << std::endl;
    
    throw;
}

printGreen("\n\t\t\t\tNondispatchable <-- Wind Tests:  PASS");
std::cout << std::endl;
