#include "display.h"

void clear_screen() {
    consoleClear();
    std::cout << SCREEN_START_POINT; // Cursor to top left
}

void clear_line(const std::string line) {
    std::cout << line << FULL_CLEAR;
}

void print_version(std::string version) {
    std::cout << SELECTED_TEXT_COLOUR;
    clear_line(VERSION_LINE);
    std::cout << version << DEFAULT_TEXT_COLOUR;
}

void print_current_line(int current_line) {
    clear_line(CURRENT_LINE_LINE);
    std::cout << "Line: " << current_line+1;
}

void print_current_directory(std::string filename) {
    consoleSelect(&bottomScreen);
    clear_line(CURRENT_DIRECTORY_LINE);
    std::string name = !filename.empty() ? filename : "(nothing)";
    std::cout << "Current file: " << name;
}

void print_log(std::string message) {
    consoleSelect(&bottomScreen);
    clear_line(LOG_LINE);
    std::cout << message;
}

void print_instructions() {
    consoleSelect(&bottomScreen);
    std::cout << INSTRUCTION_LINE;
    for (int i = 0; i < 12; i++) { // 14 == amount of lines that make up the instructions
        std::cout << FULL_CLEAR << '\n';
    }
    if (page == SEARCH_PAGE) {
        std::cout <<
        INSTRUCTION_LINE <<
        "[Search Mode]\n\n" <<
        "B: Exit Search Mode\n\n" <<
        "R: Next Match\n" <<
        "L: Previous Match\n";
        return;
    }
    std::cout <<
    INSTRUCTION_LINE <<
    (page == EDIT_PAGE ? "[Edit Mode]\n\n" : "[File Mode]\n\n") <<
    "DPad/CPad: move up/down\n\n";

    // Face buttons
    if (page == EDIT_PAGE) {
        std::cout <<
        "A: Edit Line\n" <<
        "B: Delete Line\n" <<
        "X: Insert Line Above\n" <<
        "Y: Search\n\n";
    }
    else {
        std::cout << 
        "A: Open File\n" <<
        "B: New File\n" <<
        "X: Save\n" <<
        "Y: Save As\n\n";
    }
    std::cout <<
    "R: Switch Editing Mode\n" <<
    "L: Jump to Start/End\n\n" <<
    "SELECT: Toggle Line Numbers\n" <<
    "START: Exit Notepad3DS\n";
}

void print_text(std::string str, int count, int current_line) {

    int visible_digits = std::to_string(count + scroll).length();
    unsigned int width = (show_line_numbers ? MAX_WIDTH - visible_digits - 1 : MAX_WIDTH); // Offset max line length by space taken by line numbers

    if (str.size() > width) { str = str.substr(0, width - 3) + "..."; } // Crop line if too long

    // Set colour based on selected
    std::cout << (count == current_line ? SELECTED_TEXT_COLOUR : DEFAULT_TEXT_COLOUR);

    // Print the line number
    if (show_line_numbers) { std::cout << count + scroll + 1 << "|"; }

    if (str.empty()) { // If line is empty, print a placeholder
        if (count == current_line) std::cout << "(empty line)" << DEFAULT_TEXT_COLOUR;
    }
    else std::cout << str << DEFAULT_TEXT_COLOUR; // Print line and reset to default colour

    if (count < MAX_LINES -1) { std::cout << '\n'; } // New line if not the end, otherwise skip to display more lines on-screen
}

void update_screen(File& file, int current_line) {
    consoleSelect(&bottomScreen);
    print_current_line(current_line);

    consoleSelect(&topScreen);
    clear_screen();
    auto iter = file.lines.begin() + scroll;
    for (int line = 0; iter != file.lines.end() && line < MAX_LINES; line++) {
        print_text(*iter, line, current_line - scroll);
        iter++;
    }
}