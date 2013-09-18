///////////////////////////////////////////////////////////////////////////////
// Plugin architecture example                                               //
//                                                                           //
// This code serves as an example to the plugin architecture discussed in    //
// the article and can be freely used                                        //
///////////////////////////////////////////////////////////////////////////////
#ifndef MYENGINE_CONFIG_H
#define MYENGINE_CONFIG_H

// ------------------------------------------------------------------------- //

// Platform recognition
#if defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_APP)
  #define MYENGINE_WINRT 1
#elif defined(WIN32) || defined(_WIN32)
  #define MYENGINE_WIN32 1
#else
  #define MYENGINE_LINUX 1
#endif

// ------------------------------------------------------------------------- //

// Decides whether symbols are imported from a dll (client app) or exported to
// a dll (MyEngine library). The MYENGINE_SOURCE symbol is defined by
// all source files of the library, so you don't have to worry about a thing.
#if defined(_MSC_VER)

  #if defined(MYENGINE_STATICLIB)
    #define MYENGINE_API
  #else
    #if defined(MYENGINE_SOURCE)
      // If we are building the DLL, export the symbols tagged like this
      #define MYENGINE_API __declspec(dllexport)
    #else
      // If we are consuming the DLL, import the symbols tagged like this
      #define MYENGINE_API __declspec(dllimport)
    #endif
  #endif

#elif defined(__GNUC__)

  #if defined(MYENGINE_STATICLIB)
    #define MYENGINE_API
  #else
    #if defined(MYENGINE_SOURCE)
      #define MYENGINE_API __attribute__ ((visibility ("default")))
    #else
      // If you use -fvisibility=hidden in GCC, exception handling and RTTI
      // would break if visibility wasn't set during export _and_ import
      // because GCC would immediately forget all type infos encountered.
      // See http://gcc.gnu.org/wiki/Visibility
      #define MYENGINE_API __attribute__ ((visibility ("default")))
    #endif
  #endif

#else

  #error Unknown compiler, please implement shared library macros

#endif

// ------------------------------------------------------------------------- //

#endif // MYENGINE_CONFIG_H
