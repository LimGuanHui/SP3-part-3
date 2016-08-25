#pragma once


#ifndef MOVEMENT_EXPORTS
#define MOVEMENT_API __declspec(dllexport) 
#else
#define MOVEMENT_API __declspec(dllimport) 
#endif
