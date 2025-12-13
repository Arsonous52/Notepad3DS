# Notepad3DS
Notepad3DS allows you to edit files directly from your 3DS system.

Originally created in 2017 and updated in 2025 for improved functionality and a modernised backend.

## What's New?

Version 1.2.0 (2025 update) includes: 
- Added the ability to show line numbers
- Standardised backend and added additional comments to unclear segments
- Text autofills into keyboard when editing existing line
- Can cancel keyboard prompts, reducing severity of misinputs
- Modified layout of instructions for greater immediate clarity
- Removed Herobrine

## Coming Soon

- Improved file selection through a visualised hierarchy
- Search results can be cycled (returns all matches)
- More advanced file editing
- Support for making directories

## Features
- Line editing and appending
- File opening and saving
- Searching files (returns first match)
- Display line numbers

## Usage
The bottom screen displays instructions, file information and any additional information.

When running Notepad3DS, you will start in the `/3ds` directory. Use `../` to access higher levels in the folder structure.

> **!NOTE** Notepad3DS can open and save files in existing directories but **cannot create new directories.** Saving a file to a non-existent directory will be **unsuccessful**.

> **!NOTE** Each line can hold a maximum of 1024 characters when edited. Lines over 1024 characters **WILL** be truncated, with no way of undoing this.