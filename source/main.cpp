#include <3ds.h>

#include "behaviours.h"
#include "display.h"
#include "file_io.h"
#include "file.h"

#define VERSION "Notepad3DS Version 1.2.1"

PrintConsole topScreen, bottomScreen;

int scroll = 0;
int curr_line = 0;
bool fast_scroll = false;
bool show_line_numbers = false;
std::string file_name;
unsigned curr_match = 0;
std::vector<int> searchResults;
Pages page = EDIT_PAGE;

void move_down(File& file);
void move_up(File& file);

int main(int argc, char **argv)
{
	gfxInitDefault();

	consoleInit(GFX_TOP, &topScreen);
    consoleInit(GFX_BOTTOM, &bottomScreen);
    consoleSelect(&bottomScreen);
    
    print_version(VERSION);
    print_current_directory(file_name);
    print_instructions();
    
    File file; // Default file

    update_screen(file, curr_line);
	while (aptMainLoop())
	{
		hidScanInput();
		u32 kDown = hidKeysDown();
        u32 kHeld = hidKeysHeld();
        
        // want to add visual hierarchy eventually, example:

        // this
        // |—— is
        // |—— a
        // `—— nested
        //     |—— structure
        //     `—— example

		// Terminate program
		if (kDown & KEY_START) break;

        // Toggle line numbers
        if (kDown & KEY_SELECT) {
            show_line_numbers = !show_line_numbers;
            update_screen(file, curr_line);
        }

        // Buttons do different actions depending on current page
        switch (page) {
            case EDIT_PAGE:
                if ((kDown & KEY_DDOWN) || (kHeld & KEY_CPAD_DOWN)) move_down(file);
                if ((kDown & KEY_DUP) || (kHeld & KEY_CPAD_UP)) move_up(file);
                fast_scroll = kHeld & KEY_L; // If held, allows for jumping to end and start of file

                if (kDown & KEY_A) edit_line(file);
                if (kDown & KEY_B) delete_line(file);
                if (kDown & KEY_X) insert_line(file);
                if (kDown & KEY_Y) searchResults = search_file(file);
                if (kDown & KEY_R) { page = FILE_PAGE; print_instructions(); }
                break;

            case FILE_PAGE:
                if ((kDown & KEY_DDOWN) || (kHeld & KEY_CPAD_DOWN)) move_down(file);
                if ((kDown & KEY_DUP) || (kHeld & KEY_CPAD_UP)) move_up(file);
                fast_scroll = kHeld & KEY_L; // If held, allows for jumping to end and start of file

                if (kDown & KEY_A) open_file(file);
                if (kDown & KEY_B) new_file(file);
                if (kDown & KEY_X) save_file(file);
                if (kDown & KEY_Y) save_as_file(file);
                if (kDown & KEY_R) { page = EDIT_PAGE; print_instructions(); }
                break;

            case SEARCH_PAGE:
                if (kDown & KEY_B) {
                    page = EDIT_PAGE;
                    print_instructions();
                    print_log("");
                }
                if (kDown & KEY_L && curr_match > 0) cycle_matches(file, searchResults, --curr_match);
                if (kDown & KEY_R && curr_match < searchResults.size()-1) cycle_matches(file, searchResults, ++curr_match);
                break;
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