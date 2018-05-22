//=================================
// include guard
#ifndef VIEW_H
#define VIEW_H

//=================================
//forward declared dependencies
class SDL_Texture;

//=================================
// included dependencies
#include <iostream>

//=================================
// interface

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//Loads individual image as texture
SDL_Texture* loadTexture( std::string path );


int viewmain(int argc, char *argv[]);

#endif