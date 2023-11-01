/*
 *  PGMcpp : PRIMED Grid Modelling Code (in C++) - v2.1
 *
 *  Anthony Truelove MASc, P.Eng.
 *  email:  gears1763@tutanota.com
 *  github: gears1763-2
 *
 *  See license terms
 *
 *  Example project file for PGMcpp, showing a grid which models all
 *  currently implemented assets. This example makes use of PGMcpp directly.
 */


#include "../header/includes.h"
#include "../header/Model_includes.h"
#include "../header/Model.h"


int main(int argc, char** argv) {
    // -------- \/ BEGIN EDITING HERE \/ -------- //
    
    
    // initialize Model object
    structModel model_inputs;
    model_inputs.path_2_load_data =
        "data/input/test/electrical_load_generic_peak-500kW_1yr_dt-1hr.csv";
    
    Model model(model_inputs);
    
    
    // add some renewable resources
    
    //  1. solar
    int solar_resource_key = 1;
     
    model.add1dRenewableResource(
        "solar",
        "data/input/test/solar_GHI_peak-1kWm2_1yr_dt-1hr.csv",
        solar_resource_key
    );
    
    //  2. wind
    int wind_resource_key = 2;
     
    model.add1dRenewableResource(
        "wind",
        "data/input/test/wind_speed_peak-25ms_1yr_dt-1hr.csv",
        wind_resource_key
    );
    
    //  3. tidal
    int tidal_resource_key = 3;
     
    model.add1dRenewableResource(
        "tidal",
        "data/input/test/tidal_speed_peak-3ms_1yr_dt-1hr.csv",
        tidal_resource_key
    );
    
    //  4. wave
    int wave_resource_key = 4;
    
    model.add2dRenewableResource(
        "wave",
        "data/input/test/waves_H_s_peak-8m_T_e_peak-15s_1yr_dt-1hr.csv",
        wave_resource_key
    );
    
    
    // add some renewable (Nondispatchable) assets
    
    //  1. solar
    structNondispatchable nondisp_inputs;
    nondisp_inputs.cap_kW = 150;
    
    structSolar solar_inputs;
    solar_inputs.resource_key = solar_resource_key;
    
    model.addSolar(nondisp_inputs, solar_inputs);
    
    //  2. wind
    nondisp_inputs.cap_kW = 100;
    
    structWind wind_inputs;
    wind_inputs.resource_key = wind_resource_key;
    
    model.addWind(nondisp_inputs, wind_inputs);
    
    //  3. tidal
    nondisp_inputs.cap_kW = 50;
    
    structTidal tidal_inputs;
    tidal_inputs.resource_key = tidal_resource_key;
    
    model.addTidal(nondisp_inputs, tidal_inputs);
    
    //  4. wave
    nondisp_inputs.cap_kW = 50;
    
    structWave wave_inputs;
    wave_inputs.resource_key = wave_resource_key;
    
    model.addWave(nondisp_inputs, wave_inputs);
    
    
    //  add a diesel (Dispatchable, Combustion) asset
    structDispatchable disp_inputs;
    disp_inputs.cap_kW = 1.2 * 500;
    
    structCombustion combustion_inputs;
    
    structDiesel diesel_inputs;
    
    model.addDiesel(disp_inputs, combustion_inputs, diesel_inputs);
    
    
    //  add a Li-ion (Storage, BatteryStorage) asset
    structStorage storage_inputs;
    storage_inputs.cap_kW = 150;
    storage_inputs.cap_kWh = 1000;
    
    structBatteryStorage batt_storage_inputs;
    
    structLiIon liion_inputs;
    
    model.addLiIon(storage_inputs, batt_storage_inputs, liion_inputs);
    
    
    //  run model
    model.run();
    
    
    //  write modelling results to disk
    model.writeResults("example_project");
    
    
    // -------- /\ STOP EDITING HERE /\ -------- //
    
    return 0;
}
