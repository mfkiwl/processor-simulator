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
  SCREEN_WIDTH(1120),
  SCREEN_HEIGHT(470),
  gWindow(NULL),
  gRenderer(NULL),
  gFont12(NULL),
  gFont15(NULL),
  gFont20(NULL),
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
  gFont12 = TTF_OpenFont( "source/processor/view/Roboto-Bold.ttf", 12 );
  if( gFont12 == NULL ) {
    printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
    success = false;
  }

  //Open the font (font size was originally 28)
  gFont15 = TTF_OpenFont( "source/processor/view/Roboto-Bold.ttf", 15 );
  if( gFont15 == NULL ) {
    printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
    success = false;
  }

  //Open the font (font size was originally 28)
  gFont20 = TTF_OpenFont( "source/processor/view/Roboto-Bold.ttf", 20 );
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
  gTextTexture.render( x+2, y, gRenderer);
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
  SDL_RenderDrawLine(gRenderer, x1, y1+1, x2, y2+1);
  SDL_RenderDrawLine(gRenderer, x1+1, y1, x2+1, y2);

  SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
}

void View::drawInstructions() {

  SDL_Color textColor = {0,0,0};
  TTF_Font* textFont = gFont12;
  SDL_Color lineColor = {212,132,80};

  int xPos = 350;
  int yPos = 10;
  int cellWidth = 110;
  int cellHeight = 15;

  drawTable(xPos, yPos, 1, 1, cellWidth, cellHeight, lineColor);
  renderText(xPos, yPos, "Instructions", textColor, textFont);
}

void View::drawRegisterFile(const int numRegisters, const int registerValues[], const int renameTable[],
  const bool robMapping[]) 
{

  SDL_Color textColor = {0,0,0};
  TTF_Font* textFont = gFont12;
  SDL_Color lineColor = {255,0,0};

  //table info
  int noOfHorizontalCells = 2;
  int noOfVerticalCells = numRegisters;
  int xPos = 680;
  int yPos = 100;
  int cellWidth = 30;
  int cellHeight = 15;
  int offset = 80;

  //draw the title for the register file
  drawTable(xPos + offset, yPos, 1, 1, cellWidth * 2, cellHeight, textColor);
  renderText(xPos + offset, yPos, "Registers", textColor, textFont);

  //draw the table for the register file
  drawTable(xPos + offset, yPos + cellHeight, noOfHorizontalCells, noOfVerticalCells, cellWidth, cellHeight, lineColor);

  //draw the values in the register file
  for(int i = 0; i < noOfVerticalCells; i++) {
  	renderText(xPos + offset, yPos + (i + 1) * cellHeight, "R" + intToString(i), textColor, textFont);
    renderText(xPos + offset + cellWidth, yPos + (i + 1) * cellHeight, intToString(registerValues[i]), textColor, textFont);
  }

  //draw the title for the RAT
  drawTable(xPos, yPos, 1, 1, cellWidth * 2, cellHeight, textColor);
  renderText(xPos, yPos, "RAT", textColor, textFont);

  //draw the table for the RAT
  drawTable(xPos, yPos + cellHeight, noOfHorizontalCells, noOfVerticalCells, cellWidth, cellHeight, lineColor);

  //draw the values in the RAT
  for(int i = 0; i < noOfVerticalCells; i++) {
    renderText(xPos, yPos + (i + 1) * cellHeight, "R" + intToString(i), textColor, textFont);
    std::string renameString;
    if(robMapping[i]) {
      renameString = "RB" + intToString(renameTable[i]);
    }
    else {
      renameString = "R" + intToString(renameTable[i]);
    }
    renderText(xPos + cellWidth, yPos + (i + 1) * cellHeight, renameString, textColor, textFont);
  }

  //reset the draw color to white
  SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
}

void View::drawMemory(const int memorySize, const int memoryValues[]) {

  SDL_Color textColor = {0,0,0};
  TTF_Font* textFont = gFont12;
  SDL_Color lineColor = {0,255,0};

  //table info
  int noOfHorizontalCells = memorySize;
  int noOfVerticalCells = 2;
  int xPos = 75;
  int yPos = 400;
  int cellWidth = 20;
  int cellHeight = 15;

  //draw the table to hold the register values
  drawTable(xPos, yPos + cellHeight, noOfHorizontalCells, noOfVerticalCells, cellWidth, cellHeight, lineColor);

  //draw the memory
  for(int i = 0; i < noOfHorizontalCells; i++) {
    renderText(xPos + i * cellWidth, yPos + cellHeight, intToString(i), textColor, textFont);
    renderText(xPos + i * cellWidth, yPos + cellHeight * 2, intToString(memoryValues[i]), textColor, textFont);
  }

  drawTable(xPos, yPos, 1, 1, cellWidth * noOfHorizontalCells, cellHeight, textColor);
  renderText(xPos, yPos, "Memory", textColor, textFont);

}

