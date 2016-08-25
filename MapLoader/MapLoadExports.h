#pragma once


#ifndef MAPLOAD_EXPORTS
#define MAPLOAD_API __declspec(dllexport) 
#else
#define MAPLOAD_API __declspec(dllimport) 
#endif
