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
  SCREEN_WIDTH(1000),
  SCREEN_HEIGHT(480),
  gWindow(NULL),
  gRenderer(NULL),
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

void View::renderText(const int x, const int y, const std::string text, const SDL_Color textColor) {
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

void View::drawTextCell(const int xPos, const int yPos, const int width, const int height, const std::string text, const int xOffset, const int yOffset, const SDL_Color color) {

  //set the draw color to black
  SDL_SetRenderDrawColor(gRenderer, color.r, color.g, color.b, 0xFF);

  //draw the cell
  SDL_RenderDrawLine(gRenderer, xPos, yPos, xPos + width, yPos);
  SDL_RenderDrawLine(gRenderer, xPos, yPos + height, xPos + width, yPos + height);
  SDL_RenderDrawLine(gRenderer, xPos, yPos, xPos, yPos + height);
  SDL_RenderDrawLine(gRenderer, xPos + width, yPos, xPos + width, yPos + height);

  //draw the text
  renderText(xPos + xOffset, yPos + yOffset, text, color);

  //reset the draw color to white
  SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
}

void View::drawRegisterFile(const int numRegisters, const int registerValues[], const int renameTable[],
  const bool robMapping[]) 
{

  SDL_Color textColor = {255,0,0};

  //table info
  int noOfHorizontalCells = numRegisters;
  int noOfVerticalCells = 3;
  int xPos = 0;
  int yPos = 330;
  int cellWidth = 38;
  int cellHeight = 20;
  int textCellWidth = 80;

  //the offset the text should be drawn at
  int xOffset = 1;
  int yOffset = -2;

  //render the text specifying that the component is the register file
  renderText(xPos, yPos, "Register File : ", textColor);

  //draw the table to hold the register values
  drawTable(xPos + textCellWidth, yPos + cellHeight, noOfHorizontalCells, noOfVerticalCells, cellWidth, cellHeight, textColor);
  drawTextCell(xPos, yPos + cellHeight, textCellWidth, cellHeight, "Register :", xOffset, yOffset, textColor);
  drawTextCell(xPos, yPos + 2 * cellHeight, textCellWidth, cellHeight, "Value :", xOffset, yOffset, textColor);
  drawTextCell(xPos, yPos + 3 * cellHeight, textCellWidth, cellHeight, "Mapping :", xOffset, yOffset, textColor);

  //draw the register numbers
  for(int i = 0; i < noOfHorizontalCells; i++) {
  	std::string text = intToString(i);
  	renderText(xPos + textCellWidth + i * cellWidth + xOffset, yPos + cellHeight + yOffset, text, textColor);
  }

  //draw the register values
  for(int i = 0; i < noOfHorizontalCells; i++) {
    std::string text = intToString(registerValues[i]);
    renderText(xPos + textCellWidth + i * cellWidth + xOffset, yPos + 2 * cellHeight + yOffset, text, textColor);
  }

  //draw the mapping
  for(int i = 0; i < noOfHorizontalCells; i++) {
    std::string text;
    if(robMapping[i]) {
      text = "RB" + intToString(renameTable[i]);
    } else {
      text = intToString(renameTable[i]);
    }
    renderText(xPos + textCellWidth + i * cellWidth + xOffset, yPos + 3 * cellHeight + yOffset, text, textColor);
  }

  //reset the draw color to white
  SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
}

void View::drawMemory(const int memorySize, const int memoryValues[]) {

  SDL_Color textColor = {0,255,0};

  //table info
  int noOfHorizontalCells = memorySize;
  int noOfVerticalCells = 2;
  int xPos = 40;
  int yPos = 410;
  int cellWidth = 20;
  int cellHeight = 20;
  int textCellWidth = 80;

  //the offset the text should be drawn at
  int xOffset = 1;
  int yOffset = -2;

  //render the text specifying that the component is the memory
  renderText(xPos, yPos, "Memory : ", textColor);

  //draw the table to hold the register values
  drawTable(xPos + textCellWidth, yPos + cellHeight, noOfHorizontalCells, noOfVerticalCells, cellWidth, cellHeight, textColor);
  drawTextCell(xPos, yPos + cellHeight, textCellWidth, cellHeight, "Address :", xOffset, yOffset, textColor);
  drawTextCell(xPos, yPos + 2 * cellHeight, textCellWidth, cellHeight, "Value :", xOffset, yOffset, textColor);

  //draw the register numbers
  for(int i = 0; i < noOfHorizontalCells; i++) {
  	std::string text = intToString(i);
  	renderText(xPos + textCellWidth + i * cellWidth + xOffset, yPos + cellHeight + yOffset, text, textColor);
  }

  //draw the register values
  for(int i = 0; i < noOfHorizontalCells; i++) {
  	std::string text = intToString(memoryValues[i]);
  	renderText(xPos + textCellWidth + i * cellWidth + xOffset, yPos + 2 * cellHeight + yOffset, text, textColor);
  }

}

void View::drawProcessorStats(const int numOfInstructionsExecuted, const int numOfClockCycles, const float numOfInstructionsExecutedPerCycle) {
  
  SDL_Color textColor = {0,0,255};

  int xPos = 0;
  int yPos = 0;
  int ySpace = 20;

  //render the number of instructions executed
  std::string text = "Number of instructions executed: " + intToString(numOfInstructionsExecuted);
  renderText(xPos, yPos, text, textColor);

  //render the number of clock cycles performed
  text = "Number of clock cycles performed: " + intToString(numOfClockCycles);
  renderText(xPos, yPos + ySpace, text, textColor);
  
  //render the number of the instructions executed per clock cycle
  text = "Number of instructions executed per cycle: " + floatToString(numOfInstructionsExecutedPerCycle);
  renderText(xPos, yPos + 2 * ySpace, text, textColor);
}

void View::drawPC(const int programCounter) {

  SDL_Color textColor = {255,255,0};

  int xPos = 0;
  int yPos = 60;

  renderText(xPos, yPos, "PC : " + intToString(programCounter), textColor);
}

void View::drawFetchUnit(const int issueWindowSize, const Instruction instructions[]) {

  SDL_Color textColor = {0,255,255};

  int xPos = 60;
  int yPos = 60;
  int numOfHorizontalCells = 1;
  int numOfVerticalCells = issueWindowSize;
  int cellWidth = 150;
  int cellHeight = 20;

  renderText(xPos, yPos, "Fetch Unit : ", textColor);
  drawTable(xPos, yPos + cellHeight, numOfHorizontalCells, numOfVerticalCells, cellWidth, cellHeight, textColor);
  for(int i = 0; i < issueWindowSize; i++) {
    std::string instructionString = instructionToString(instructions[i]);
    if(instructionString != "NOOP") {
      renderText(xPos, yPos + (i + 1) * cellHeight, instructionString, textColor);
    }
  }
}

void View::drawDecodeIssueUnit(const int issueWindowSize, const Instruction instructions[], 
  const int reorderBufferIndexes[], const OperandType operandTypes[][numOfOperands]) 
{

  SDL_Color textColor = {255,0,255};

  int xPos = 260;
  int yPos = 60;
  int numOfHorizontalCells = 1;
  int numOfVerticalCells = issueWindowSize;
  int cellWidth = 150;
  int cellHeight = 20;

  renderText(xPos, yPos, "Decode/Issue unit", textColor);
  drawTable(xPos, yPos + cellHeight, 1, numOfVerticalCells, 20, cellHeight, textColor);
  drawTable(xPos + 20, yPos + cellHeight, numOfHorizontalCells, numOfVerticalCells, cellWidth, cellHeight, textColor);
  for(int i = 0; i < issueWindowSize; i++) {
    if(reorderBufferIndexes[i] != -1) {
      renderText(xPos, yPos + (i + 1) * cellHeight, intToString(reorderBufferIndexes[i]), textColor);
    }
    std::string instructionString = opcodeToString(instructions[i].opcode) + " " +
      operandsToString(instructions[i].operands, operandTypes[i]);
    if(instructionString != "NOOP") {
      renderText(xPos + 20, yPos + (i + 1) * cellHeight, instructionString, textColor);
    }
  }
}

void View::drawAluReservationStation(const int reservationStationSize, const Instruction instructions[], 
  const int reorderBufferIndexes[], const OperandType operandTypes[][3]) 
{

  SDL_Color textColor = {127,127,127};

  //specification
  int xPos = 10;
  int yPos = 160;
  int numOfVerticalCells = reservationStationSize;
  int cellWidth = 50;
  int cellHeight = 20;
  int instructionCellWidth = 50;

  //draw label
  renderText(xPos, yPos, "ALU", textColor);
  renderText(xPos, yPos + cellHeight, "Reservation Station : ", textColor);

  //draw table
  drawTable(xPos, yPos + cellHeight * 2, 1, numOfVerticalCells, cellWidth, cellHeight, textColor);
  drawTable(xPos + cellWidth, yPos + cellHeight * 2, 1, numOfVerticalCells, instructionCellWidth, cellHeight, textColor);
  drawTable(xPos + cellWidth + instructionCellWidth, yPos + cellHeight * 2, 2, numOfVerticalCells, cellWidth, cellHeight, textColor);

  for(int i = 0; i < reservationStationSize; i++) {
    if(reorderBufferIndexes[i] != -1) {
      //draw reorder buffer index
      renderText(xPos, yPos + (2 + i) * cellHeight, intToString(reorderBufferIndexes[i]), textColor);
      //draw opcode
      renderText(xPos + cellWidth, yPos + (2 + i) * cellHeight, opcodeToString(instructions[i].opcode), textColor);
      //draw operands
      for(int j = 1; j < 3; j++) {
        std::string operandString = operandToString(instructions[i].operands[j], operandTypes[i][j]);
        renderText(xPos + instructionCellWidth + j * cellWidth, yPos + (2 + i) * cellHeight, operandString, textColor);
      }
    }
  }
}

void View::drawBranchUnitReservationStation(const int reservationStationSize, const Instruction instructions[], 
  const int reorderBufferIndexes[], const OperandType operandTypes[][3]) 
{

  SDL_Color textColor = {127,0,0};

  //specification
  int xPos = 250;
  int yPos = 160;
  int numOfVerticalCells = reservationStationSize;
  int cellWidth = 40;
  int cellHeight = 20;
  int instructionCellWidth = 50;

  //draw label
  renderText(xPos, yPos, "Branch Unit", textColor);
  renderText(xPos, yPos + cellHeight, "Reservation Station : ", textColor);

  //draw table
  drawTable(xPos, yPos + cellHeight * 2, 1, numOfVerticalCells, cellWidth, cellHeight, textColor);
  drawTable(xPos + cellWidth, yPos + cellHeight * 2, 1, numOfVerticalCells, instructionCellWidth, cellHeight, textColor);
  drawTable(xPos + cellWidth + instructionCellWidth, yPos + cellHeight * 2, 2, numOfVerticalCells, cellWidth, cellHeight, textColor);

  for(int i = 0; i < reservationStationSize; i++) {
    if(reorderBufferIndexes[i] != -1) {
      //draw reorder buffer index
      renderText(xPos, yPos + (2 + i) * cellHeight, intToString(reorderBufferIndexes[i]), textColor);
      //draw opcode
      renderText(xPos + cellWidth, yPos + (2 + i) * cellHeight, opcodeToString(instructions[i].opcode), textColor);
      //draw operands
      for(int j = 0; j < 2; j++) {
        std::string operandString = operandToString(instructions[i].operands[j], operandTypes[i][j]);
        renderText(xPos + instructionCellWidth + (j + 1) * cellWidth, yPos + (2 + i) * cellHeight, operandString, textColor);
      }
    }
  }
}

void View::drawStoreQueue(const int size, const Instruction instructions[], const int reorderBufferIndexes[], 
  const OperandType operandTypes[][3])
{

  SDL_Color textColor = {0,127,0};

  //specifications
  int xPos = 450;
  int yPos = 220;
  int numOfVerticalCells = size;
  int cellWidth = 40;
  int cellHeight = 20;
  int instructionCellWidth = 50;

  //draw label
  renderText(xPos, yPos, "Store Queue", textColor);

  //draw table
  drawTable(xPos, yPos + cellHeight, 1, numOfVerticalCells, cellWidth, cellHeight, textColor);
  drawTable(xPos + cellWidth, yPos + cellHeight, 1, numOfVerticalCells, instructionCellWidth, cellHeight, textColor);
  drawTable(xPos + cellWidth + instructionCellWidth, yPos + cellHeight, 2, numOfVerticalCells, cellWidth, cellHeight, textColor);

  for(int i = 0; i < size; i++) {
    if(reorderBufferIndexes[i] != -1) {
      //draw reorder buffer index
      renderText(xPos, yPos + (1 + i) * cellHeight, intToString(reorderBufferIndexes[i]), textColor);
      //draw opcode
      renderText(xPos + cellWidth, yPos + (1 + i) * cellHeight, opcodeToString(instructions[i].opcode), textColor);
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
        renderText(xPos + instructionCellWidth + (1 + j) * cellWidth, yPos + (1 + i) * cellHeight, operandString, textColor);
      }
    }
  }
}

