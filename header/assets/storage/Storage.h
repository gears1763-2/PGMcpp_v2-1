/*
 *  Storage class header file.
 */


#ifndef STORAGE_H
#define STORAGE_H


#include "../../includes.h"


enum StorageType {
    STORAGE_HYDRO,
    STORAGE_LIION
};


struct structStorage {
    bool print_flag = false;
    bool test_flag = false;
    
    StorageType storage_type = STORAGE_HYDRO;
    
    int n_timesteps = 8760;
};


class Storage {
    public:
        // attributes
        structStorage struct_storage;
        
        //...
        
        
        // methods
        Storage(structStorage);
        
        //...
        
        virtual ~Storage(void);
};


#endif  /* STORAGE_H */
