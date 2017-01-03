#pragma once
#include "Coord.hpp"
#include <string>
class Entity
{
private:
	string name;
	unsigned int id;
	Coord* pos;
public:
	Entity(unsigned int id, std::string name, Coord* pos);
	string toString();
};

Entity::Entity(unsigned int id, std::string name, Coord* pos) {
	this->id = id;
	this->name = name;
	this->pos = pos;
}

string Entity::toString() {
	char buff[256];
	snprintf(buff, sizeof(buff), "ID=%08X Name=%s Pos=(%s)", this->id, this->name.c_str(), this->pos->toString().c_str());
	return buff;
}