void View::drawLoadQueue(const int size, const Instruction instructions[], const int reorderBufferIndexes[], 
  const OperandType operandTypes[][3])
{

  SDL_Color textColor = {0,0,127};

  //specification
  int xPos = 450;
  int yPos = 120;
  int numOfVerticalCells = size;
  int cellWidth = 40;
  int cellHeight = 20;
  int instructionCellWidth = 50;

  //draw label
  renderText(xPos, yPos, "Load Queue", textColor);

  //draw table
  drawTable(xPos, yPos + cellHeight, 1, numOfVerticalCells, cellWidth, cellHeight, textColor);
  drawTable(xPos + cellWidth, yPos + cellHeight, 1, numOfVerticalCells, instructionCellWidth, cellHeight, textColor);
  drawTable(xPos + cellWidth + instructionCellWidth, yPos + cellHeight, 2, numOfVerticalCells, cellWidth, cellHeight, textColor);

  for(int i = 0; i < size; i++) {
    if(reorderBufferIndexes[i] != -1) {
      //draw reorder buffer index
      renderText(xPos, yPos + (1 + i) * cellHeight, intToString(reorderBufferIndexes[i]), textColor);
      //draw opcode
      renderText(xPos + cellWidth, yPos + (1 + i) * cellHeight, opcodeToString(instructions[i].opcode), textColor);
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
        renderText(xPos + instructionCellWidth + (1 + j) * cellWidth, yPos + (1 + i) * cellHeight, operandString, textColor);
      }
    }
  }
}

