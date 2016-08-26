#pragma once

#ifndef MONSTER_ATTRIBUTE_EXPORTS
#define MONSTER_ATTRIBUTE_API __declspec(dllexport)
#else
#define MONSTER_ATTRIBUTE_API __declspec(dllimport)
#endif