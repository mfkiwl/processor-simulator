/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

//===========================================
//included header file containing interface
#include "view.h"

//Using SDL, SDL_image, standard IO, and strings
//===========================================
// included dependencies
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string>

#include "lTexture.h"
#include "../model/instructions.h"

//===========================================
// implementation

View::View(const int numOfRegisters, const int memorySize, const int aluReservationStationSize, 
  const int branchUnitReservationStationSize, const int decodeIssueUnitReservationStationSize) :
  SCREEN_WIDTH(640),
  SCREEN_HEIGHT(480),
  gWindow(NULL),
  gRenderer(NULL),
  gFont(NULL),
  quit(false),
  numOfRegisters(numOfRegisters),
  memorySize(memorySize),
  aluReservationStationSize(aluReservationStationSize),
  branchUnitReservationStationSize(branchUnitReservationStationSize),
  decodeIssueUnitReservationStationSize(decodeIssueUnitReservationStationSize)
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

  //Open the font (font size was originally 28)
  gFont = TTF_OpenFont( "source/processor/view/OpenSans-Bold.ttf", 15 );
  if( gFont == NULL ) {
    printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
    success = false;
  }

  return success;
}

void View::close()
{

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
  SDL_Quit();
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

void View::renderText(const int x, const int y, const std::string text) {
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

void View::drawTable(const int xPos, const int yPos, const int noOfHorizontalCells, const int noOfVerticalCells, const int cellWidth, const int cellHeight) {
  //set the draw color to black
  SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);

  //draw the horizontal lines
  int x1 = xPos;
  int x2 = xPos + cellWidth * noOfHorizontalCells;
  for(int i = 0; i < noOfVerticalCells + 1; i++) {
  	int y1 = yPos + i * cellHeight;
  	int y2 = yPos + i * cellHeight;
  	SDL_RenderDrawLine(gRenderer, x1, y1, x2, y2);
  }

  //draw the vertical lines
  int y1 = yPos;
  int y2 = yPos + noOfVerticalCells * cellHeight;
  for(int i = 0; i < noOfHorizontalCells + 1; i++) {
  	int x1 = xPos + i * cellWidth;
  	int x2 = xPos + i * cellWidth;
  	SDL_RenderDrawLine(gRenderer, x1, y1, x2, y2);
  }

  //reset the draw color to white
  SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
}

void View::drawTextCell(const int xPos, const int yPos, const int width, const int height, const std::string text, const int xOffset, const int yOffset) {

  //set the draw color to black
  SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);

  //draw the cell
  SDL_RenderDrawLine(gRenderer, xPos, yPos, xPos + width, yPos);
  SDL_RenderDrawLine(gRenderer, xPos, yPos + height, xPos + width, yPos + height);
  SDL_RenderDrawLine(gRenderer, xPos, yPos, xPos, yPos + height);
  SDL_RenderDrawLine(gRenderer, xPos + width, yPos, xPos + width, yPos + height);

  //draw the text
  renderText(xPos + xOffset, yPos + yOffset, text);

  //reset the draw color to white
  SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
}

void View::drawRegisterFile(const int* const registerValues) {

  //table info
  int noOfHorizontalCells = numOfRegisters;
  int noOfVerticalCells = 2;
  int xPos = 40;
  int yPos = 300;
  int cellWidth = 30;
  int cellHeight = 20;
  int textCellWidth = 80;

  //the offset the text should be drawn at
  int xOffset = 1;
  int yOffset = -2;

  //draw the table to hold the register values
  drawTable(xPos + textCellWidth, yPos, noOfHorizontalCells, noOfVerticalCells, cellWidth, cellHeight);
  drawTextCell(xPos, yPos, textCellWidth, cellHeight, "Register :", xOffset, yOffset);
  drawTextCell(xPos, yPos + cellHeight, textCellWidth, cellHeight, "Value :", xOffset, yOffset);

  //draw the register numbers
  for(int i = 0; i < noOfHorizontalCells; i++) {
  	std::string text = "R" + intToString(i);
  	renderText(xPos + textCellWidth + i * cellWidth + xOffset, yPos + yOffset, text);
  }

  //draw the register values
  for(int i = 0; i < noOfHorizontalCells; i++) {
  	std::string text = intToString(registerValues[i]);
  	renderText(xPos + textCellWidth + i * cellWidth + xOffset, yPos + cellHeight + yOffset, text);
  }

  //reset the draw color to white
  SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
}

