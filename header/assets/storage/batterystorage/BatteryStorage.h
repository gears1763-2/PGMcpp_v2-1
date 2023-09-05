/*
 *  Storage <-- BatteryStorage class header file
 */


#ifndef BATTERYSTORAGE_H
#define BATTERYSTORAGE_H


#include "../../../includes.h"
#include "../Storage.h"


struct structBatteryStorage {
    double init_SOC = 0.5;  // SOC = state of charge
    double min_SOC = 0.2;
    double max_SOC = 0.9;
    
    double charge_efficiency = 0.9;
    double discharge_efficiency = 0.9;
};


class BatteryStorage : public Storage {
    public:
        //  1. attributes
        structBatteryStorage struct_battery_storage;
        
        
        //  2. methods
        BatteryStorage(structStorage, structBatteryStorage);

        double getAcceptablekW(double);
        double getAvailablekW(double);
        
        virtual void commitChargekW(double, double, int);
        virtual void commitDischargekW(double, double, int);
        
        ~BatteryStorage(void);
};


#endif  /* BATTERYSTORAGE_H */