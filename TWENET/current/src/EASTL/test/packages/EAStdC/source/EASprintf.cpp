/////////////////////////////////////////////////////////////////////////////
// EASprintf.h
//
// Copyright (c) Electronic Arts. All rights reserved.
///////////////////////////////////////////////////////////////////////////////

#if defined(JN516x) || defined(CPU_JN518X)
#include <EABase/eabase.h>
#include <EAStdC/EASprintf.h>
// use TWENET lib's printf library (see TWENETmcu/printf for details)
extern "C" int vsnprintf_(char* buffer, size_t count, const char* format, va_list va);
#else
#include <EABase/eabase.h>
#include <EAAssert/eaassert.h>
#include <EAStdC/EASprintf.h>
#include <cwchar>
#endif


namespace EA {
namespace StdC {

#if defined(JN516x) || defined(CPU_JN518X)
EASTDC_API int Vsnprintf(char8_t* EA_RESTRICT pDestination, size_t n, const char8_t* EA_RESTRICT pFormat, va_list arguments)
	{ return vsnprintf_(pDestination, n, pFormat, arguments); }
// char16_t, char32_t is not defined so far.
#else
EASTDC_API int Vsnprintf(char8_t* EA_RESTRICT pDestination, size_t n, const char8_t* EA_RESTRICT pFormat, va_list arguments)
	{ return vsnprintf(pDestination, n, pFormat, arguments); }

EASTDC_API int Vsnprintf(char16_t* EA_RESTRICT pDestination, size_t n, const char16_t* EA_RESTRICT pFormat, va_list arguments)
    { return vswprintf((wchar_t*)pDestination, n, (wchar_t*)pFormat, arguments); }

EASTDC_API int Vsnprintf(char32_t* EA_RESTRICT pDestination, size_t n, const char32_t* EA_RESTRICT pFormat, va_list arguments)
 	{ return vswprintf((wchar_t*)pDestination, n, (wchar_t*)pFormat, arguments); }

#if defined(EA_WCHAR_UNIQUE) && EA_WCHAR_UNIQUE
EASTDC_API int Vsnprintf(wchar_t* EA_RESTRICT pDestination, size_t n, const wchar_t* EA_RESTRICT pFormat, va_list arguments)
	{ return vswprintf(pDestination, n, pFormat, arguments); }
#endif
#endif // JN516x || CPU_JN518X

}}
