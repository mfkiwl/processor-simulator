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
#include "../model/constants.h"

//===========================================
// implementation

View::View() :
  SCREEN_WIDTH(1300),
  SCREEN_HEIGHT(600),
  gWindow(NULL),
  gRenderer(NULL),
  gFont15(NULL),
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
  gFont12 = TTF_OpenFont( "source/processor/view/OpenSans-Bold.ttf", 10 );
  if( gFont12 == NULL ) {
    printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
    success = false;
  }

  //Open the font (font size was originally 28)
  gFont15 = TTF_OpenFont( "source/processor/view/OpenSans-Bold.ttf", 15 );
  if( gFont15 == NULL ) {
    printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
    success = false;
  }

  //Open the font (font size was originally 28)
  gFont20 = TTF_OpenFont( "source/processor/view/OpenSans-Bold.ttf", 20 );
  if( gFont20 == NULL ) {
    printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
    success = false;
  }

  return success;
}

void View::close()
{

  //Free global font
  TTF_CloseFont( gFont12 );
  TTF_CloseFont( gFont15 );
  TTF_CloseFont( gFont20 );
  gFont15 = NULL;

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

void View::renderText(const int x, const int y, const std::string text, const SDL_Color textColor, TTF_Font* gFont) {
  //Render text
  if( !gTextTexture.loadFromRenderedText( text.c_str(), textColor, gFont, gRenderer ) ) {
    printf( "Failed to render text texture!\n" );
  }

  //Render current frame
  gTextTexture.render( x, y, gRenderer);
}

void View::drawTable(const int xPos, const int yPos, const int noOfHorizontalCells, const int noOfVerticalCells, const int cellWidth, const int cellHeight, const SDL_Color color) {
  //set the draw color to black
  SDL_SetRenderDrawColor(gRenderer, color.r, color.g, color.b, 0xFF);

  //draw the horizontal lines
  int x1 = xPos;
  int x2 = xPos + cellWidth * noOfHorizontalCells;
  for(int i = 0; i < noOfVerticalCells + 1; i++) {
  	int y1 = yPos + i * cellHeight;
  	int y2 = yPos + i * cellHeight;
  	drawLine(x1, y1, x2, y2, color);
  }

  //draw the vertical lines
  int y1 = yPos;
  int y2 = yPos + noOfVerticalCells * cellHeight;
  for(int i = 0; i < noOfHorizontalCells + 1; i++) {
  	int x1 = xPos + i * cellWidth;
  	int x2 = xPos + i * cellWidth;
  	drawLine(x1, y1, x2, y2, color);
  }

  //reset the draw color to white
  SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
}

void View::drawTextCell(const int xPos, const int yPos, const int width, const int height, const SDL_Color lineColor,
  const std::string text, const int xOffset, const int yOffset, const SDL_Color textColor, TTF_Font* gFont) {

  //draw the cell
  drawLine(xPos, yPos, xPos + width, yPos, lineColor);
  drawLine(xPos, yPos + height, xPos + width, yPos + height, lineColor);
  drawLine(xPos, yPos, xPos, yPos + height, lineColor);
  drawLine(xPos + width, yPos, xPos + width, yPos + height, lineColor);

  //draw the text
  renderText(xPos + xOffset, yPos + yOffset, text, textColor, gFont);
}

void View::drawLine(const int x1, const int y1, const int x2, const int y2, const SDL_Color color) {
  SDL_SetRenderDrawColor(gRenderer, color.r, color.g, color.b, 0xFF);

  SDL_RenderDrawLine(gRenderer, x1, y1, x2, y2);

  SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
}

void View::drawInstructions() {

  SDL_Color textColor = {0,0,0};
  TTF_Font* textFont = gFont15;
  SDL_Color lineColor = {212,132,80};

  int xPos = 525;
  int yPos = 20;
  int cellWidth = 150;
  int cellHeight = 20;

  drawTable(xPos, yPos, 1, 1, cellWidth, cellHeight, lineColor);
  renderText(xPos + 25, yPos, "Instructions", textColor, textFont);
}

void View::drawRegisterFile(const int numRegisters, const int registerValues[], const int renameTable[],
  const bool robMapping[]) 
{

  SDL_Color textColor = {0,0,0};
  TTF_Font* textFont = gFont15;
  SDL_Color lineColor = {255,0,0};

  //table info
  int noOfHorizontalCells = 2;
  int noOfVerticalCells = numRegisters;
  int xPos = 20;
  int yPos = 150;
  int cellWidth = 40;
  int cellHeight = 20;

  //draw the table to hold the register values
  drawTable(xPos, yPos, noOfHorizontalCells, noOfVerticalCells, cellWidth, cellHeight, lineColor);

  //draw the register numbers
  for(int i = 0; i < noOfVerticalCells; i++) {
  	renderText(xPos, yPos + i * cellHeight, "R" + intToString(i), textColor, textFont);
    renderText(xPos + cellWidth, yPos + i * cellHeight, intToString(registerValues[i]), textColor, textFont);
  }

  drawTable(xPos + 100, yPos, noOfHorizontalCells, noOfVerticalCells, cellWidth, cellHeight, lineColor);

  for(int i = 0; i < noOfVerticalCells; i++) {
    renderText(xPos + 100, yPos + i * cellHeight, "R" + intToString(i), textColor, textFont);
    std::string renameString;
    if(robMapping[i]) {
      renameString = "RB" + intToString(renameTable[i]);
    }
    else {
      renameString = "R" + intToString(renameTable[i]);
    }
    renderText(xPos + 100 + cellWidth, yPos + i * cellHeight, renameString, textColor, textFont);
  }

  //reset the draw color to white
  SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
}

void View::drawMemory(const int memorySize, const int memoryValues[]) {

  SDL_Color textColor = {0,0,0};
  TTF_Font* textFont = gFont15;
  SDL_Color lineColor = {0,255,0};

  //table info
  int noOfHorizontalCells = memorySize;
  int noOfVerticalCells = 2;
  int xPos = 50;
  int yPos = 500;
  int cellWidth = 20;
  int cellHeight = 20;

  //draw the table to hold the register values
  drawTable(xPos, yPos, noOfHorizontalCells, noOfVerticalCells, cellWidth, cellHeight, lineColor);

  //draw the memory
  for(int i = 0; i < noOfHorizontalCells; i++) {
    renderText(xPos + i * cellWidth, yPos, intToString(i), textColor, textFont);
    renderText(xPos + i * cellWidth, yPos + cellHeight, intToString(memoryValues[i]), textColor, textFont);
  }

}

void View::drawProcessorStats(const int numOfInstructionsExecuted, const int numOfClockCycles, const float numOfInstructionsExecutedPerCycle) {
  
  SDL_Color textColor = {0,0,0};
  TTF_Font* textFont = gFont15;

  int xPos = 0;
  int yPos = 0;
  int ySpace = 20;

  //render the number of instructions executed
  std::string text = "Number of instructions executed: " + intToString(numOfInstructionsExecuted);
  renderText(xPos, yPos, text, textColor, textFont);

  //render the number of clock cycles performed
  text = "Number of clock cycles performed: " + intToString(numOfClockCycles);
  renderText(xPos, yPos + ySpace, text, textColor, textFont);
  
  //render the number of the instructions executed per clock cycle
  text = "Number of instructions executed per cycle: " + floatToString(numOfInstructionsExecutedPerCycle);
  renderText(xPos, yPos + 2 * ySpace, text, textColor, textFont);
}

void View::drawPC(const int programCounter) {

  SDL_Color textColor = {0,0,0};
  TTF_Font* textFont = gFont15;

  int xPos = 0;
  int yPos = 60;

  renderText(xPos, yPos, "PC : " + intToString(programCounter), textColor, textFont);
}

void View::drawFetchUnit(const int issueWindowSize, const Instruction instructions[]) {

  SDL_Color textColor = {0,0,0};
  TTF_Font* textFont = gFont15;
  SDL_Color lineColor = {0,255,255};

  int xPos = 525;
  int yPos = 70;
  int numOfHorizontalCells = 1;
  int numOfVerticalCells = issueWindowSize;
  int cellWidth = 150;
  int cellHeight = 20;

  drawTable(xPos, yPos, numOfHorizontalCells, numOfVerticalCells, cellWidth, cellHeight, lineColor);
  for(int i = 0; i < issueWindowSize; i++) {
    std::string instructionString = instructionToString(instructions[i]);
    if(instructionString != "NOOP") {
      renderText(xPos, yPos + i * cellHeight, instructionString, textColor, textFont);
    }
  }
}

void View::drawDecodeIssueUnit(const int issueWindowSize, const Instruction instructions[], 
  const int reorderBufferIndexes[], const OperandType operandTypes[][numOfOperands]) 
{

  SDL_Color textColor = {0,0,0};
  TTF_Font* textFont = gFont15;
  SDL_Color lineColor = {255,0,255};

  int xPos = 515;
  int yPos = 190;
  int numOfHorizontalCells = 1;
  int numOfVerticalCells = issueWindowSize;
  int cellWidth = 150;
  int cellHeight = 20;

  drawTable(xPos, yPos, 1, numOfVerticalCells, cellHeight, cellHeight, lineColor);
  drawTable(xPos + cellHeight, yPos, numOfHorizontalCells, numOfVerticalCells, cellWidth, cellHeight, lineColor);
  for(int i = 0; i < issueWindowSize; i++) {
    if(reorderBufferIndexes[i] != -1) {
      renderText(xPos, yPos + i * cellHeight, intToString(reorderBufferIndexes[i]), textColor, textFont);
    }
    std::string instructionString = opcodeToString(instructions[i].opcode) + " " +
      operandsToString(instructions[i].operands, operandTypes[i]);
    if(instructionString != "NOOP") {
      renderText(xPos + 20, yPos + i * cellHeight, instructionString, textColor, textFont);
    }
  }
}

void View::drawAluReservationStation(const int reservationStationSize, const Instruction instructions[], 
  const int reorderBufferIndexes[], const OperandType operandTypes[][3]) 
{

  SDL_Color textColor = {0,0,0};
  TTF_Font* textFont = gFont15;
  SDL_Color lineColor = {127,127,127};

  //specification
  int xPos = 215;
  int yPos = 320;
  int numOfVerticalCells = reservationStationSize;
  int cellWidth = 40;
  int cellHeight = 20;
  int instructionCellWidth = 50;

  //draw table
  drawTable(xPos, yPos, 1, numOfVerticalCells, cellWidth, cellHeight, lineColor);
  drawTable(xPos + cellWidth, yPos, 1, numOfVerticalCells, instructionCellWidth, cellHeight, lineColor);
  drawTable(xPos + cellWidth + instructionCellWidth, yPos, 2, numOfVerticalCells, cellWidth, cellHeight, lineColor);

  for(int i = 0; i < reservationStationSize; i++) {
    if(reorderBufferIndexes[i] != -1) {
      //draw reorder buffer index
      renderText(xPos, yPos + i * cellHeight, intToString(reorderBufferIndexes[i]), textColor, textFont);
      //draw opcode
      renderText(xPos + cellWidth, yPos + i * cellHeight, opcodeToString(instructions[i].opcode), textColor, textFont);
      //draw operands
      for(int j = 1; j < 3; j++) {
        std::string operandString = operandToString(instructions[i].operands[j], operandTypes[i][j]);
        renderText(xPos + instructionCellWidth + j * cellWidth, yPos + i * cellHeight, operandString, textColor, textFont);
      }
    }
  }
}

void View::drawBranchUnitReservationStation(const int reservationStationSize, const Instruction instructions[], 
  const int reorderBufferIndexes[], const OperandType operandTypes[][3]) 
{

  SDL_Color textColor = {0,0,0};
  TTF_Font* textFont = gFont15;
  SDL_Color lineColor = {127,0,0};

  //specification
  int xPos = 415;
  int yPos = 320;
  int numOfVerticalCells = reservationStationSize;
  int cellWidth = 40;
  int cellHeight = 20;
  int instructionCellWidth = 50;

  //draw table
  drawTable(xPos, yPos, 1, numOfVerticalCells, cellWidth, cellHeight, lineColor);
  drawTable(xPos + cellWidth, yPos, 1, numOfVerticalCells, instructionCellWidth, cellHeight, lineColor);
  drawTable(xPos + cellWidth + instructionCellWidth, yPos, 2, numOfVerticalCells, cellWidth, cellHeight, lineColor);

  for(int i = 0; i < reservationStationSize; i++) {
    if(reorderBufferIndexes[i] != -1) {
      //draw reorder buffer index
      renderText(xPos, yPos + i * cellHeight, intToString(reorderBufferIndexes[i]), textColor, textFont);
      //draw opcode
      renderText(xPos + cellWidth, yPos + i * cellHeight, opcodeToString(instructions[i].opcode), textColor, textFont);
      //draw operands
      for(int j = 0; j < 2; j++) {
        std::string operandString = operandToString(instructions[i].operands[j], operandTypes[i][j]);
        renderText(xPos + instructionCellWidth + (j + 1) * cellWidth, yPos + i * cellHeight, operandString, textColor, textFont);
      }
    }
  }
}

void View::drawStoreQueue(const int size, const Instruction instructions[], const int reorderBufferIndexes[], 
  const OperandType operandTypes[][3])
{

  SDL_Color textColor = {0,0,0};
  TTF_Font* textFont = gFont15;
  SDL_Color lineColor = {0,127,0};

  //specifications
  int xPos = 815;
  int yPos = 320;
  int numOfVerticalCells = size;
  int cellWidth = 40;
  int cellHeight = 20;
  int instructionCellWidth = 50;

  //draw table
  drawTable(xPos, yPos, 1, numOfVerticalCells, cellWidth, cellHeight, lineColor);
  drawTable(xPos + cellWidth, yPos, 1, numOfVerticalCells, instructionCellWidth, cellHeight, lineColor);
  drawTable(xPos + cellWidth + instructionCellWidth, yPos, 2, numOfVerticalCells, cellWidth, cellHeight, lineColor);

  for(int i = 0; i < size; i++) {
    if(reorderBufferIndexes[i] != -1) {
      //draw reorder buffer index
      renderText(xPos, yPos + i * cellHeight, intToString(reorderBufferIndexes[i]), textColor, textFont);
      //draw opcode
      renderText(xPos + cellWidth, yPos + i * cellHeight, opcodeToString(instructions[i].opcode), textColor, textFont);
      //draw operands
      for(int j = 0; j < 2; j++) {
        std::string operandString;
        if(operandTypes[i][j] == ROB) {
          operandString = "RB" + intToString(instructions[i].operands[j]);
        }
        else if(operandTypes[i][j] == REGISTER) {
          operandString = "R" + intToString(instructions[i].operands[j]);
        }
        if(operandTypes[i][j] == CONSTANT) {
          operandString = intToString(instructions[i].operands[j]);
        }
        renderText(xPos + instructionCellWidth + (1 + j) * cellWidth, yPos + i * cellHeight, operandString, textColor, textFont);
      }
    }
  }
}

void View::drawLoadQueue(const int size, const Instruction instructions[], const int reorderBufferIndexes[], 
  const OperandType operandTypes[][3])
{

  SDL_Color textColor = {0,0,0};
  TTF_Font* textFont = gFont15;
  SDL_Color lineColor = {0,0,127};

  //specification
  int xPos = 615;
  int yPos = 320;
  int numOfVerticalCells = size;
  int cellWidth = 40;
  int cellHeight = 20;
  int instructionCellWidth = 50;

  //draw table
  drawTable(xPos, yPos, 1, numOfVerticalCells, cellWidth, cellHeight, lineColor);
  drawTable(xPos + cellWidth, yPos, 1, numOfVerticalCells, instructionCellWidth, cellHeight, lineColor);
  drawTable(xPos + cellWidth + instructionCellWidth, yPos, 2, numOfVerticalCells, cellWidth, cellHeight, lineColor);

  for(int i = 0; i < size; i++) {
    if(reorderBufferIndexes[i] != -1) {
      //draw reorder buffer index
      renderText(xPos, yPos + i * cellHeight, intToString(reorderBufferIndexes[i]), textColor, textFont);
      //draw opcode
      renderText(xPos + cellWidth, yPos + i * cellHeight, opcodeToString(instructions[i].opcode), textColor, textFont);
      //draw operands
      for(int j = 0; j < 2; j++) {
        std::string operandString;
        if(operandTypes[i][j] == ROB) {
          operandString = "RB" + intToString(instructions[i].operands[j]);
        }
        else if(operandTypes[i][j] == REGISTER) {
          operandString = "R" + intToString(instructions[i].operands[j]);
        }
        else if(operandTypes[i][j] == CONSTANT) {
          operandString = intToString(instructions[i].operands[j]);
        }
        renderText(xPos + instructionCellWidth + (1 + j) * cellWidth, yPos + i * cellHeight, operandString, textColor, textFont);
      }
    }
  }
}

void View::drawAlu(const int numALUs, const int results[], const int reorderBufferIndexes[]) {

  SDL_Color textColor = {0,0,0};
  TTF_Font* textFont = gFont15;
  SDL_Color lineColor = {127,127,0};

  int xPos = 250;
  int yPos = 450;
  int numOfHorizontalCells = 2;
  int numOfVerticalCells = 1;
  int cellWidth = 20;
  int cellHeight = 20;
  int space = 20;

  for(int i = 0; i < numALUs; i++) {
    drawTable(xPos + i * (40 + space), yPos, numOfHorizontalCells, numOfVerticalCells, cellWidth, cellHeight, lineColor);
    if(reorderBufferIndexes[i] != -1) {
      renderText(xPos + i * (40 + space), yPos, intToString(reorderBufferIndexes[i]), textColor, textFont);
      renderText(xPos + i * (40 + space) + cellWidth, yPos, intToString(results[i]), textColor, textFont);
    }
  }
}

void View::drawBranchUnit(const bool successful, const int reorderBufferIndex) {

  SDL_Color textColor = {0,0,0};
  TTF_Font* textFont = gFont15;
  SDL_Color lineColor = {127,0,127};

  int xPos = 480;
  int yPos = 450;
  int numOfHorizontalCells = 2;
  int numOfVerticalCells = 1;
  int cellWidth = 20;
  int cellHeight = 20;

  drawTable(xPos, yPos, numOfHorizontalCells, numOfVerticalCells, cellWidth, cellHeight, lineColor);
  if(reorderBufferIndex != -1) {
    renderText(xPos, yPos, intToString(reorderBufferIndex), textColor, textFont);
    renderText(xPos + cellWidth, yPos, intToString(successful), textColor, textFont);
  }
}

void View::drawLoadStoreUnit() {

  SDL_Color textColor = {0,0,0};
  TTF_Font* textFont = gFont15;
  SDL_Color lineColor = {127,127,127};

  int xPos = 725;
  int yPos = 450;
  int cellWidth = 150;
  int cellHeight = 20;

  drawTable(xPos, yPos, 1, 1, cellWidth, cellHeight, lineColor);
  renderText(xPos + 10, yPos, "Load/Store Unit", textColor, textFont);
}

void View::drawReorderBuffer(const int size, const int tailIndex, const int headIndex, 
  const Instruction instructions[], int fields[][ReorderBufferIndex::COUNT]) 
{

  SDL_Color textColor = {0,0,0};
  TTF_Font* textFont = gFont15;
  SDL_Color lineColor = {0,127,127};

  int numFields = ReorderBufferIndex::COUNT;

  int xPos = 1015;
  int yPos = 60;
  int numOfHorizontalCells = numFields - 1;
  int numOfVerticalCells = size;
  int cellWidth = 20;
  int cellHeight = 20;
  int textCellWidth = 150;

  drawTable(xPos, yPos, 1, numOfVerticalCells, cellWidth, cellHeight, lineColor);
  drawTable(xPos + cellWidth, yPos, 1, numOfVerticalCells, textCellWidth, cellHeight, lineColor);
  drawTable(xPos + cellWidth + textCellWidth, yPos, numOfHorizontalCells, numOfVerticalCells, cellWidth, cellHeight, lineColor);

  for(int i = 0; i < size; i++) {
    renderText(xPos, yPos + i * cellHeight, intToString(i), textColor, textFont);
    if(fields[i][TYPE] != -1) {
      renderText(xPos + cellWidth, yPos + i * cellHeight, instructionToString(instructions[i]), textColor, textFont);
      for(int j = 1; j < numFields; j++) {
        renderText(xPos + textCellWidth + (j) * cellWidth, yPos + i * cellHeight, intToString(fields[i][j]), textColor, textFont);
      }
    }
  }

  renderText(xPos - 10, yPos + tailIndex * cellHeight, "T", textColor, gFont15);
  renderText(xPos - 20, yPos + headIndex * cellHeight, "H", textColor, gFont15);
}

void View::drawConnections() {

  SDL_Color lineColor = {0,0,0};

  //draw from instruction to fetch unit
  drawLine(600,40,600,70,lineColor);
  //draw from fetch unit to decode issue unit
  drawLine(600,150,600,190,lineColor);
  //draw line from decode issue unit
  drawLine(600,270,600,290,lineColor);
  //draw horizontal line
  drawLine(300,290,900,290,lineColor);
  //draw line to alu reservatin station
  drawLine(300,290,300,320,lineColor);
  //draw line to branch unit reservaion station
  drawLine(500,290,500,320,lineColor);
  //draw line to load queue
  drawLine(700,290,700,320,lineColor);
  //draw line to store queue
  drawLine(900,290,900,320,lineColor);
  //draw line from RAT to decode issue unit
  drawLine(200,230,515,230,lineColor);
  //draw line coming from alu reservation station
  drawLine(300,400,300,420,lineColor);
  //draw alu reservation station horizontal line
  drawLine(270,420,330,420,lineColor);
  //draw alu reservation station parallel vertical lines
  drawLine(270,420,270,450,lineColor);
  drawLine(330,420,330,450,lineColor);
  //draw line from branch unit reservation station to branch unit
  drawLine(500,400,500,450,lineColor);
  //draw line coming from load queue
  drawLine(700,400,700,430,lineColor);
  //draw line coming from store queue
  drawLine(900,400,900,430,lineColor);
  //draw load queue and store queue parallel line
  drawLine(700,430,900,430,lineColor);
  //draw line to load/store unit
  drawLine(800,430,800,450,lineColor);

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