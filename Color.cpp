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