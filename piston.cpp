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
    const int w = 512;
    const int h = 512;
    std::ofstream f("piston.ppm");

    if (!f.is_open()) {
        std::cerr << "Error: Could not open file.\n";
        return 1;
    }

    f << "P3\n" << w << " " << h << "\n255\n";

    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            float dx = x - 256;
            float dy = y - 256;
            float dist = std::sqrt(dx * dx + dy * dy);
            float ang = std::atan2(dy, dx);

            // Shape definition for a heavy mechanical object (piston w/ rings)
            bool piston_body = (std::abs(dx) < 140.0f && std::abs(dy) < 180.0f);
            bool piston_rings = (std::abs(dx) < 160.0f && dy > -150.0f && dy < 50.0f);
            bool piston_rings_groove = (piston_rings && std::abs((int)dy % 30) < 6);

            int r = 0, g = 0, b = 0;

            if (piston_body || (piston_rings && !piston_rings_groove)) {
                // Primary color: Varying dark greys (brushed steel)
                float steel_base = 50.0f + 5.0f * (std::sin(dx * 0.1f) * std::cos(dy * 0.1f));
                
                // textural noise for gritty texture
                float texture_noise = (rand() % 20);
                steel_base += texture_noise;

                // lighting/shadows
                float lighting = 1.0f - (std::sqrt((dx-70)*(dx-70) + (dy+70)*(dy+70)) / 450.0f);
                lighting = std::max(0.2f, lighting);
                
                r = g = b = static_cast<int>(steel_base * lighting);

                // a royal crown-like emblem in the center (darker etching)
                if (std::abs(dx) < 60.0f && dy < -50.0f && dy > -110.0f) {
                    if (std::abs(dx) > 10.0f) {
                        float crown_shape = std::abs(dx) - (dy + 110.0f) * 0.7f;
                        if (crown_shape > 10.0f) {
                           r = g = b = static_cast<int>(std::max(0, r - 50)); // Etch it darker
                        }
                    } else if (dy > -105.0f) {
                        r = g = b = static_cast<int>(std::max(0, r - 50));
                    }
                }

                // Make the piston rings stand out with more reflection
                if (piston_rings && !piston_rings_groove) {
                    r = g = b += 40;
                }

            } else {
                // Background: Dark, blurry workshop abyss
                float glow = std::max(0.0f, 1.0f - (dist) / 350.0f);
                r = static_cast<int>(20 * glow);
                g = static_cast<int>(10 * glow);
                b = static_cast<int>(15 * glow);
            }

            // Simple clamping
            r = std::max(0, std::min(255, r));
            g = std::max(0, std::min(255, g));
            b = std::max(0, std::min(255, b));

            f << r << " " << g << " " << b << " ";
        }
        f << "\n";
    }

    f.close();
    std::cout << "Forged successfully: piston.ppm\n";
    return 0;
}
