#ifndef _COLOR_H_
#define _COLOR_H_

class Color {
private:
	double r, g, b, special;
public:
	
	Color();
	Color(float r, float g, float b, float special);

	// Color getters
	double getColorR();
	double getColorG();
	double getColorB();
	double getColorSpecial();

	// Color Setters 
	void setColorR(double val);
	void setColorG(double val);
	void setColorB(double val);
	void setColorSpecial(double val);

	// Color Math
	Color operator+ (const Color &c);
	Color operator* (const Color &c);
	Color operator* (const double scalar);
	// Average the colors with the one passed in
	Color colorAvg (const Color& c);

	Color colorClip();
};

#endif