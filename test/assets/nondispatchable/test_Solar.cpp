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
