#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;
using namespace std;


int main() {

    // Replace with your target disk/path
    fs::path diskPath = "F:\\"; 
    cout<<"HGhfhfhfh"<<endl;
      
       try {
        if (fs::exists(diskPath) && fs::is_directory(diskPath)) {
            std::cout << "Iterating top-level of: " << diskPath << "\n\n";
            
            for (const auto& entry : fs::directory_iterator(diskPath)) {
                // entry.path() gives the full path
                std::cout << entry.path().string() << "\n";
            }
        } else {
            std::cerr << "Path does not exist or is not a directory.\n";
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

      
   
    return 0;
}