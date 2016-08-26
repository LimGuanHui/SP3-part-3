#pragma once


#ifndef BOSS_ATTRIBUTE_EXPORTS
#define BOSS_ATTRIBUTE_API __declspec(dllexport) 
#else
#define BOSS_ATTRIBUTE_API __declspec(dllimport) 
#endif