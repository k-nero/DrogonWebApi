// file      : odb/gcc-fwd.hxx
// copyright : Copyright (c) 2009-2015 Code Synthesis Tools CC
// license   : GNU GPL v3; see accompanying LICENSE file

#ifndef ODB_GCC_FWD_HXX
#define ODB_GCC_FWD_HXX

#include <bversion.h>

#if BUILDING_GCC_MAJOR > 4 || BUILDING_GCC_MAJOR == 4 && BUILDING_GCC_MINOR > 8
#  include <limits.h> // CHAR_BIT
#  include <config.h>
#endif

#if BUILDING_GCC_MAJOR >= 5
#  include <stdint.h> // Needed by coretypes.h
#endif

extern "C"
{
// The hwint.h header uses gcc_checking_assert macro from system.h. But
// if we include system.h here, it pulls in all kinds of GCC trouble that
// "poisons" standard C/C++ declarations (see safe-ctype.h for an example).
// Instead we are just going to provide the no-op definition of this macro.
//
#if BUILDING_GCC_MAJOR > 4 || BUILDING_GCC_MAJOR == 4 && BUILDING_GCC_MINOR > 8
#  define gcc_checking_assert(expr) ((void)(0 & (expr)))
#  include <hwint.h>
#  undef gcc_checking_assert
#endif

#include <coretypes.h>

typedef unsigned int source_location; // <line-map.h>
typedef source_location location_t;   // <input.h>
}

#endif // ODB_GCC_FWD_HXX
