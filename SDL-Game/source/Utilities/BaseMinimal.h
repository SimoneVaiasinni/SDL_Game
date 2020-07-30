#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <iostream>
#include <string>
#include <cstdlib>

struct Location {
	int x, y;

	Location() : x{ 0 }, y{ 0 }{}
	Location(int x, int y) : x{ x }, y{ y }{}

	std::string ToString() { return "X: " + std::to_string(x) + " Y: " + std::to_string(y); }
};

struct Dimension {
	int w, h;

	Dimension() :w{ 0 }, h{ 0 }{}
	Dimension(int w, int h) : w{ w }, h{ h }{}

	std::string ToString() { return "Width: " + std::to_string(w) + " Heigth: " + std::to_string(h); }
};