#pragma once

#include "zmsg_cmm.hpp"

namespace zmsg {

/**
 * \brief typedefs
 */
typedef signed char CHAR;
typedef unsigned char UCHAR;

typedef signed short SHORT;
typedef unsigned short USHORT;

typedef int8_t INT8;
typedef uint8_t UINT8;
typedef int16_t INT16;
typedef uint16_t UINT16;
typedef int32_t INT32;
typedef uint32_t UINT32;

typedef signed long LONG;
typedef unsigned long ULONG;

typedef float FLOAT;

typedef ULONG BOOL;

typedef void VOID;

} /* namespace zmsg */


/**
 * \brief service state
 */
enum class svc_state_t : uint16_t {
	fs_reset,
	fs_ready,
	fs_clring,
	///
	heat_idle,
};

/**
 * \brief motor id
 */
enum motorId_t : uint8_t {
	LZ = 0,	// left z
	RZ,	// right z
	X,	// x
	Y,	// y
	NUM,	// total number
};