void View::drawAlu(const int numALUs, const int results[], const int reorderBufferIndexes[]) {

  SDL_Color textColor = {127,127,0};

  int xPos = 50;
  int yPos = 300;
  int numOfHorizontalCells = 2;
  int numOfVerticalCells = 1;
  int cellWidth = 20;
  int cellHeight = 20;

  for(int i = 0; i < numALUs; i++) {
    renderText(xPos + i * 50, yPos - cellHeight, "ALU : ", textColor);
    drawTable(xPos + i * 50, yPos, numOfHorizontalCells, numOfVerticalCells, cellWidth, cellHeight, textColor);
    if(reorderBufferIndexes[i] != -1) {
      renderText(xPos + i * 50, yPos, intToString(reorderBufferIndexes[i]), textColor);
      renderText(xPos + i * 50 + cellWidth, yPos, intToString(results[i]), textColor);
    }
  }
}

void View::drawBranchUnit(const bool successful, const int reorderBufferIndex) {

  SDL_Color textColor = {127, 0, 127};

  int xPos = 250;
  int yPos = 300;
  int numOfHorizontalCells = 2;
  int numOfVerticalCells = 1;
  int cellWidth = 20;
  int cellHeight = 20;

  renderText(xPos, yPos - cellHeight, "Branch Unit : ", textColor);
  drawTable(xPos, yPos, numOfHorizontalCells, numOfVerticalCells, cellWidth, cellHeight, textColor);
  if(reorderBufferIndex != -1) {
    renderText(xPos, yPos, intToString(reorderBufferIndex), textColor);
    renderText(xPos + cellWidth, yPos, intToString(successful), textColor);
  }
}

