#pragma once


#ifndef ATTRIBUTE_EXPORTS
#define ATTRIBUTE_API __declspec(dllexport) 
#else
#define ATTRIBUTE_API __declspec(dllimport) 
#endif
