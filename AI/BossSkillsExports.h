#pragma once


#ifndef BOSS_SKILL_EXPORTS
#define BOSS_SKILL_API __declspec(dllexport) 
#else
#define BOSS_SKILL_API __declspec(dllimport) 
#endif