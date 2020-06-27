#include "Color.h"

Color::Color() {
	this->r = 0.5f;
	this->g = 0.5f;
	this->b = 0.5f;
	this->special = 0.0f;
}

Color::Color(float r, float g, float b, float special) {
	this->r = r;
	this->g = g;
	this->b = b;
	this->special = special;
}

double Color::getColorR() {
	return this->r;
}
double Color::getColorG() {
	return this->g;
}
double Color::getColorB() {
	return this->b;

}

double Color::getColorSpecial() {
	return this->special;

}

void Color::setColorR(double val) {
	this->r = val;
}
void Color::setColorG(double val) {
	this->g = val;
}
void Color::setColorB(double val) {
	this->b = val;
}
void Color::setColorSpecial(double val) {
	this->special = val;
}

Color Color::operator+(const Color &c)
{
	Color newColor;
	newColor.r = this->r + c.r;
	newColor.g = this->g + c.g;
	newColor.b = this->b + c.b;
	newColor.special = this->special;

	return newColor;
}

Color Color::operator*(const Color &c)
{
	Color newColor;
	newColor.r = this->getColorR() * c.r;
	newColor.g = this->getColorG() * c.g;
	newColor.b = this->getColorB() * c.b;
	newColor.special = this->special;

	return newColor;
}

Color Color::operator*(const double scalar)
{
	Color newColor;
	newColor.r = this->getColorR() * scalar;
	newColor.g = this->getColorG() * scalar;
	newColor.b = this->getColorB() * scalar;
	newColor.special = this->special;

	return newColor;
}

Color Color::colorAvg(const Color& c)
{
	Color newColor;
	newColor.r = ( this->getColorR() + c.r ) / 2.0f;
	newColor.g = (this->getColorB() + c.g) / 2.0f;
	newColor.b = (this->getColorR() + c.b) / 2.0f;
	newColor.special = this->special;

	return newColor;
}


