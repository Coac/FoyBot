#pragma once

#include "Entity.hpp"

#include <map>

using namespace std;

class Store
{
public:
	static map<unsigned int, Entity*> entities;
	static void printEntities();
};

map<unsigned int, Entity*> Store::entities;

inline void Store::printEntities() {
	cout << endl  << "Entities : " << endl;
	for (auto const& entity : Store::entities)
	{
		cout
			<< entity.first  // (key)
			<< ':'
			<< entity.second->toString() // value 
			<< endl;
	}
}