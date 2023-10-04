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
    
    
    //  test post-construction attributes
    std::cout << "\tTesting post-construction attributes ..." <<
        std::endl;
    
    testFloatEquals(
        test_hydro.resource_key,
        0,
        FLOAT_TOLERANCE,
        __FILE__,
        __LINE__
    );
    
    testFloatEquals(
        test_hydro.reservoir_capacity_m3,
        100,
        FLOAT_TOLERANCE,
        __FILE__,
        __LINE__
    );
    
    testFloatEquals(
        test_hydro.reservoir_volume_m3,
        50,
        FLOAT_TOLERANCE,
        __FILE__,
        __LINE__
    );
    
    testFloatEquals(
        test_hydro.minimum_discharge_m3hr,
        10,
        FLOAT_TOLERANCE,
        __FILE__,
        __LINE__
    );
    
    testFloatEquals(
        test_hydro.reservoir_volume_vec_m3.size(),
        16,
        FLOAT_TOLERANCE,
        __FILE__,
        __LINE__
    );
    
    
    //  test requestProductionkW()
    std::cout << "\tTesting Hydro::requestProductionkW() ..." <<
        std::endl;
    //...
    
    
    //  test commitProductionkW()
    std::cout << "\tTesting Hydro::commitProductionkW() ..." <<
        std::endl;
    //...
    
} catch (...) {
    printRed("\n\t\t\t\tDispatchable <-- Hydro Tests:  FAIL\n");
    std::cout << std::endl;
    
    throw;
}

printGreen("\n\t\t\t\tDispatchable <-- Hydro Tests:  PASS");
std::cout << std::endl;