void View::drawProcessorStats(const int numOfInstructionsExecuted, const int numOfClockCycles, const float numOfInstructionsExecutedPerCycle) {
  
  SDL_Color textColor = {0,0,0};
  TTF_Font* textFont = gFont15;

  int xPos = 0;
  int yPos = 0;
  int ySpace = 20;

  std::string text;

  //render the number of clock cycles performed
  text = "Number of clock cycles performed : " + intToString(numOfClockCycles);
  renderText(xPos, yPos, text, textColor, textFont);

  //render the number of instructions executed
  text = "Number of instructions executed : " + intToString(numOfInstructionsExecuted);
  renderText(xPos, yPos + ySpace, text, textColor, textFont);
  
  //render the number of the instructions executed per clock cycle
  text = "Number of instructions executed per cycle : " + floatToString(numOfInstructionsExecutedPerCycle);
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
  TTF_Font* textFont = gFont12;
  SDL_Color lineColor = {0,255,255};

  int xPos = 350;
  int yPos = 45;
  int numOfHorizontalCells = 1;
  int numOfVerticalCells = issueWindowSize;
  int cellWidth = 110;
  int cellHeight = 15;

  drawTable(xPos, yPos + cellHeight, numOfHorizontalCells, numOfVerticalCells, cellWidth, cellHeight, lineColor);

  for(int i = 0; i < issueWindowSize; i++) {
    std::string instructionString = instructionToString(instructions[i]);
    if(instructionString != "NOOP") {
      renderText(xPos, yPos + (i + 1) * cellHeight, instructionString, textColor, textFont);
    }
  }

  drawTable(xPos, yPos, 1, 1, cellWidth, cellHeight, textColor);
  renderText(xPos, yPos, "Fetch Unit", textColor, textFont);
}

void View::drawDecodeIssueUnit(const int issueWindowSize, const Instruction instructions[], 
  const int reorderBufferIndexes[], const OperandType operandTypes[][numOfOperands]) 
{

  SDL_Color textColor = {0,0,0};
  TTF_Font* textFont = gFont12;
  SDL_Color lineColor = {255,0,255};

  int xPos = 340;
  int yPos = 140;
  int numOfHorizontalCells = 1;
  int numOfVerticalCells = issueWindowSize;
  int cellWidth = 130;
  int cellHeight = 15;

  drawTable(xPos, yPos + cellHeight, 1, numOfVerticalCells, cellHeight, cellHeight, lineColor);
  drawTable(xPos + cellHeight, yPos + cellHeight, numOfHorizontalCells, numOfVerticalCells, cellWidth, cellHeight, lineColor);
  for(int i = 0; i < issueWindowSize; i++) {
    if(reorderBufferIndexes[i] != -1) {
      renderText(xPos, yPos + (i + 1) * cellHeight, intToString(reorderBufferIndexes[i]), textColor, textFont);
    }
    std::string instructionString = opcodeToString(instructions[i].opcode) + " " +
      operandsToString(instructions[i].operands, operandTypes[i]);
    if(instructionString != "NOOP") {
      renderText(xPos + 20, yPos + (i + 1) * cellHeight, instructionString, textColor, textFont);
    }
  }

  drawTable(xPos, yPos, 1, 1, cellWidth + cellHeight, cellHeight, textColor);
  renderText(xPos, yPos, "Decode Issue Unit", textColor, textFont);
}

