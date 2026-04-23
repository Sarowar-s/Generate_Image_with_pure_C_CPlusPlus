//
// Copyright 2026 MD Sarowar Hossain
// License Identifier : Apache 2.0
//
//

#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <ctime>

int main() {
    std::srand(std::time(0)); 

    const int w = 512;
    const int h = 512;
    std::ofstream f("king.ppm");

    if (!f.is_open()) {
        std::cerr << "Error: Could not open file.\n";
        return 1;
    }

    // PPM Header (P3: ASCII RGB)
    f << "P3\n" << w << " " << h << "\n255\n";

    // Center coordinates
    const float cx = w / 2.0f;
    const float cy = h / 2.0f;

    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            float dx = x - cx;
            float dy = y - cy;
            float dist_from_center = std::sqrt(dx*dx + dy*dy);

            // define regions based on y and x
            bool is_head = (std::abs(dx) < 80.0f && y > 50 && y < 280);
            bool is_collar = (y >= 280 && y < 310 && std::abs(dx) < 130);
            bool is_body = (y >= 310 && y < 512 && std::abs(dx) < 220);

            bool is_structure = is_head || is_collar || is_body;

            int r = 0, g = 0, b = 0;

            if (is_structure) {
                // Base metallic grey colors
                int grey_base = 65; 
                r = g = b = grey_base;

                // --- Metallic Shading & Texture ---
                // Random texture noise/grain
                int texture_noise = (rand() % 15) - 7;
                r += texture_noise; g += texture_noise; b += texture_noise;

                // Simple directional shading gradient (top-left)
                float shading = 1.0f + (256.0f - y) / 1024.0f + (256.0f - x) / 1024.0f;
                r = static_cast<int>(r * shading); g = static_cast<int>(g * shading); b = static_cast<int>(b * shading);

                // --- Face Features ---
                if (is_head) {
                    // Both Eyes 
                    if (y > 150 && y < 170) {
                        // Left eye on screen
                        if (dx > -55 && dx < -25) { r = 255; g = 160; b = 0; }
                        // Right eye on screen
                        if (dx > 25 && dx < 55) { r = 255; g = 160; b = 0; }
                    }

                    // Scar on the left side (precise overlap over left eye)
                    if (y > 110 && y < 200 && dx > -110 && dx < -10) {
                        if (std::abs(dx+58)<3 || std::abs(dx+72)<3 || std::abs(dx+44)<3) {
                             r = g = b = std::max(0, static_cast<int>(grey_base * shading) - 60); 
                        }
                    }

                    // Tiny fine slit mouth
                    if (y > 240 && y < 244 && std::abs(dx) < 20) {
                        r = g = b = std::max(0, static_cast<int>(grey_base * shading) - 30);
                    }
                }

                // --- Body / Jacket Structure ---
                if (is_body) {
                    // Center 'zipper' line
                    if (std::abs(dx) < 8.0f) {
                        r = g = b = std::max(0, static_cast<int>(grey_base * shading) - 50);
                    }

                    // Integrated Bangladesh Flag Chest Patch
                    // Green background
                    if (std::abs(dx) < 72.0f && y > 360 && y < 465) {
                         r = 0; g = 103; b = 71; // Official BD Green
                        
                         //  shading to the green patch
                         r = static_cast<int>(r * shading); g = static_cast<int>(g * shading); b = static_cast<int>(b * shading);
                    }

                    // Red Circle
                    // center coordinates for circle on patch
                    float dist_from_patch_center = std::sqrt(dx*dx + (y - 412)*(y - 412));
                    if (dist_from_patch_center < 31.0f && std::abs(dx) < 72.0f && y > 360 && y < 465) {
                        r = 210; g = 30; b = 40; // Official BD Red
                        
                        //   shading to the red patch
                        r = static_cast<int>(r * shading); g = static_cast<int>(g * shading); b = static_cast<int>(b * shading);
                    }

                    //  subtle texture grain to body panels (excluding flag patch)
                    if (!(std::abs(dx) < 80.0f && y > 350 && y < 480)) {
                         if (rand()%20 == 0) {
                             r = g = b = static_cast<int>(grey_base * shading) + 15;
                         }
                    }
                }

                // Darken collar slightly to define it
                if (is_collar) {
                    r = g = b = std::max(0, static_cast<int>(grey_base * shading) - 10);
                }

            } else {
                // Background: Dark minimal grey with subtle noise
                int bg_base = 25;
                r = g = b = bg_base;
                int bg_noise = (rand() % 9) - 4;
                r += bg_noise; g += bg_noise; b += bg_noise;
                // Fade gradient outwards
                float bg_shading = std::max(0.5f, 1.0f - dist_from_center / 500.0f);
                r = static_cast<int>(r * bg_shading); g = static_cast<int>(g * bg_shading); b = static_cast<int>(b * bg_shading);
            }

            // Simple clamping to valid 8-bit bounds
            r = std::max(0, std::min(255, r));
            g = std::max(0, std::min(255, g));
            b = std::max(0, std::min(255, b));

            // Write RGB values to file
            f << r << " " << g << " " << b << " ";
        }
        
        f << "\n";
    }

    f.close();
    std::cout << "Forged successfully: king.ppm\n";
    return 0;
}
