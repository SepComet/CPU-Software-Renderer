#include "Color.h"

Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}

Color::Color(uint8_t r, uint8_t g, uint8_t b)
{
	Color(r, g, b, 255);
}

Color::Color(Color& other)
{
	this->r = other.r;
	this->g = other.g;
	this->b = other.b;
	this->a = other.a;
}