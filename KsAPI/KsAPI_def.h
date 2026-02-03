////////////////////////////////////////////////////////////////////////////////
//
/**
  \file  
  \brief 
*/
//
////////////////////////////////////////////////////////////////////////////////
#pragma once

#if defined(KSAPI_EXPORTS)

#ifdef _MSC_VER
#define KSAPI_FUNC(retType) __declspec(dllexport) retType __cdecl
#define KSAPI_CLASS         __declspec(dllexport)
#else
#define KSAPI_FUNC(retType) __attribute__((visibility("default"))) retType
#define KSAPI_CLASS         __attribute__((visibility("default")))
#endif

#else

#ifdef _MSC_VER
#define KSAPI_FUNC(retType) __declspec(dllimport) retType __cdecl
#define KSAPI_CLASS         __declspec(dllimport)
#else
#define KSAPI_FUNC(retType) retType
#define KSAPI_CLASS
#endif

#endif
