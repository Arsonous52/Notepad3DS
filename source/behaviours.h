#pragma once
#include <3ds.h>
#include <iostream>

#include "display.h"
#include "file_io.h"

#define BUFFER_SIZE 1025 // Notepad's line limit + 1 (for null character \0)
#define LINES_UNTIL_SCROLL 28 // Begin scrolling past this line

extern int curr_line;
extern unsigned curr_match;

// SEARCH PAGE COMMAND
void cycle_matches(File& file, std::vector<int>& results, int match);

// EDIT PAGE COMMANDS
void edit_line(File& file);
void delete_line(File& file);
void insert_line(File& file);
std::vector<int> search_file(File& file);

// FILE PAGE COMMANDS
void save_file(File& file);
void new_file(File& file);
void open_file(File& file);
void save_as_file(File& file);