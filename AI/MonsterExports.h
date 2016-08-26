#pragma once


#ifndef MONSTER_EXPORTS
#define MONSTER_API __declspec(dllexport) 
#else
#define MONSTER_API __declspec(dllimport) 
#endif
