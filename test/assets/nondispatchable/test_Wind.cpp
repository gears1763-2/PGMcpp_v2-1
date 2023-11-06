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
    
    
    // test input bounds checking
    bool error_flag = true;
    
    try {
        // bad design_speed_ms
        structWind bad_struct_wind;
        bad_struct_wind.design_speed_ms = 0;
        
        Wind bad_test_wind(struct_nondisp, bad_struct_wind, 16);
    }
    
    catch (...) {
        // task failed successfully! =P
    }
    
    if (not error_flag) {
        expectedErrorNotDetected(__LINE__, __FILE__);
    }
    
} catch (...) {
    printRed("\n\t\t\t\tNondispatchable <-- Wind Tests:  FAIL\n");
    std::cout << std::endl;
    
    throw;
}

printGreen("\n\t\t\t\tNondispatchable <-- Wind Tests:  PASS");
std::cout << std::endl;
