#pragma once


#ifndef CHARACTER_EXPORTS
#define CHARACTER_API __declspec(dllexport) 
#else
#define CHARACTER_API __declspec(dllimport) 
#endif
