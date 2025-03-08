
## Structure of repo

- **./** (directory): contains all directories and core files to run the whole game.

    - **./menu** (directory): contains all menus and one main file named (menu.c) to run all menus.
        * each menu has its own directory.
        * each directory has 2 files (.c & .h)
        * header.h contains base functions shared by all menus (**TODO: turn it into menu.h (functions only needed for menus (i.e buttons))**)
        * one Makefile that compiles all menus into one executable --> **./bin/menu**

    - **./core** (directory): contains all core components of the game and one main file (core.c) to run all game components
        * each component has its own directory.
        * (under development: component directory structure still undefined)
        * core.h contains base functions shared by all core components
        * one Makefile that compiles all core game components into one executable --> **./bin/core**

    - **./controller** (directory): ***Not There Yet***

    - **./assets** (directory): contains all media files (png, mp3, wav)
        - ./assets/buttons: contains button png files
        - ./assets/sounds: contains sound wav files
        - ./assets/music: contains music mp3 files
        - ./assets/game: contains background & characters jpeg files
        - ./assets/fonts: contains fonts ttf files
