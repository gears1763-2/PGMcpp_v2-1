/*
 *  PGMcpp : PRIMED Grid Modelling Code (in C++) - v2.1
 *
 *  Anthony Truelove MASc, P.Eng.
 *  email:  gears1763@tutanota.com
 *  github: gears1763-2
 *
 *  See license terms
 *
 *  Testing suite for the Nondispatchable <-- Solar class.
 */

std::cout << std::endl;
printGold("Testing Nondispatchable <-- Solar ... ");
std::cout << std::endl;

try {
    //  construction
    structNondispatchable struct_nondisp;
    struct_nondisp.test_flag = true;
    
    structSolar struct_solar;
    
    Solar test_solar(struct_nondisp, struct_solar, 16);
    
    
    // test input bounds checking
    bool error_flag = true;
    
    try {
        // bad cap_kW
        structNondispatchable bad_struct_nondisp;
        bad_struct_nondisp.test_flag = true;
        
        bad_struct_nondisp.cap_kW = 0;
        Solar bad_test_solar(bad_struct_nondisp, struct_solar, 16);
        
        error_flag = false;
    }
    
    catch (...) {
        // task failed successfully! =P
    }
    
    if (not error_flag) {
        expectedErrorNotDetected(__LINE__, __FILE__);
    }
    
    
    try {
        // bad replace_running_hrs
        structNondispatchable bad_struct_nondisp;
        bad_struct_nondisp.test_flag = true;
        
        bad_struct_nondisp.replace_running_hrs = 0;
        Solar bad_test_solar(bad_struct_nondisp, struct_solar, 16);
        
        error_flag = false;
    }
    
    catch (...) {
        // task failed successfully! =P
    }
    
    if (not error_flag) {
        expectedErrorNotDetected(__LINE__, __FILE__);
    }
    
    
    try {
        // bad derating
        structSolar bad_struct_solar;
        
        if (rand() % 2 == 0) {
            bad_struct_solar.derating = -2;
        }
        
        else {
            bad_struct_solar.derating = 2;
        }
        
        Solar bad_test_solar(struct_nondisp, bad_struct_solar, 16);
        
        error_flag = false;
    }
    
    catch (...) {
        // task failed successfully! =P
    }
    
    if (not error_flag) {
        expectedErrorNotDetected(__LINE__, __FILE__);
    }
    
    
    try {
        // bad capital_cost_per_kW
        structSolar bad_struct_solar;
        
        bad_struct_solar.capital_cost_per_kW = -1;
        Solar bad_test_solar(struct_nondisp, bad_struct_solar, 16);
        
        error_flag = false;
    }
    
    catch (...) {
        // task failed successfully! =P
    }
    
    if (not error_flag) {
        expectedErrorNotDetected(__LINE__, __FILE__);
    }
    
    
    //  test post-construction attributes
    testFloatEquals(
        test_solar.struct_nondisp.capital_cost,
        300000,
        FLOAT_TOLERANCE,
        __FILE__,
        __LINE__
    );
    
    testFloatEquals(
        test_solar.struct_solar.derating,
        0.8,
        FLOAT_TOLERANCE,
        __FILE__,
        __LINE__
    );
    
} catch (...) {
    printRed("\n\t\t\t\tNondispatchable <-- Solar Tests:  FAIL\n");
    std::cout << std::endl;
    
    throw;
}

printGreen("\n\t\t\t\tNondispatchable <-- Solar Tests:  PASS");
std::cout << std::endl;
