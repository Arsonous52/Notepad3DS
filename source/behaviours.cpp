#include "behaviours.h"

static char buf[BUFFER_SIZE];
SwkbdState swkbd;

// Invokes keyboard, returning button presssed
SwkbdButton get_keyboard(const char* initText, const char* hint) {
    buf[0] = '\0'; // Clear buffer
    swkbdInit(&swkbd, SWKBD_TYPE_NORMAL, 2, -1);
    swkbdSetValidation(&swkbd, SWKBD_ANYTHING, SWKBD_ANYTHING, 2);
    swkbdSetFeatures(&swkbd, SWKBD_DARKEN_TOP_SCREEN);
    swkbdSetInitialText(&swkbd, initText);
    swkbdSetHintText(&swkbd, hint);
    return swkbdInputText(&swkbd, buf, sizeof(buf));
}

// Raises a dialog prompt, halting all other logic until answered. returns user's choice
bool confirm(std::string text) {
    print_log(text + " [A] Yes [B] No");

    while (aptMainLoop()) {
        hidScanInput();
        u32 kDown = hidKeysDown();

        if (kDown & KEY_A) return true;
        if (kDown & KEY_B) return false;

        gfxFlushBuffers();
        gfxSwapBuffers();
        gspWaitForVBlank();
    }
    return false;
}

void cycle_matches(File& file, std::vector<int>& results, int match) {
    print_instructions();
    unsigned result = results[match];

    print_log("Showing " + std::to_string(match+1) + " of " + std::to_string(results.size()) + " matches");
    curr_line = result;
    if (curr_line > LINES_UNTIL_SCROLL) {
        scroll = curr_line - LINES_UNTIL_SCROLL;
    }
    update_screen(file, curr_line);
}

// EDIT PAGE
void edit_line(File& file) {
    SwkbdButton button = get_keyboard(file.lines[curr_line].c_str(), "Input text here.");
    
    // Check if user confirmed input (do nothing if user cancelled)
    if (button == SWKBD_BUTTON_RIGHT) {
        std::string new_text = buf;

        // Add new line if final line, otherwise, just edit
        if (curr_line == file.size() -1)
            file.add_line(new_text);
        else
            file.edit_line(new_text, curr_line);

        update_screen(file, curr_line);
    }
}

void delete_line(File& file) {
    bool choice = confirm("Delete Line?");

    // Confirm choice, dont allow the user to delete the final line
    if (choice && curr_line != file.size() -1) {
        file.delete_line(curr_line);
        print_log("Line deleted successfully");
        update_screen(file, curr_line);
    } else {
        print_log("Did not delete line");
    }
}

void insert_line(File& file) {
    file.insert_line(curr_line);
    update_screen(file, curr_line);
}

std::vector<int> search_file(File& file) {
    SwkbdButton button = get_keyboard(NULL, "Input search term here."); // Get term to search for
    std::string searchTerm = buf;
    std::vector<int> results;

    // Check if user confirmed input (do nothing if user cancelled)
    if (button == SWKBD_BUTTON_RIGHT) {
        results = file.find(searchTerm);
        if (results.empty())
            if (searchTerm.empty())
                print_log("No search term provided");
            else
                print_log("Could not find \"" + searchTerm + "\"");
        else {
            page = SEARCH_PAGE;
            curr_match = 0;
            cycle_matches(file, results, curr_match);
            return results;
        }
    }
    return results;
}

// FILE PAGE
void save_file(File& file) {
    if (file_name.empty()) {
        save_as_file(file);
        return;
    }

    bool choice = confirm("Save file?");
    if (choice) {
        bool success = write_file(file_name, file); // Write out characters to file
        if (success)
            print_log("File saved successfully");
        else
            print_log("Failed to save");
    } else
        print_log("Save aborted");
    
}

void new_file(File& file) {
    bool choice = confirm("Create new file?");

    if (choice) { // New file created
        curr_line = scroll = 0;
        file_name.clear();
        print_current_directory(file_name);
        print_log("File created.");
        file = File();
        update_screen(file, curr_line);
    } else { // New file aborted
        print_log("File creation aborted.");
    }
}

void open_file(File& file) {
    curr_line = 0;
    scroll = 0;

    buf[0] = '\0'; // Clear buffer
    SwkbdButton button = get_keyboard(NULL, "Input filename to open."); // Get file name
    
    // Check if user confirmed input (do nothing if user cancelled)
    if (button == SWKBD_BUTTON_RIGHT) {
        std::string filename = buf;

        if (filename.empty()) {
            print_log("No filename provided.");
        } else if (button != SWKBD_BUTTON_NONE) {
            File oldfile = file;
            file = open_file(filename);
            
            if (file.read_success) {
                file_name = filename;
                print_current_directory(file_name);
                print_log("Successfully opened " + filename);
                update_screen(file, curr_line);
            } else {
                file = oldfile;
                update_screen(file, curr_line);
                print_log("Failed to open " + filename);
            }
        }
    }
}

void save_as_file(File& file) {
    SwkbdButton button = get_keyboard(NULL, "Save as filename"); // Get file name
    // Check if user confirmed input (do nothing if user cancelled)
    if (button == SWKBD_BUTTON_RIGHT) {
        std::string filename = buf;
        if (filename.empty()) {
            print_log("No filename provided.");
        } else if (button != SWKBD_BUTTON_NONE) {
            bool success = write_file(filename, file); // Write out characters to file
            if (success) {
                file_name = filename;
                print_current_directory(file_name);
                print_log("File written to " + filename);
            } else 
                print_log("Failed to write " + filename);
        }
    }
}