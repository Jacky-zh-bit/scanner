#pragma once
#ifdef SCANNER_EXPORTS
#define SCANNER_API __declspec(dllexport)
#else
#define SCANNER_API __declspec(dllimport)
#endif



extern "C" SCANNER_API int function(
    int function);