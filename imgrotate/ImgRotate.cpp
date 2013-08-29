#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>

#include "PngReader.h"
#include "Image.h"
#include "ImageComparer.h"

int main(int argc, char* argv[]) {
	try {
		if (argc < 3)
			throw std::runtime_error("Not enough parameters");

		std::string fname(argv[1]);
		std::string fnameRotated(argv[2]);
		
        size_t passes = 3;
        double crop = 0.5;
		
        if (argc >= 5) {
			std::istringstream is(argv[4]);
			is >> crop;
		}
        
        if (argc >= 4) {
            std::istringstream is(argv[3]);
            is >> passes;
        }

		RealImage imgOrig(read(fname));
		RealImage imgRotated(read(fnameRotated));

		double start = 0.0, end = 360.0, step = 1.0;
		for (size_t i = 1; i <= passes; ++i) {
			auto result = compareInRange(imgOrig, imgRotated, start, end, step, crop);
			std::cout << "Pass " << i << ":\n";
			std::cout << "Angle: " << result.second << " (" << result.first << ")\n";
			start = result.second - step;
			end = result.second + step;
			step /= 100.0;
		}

	} catch (std::exception& ex) {
		std::cerr << ex.what() << "\n";
		return 1;
	}
	return 0;
}

