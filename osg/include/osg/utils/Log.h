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
#ifndef OSG_UTILS_LOG_H
#define OSG_UTILS_LOG_H

#ifdef __cplusplus
extern "C" {
#endif

/// @defgroup Log Log
/// @ingroup Utils

/// @brief Error handling routine. It never returns.
/// This is an internal method. Please use the simplified macro wrappers:
/// * osg_error
/// * osg_assert
/// @ingroup Log
/// @private
/// @param msg The error message.
/// @param file The file name rising the error
/// @param line The line in the file raising the error
/// @param func The name of the function raising the error
void _osg_error(const char * msg, const char * file, const int line, const char * func);

/// @brief Reports an error, making the program to stop executing
/// @ingroup Log
/// @param msg A message to describe the error
#define osg_error(msg) _osg_error((msg), __FILE__, __LINE__, __func__)
/// @brief Asserts the condition, and in case of failure calls osg_error
/// @ingroup Log
/// @param cond The condition
/// @param msg A message to describe the error
#define osg_assert(cond, msg) {if (!(cond)) osg_error(msg); }

#ifdef __cplusplus
}
#endif

#endif
