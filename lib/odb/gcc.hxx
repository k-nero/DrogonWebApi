// file      : odb/gcc.hxx
// copyright : Copyright (c) 2009-2015 Code Synthesis Tools CC
// license   : GNU GPL v3; see accompanying LICENSE file

#ifndef ODB_GCC_HXX
#define ODB_GCC_HXX

#include <odb/gcc-fwd.hxx>

// GCC header includes to get the plugin and parse tree declarations.
// The order is important and doesn't follow any kind of logic.
//

#include <stdlib.h>
#include <gmp.h>

#include <cstdlib> // Include before GCC poisons some declarations.

// GCC 4.7 can be built using either C or C++ compiler. From 4.8 it
// is always built as C++.
//
#if BUILDING_GCC_MAJOR == 4 && BUILDING_GCC_MINOR <= 6
#  define ODB_GCC_PLUGIN_C
#elif BUILDING_GCC_MAJOR == 4 && BUILDING_GCC_MINOR <= 7
#  include <config.h>
#  ifndef ENABLE_BUILD_WITH_CXX
#    define ODB_GCC_PLUGIN_C
#  endif
#endif

#ifdef ODB_GCC_PLUGIN_C
extern "C"
{
#endif

// GCC's system.h below includes safe-ctype.h which "disables" versions
// from ctype.h. Well, now it's gonna learn how it feels to be disabled.
//
#define SAFE_CTYPE_H

#include <gcc-plugin.h>

#include <config.h>
#include <system.h>
#include <coretypes.h>
#include <tree.h>
#include <real.h>

#include <tm.h>

#include <cpplib.h>
#include <cp/cp-tree.h>

#if BUILDING_GCC_MAJOR > 4 || BUILDING_GCC_MAJOR == 4 && BUILDING_GCC_MINOR > 5
#  include <c-family/c-common.h>
#  include <c-family/c-pragma.h>
#else
#  include <c-common.h>
#  include <c-pragma.h>
#endif

#if BUILDING_GCC_MAJOR > 4 || BUILDING_GCC_MAJOR == 4 && BUILDING_GCC_MINOR > 8
#  include <stringpool.h> // get_identifier
#endif

#include <diagnostic.h>
#include <output.h>

#ifdef ODB_GCC_PLUGIN_C
} // extern "C"
#endif

// Get the value of INTEGER_CST reinterpreted as unsigned.
//
inline unsigned long long
integer_value (tree n)
{
  unsigned long long val;

#if BUILDING_GCC_MAJOR >= 5
  if (tree_fits_uhwi_p (n))
    val = static_cast<unsigned long long> (tree_to_uhwi (n));
  else
    val = static_cast<unsigned long long> (tree_to_shwi (n));
#else
  HOST_WIDE_INT hwl (TREE_INT_CST_LOW (n));
  HOST_WIDE_INT hwh (TREE_INT_CST_HIGH (n));
  unsigned short width (HOST_BITS_PER_WIDE_INT);

  if (hwh == 0)
    val = static_cast<unsigned long long> (hwl);
  else if (hwh == -1 && hwl != 0)
    val = static_cast<unsigned long long> (hwl);
  else
    val = static_cast<unsigned long long> ((hwh << width) + hwl);
#endif

  return val;
}

// Since 4.7.0 the location may point inside a macro rather than at
// the expansion point. We are only really interested in the expansion
// points so we use the real_source_location() wrapper rather than
// DECL_SOURCE_LOCATION() to do this at the source.
//
inline location_t
real_source_location (tree n)
{
  location_t l (DECL_SOURCE_LOCATION (n));

#if BUILDING_GCC_MAJOR > 4 || BUILDING_GCC_MAJOR == 4 && BUILDING_GCC_MINOR > 6
  l = linemap_resolve_location (line_table, l, LRK_MACRO_EXPANSION_POINT, 0);
#endif

  return l;
}

// In 4.9.0 the tree code type was changed from int to enum tree_code.
// the tree_code_name array is also gone with the get_tree_code_name()
// function in its place.
//
#if BUILDING_GCC_MAJOR > 4 || BUILDING_GCC_MAJOR == 4 && BUILDING_GCC_MINOR > 8
typedef enum tree_code gcc_tree_code_type;

inline const char*
gcc_tree_code_name (gcc_tree_code_type tc) {return get_tree_code_name (tc);}
#else
typedef int gcc_tree_code_type;

inline const char*
gcc_tree_code_name (gcc_tree_code_type tc) {return tree_code_name[tc];}
#endif

// Only since GCC 4.7.0.
//
#ifndef LOCATION_COLUMN
#define LOCATION_COLUMN(LOC) (expand_location (LOC).column)
#endif

#ifndef DECL_SOURCE_COLUMN
#define DECL_SOURCE_COLUMN(NODE) LOCATION_COLUMN (DECL_SOURCE_LOCATION (NODE))
#endif

// Only since GCC 4.6.0.
//
#ifndef DECL_CHAIN
#define DECL_CHAIN(x) TREE_CHAIN(x)
#endif

#endif // ODB_GCC_HXX