void View::drawAluReservationStation(const int reservationStationSize, const Instruction instructions[], 
  const int reorderBufferIndexes[], const OperandType operandTypes[][3]) 
{

  SDL_Color textColor = {0,0,0};
  TTF_Font* textFont = gFont12;
  SDL_Color lineColor = {127,127,127};

  //specification
  int xPos = 340;
  int yPos = 250;
  int numOfVerticalCells = reservationStationSize;
  int cellWidth = 35;
  int cellHeight = 15;
  int instructionCellWidth = 40;

  //draw table
  drawTable(xPos, yPos + cellHeight, 1, numOfVerticalCells, cellWidth, cellHeight, lineColor);
  drawTable(xPos + cellWidth, yPos + cellHeight, 1, numOfVerticalCells, instructionCellWidth, cellHeight, lineColor);
  drawTable(xPos + cellWidth + instructionCellWidth, yPos + cellHeight, 2, numOfVerticalCells, cellWidth, cellHeight, lineColor);

  for(int i = 0; i < reservationStationSize; i++) {
    if(reorderBufferIndexes[i] != -1) {
      //draw reorder buffer index
      renderText(xPos, yPos + (i + 1) * cellHeight, intToString(reorderBufferIndexes[i]), textColor, textFont);
      //draw opcode
      renderText(xPos + cellWidth, yPos + (i + 1) * cellHeight, opcodeToString(instructions[i].opcode), textColor, textFont);
      //draw operands
      for(int j = 1; j < 3; j++) {
        std::string operandString = operandToString(instructions[i].operands[j], operandTypes[i][j]);
        renderText(xPos + instructionCellWidth + j * cellWidth, yPos + (i + 1) * cellHeight, operandString, textColor, textFont);
      }
    }
  }

  drawTable(xPos, yPos, 1, 1, cellWidth * 3 + instructionCellWidth, cellHeight, textColor);
  renderText(xPos, yPos, "ALU RS", textColor, textFont);
}

void View::drawBranchUnitReservationStation(const int reservationStationSize, const Instruction instructions[], 
  const int reorderBufferIndexes[], const OperandType operandTypes[][3]) 
{

  SDL_Color textColor = {0,0,0};
  TTF_Font* textFont = gFont12;
  SDL_Color lineColor = {127,0,0};

  //specification
  int xPos = 500;
  int yPos = 250;
  int numOfVerticalCells = reservationStationSize;
  int cellWidth = 35;
  int cellHeight = 15;
  int instructionCellWidth = 40;

  //draw table
  drawTable(xPos, yPos + cellHeight, 1, numOfVerticalCells, cellWidth, cellHeight, lineColor);
  drawTable(xPos + cellWidth, yPos + cellHeight, 1, numOfVerticalCells, instructionCellWidth, cellHeight, lineColor);
  drawTable(xPos + cellWidth + instructionCellWidth, yPos + cellHeight, 2, numOfVerticalCells, cellWidth, cellHeight, lineColor);

  for(int i = 0; i < reservationStationSize; i++) {
    if(reorderBufferIndexes[i] != -1) {
      //draw reorder buffer index
      renderText(xPos, yPos + (i + 1) * cellHeight, intToString(reorderBufferIndexes[i]), textColor, textFont);
      //draw opcode
      renderText(xPos + cellWidth, yPos + (i + 1) * cellHeight, opcodeToString(instructions[i].opcode), textColor, textFont);
      //draw operands
      for(int j = 0; j < 2; j++) {
        std::string operandString = operandToString(instructions[i].operands[j], operandTypes[i][j]);
        renderText(xPos + instructionCellWidth + (j + 1) * cellWidth, yPos + (i + 1) * cellHeight, operandString, textColor, textFont);
      }
    }
  }

  drawTable(xPos, yPos, 1, 1, cellWidth * 3 + instructionCellWidth, cellHeight, textColor);
  renderText(xPos, yPos, "Branch Unit RS", textColor, textFont);
}

void View::drawStoreQueue(const int size, const Instruction instructions[], const int reorderBufferIndexes[], 
  const OperandType operandTypes[][3])
{

  SDL_Color textColor = {0,0,0};
  TTF_Font* textFont = gFont12;
  SDL_Color lineColor = {0,127,0};

  //specifications
  int xPos = 20;
  int yPos = 250;
  int numOfVerticalCells = size;
  int cellWidth = 35;
  int cellHeight = 15;
  int instructionCellWidth = 40;

  //draw table
  drawTable(xPos, yPos + cellHeight, 1, numOfVerticalCells, cellWidth, cellHeight, lineColor);
  drawTable(xPos + cellWidth, yPos + cellHeight, 1, numOfVerticalCells, instructionCellWidth, cellHeight, lineColor);
  drawTable(xPos + cellWidth + instructionCellWidth, yPos + cellHeight, 2, numOfVerticalCells, cellWidth, cellHeight, lineColor);

  for(int i = 0; i < size; i++) {
    if(reorderBufferIndexes[i] != -1) {
      //draw reorder buffer index
      renderText(xPos, yPos + (i + 1) * cellHeight, intToString(reorderBufferIndexes[i]), textColor, textFont);
      //draw opcode
      renderText(xPos + cellWidth, yPos + (i + 1) * cellHeight, opcodeToString(instructions[i].opcode), textColor, textFont);
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
        renderText(xPos + instructionCellWidth + (1 + j) * cellWidth, yPos + (i + 1) * cellHeight, operandString, textColor, textFont);
      }
    }
  }

  drawTable(xPos, yPos, 1, 1, cellWidth * 3 + instructionCellWidth, cellHeight, textColor);
  renderText(xPos, yPos, "Store Queue", textColor, textFont);
}

