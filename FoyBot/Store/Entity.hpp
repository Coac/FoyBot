#pragma once
#include "Coord.hpp"
#include <string>
class Entity
{
private:
	string name;
	unsigned int id;
	unsigned int entityType;
	Coord* pos;
public:
	Entity(unsigned int id, unsigned int entityType, std::string name, Coord* pos);
	string toString() const;
	void setPos(Coord* newPos);
	Coord* getPos() const;
	unsigned int getId() const;
	bool isMob() const;
};

inline Entity::Entity(unsigned int id, unsigned int entityType, std::string name, Coord* pos) {
	this->id = id;
	this->entityType = entityType;
	this->name = name;
	this->pos = pos;
}

inline string Entity::toString() const
{
	char buff[256];
	snprintf(buff, sizeof(buff), "ID=%08X Type=%01X Name=%s Pos=(%s)", this->id, this->entityType, this->name.c_str(), this->pos->toString().c_str());
	return buff;
}

inline void Entity::setPos(Coord* newPos) {
	this->pos = newPos;
}

inline Coord* Entity::getPos() const
{
	return this->pos;
}

inline bool Entity::isMob() const
{
	return this->entityType == 0x5;
}

inline unsigned int Entity::getId() const
{
	return this->id;
}
