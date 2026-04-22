//
// Copyright 2026 MD Sarowar Hossain
// License Identifier : Apache 2.0
//
//


#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>

int main() {
    const int width = 512;
    const int height = 512;
    std::ofstream img("image.ppm");

    if (!img.is_open()) {
        std::cerr << "Failed to open file for writing.\n";
        return 1;
    }

    // PPM Header (P3: ASCII RGB)
    img << "P3\n" << width << " " << height << "\n255\n";

    // Center coordinates
    const double cx = width / 2.0;
    const double cy = height / 2.0;

	// The loop fills the pixels with certain numbers that represents certain color.(Traversally)
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            double dx = x - cx;
            double dy = y - cy;
            double distance = std::sqrt(dx * dx + dy * dy);
            double angle = std::atan2(dy, dx);

            // Procedural math for a mechanical "gear" shape with 8 teeth
            double gear_radius = 130.0 + 25.0 * std::sin(8.0 * angle) + 10.0 * std::cos(16.0 * angle);

            int r = 0, g = 0, b = 0;

            if (distance < gear_radius) {
                // Inside the engine core: Golden/Orange gradient
                double intensity = 1.0 - (distance / gear_radius);
                
                r = 255; 
                g = static_cast<int>(130 + 120 * intensity); // Shifts from orange edge to yellow center
                b = static_cast<int>(40 * intensity);

                // Central "reactor core" rings
                if (distance < 40.0) {
                    r = 255; g = 255; b = 255; // White-hot center
                } else if (distance < 55.0) {
                    r = 30; g = 20; b = 20; // Dark mechanical separation ring
                }
            } else {
                // Background: Deep dark abyss with a subtle ambient glow from the engine
                double glow = std::max(0.0, 1.0 - (distance - gear_radius) / 250.0);
                r = static_cast<int>(45 * glow);
                g = static_cast<int>(15 * glow);
                b = static_cast<int>(25 * glow);
            }

            // Clamp colors to valid 8-bit bounds
            r = std::clamp(r, 0, 255);
            g = std::clamp(g, 0, 255);
            b = std::clamp(b, 0, 255);

            img << r << " " << g << " " << b << " ";
        }
        img << "\n";
    }

    img.close();
    std::cout << "Forged successfully: image.ppm\n";
    return 0;
}