void View::drawLoadQueue(const int size, const Instruction instructions[], const int reorderBufferIndexes[], 
  const OperandType operandTypes[][3])
{

  SDL_Color textColor = {0,0,0};
  TTF_Font* textFont = gFont12;
  SDL_Color lineColor = {0,0,127};

  //specification
  int xPos = 180;
  int yPos = 250;
  int numOfVerticalCells = size;
  int cellWidth = 35;
  int cellHeight = 15;
  int instructionCellWidth = 40;

  //draw table
  drawTable(xPos, yPos + cellHeight, 1, numOfVerticalCells, cellWidth, cellHeight, lineColor);
  drawTable(xPos + cellWidth, yPos + cellHeight, 1, numOfVerticalCells, instructionCellWidth, cellHeight, lineColor);
  drawTable(xPos + cellWidth + instructionCellWidth, yPos + cellHeight, 2, numOfVerticalCells, cellWidth, cellHeight, lineColor);

  for(int i = 0; i < size; i++) {
    if(reorderBufferIndexes[i] != -1) {
      //draw reorder buffer index
      renderText(xPos, yPos + (i + 1) * cellHeight, intToString(reorderBufferIndexes[i]), textColor, textFont);
      //draw opcode
      renderText(xPos + cellWidth, yPos + (i + 1) * cellHeight, opcodeToString(instructions[i].opcode), textColor, textFont);
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
        renderText(xPos + instructionCellWidth + (1 + j) * cellWidth, yPos + (i + 1) * cellHeight, operandString, textColor, textFont);
      }
    }
  }

  drawTable(xPos, yPos, 1, 1, cellWidth * 3 + instructionCellWidth, cellHeight, textColor);
  renderText(xPos, yPos, "Load Queue", textColor, textFont);
}

void View::drawAlu(const int numALUs, const int results[], const int reorderBufferIndexes[]) {

  SDL_Color textColor = {0,0,0};
  TTF_Font* textFont = gFont12;
  SDL_Color lineColor = {127,127,0};

  int xPos = 350;
  int yPos = 350;
  int numOfHorizontalCells = 2;
  int numOfVerticalCells = 1;
  int cellWidth = 20;
  int cellHeight = 15;
  int space = 20;

  for(int i = 0; i < numALUs; i++) {
    drawTable(xPos + i * (40 + space), yPos + cellHeight, numOfHorizontalCells, numOfVerticalCells, cellWidth, cellHeight, lineColor);
    if(reorderBufferIndexes[i] != -1) {
      renderText(xPos + i * (40 + space), yPos + cellHeight, intToString(reorderBufferIndexes[i]), textColor, textFont);
      renderText(xPos + i * (40 + space) + cellWidth, yPos + cellHeight, intToString(results[i]), textColor, textFont);
    }
    drawTable(xPos + i * (40 + space), yPos, 1, 1, cellWidth * 2, cellHeight, textColor);
    renderText(xPos + i * (40 + space), yPos, "ALU", textColor, textFont);
  }
}

void View::drawBranchUnit(const bool successful, const int reorderBufferIndex) {

  SDL_Color textColor = {0,0,0};
  TTF_Font* textFont = gFont12;
  SDL_Color lineColor = {127,0,127};

  int xPos = 550;
  int yPos = 350;
  int numOfHorizontalCells = 2;
  int numOfVerticalCells = 1;
  int cellWidth = 20;
  int cellHeight = 15;

  drawTable(xPos, yPos + cellHeight, numOfHorizontalCells, numOfVerticalCells, cellWidth, cellHeight, lineColor);
  if(reorderBufferIndex != -1) {
    renderText(xPos, yPos + cellHeight, intToString(reorderBufferIndex), textColor, textFont);
    renderText(xPos + cellWidth, yPos + cellHeight, intToString(successful), textColor, textFont);
  }
  drawTable(xPos - cellWidth * 1.25, yPos, 1, 1, cellWidth * 5, cellHeight, textColor);
  renderText(xPos - cellWidth * 1.25, yPos, "Branch Unit", textColor, textFont);
}

