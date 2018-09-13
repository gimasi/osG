// This file is part of osG.
//
// osG is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// osG is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Foobar.  If not, see <https://www.gnu.org/licenses/>.
//
// osG is also available under a commercial license.
// Please contact GIMASI at info@gimasi.ch for further information.
//
#ifndef OSG_H
#define OSG_H

/// @mainpage osG library
///
/// @section intro_sec Introduction
/// The osG library aims to simplify the firmware programming,
/// by absrtacting the low-level API into a standard interface.
/// The advantages are:
/// * Code reusability: since the API are platform-independent,
///   the application can be easily ported between different boards
/// * Programming easyness: programmers can focus on application logic instead
///   of board-bsp specific issues and API
/// * Increased productivity: low-level details are encapsulateed into the API,
///   avoiding to programmers to waste time to understand how the specific board
///   works
///
/// @section modules_sec Modules
/// There are threee main modules:
/// * **OSG**: the user-level module. It is the only one that should be used directly by applications.
/// * **RTOS**: optional internal module, which extends the features by implementing multitasking support
/// * **Board**: the low-level internal module which wraps the calls to BSP-specific methods
///
/// @section license_sec License
///
/// osG has a dual license scheme:
/// * **GPL v3**: the library is freely available under GPL v3 (or newer) license
/// * **Commercial**: the library is available also with a commercial license,
///   which frees users from GPL restrictions. Please contact <a href="mailto:info@gimasi.ch">GIMASI</a> for pricing
///   and information.
///

/// @defgroup OSG OSG
/// The user-level API

/// @defgroup Base Base
/// @ingroup OSG
/// General data types and base HW methods
#include "osg/osgDataTypes.h"
#include "osg/BaseHw.h"
/// @defgroup Utils Utils
/// @ingroup OSG
/// General utilities
#include "osg/utils.h"
/// @defgroup Drivers Drivers
/// @ingroup OSG
/// Hardware driver classes
#include "osg/drivers.h"



#endif
