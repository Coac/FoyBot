#pragma once

#include "Entity.hpp"

#include <map>

using namespace std;

class Store
{
public:
	static map<unsigned int, Entity*> entities;
};

map<unsigned int, Entity*> Store::entities;
