/*
Copyright (c) 2009-2010 Sony Pictures Imageworks Inc., et al.
All Rights Reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:
* Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in the
  documentation and/or other materials provided with the distribution.
* Neither the name of Sony Pictures Imageworks nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once

/////////////////////////////////////////////////////////////////////////
/// \file
/// Various compile-time defaults are defined here that could, in
/// principle, be redefined if you are using OSL in some particular
/// renderer that wanted things a different way.
/////////////////////////////////////////////////////////////////////////

// Detect if we're C++11.
//
// Note: oslversion.h defined OSL_BUILD_CPP11 to be 1 if OSL was built
// using C++11. In contrast, OSL_CPLUSPLUS_VERSION defined below will be set
// to the right number for the C++ standard being compiled RIGHT NOW. These
// two things may be the same when compiling OSL, but they may not be the
// same if another packages is compiling against OSL and using these headers
// (OSL may be C++11 but the client package may be older, or vice versa --
// use these two symbols to differentiate these cases, when important).
#if (__cplusplus >= 201402L)
#  define OSL_CPLUSPLUS_VERSION  14
#elif (__cplusplus >= 201103L) || _MSC_VER >= 1900
#  define OSL_CPLUSPLUS_VERSION  11
#else
#  error "This version of OSL requires C++11"
#endif

#ifndef OSL_DEBUG
    #ifdef NDEBUG
        #define OSL_DEBUG 0
    #else
        #ifdef _DEBUG
            #define OSL_DEBUG _DEBUG
        #else
            #define OSL_DEBUG 0
        #endif
    #endif
#endif // OSL_DEBUG

#if defined(_MSC_VER)
	#define OSL_PRAGMA(aUnQuotedPragma) __pragma(aUnQuotedPragma)
#else
	#define OSL_PRAGMA(aUnQuotedPragma) _Pragma(#aUnQuotedPragma)
#endif

#if __INTEL_COMPILER >= 1100
	#define OSL_INTEL_PRAGMA(aUnQuotedPragma) OSL_PRAGMA(aUnQuotedPragma)
#else
	#define OSL_INTEL_PRAGMA(aUnQuotedPragma)
#endif

#ifdef __clang__
	#define OSL_CLANG_PRAGMA(aUnQuotedPragma) OSL_PRAGMA(aUnQuotedPragma)
	#define OSL_CLANG_ATTRIBUTE(value) __attribute__((value))
#else
	#define OSL_CLANG_PRAGMA(aUnQuotedPragma)
	#define OSL_CLANG_ATTRIBUTE(value)
#endif

#ifdef OSL_OPENMP_SIMD
	#define OSL_OMP_PRAGMA(aUnQuotedPragma) OSL_PRAGMA(aUnQuotedPragma)
	#ifdef __clang__
		#define OSL_OMP_AND_CLANG_PRAGMA(aUnQuotedPragma) OSL_PRAGMA(aUnQuotedPragma)
		#define OSL_OMP_NOT_CLANG_PRAGMA(aUnQuotedPragma)
#else
		#define OSL_OMP_AND_CLANG_PRAGMA(aUnQuotedPragma)
		#define OSL_OMP_NOT_CLANG_PRAGMA(aUnQuotedPragma) OSL_PRAGMA(aUnQuotedPragma)
	#endif
#else
	#define OSL_OMP_PRAGMA(aUnQuotedPragma)
	#define OSL_OMP_AND_CLANG_PRAGMA(aUnQuotedPragma)
	#define OSL_OMP_NOT_CLANG_PRAGMA(aUnQuotedPragma)
#endif

#ifndef OSL_INLINE
	#if OSL_DEBUG
		#define OSL_INLINE inline
	#else
		#if __INTEL_COMPILER >= 1100 || (defined(_WIN32) || defined(_WIN64))
			#define OSL_INLINE __forceinline
		#else
			#define OSL_INLINE inline __attribute__((always_inline))
		#endif
	#endif
#endif

#ifndef OSL_NOINLINE
	#if (defined(_WIN32) || defined(_WIN64))
		#define OSL_NOINLINE __declspec(noinline)
	#else
		#define OSL_NOINLINE __attribute__((noinline))
	#endif
#endif

#ifndef OSL_RESTRICT
    #if defined(__INTEL_COMPILER)
        #define OSL_RESTRICT __restrict
    #else
        #define OSL_RESTRICT
    #endif
#endif


// Symbol export defines
#include "export.h"

// All the things we need from Imath
#include <OpenEXR/ImathVec.h>
#include <OpenEXR/ImathColor.h>
#include <OpenEXR/ImathMatrix.h>

// All the things we need from OpenImageIO
#include <OpenImageIO/version.h>
#include <OpenImageIO/errorhandler.h>
#include <OpenImageIO/texture.h>
#include <OpenImageIO/typedesc.h>
#include <OpenImageIO/ustring.h>
#include <OpenImageIO/platform.h>

// Extensions to Imath
#include "matrix22.h"

#include "oslversion.h"

OSL_NAMESPACE_ENTER


/// By default, we operate with single precision float.  Change this
/// definition to make a shading system that fundamentally operates
/// on doubles.
/// FIXME: it's very likely that all sorts of other things will break
/// if you do this, but eventually we should make sure it works.
typedef float Float;

/// By default, use the excellent Imath vector, matrix, and color types
/// from the IlmBase package from: http://www.openexr.com
///
/// It's permissible to override these types with the vector, matrix,
/// and color classes of your choice, provided that (a) your vectors
/// have the same data layout as a simple Float[n]; (b) your
/// matrices have the same data layout as Float[n][n]; and (c) your
/// classes have most of the obvious constructors and overloaded
/// operators one would expect from a C++ vector/matrix/color class.
typedef Imath::Vec3<Float>     Vec3;
typedef Imath::Matrix33<Float> Matrix33;
typedef Imath::Matrix44<Float> Matrix44;
typedef Imath::Color3<Float>   Color3;
typedef Imath::Vec2<Float>     Vec2;

typedef Imathx::Matrix22<Float> Matrix22;

/// Assume that we are dealing with OpenImageIO's texture system.  It
/// doesn't literally have to be OIIO's... it just needs to have the
/// same API as OIIO's TextureSystem class, it's a purely abstract class
/// anyway.
using OIIO::TextureSystem;
using OIIO::TextureOpt;

// And some other things we borrow from OIIO...
using OIIO::ErrorHandler;
using OIIO::TypeDesc;
using OIIO::ustring;
using OIIO::ustringHash;
using OIIO::string_view;


#ifndef __has_attribute
#  define __has_attribute(x) 0
#endif

#if OSL_CPLUSPLUS_VERSION >= 14 && __has_attribute(deprecated)
#  define OSL_DEPRECATED(msg) [[deprecated(msg)]]
#elif (defined(__GNUC__) && OIIO_GNUC_VERSION >= 40600) || defined(__clang__)
#  define OSL_DEPRECATED(msg) __attribute__((deprecated(msg)))
#elif defined(__GNUC__) /* older gcc -- only the one with no message */
#  define OSL_DEPRECATED(msg) __attribute__((deprecated))
#elif defined(_MSC_VER)
#  define OSL_DEPRECATED(msg) __declspec(deprecated(msg))
#else
#  define OSL_DEPRECATED(msg)
#endif

