#pragma once
#include <3ds.h>
#include <iostream>

#include "file.h"

#define MAX_LINES 30 // 30 Lines down fit on screen
#define MAX_WIDTH 50 // 50 Characters across fit on top screen (40 on bottom screen)

#define SCREEN_START_POINT "\x1b[0;0H" // Top Left

#define VERSION_LINE "\x1b[1;0H"
#define CURRENT_DIRECTORY_LINE "\x1b[2;0H"
#define CURRENT_LINE_LINE "\x1b[3;0H"

#define LOG_LINE "\x1b[7;0H"
#define INSTRUCTION_LINE "\x1b[11;0H"

#define EXTENDED_CONTENT_LINE "\x1b[24;0H"

#define DEFAULT_TEXT_COLOUR "\x1b[0m" // Black BG
#define SELECTED_TEXT_COLOUR "\x1b[47;30m" // White BG

#define FULL_CLEAR "\x1b[2K" // Clears entire line

extern PrintConsole topScreen, bottomScreen; 
extern int scroll;
extern bool show_line_numbers;
extern std::string file_name;

void clear_screen(); // Clear entire top screen

void clear_line(std::string line); // Clear singular line

void print_version(std::string version);
void print_current_line(std::string current_line);
void print_current_directory(std::string filename);

void print_log(std::string message);

void print_instructions();

void print_text(std::string str, int count, int current_line); // Called MAX_LINES times in update_screen, drawing each line
void update_screen(File& file, int current_line); // Update screen with contents of file