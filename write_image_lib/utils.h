#ifndef _LODE_PNG_H 
#define _LODE_PNG_H
// A library for outputing images
#include "lodepng.h"

// STL
#include <vector>
#include <string>
#include <iostream>

/**
 * Output a png image of the rendered result.
 */
void resultToPNG(std::string outputFilename, int width, int height, 
    const std::vector<unsigned char>& result);

#endif