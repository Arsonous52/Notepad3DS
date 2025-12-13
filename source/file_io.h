#pragma once
#include <fstream>

#include "file.h"

// Attempts to write contents of file to filename.
// Returns false if failed to write
bool write_file(std::string& filename, File& file);

// Attempts to open filename
// Returns the opened File
File open_file(std::string& filename);