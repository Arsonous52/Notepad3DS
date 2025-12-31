#include "file.h"

void File::add_line(const std::string& new_text) {
    lines.back() = new_text;
    lines.push_back("");
}

void File::edit_line(std::string& new_text, int& line) {
    lines[line] = new_text;
}

void File::insert_line(int& line) {
    lines.insert(lines.begin() + line, "");
}

// Eventually i want to add support for deleting lines
void File::delete_line(int& line) {
    auto line_iter = lines.begin() + line;

    line_iter = lines.erase(line_iter); // Delete line
}

std::vector<int> File::find(std::string search_term) {

    std::vector<int> results;

    int line_number = 0;
    if (search_term.empty())
        return results; // Early return if search term is null

    for(auto& line : this->lines) {
        size_t pos = line.find(search_term);
        if (pos != std::string::npos)
            results.push_back(line_number); // Add the line number to the results vector
        line_number++;
    }
    return results;
}