#include <3ds.h>

#include "display.h"
#include "file_io.h"
#include "file.h"

#define BUFFER_SIZE 1025 // Notepad's line limit + 1 (for null character: \0)
#define LINES_UNTIL_SCROLL 28 // Begin scrolling past this line

#define VERSION "Notepad3DS Version 1.2.0"

PrintConsole topScreen, bottomScreen;
SwkbdState swkbd;

int scroll = 0;
int curr_line = 0;
bool fast_scroll = false;
bool show_line_numbers = false;
std::string file_name = "(empty)";
static char buf[BUFFER_SIZE];

void move_down(File& file);
void move_up(File& file);
SwkbdButton get_keyboard(const char* initText, const char* hint);
bool confirm(std::string text);

int main(int argc, char **argv)
{
	gfxInitDefault();
    swkbdInit(&swkbd, SWKBD_TYPE_NORMAL, 2, -1);
    swkbdSetValidation(&swkbd, SWKBD_ANYTHING, SWKBD_ANYTHING, 2);
    swkbdSetFeatures(&swkbd, SWKBD_DARKEN_TOP_SCREEN);

	consoleInit(GFX_TOP, &topScreen);
    consoleInit(GFX_BOTTOM, &bottomScreen);
    consoleSelect(&bottomScreen);
    
    print_instructions();
    print_version(VERSION);
    
    File file; // Default file

    update_screen(file, curr_line);
	while (aptMainLoop())
	{
		hidScanInput();
		u32 kDown = hidKeysDown();
        u32 kHeld = hidKeysHeld();

		// Terminate program
		if (kDown & KEY_START)
			break;

        // Edit current line
        if (kDown & KEY_A) {
            buf[0] = '\0'; // Clear buffer
            SwkbdButton button = get_keyboard(file.lines[curr_line].c_str(), "Input text here.");
            
            // Check if user confirmed input (do nothing if user cancelled)
            if (button == SWKBD_BUTTON_RIGHT)
            {
                std::string new_text = buf;

                // Add new line if final line, otherwise, just edit
                if (curr_line == file.size() -1)
                    file.add_line(new_text);
                else
                    file.edit_line(new_text, curr_line);

                update_screen(file, curr_line);
            }
        }

        // Create new file
        if (kDown & KEY_B) {
            bool choice = confirm("Create new file?");

            if (choice) { // New file created
                file = File();
                curr_line = 0;
                scroll = 0;
                file_name = "(empty)";
                update_screen(file, curr_line);
                print_log("File created.");
            } else { // New file aborted
                print_log("File creation aborted.");
            }
        }

        // Save file
        if (kDown & KEY_X) {

            buf[0] = '\0'; // Clear buffer
            SwkbdButton button = get_keyboard(NULL, "Save as filename"); // Get file name

            // Check if user confirmed input (do nothing if user cancelled)
            if (button == SWKBD_BUTTON_RIGHT) {
                std::string filename = buf;
                
                if (filename.empty()) {
                    print_log("No filename provided.");
                } else if (button != SWKBD_BUTTON_NONE) {
                    bool success = write_file(filename, file); // Write out characters to file
                    if (success) 
                        print_log("File written to " + filename);
                    else 
                        print_log("Failed to write " + filename);
                }
            }
        }
        
        // want to add visual hierarchy eventually, example:

        // this
        // |—— is
        // |—— a
        // `—— nested
        //     |—— structure
        //     `—— example

        // Open file
        if (kDown & KEY_Y) {
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
                        update_screen(file, curr_line);
                        print_log("Successfully opened " + filename);
                    } else {
                        file = oldfile;
                        update_screen(file, curr_line);
                        print_log("Failed to open " + filename);
                    }
                }
            }
        }

        // Search file
        if (kDown & KEY_R) {
            buf[0] = '\0'; // Clear buffer
            SwkbdButton button = get_keyboard(NULL, "Input search term here."); // Get term to search for
            std::string searchTerm = buf;

            // Check if user confirmed input (do nothing if user cancelled)
            if (button == SWKBD_BUTTON_RIGHT) {
                std::vector<int> results = file.find(searchTerm);
                if (results.empty())
                    if (searchTerm.empty())
                        print_log("No search term provided");
                    else
                        print_log("Could not find \"" + searchTerm + "\"");
                else {
                    int result = results[0]; // Eventually change this so that we can cycle through all results
                    print_log("Found \"" + searchTerm + "\" at line " + std::to_string(result + 1));
                    curr_line = result;
                    if (curr_line > LINES_UNTIL_SCROLL) {
                        scroll = curr_line - LINES_UNTIL_SCROLL;
                    }
                    update_screen(file, curr_line);
                }
            }
        }

        // If held, allows for jumping to end and start of file
        fast_scroll = kHeld & KEY_L;

        // Toggle line numbers
        if (kDown & KEY_SELECT) {
            show_line_numbers = !show_line_numbers;
            update_screen(file, curr_line);
        }

        // Move a line down (towards bottom of screen)
        if ((kDown & KEY_DDOWN) || (kHeld & KEY_CPAD_DOWN)) {
            move_down(file); 
        }

        // Move a line up (towards top of screen)
        if ((kDown & KEY_DUP) || (kHeld & KEY_CPAD_UP)) {
            move_up(file);
        }

		// Flush and swap framebuffers
		gfxFlushBuffers();
		gfxSwapBuffers();
		gspWaitForVBlank();
	}
	gfxExit();
	return 0;
}

//Move a line down (towards bottom of screen)
void move_down(File& file) {
    if (file.size() == 0) return;
    if (curr_line < file.size() - 1) { // Check if already at bottom

        // Jump if holding L
        if (fast_scroll) {
            curr_line = file.size()-1;
            scroll = curr_line - LINES_UNTIL_SCROLL;
        }
        // Normal scroll
        else {
            if ( (curr_line - scroll >= LINES_UNTIL_SCROLL) && (curr_line < file.size() ) ) {
                scroll++;
            }
            curr_line++;
        }
        update_screen(file, curr_line);
    }
}

//Move a line up (towards top of screen)
void move_up(File& file) {
    if (curr_line != 0) { // Check if already at top

        //Jump if holding L
        if (fast_scroll) {
            curr_line = 0;
            scroll = 0;
        }
        // Normal scroll
        else {
            curr_line--;
            if (curr_line - scroll <= 0 && scroll != 0) {
                scroll--;
            }
        }
        update_screen(file, curr_line);
    }
}

// Invokes keyboard, returning button presssed
SwkbdButton get_keyboard(const char* initText, const char* hint) {
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