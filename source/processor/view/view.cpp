/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

//===========================================
//included header file containing interface
#include "view.h"

//Using SDL, SDL_image, standard IO, and strings
//===========================================
// included dependencies
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string>

#include "lTexture.h"

//===========================================
// implementation

View::View() :
  SCREEN_WIDTH(640),
  SCREEN_HEIGHT(480),
  gWindow(NULL),
  gRenderer(NULL),
  gTexture(NULL),
  gFont(NULL),
  quit(false)
{}

bool View::init()
{

  //Initialize SDL
  if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
    printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
    return false;
  }

  //Set texture filtering to linear
  if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) ) {
    printf( "Warning: Linear texture filtering not enabled!" );
  }

  //Create window
  gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
  if( gWindow == NULL ) {
    printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
    return false;
  }

  //Create renderer for window
  gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
  if( gRenderer == NULL ) {
    printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
    return false;
  }

  //Initialize renderer color
  SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
	
  //Initialize PNG loading
  int imgFlags = IMG_INIT_PNG;
  if( !( IMG_Init( imgFlags ) & imgFlags ) ) {
    printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
    return false;
  }

  //Initialize SDL_ttf
  if( TTF_Init() == -1 ) {
    printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
    return false;
  }

  return true;
}

bool View::loadMedia()
{
  //Loading success flag
  bool success = true;

  //Load PNG texture
  gTexture = loadTexture( "source/processor/view/texture.png" );
  if( gTexture == NULL ) {
    printf( "Failed to load texture image!\n" );
    success = false;
  }

  //Open the font (font size was originally 28)
  gFont = TTF_OpenFont( "source/processor/view/OpenSans-Bold.ttf", 20 );
  if( gFont == NULL ) {
    printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
    success = false;
  }

  return success;
}

void View::close()
{

  //Free loaded images
  gTextTexture.free();
  SDL_DestroyTexture( gTexture );
  gTexture = NULL;

  //Free global font
  TTF_CloseFont( gFont );
  gFont = NULL;

  //Destroy window	
  SDL_DestroyRenderer( gRenderer );
  SDL_DestroyWindow( gWindow );
  gWindow = NULL;
  gRenderer = NULL;

  //Quit SDL subsystems
  TTF_Quit();
  IMG_Quit();
  SDL_Quit();
}

SDL_Texture* View::loadTexture( std::string path )
{
  //The final texture
  SDL_Texture* newTexture = NULL;

  //Load image at specified path
  SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
  if( loadedSurface == NULL ) {
    printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
  }
  else {
    //Create texture from surface pixels
    newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
    if( newTexture == NULL ) {
      printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
    }

    //Get rid of old loaded surface
    SDL_FreeSurface( loadedSurface );
  }

  return newTexture;
}

void View::renderText(int x, int y, std::string text) {
  //Render text
  SDL_Color textColor = { 0, 0, 0 };
  if( !gTextTexture.loadFromRenderedText( text.c_str(), textColor, gFont, gRenderer ) ) {
    printf( "Failed to render text texture!\n" );
  }
  //Render current frame
  //gTextTexture.render( ( SCREEN_WIDTH - gTextTexture.getWidth() ) / 2, ( SCREEN_HEIGHT - gTextTexture.getHeight() ) / 2, gRenderer);

  //Render current frame
  gTextTexture.render( x, y, gRenderer);
}

void View::eventHandler() {
  //Event handler
  SDL_Event e;

  //Handle events on queue
  while( SDL_PollEvent( &e ) != 0 )  {
    //User requests quit
    if( e.type == SDL_QUIT ) {
      quit = true;
    }
  }
}

void View::clearScreen() {
	SDL_RenderClear(gRenderer);
}

void View::renderImage() {
	SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);
}

void View::showScreen() {
	SDL_RenderPresent(gRenderer);
}

void View::frame() {
  
  //handle events
  eventHandler();

  //Clear screen
  clearScreen();

  //Render texture to screen
  renderImage();

  //Render current frame
  renderText(0, 0, "nice");

  //Update screen
  showScreen();
}