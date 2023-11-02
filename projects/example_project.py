"""
    PGMcpp : PRIMED Grid Modelling Code (in C++) - v2.1

    Anthony Truelove MASc, P.Eng.
    email:  gears1763@tutanota.com
    github: gears1763-2

    See license terms

    Example project file for PGMcpp, showing a grid which models all
    currently implemented assets. This example makes use of the Python 3 bindings
    for PGMcpp.
"""


import os
import sys
sys.path.insert(0, "../pybindings")  # <-- just need to point to directory with PGMcpp extension

import PGMcpp


#   1. construct Model
struct_model = PGMcpp.structModel()
struct_model.path_2_load_data = "../data/input/test/electrical_load_generic_peak-500kW_1yr_dt-1hr.csv"
struct_model.dispatch_mode = PGMcpp.DispatchMode.LOAD_FOLLOWING_IN_ORDER

model = PGMcpp.Model(struct_model)


#   2. load renewable resources

#   2.1. Solar
solar_resource_key = 1
model.add1dRenewableResource(
    "solar",
    "../data/input/test/solar_GHI_peak-1kWm2_1yr_dt-1hr.csv",
    solar_resource_key
)

#   2.2. Tidal
tidal_resource_key = 2
model.add1dRenewableResource(
    "tidal",
    "../data/input/test/tidal_speed_peak-3ms_1yr_dt-1hr.csv",
    tidal_resource_key
)

#   2.3. Wave
wave_resource_key = 3
model.add2dRenewableResource(
    "wave",
    "../data/input/test/waves_H_s_peak-8m_T_e_peak-15s_1yr_dt-1hr.csv",
    wave_resource_key
)

#   2.4. Wind
wind_resource_key = 4
model.add1dRenewableResource(
    "wind",
    "../data/input/test/wind_speed_peak-25ms_1yr_dt-1hr.csv",
    wind_resource_key
)


#   3. add Nondispatchable assets

#   3.1. Solar
struct_nondisp = PGMcpp.structNondispatchable()
struct_nondisp.cap_kW = 150

struct_solar = PGMcpp.structSolar()
struct_solar.resource_key = solar_resource_key

model.addSolar(struct_nondisp, struct_solar)

#   3.2. Wind
struct_nondisp.cap_kW = 100

struct_wind = PGMcpp.structWind()
struct_wind.resource_key = wind_resource_key

model.addWind(struct_nondisp, struct_wind)

#   3.3. Tidal
struct_nondisp.cap_kW = 50;

struct_tidal = PGMcpp.structTidal()
struct_tidal.resource_key = tidal_resource_key

model.addTidal(struct_nondisp, struct_tidal)

#   3.4. Wave
struct_nondisp.cap_kW = 50

struct_wave = PGMcpp.structWave()
struct_wave.resource_key = wave_resource_key

model.addWave(struct_nondisp, struct_wave)


#   4. add Diesel asset
struct_disp = PGMcpp.structDispatchable()
struct_disp.cap_kW = 1.2 * max(model.load_vec_kW)

struct_combustion = PGMcpp.structCombustion()

struct_diesel = PGMcpp.structDiesel()

model.addDiesel(struct_disp, struct_combustion, struct_diesel)


#   5. add LiIon asset
struct_storage = PGMcpp.structStorage()
struct_storage.cap_kW = 150
struct_storage.cap_kWh = 1000

struct_battery_storage = PGMcpp.structBatteryStorage()

struct_liion = PGMcpp.structLiIon()

model.addLiIon(struct_storage, struct_battery_storage, struct_liion)


#   6. run
model.run()


#   7. write outputs
model.writeResults("example_project_py")
