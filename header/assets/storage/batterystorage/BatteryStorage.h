/*
 *  PGMcpp : PRIMED Grid Modelling Code (in C++) - v2.1
 *
 *  Anthony Truelove MASc, P.Eng.
 *  email:  gears1763@tutanota.com
 *  github: gears1763-2
 *
 *  See license terms
 *
 *  Header file for the BatteryStorage class, which is a child class of
 *  Storage and the parent class to LiIon.
 *
 *  This class implements the general dynamics of (chemical) battery
 *  storage assets.
 */


#ifndef BATTERYSTORAGE_H
#define BATTERYSTORAGE_H


#include "../../../includes.h"
#include "../Storage.h"


struct structBatteryStorage {
    //  input attributes (structured)
    //  these are the only attributes the user should interact with
    double init_SOC = 0.5;  // SOC = state of charge
    double min_SOC = 0.2;
    double max_SOC = 0.9;
    
    double charge_efficiency = 0.9;
    double discharge_efficiency = 0.9;
};


class BatteryStorage : public Storage {
    public:
        //  modelling and output attributes (unstructured)
        //  the user should not interact with these attributes
        structBatteryStorage struct_battery_storage;
        
        
        //  methods
        BatteryStorage(structStorage, structBatteryStorage, int);

        double getAcceptablekW(double);
        double getAvailablekW(double);
        
        virtual void commitChargekW(double, int);
        virtual void commitDischargekW(double, int);
        
        virtual void writeResults(std::string, int) {return;}
        
        ~BatteryStorage(void);
};


#endif  /* BATTERYSTORAGE_H */
