/*
 *  Testing for Nondispatchable <-- Tidal class
 */

std::cout << std::endl;
printGold("Testing Nondispatchable <-- Tidal ... ");
std::cout << std::endl;

try {
    //  construction
    structNondispatchable struct_nondisp;
    struct_nondisp.test_flag = true;
    
    structTidal struct_tidal;
    
    Tidal test_tidal(struct_nondisp, struct_tidal);
    
    
    //...
    
} catch (...) {
    printRed("\n\t\t\t\tNondispatchable <-- Tidal Tests:  FAIL\n");
    std::cout << std::endl;
    
    throw;
}

printGreen("\n\t\t\t\tNondispatchable <-- Tidal Tests:  PASS");
std::cout << std::endl;