void View::drawLoadStoreUnit() {

  SDL_Color textColor = {0,0,0};
  TTF_Font* textFont = gFont12;
  SDL_Color lineColor = {127,127,127};

  int xPos = 100;
  int yPos = 350;
  int cellWidth = 150;
  int cellHeight = 15;

  drawTable(xPos, yPos, 1, 1, cellWidth, cellHeight, lineColor);
  renderText(xPos + 10, yPos, "Load/Store Unit", textColor, textFont);
}

void View::drawReorderBuffer(const int size, const int tailIndex, const int headIndex, 
  const Instruction instructions[], int fields[][ReorderBufferIndex::COUNT]) 
{

  SDL_Color textColor = {0,0,0};
  TTF_Font* textFont = gFont12;
  SDL_Color lineColor = {0,127,127};

  int numFields = ReorderBufferIndex::COUNT;

  int xPos = 850;
  int yPos = 40;
  int numOfHorizontalCells = numFields - 1;
  int numOfVerticalCells = size;
  int cellWidth = 20;
  int cellHeight = 15;
  int textCellWidth = 110;

 
  drawTable(xPos, yPos + cellHeight, 1, numOfVerticalCells, cellWidth, cellHeight, lineColor);
  drawTable(xPos + cellWidth, yPos + cellHeight, 1, numOfVerticalCells, textCellWidth, cellHeight, lineColor);
  drawTable(xPos + cellWidth + textCellWidth, yPos + cellHeight, numOfHorizontalCells, numOfVerticalCells, cellWidth, cellHeight, lineColor);

  for(int i = 0; i < size; i++) {
    renderText(xPos, yPos + (i + 1) * cellHeight, intToString(i), textColor, textFont);
    if(fields[i][TYPE] != -1) {
      renderText(xPos + cellWidth, yPos + (i + 1) * cellHeight, instructionToString(instructions[i]), textColor, textFont);
      for(int j = 1; j < numFields; j++) {
        renderText(xPos + textCellWidth + (j) * cellWidth, yPos + (i + 1) * cellHeight, intToString(fields[i][j]), textColor, textFont);
      }
    }
  }

  renderText(xPos - 10, yPos + (tailIndex + 1) * cellHeight, "T", textColor, textFont);
  renderText(xPos - 20, yPos + (headIndex + 1) * cellHeight, "H", textColor, textFont);

  drawTable(xPos, yPos, 1, 1, cellWidth * (numOfHorizontalCells + 1) + textCellWidth, cellHeight, textColor);
  renderText(xPos, yPos, "Reorder Buffer", textColor, textFont);
}

void View::drawConnections() {

  SDL_Color lineColor = {0,0,0};

  //draw from instruction to fetch unit
  drawLine(405,25,405,45,lineColor);
  //draw from fetch unit to decode issue unit
  drawLine(405,120,405,140,lineColor);
  //draw line from decode issue unit
  drawLine(405,215,405,230,lineColor);
  //draw horizontal line
  drawLine(90,230,570,230,lineColor);
  //draw line to store queue
  drawLine(90,230,90,250,lineColor);
  //draw line to load queue
  drawLine(250,230,250,250,lineColor);
  //draw line to alu reservation station
  drawLine(405,230,405,250,lineColor);
  //draw line to branch unit reservation station
  drawLine(570,230,570,250,lineColor);
  //draw line from store queue to load/store unit
  drawLine(120,325,120,350,lineColor);
  //draw line from load queue to load/store unit
  drawLine(220,325,220,350,lineColor);
  //draw line coming from alu reservation station
  drawLine(400,325,400,335,lineColor);
  //draw alu reservation station horizontal line
  drawLine(370,335,430,335,lineColor);
  //draw lines to ALUs
  drawLine(370,335,370,350,lineColor);
  drawLine(430,335,430,350,lineColor);
  //draw line from branch unit reservation station to branch unit
  drawLine(570,325,570,350,lineColor);
  //draw line from load/store unit to memory
  drawLine(170,365,170,400,lineColor);
  //draw line from load/store unit
  drawLine(190,365,190,390,lineColor);
  //draw lines from ALUs
  drawLine(370,380,370,390,lineColor);
  drawLine(430,380,430,390,lineColor);
  //draw line from branch unit
  drawLine(570,380,570,390,lineColor);
  //draw line from execution units to reorder buffer
  drawLine(190,390,900,390,lineColor);
  drawLine(900,390,900,385,lineColor);

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