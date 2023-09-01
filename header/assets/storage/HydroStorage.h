/*
 *  Storage <-- HydroStorage class header file
 */


#ifndef HYDROSTORAGE_H
#define HYDROSTORAGE_H


#include "../../includes.h"
#include "Storage.h"


struct structHydroStorage {
    //...
};


class HydroStorage : public Storage {
    public:
        //  1. attributes
        structHydroStorage struct_hydro_storage;
        
        
        //  2. methods
        HydroStorage(structStorage, structHydroStorage);
        
        //...
        
        ~HydroStorage(void);
};


#endif  /* HYDROSTORAGE_H */
