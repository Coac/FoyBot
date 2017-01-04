#pragma once

class Coord
{
private:
	unsigned int x;
	unsigned int y;
public:
	Coord(unsigned int const x1, unsigned int const y1);
	unsigned int getX() const;
	unsigned int getY() const;
	void set(unsigned int x1, unsigned int y1);
	void setX(unsigned int x1);
	void setY(unsigned int y1);
	void operator=(const Coord& c);
	bool operator==(const Coord& a) const;
	string toString() const;

};

inline Coord::Coord(unsigned int x1, unsigned int y1)
{
	this->x = x1;
	this->y = y1;
}

inline void Coord::set(unsigned int x1, unsigned int y1)
{
	this->x = x1;
	this->y = y1;
}

inline void Coord::setX(unsigned int x1)
{
	this->x = x1;
}

inline void Coord::setY(unsigned int y1)
{
	this->y = y1;
}

inline unsigned int Coord::getX() const
{
	return x;
}

inline unsigned int Coord::getY() const
{
	return y;
}

inline void Coord::operator=(const Coord& c)
{
	x = c.getX();
	y = c.getY();
}

inline bool Coord::operator==(const Coord& a) const
{
	return (getX() == a.getX() && getY() == a.getY());
}

inline string Coord::toString() const
{
	char buff[256];
	snprintf(buff, sizeof(buff), "X=%d Y=%d", this->x, this->y);
	return buff;
}
