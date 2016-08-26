#pragma once

#include <Windows.h>
#include <iostream> 
#include <io.h>
#include <fcntl.h>

class Console
{
public:
	Console();
	~Console();

	static void create();
	static void setColor(const int &color);
	static void writeLine(const char* fmt, ...);
	static void write(const char* fmt, ...);


	/*

	Name         | Value
				 |
	Black        |   0
	Blue         |   1
	Green        |   2
	Cyan         |   3
	Red          |   4
	Magenta      |   5
	Brown        |   6
	Light Gray   |   7
	Dark Gray    |   8
	Light Blue   |   9
	Light Green  |   10
	Light Cyan   |   11
	Light Red    |   12
	Light Magenta|   13
	Yellow       |   14
	White        |   15

	*/
};

