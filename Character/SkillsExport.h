#pragma once


#ifndef SKILLS_EXPORTS
#define SKILLS_API __declspec(dllexport) 
#else
#define SKILLS_API __declspec(dllimport) 
#endif
