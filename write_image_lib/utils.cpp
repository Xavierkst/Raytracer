#include "utils.h"

void resultToPNG(std::string outputFilename, int width, int height, 
    const std::vector<unsigned char>& result)
{
    // Attempt to use lodepng to make the png image
    unsigned error = lodepng::encode(outputFilename, result, width, height);
    if (error)
    {
        throw std::runtime_error(lodepng_error_text(error));
    }

    std::cout << outputFilename << " is saved." << std::endl;
}