// Macro helpers for xmacro include files
#define __OSL_EXPAND(A) A
#define __OSL_XMACRO_ARG1(A,...) A
#define __OSL_XMACRO_ARG2(A,B,...) B
#define __OSL_XMACRO_ARG3(A,B,C,...) C
#define __OSL_XMACRO_ARG4(A,B,C,D,...) D

// Macro helpers to build function names based on other macros
#define __OSL_CONCAT_INDIRECT(A, B) A ## B
#define __OSL_CONCAT(A, B) __OSL_CONCAT_INDIRECT(A,B)
#define __OSL_CONCAT3(A, B, C) __OSL_CONCAT(__OSL_CONCAT(A,B),C)
#define __OSL_CONCAT4(A, B, C, D) __OSL_CONCAT(__OSL_CONCAT3(A,B,C),D)
#define __OSL_CONCAT5(A, B, C, D, E) __OSL_CONCAT(__OSL_CONCAT4(A,B,C,D),E)
#define __OSL_CONCAT6(A, B, C, D, E, F) __OSL_CONCAT(__OSL_CONCAT5(A,B,C,D,E),F)
#define __OSL_CONCAT7(A, B, C, D, E, F, G) __OSL_CONCAT(__OSL_CONCAT6(A,B,C,D,E,F),G)



// During development it can be useful to output extra information
// NOTE:  internal use only
//#define OSL_DEV
#ifdef OSL_DEV
	#define OSL_DEV_ONLY(STUFF) STUFF
#else
	#define OSL_DEV_ONLY(STUFF)
#endif

OSL_NAMESPACE_EXIT
