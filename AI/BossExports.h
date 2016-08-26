#pragma once


#ifndef BOSS_EXPORTS
#define BOSS_API __declspec(dllexport) 
#else
#define BOSS_API __declspec(dllimport) 
#endif