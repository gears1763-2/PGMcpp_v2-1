/*
 *  Testing for Nondispatchable <-- Wind class
 */

std::cout << std::endl;
printGold("Testing Nondispatchable <-- Wind ... ");
std::cout << std::endl;

try {
    //  construction
    structNondispatchable struct_nondisp;
    struct_nondisp.test_flag = true;
    
    structWind struct_wind;
    
    Wind test_wind(struct_nondisp, struct_wind);
    
    
    //...
    
} catch (...) {
    printRed("\n\t\t\t\tNondispatchable <-- Wind Tests:  FAIL\n");
    std::cout << std::endl;
    
    throw;
}

printGreen("\n\t\t\t\tNondispatchable <-- Wind Tests:  PASS");
std::cout << std::endl;
