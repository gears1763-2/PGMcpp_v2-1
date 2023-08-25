/*
 *  Testing for Nondispatchable <-- Solar class
 */

std::cout << std::endl;
printGreen("Testing Nondispatchable <-- Solar ... ");
std::cout << std::endl;

try {
    //  construction
    //...
    
    
    //...
    
} catch (...) {
    printRed("\n\t\t\t\tNondispatchable <-- Solar Tests:  FAIL\n");
    std::cout << std::endl;
    
    throw;
}

printGreen("\n\t\t\t\tNondispatchable <-- Solar Tests:  PASS");
std::cout << std::endl;
