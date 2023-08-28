/*
 *  Testing for Nondispatchable <-- Wind class
 */

std::cout << std::endl;
printGold("Testing Nondispatchable <-- Wind ... ");
std::cout << std::endl;

try {
    //  construction
    //...
    
    
    //...
    
} catch (...) {
    printRed("\n\t\t\t\tNondispatchable <-- Wind Tests:  FAIL\n");
    std::cout << std::endl;
    
    throw;
}

printGreen("\n\t\t\t\tNondispatchable <-- Wind Tests:  PASS");
std::cout << std::endl;
