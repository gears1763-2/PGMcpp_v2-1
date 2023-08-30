/*
 *  Testing for Nondispatchable <-- Solar class
 */

std::cout << std::endl;
printGold("Testing Nondispatchable <-- Solar ... ");
std::cout << std::endl;

try {
    //  construction
    structNondispatchable struct_nondisp;
    struct_nondisp.test_flag = true;
    
    structSolar struct_solar;
    
    Solar test_solar(struct_nondisp, struct_solar);
    
    
    //...
    
} catch (...) {
    printRed("\n\t\t\t\tNondispatchable <-- Solar Tests:  FAIL\n");
    std::cout << std::endl;
    
    throw;
}

printGreen("\n\t\t\t\tNondispatchable <-- Solar Tests:  PASS");
std::cout << std::endl;
