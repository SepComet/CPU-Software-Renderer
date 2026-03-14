#pragma once

#include<cstdint>

class Color
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;

public:
	Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
	Color(uint8_t r, uint8_t g, uint8_t b);
	Color(Color& other);

	uint8_t GetR() const { return r; }
	void SetR(const uint8_t r) { this->r = r; }

	uint8_t GetG() const { return g; }
	void SetG(const uint8_t g) { this->g = g; }

	uint8_t GetB() const { return b; }
	void SetB(const uint8_t b) { this->b = b; }

	uint8_t GetA() const { return a; }
	void SetA(const uint8_t a) { this->a = a; }

	operator uint32_t() const
	{
		return (r << 24) | (g << 16) | (b << 8) | a;
	}

};