void View::drawReorderBuffer(const int size, const int tailIndex, const int headIndex, 
  const Instruction instructions[], int fields[][ReorderBufferIndex::COUNT]) 
{

  SDL_Color textColor = {0,127,127};

  int numFields = ReorderBufferIndex::COUNT;

  int xPos = 690;
  int yPos = 10;
  int numOfHorizontalCells = numFields;
  int numOfVerticalCells = size;
  int cellWidth = 20;
  int cellHeight = 20;
  int textCellWidth = 150;

  renderText(xPos, yPos, "Reorder Buffer : ", textColor);

  drawTable(xPos, yPos + cellHeight, 1, numOfVerticalCells, cellWidth, cellHeight, textColor);
  drawTable(xPos + cellWidth, yPos + cellHeight, 1, numOfVerticalCells, textCellWidth, cellHeight, textColor);
  drawTable(xPos + cellWidth + textCellWidth, yPos + cellHeight, numOfHorizontalCells, numOfVerticalCells, cellWidth, cellHeight, textColor);

  for(int i = 0; i < size; i++) {
    renderText(xPos, yPos + (i+1) * cellHeight, intToString(i), textColor);
    if(fields[i][3] != -1) {
      renderText(xPos + cellWidth, yPos + (i+1) * cellHeight, instructionToString(instructions[i]), textColor);
      for(int j = 0; j < numFields; j++) {
        renderText(xPos + textCellWidth + (j+1) * cellWidth, yPos + (i+1) * cellHeight, intToString(fields[i][j]), textColor);
      }
    }
  }

  renderText(xPos - 10, yPos + (tailIndex + 1) * cellHeight, "T", textColor);
  renderText(xPos - 20, yPos + (headIndex + 1) * cellHeight, "H", textColor);
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