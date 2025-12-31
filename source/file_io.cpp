#include "file_io.h"

bool write_file(std::string& filename, File& file) {
    std::ofstream new_file(filename);

    for (std::string line : file.lines) {
        new_file << line;
        if (line != file.lines.back()) { new_file << '\n'; } // Dont create new line if its the end of the file
    }
    
    new_file.close();
    return !new_file.bad();
}

File open_file(std::string& filename) {
    std::ifstream file_open(filename);
    File file;
    file.read_success = false;
    
    if (file_open.is_open()) {
        std::string line;
        while (std::getline(file_open, line)) {
            file.add_line(line);
        }
        file.read_success = true;
        file_open.close();
    }
    return file;
}