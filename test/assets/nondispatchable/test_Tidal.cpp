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
    
    
    // test input bounds checking
    bool error_flag = true;
    
    try {
        // bad design_speed_ms
        structTidal bad_struct_tidal;
        bad_struct_tidal.design_speed_ms = 0;
        
        Tidal bad_test_tidal(struct_nondisp, bad_struct_tidal, 16);
    }
    
    catch (...) {
        // task failed successfully! =P
    }
    
    if (not error_flag) {
        expectedErrorNotDetected(__LINE__, __FILE__);
    }
    
} catch (...) {
    printRed("\n\t\t\t\tNondispatchable <-- Tidal Tests:  FAIL\n");
    std::cout << std::endl;
    
    throw;
}

printGreen("\n\t\t\t\tNondispatchable <-- Tidal Tests:  PASS");
std::cout << std::endl;
