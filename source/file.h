#pragma once
#include <iostream>
#include <vector>

// File struct to store lines in a file
// Uses a vector of strings to store lines.
struct File {
    
    std::vector<std::string> lines;
    bool read_success = false; // Check if file open was successful

    File() { lines.push_back(""); }

    void add_line(const std::string& new_text);

    void edit_line(std::string& new_text, int& line);

    void insert_line(int& line);

    void delete_line(int& line);

    std::vector<int> find(std::string search_term);

    int size() { return lines.size(); }

};