/*
 *  Testing for Storage <-- HydroStorage class
 */

std::cout << std::endl;
printGold("Testing Storage <-- HydroStorage ... ");
std::cout << std::endl;

try {
    //  construction
    structStorage struct_storage;
    struct_storage.test_flag = true;
    
    structHydroStorage struct_hydro_storage;
    
    HydroStorage test_hydro_storage(struct_storage, struct_hydro_storage);
    
    
    //...
    
} catch (...) {
    printRed("\n\t\t\t\tStorage <-- HydroStorage Tests:  FAIL\n");
    std::cout << std::endl;
    
    throw;
}

printGreen("\n\t\t\t\tStorage <-- HydroStorage Tests:  PASS");
std::cout << std::endl;
