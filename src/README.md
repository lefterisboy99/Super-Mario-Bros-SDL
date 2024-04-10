# HY454 Project

## Local Setup
To build the game, you need to install cmake:
1. Clone the repo 
2. In the root folder of the repo _create a build folder_
3. That'll be the build folder for cmake, so generate your build files, and you're good to go

You don't need to download SDL2, or link the libraries, or copy dll files, so when the game is done building, run it ("game.exe" or "./game"). Just to provide a bit of context, instead of downloading libs on Windows or installing them from package managers in linux, we just use the submodule of SDL2, build it and link it statically to prevent depedencies.