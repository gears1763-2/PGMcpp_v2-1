/*
 *  PGMcpp : PRIMED Grid Modelling Code (in C++) - v2.1
 *
 *  Anthony Truelove MASc, P.Eng.
 *  email:  gears1763@tutanota.com
 *  github: gears1763-2
 *
 *  See license terms
 *
 *  Header file for all PGMcpp includes, as needed by the Model class.
 */


#ifndef MODEL_INCLUDES_H
#define MODEL_INCLUDES_H


#include "assets/nondispatchable/Nondispatchable.h"
#include "assets/nondispatchable/Solar.h"
#include "assets/nondispatchable/Tidal.h"
#include "assets/nondispatchable/Wave.h"
#include "assets/nondispatchable/Wind.h"

#include "assets/dispatchable/Dispatchable.h"
#include "assets/dispatchable/Hydro.h"
#include "assets/dispatchable/combustion/Combustion.h"
#include "assets/dispatchable/combustion/Diesel.h"

#include "assets/storage/Storage.h"
#include "assets/storage/batterystorage/BatteryStorage.h"
#include "assets/storage/batterystorage/LiIon.h"


#endif  /* MODEL_INCLUDES_H */
