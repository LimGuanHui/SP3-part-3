#pragma once


#ifndef AI_EXPORTS
#define AI_API __declspec(dllexport) 
#else
#define AI_API __declspec(dllimport) 
#endif
