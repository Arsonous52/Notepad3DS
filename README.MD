# Notepad3DS
Notepad3DS allows you to edit files directly from your 3DS system.

Originally created in 2017 and updated in 2025 for improved functionality and a modernised backend.

## What's New?

**Version 1.2.1 includes:**
- More advanced file editing
- Search results can be cycled
- Fixed a bug where saved files would hold all content in 1 line

## Coming Soon
- Improved file selection through a visualised hierarchy
- Support for making directories

## Features
- Line editing, inserting and deleting
- File creation, opening and saving
- File searching
- Display line numbers

## Usage
The bottom screen displays instructions, file information and any additional information.

When running Notepad3DS, you will start in the `/3ds` directory. Use `../` to access higher levels in the folder structure.

> [!NOTE] Notepad3DS can open and save files in existing directories but **cannot create new directories.** Saving a file to a non-existent directory will be **unsuccessful**.

> [!NOTE] Each line can hold a maximum of 1024 characters when edited. Lines over 1024 characters **WILL** be truncated, with no way of undoing this.