void View::drawMemory(const int* const memoryValues) {
  //table info
  int noOfHorizontalCells = memorySize;
  int noOfVerticalCells = 2;
  int xPos = 40;
  int yPos = 350;
  int cellWidth = 20;
  int cellHeight = 20;
  int textCellWidth = 80;

  int xOffset = 1;
  int yOffset = -2;

  //draw the table to hold the register values
  drawTable(xPos + textCellWidth, yPos, noOfHorizontalCells, noOfVerticalCells, cellWidth, cellHeight);
  drawTextCell(xPos, yPos, textCellWidth, cellHeight, "Address :", xOffset, yOffset);
  drawTextCell(xPos, yPos + cellHeight, textCellWidth, cellHeight, "Value :", xOffset, yOffset);

  //draw the register numbers
  for(int i = 0; i < noOfHorizontalCells; i++) {
  	std::string text = intToString(i);
  	renderText(xPos + textCellWidth + i * cellWidth + xOffset, yPos + yOffset, text);
  }

  //draw the register values
  for(int i = 0; i < noOfHorizontalCells; i++) {
  	std::string text = intToString(memoryValues[i]);
  	renderText(xPos + textCellWidth + i * cellWidth + xOffset, yPos + cellHeight + yOffset, text);
  }

}

void View::drawProcessorStats(const int numOfInstructionsExecuted, const int numOfClockCycles, const float numOfInstructionsExecutedPerCycle) {
  
  int xPos = 0;
  int yPos = 0;
  int ySpace = 20;

  //render the number of instructions executed
  std::string text = "Number of instructions executed: " + intToString(numOfInstructionsExecuted);
  renderText(xPos, yPos, text);

  //render the number of clock cycles performed
  text = "Number of clock cycles performed: " + intToString(numOfClockCycles);
  renderText(xPos, yPos + ySpace, text);

  //render the number of the instructions executed per clock cycle
  text = "Number of instructions executed per cycle: " + intToString(numOfInstructionsExecutedPerCycle);
  renderText(xPos, yPos + 2 * ySpace, text);
}

void View::drawFetchUnit(const Instruction instruction) {
  int xPos = 0;
  int yPos = 80;

  std::string text = "Fetch Unit : " + instructionToString(instruction);
  renderText(xPos, yPos, text);
}

void View::drawDecodeIssueUnit(const Instruction instruction) {
  int xPos = 0;
  int yPos = 100;

  std::string text = "Decode Issue Unit : " + instructionToString(instruction);
  renderText(xPos, yPos, text);
}

void View::drawAluReservationStation(const Instruction* const instructions) {
  int xPos = 0;
  int yPos = 120;
  int xOffset = 200;
  int ySpace = 20;

  std::string text = "ALU Reservation Station : ";
  renderText(xPos, yPos, text);

  for(int i = 0; i < aluReservationStationSize; i++) {
    renderText(xPos + xOffset, yPos + i * ySpace, instructionToString(instructions[i]));
  }
}

void View::drawBranchUnitReservationStation(const Instruction* const instructions) {
  int xPos = 0;
  int yPos = 200;
  int xOffset = 250;
  int ySpace = 20;

  std::string text = "Branch Unit ReservationStation : ";
  renderText(xPos, yPos, text);

  for(int i = 0; i < branchUnitReservationStationSize; i++) {
    renderText(xPos + xOffset, yPos + i * ySpace, instructionToString(instructions[i]));
  }
}

void View::clearScreen() {
  SDL_RenderClear(gRenderer);
}

void View::updateScreen() {
  SDL_RenderPresent(gRenderer);
}

//====================================
// getter functions

int View::hasQuit() const {
  return quit;
}