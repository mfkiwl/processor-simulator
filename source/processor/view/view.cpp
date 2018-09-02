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

void View::drawRegisterFile(const int numRegisters, const int* const registerValues, const int* const renameTable) {

  //table info
  int noOfHorizontalCells = numRegisters;
  int noOfVerticalCells = 3;
  int xPos = 40;
  int yPos = 330;
  int cellWidth = 30;
  int cellHeight = 20;
  int textCellWidth = 120;

  //the offset the text should be drawn at
  int xOffset = 1;
  int yOffset = -2;

  //render the text specifying that the component is the register file
  renderText(xPos, yPos, "Register File : ");

  //draw the table to hold the register values
  drawTable(xPos + textCellWidth, yPos + cellHeight, noOfHorizontalCells, noOfVerticalCells, cellWidth, cellHeight);
  drawTextCell(xPos, yPos + cellHeight, textCellWidth, cellHeight, "Architectural :", xOffset, yOffset);
  drawTextCell(xPos, yPos + 2 * cellHeight, textCellWidth, cellHeight, "Physical :", xOffset, yOffset);
  drawTextCell(xPos, yPos + 3 * cellHeight, textCellWidth, cellHeight, "Latest Value :", xOffset, yOffset);

  //draw the register numbers
  for(int i = 0; i < noOfHorizontalCells; i++) {
  	std::string text = intToString(i);
  	renderText(xPos + textCellWidth + i * cellWidth + xOffset, yPos + cellHeight + yOffset, text);
  }

  //draw the register values
  for(int i = 0; i < noOfHorizontalCells; i++) {
    std::string text = intToString(renameTable[i]);
    renderText(xPos + textCellWidth + i * cellWidth + xOffset, yPos + 2 * cellHeight + yOffset, text);
  }

  //draw the register values
  for(int i = 0; i < noOfHorizontalCells; i++) {
    std::string text = intToString(registerValues[i]);
    renderText(xPos + textCellWidth + i * cellWidth + xOffset, yPos + 3 * cellHeight + yOffset, text);
  }

  //reset the draw color to white
  SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
}

void View::drawMemory(const int memorySize, const int* const memoryValues) {
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
  renderText(xPos, yPos, "Memory : ");

  //draw the table to hold the register values
  drawTable(xPos + textCellWidth, yPos + cellHeight, noOfHorizontalCells, noOfVerticalCells, cellWidth, cellHeight);
  drawTextCell(xPos, yPos + cellHeight, textCellWidth, cellHeight, "Address :", xOffset, yOffset);
  drawTextCell(xPos, yPos + 2 * cellHeight, textCellWidth, cellHeight, "Value :", xOffset, yOffset);

  //draw the register numbers
  for(int i = 0; i < noOfHorizontalCells; i++) {
  	std::string text = intToString(i);
  	renderText(xPos + textCellWidth + i * cellWidth + xOffset, yPos + cellHeight + yOffset, text);
  }

  //draw the register values
  for(int i = 0; i < noOfHorizontalCells; i++) {
  	std::string text = intToString(memoryValues[i]);
  	renderText(xPos + textCellWidth + i * cellWidth + xOffset, yPos + 2 * cellHeight + yOffset, text);
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
  text = "Number of instructions executed per cycle: " + floatToString(numOfInstructionsExecutedPerCycle);
  renderText(xPos, yPos + 2 * ySpace, text);
}

void View::drawPC(const int programCounter) {
  int xPos = 0;
  int yPos = 60;

  renderText(xPos, yPos, "PC : " + intToString(programCounter));
}

void View::drawFetchUnit(const int issueWindowSize, const Instruction* const instructions) {
  int xPos = 60;
  int yPos = 60;
  int numOfHorizontalCells = 1;
  int numOfVerticalCells = issueWindowSize;
  int cellWidth = 150;
  int cellHeight = 20;

  renderText(xPos, yPos, "Fetch Unit : ");
  drawTable(xPos, yPos + cellHeight, numOfHorizontalCells, numOfVerticalCells, cellWidth, cellHeight);
  for(int i = 0; i < issueWindowSize; i++) {
    std::string instructionString = instructionToString(instructions[i]);
    if(instructionString != "NOOP") {
      renderText(xPos, yPos + (i + 1) * cellHeight, instructionString);
    }
  }
}

void View::drawDecodeIssueUnit(const int issueWindowSize, const Instruction instructions[], 
  const int reorderBufferIndexes[]) 
{
  int xPos = 260;
  int yPos = 60;
  int numOfHorizontalCells = 1;
  int numOfVerticalCells = issueWindowSize;
  int cellWidth = 150;
  int cellHeight = 20;

  renderText(xPos, yPos, "Decode/Issue unit");
  drawTable(xPos, yPos + cellHeight, 1, numOfVerticalCells, 20, cellHeight);
  drawTable(xPos + 20, yPos + cellHeight, numOfHorizontalCells, numOfVerticalCells, cellWidth, cellHeight);
  for(int i = 0; i < issueWindowSize; i++) {
    if(reorderBufferIndexes[i] != -1) {
      renderText(xPos, yPos + (i + 1) * cellHeight, intToString(reorderBufferIndexes[i]));
    }
    std::string instructionString = instructionToString(instructions[i]);
    if(instructionString != "NOOP") {
      renderText(xPos + 20, yPos + (i + 1) * cellHeight, instructionString);
    }
  }
}

void View::drawAluReservationStation(const int reservationStationSize, const Instruction instructions[], 
  const int reorderBufferIndexes[], const OperandType operandTypes[][3]) 
{
  //specification
  int xPos = 50;
  int yPos = 160;
  int numOfVerticalCells = reservationStationSize;
  int cellWidth = 40;
  int cellHeight = 20;
  int instructionCellWidth = 50;

  //draw label
  renderText(xPos, yPos, "ALU");
  renderText(xPos, yPos + cellHeight, "Reservation Station : ");

  //draw table
  drawTable(xPos, yPos + cellHeight * 2, 1, numOfVerticalCells, cellWidth, cellHeight);
  drawTable(xPos + cellWidth, yPos + cellHeight * 2, 1, numOfVerticalCells, instructionCellWidth, cellHeight);
  drawTable(xPos + cellWidth + instructionCellWidth, yPos + cellHeight * 2, 2, numOfVerticalCells, cellWidth, cellHeight);

  for(int i = 0; i < reservationStationSize; i++) {
    if(reorderBufferIndexes[i] != -1) {
      //draw reorder buffer index
      renderText(xPos, yPos + (2 + i) * cellHeight, intToString(reorderBufferIndexes[i]));
      //draw opcode
      renderText(xPos + cellWidth, yPos + (2 + i) * cellHeight, opcodeToString(instructions[i].opcode));
      //draw operands
      for(int j = 1; j < 3; j++) {
        std::string operandText;
        if(operandTypes[i][j] == REGISTER) {
          operandText = "R" + intToString(instructions[i].operands[j]);
        }
        else if(operandTypes[i][j] == CONSTANT) {
          operandText = intToString(instructions[i].operands[j]);
        }
        renderText(xPos + instructionCellWidth + j * cellWidth, yPos + (2 + i) * cellHeight, operandText);
      }
    }
  }
}

void View::drawBranchUnitReservationStation(const int reservationStationSize, const Instruction instructions[], 
  const int reorderBufferIndexes[], const OperandType operandTypes[][3]) 
{
  //specification
  int xPos = 250;
  int yPos = 160;
  int numOfVerticalCells = reservationStationSize;
  int cellWidth = 40;
  int cellHeight = 20;
  int instructionCellWidth = 50;

  //draw label
  renderText(xPos, yPos, "Branch Unit");
  renderText(xPos, yPos + cellHeight, "Reservation Station : ");

  //draw table
  drawTable(xPos, yPos + cellHeight * 2, 1, numOfVerticalCells, cellWidth, cellHeight);
  drawTable(xPos + cellWidth, yPos + cellHeight * 2, 1, numOfVerticalCells, instructionCellWidth, cellHeight);
  drawTable(xPos + cellWidth + instructionCellWidth, yPos + cellHeight * 2, 2, numOfVerticalCells, cellWidth, cellHeight);

  for(int i = 0; i < reservationStationSize; i++) {
    if(reorderBufferIndexes[i] != -1) {
      //draw reorder buffer index
      renderText(xPos, yPos + (2 + i) * cellHeight, intToString(reorderBufferIndexes[i]));
      //draw opcode
      renderText(xPos + cellWidth, yPos + (2 + i) * cellHeight, opcodeToString(instructions[i].opcode));
      //draw operands
      for(int j = 0; j < 2; j++) {
        std::string operandString;
        if(operandTypes[i][j] == REGISTER) {
          operandString = "R" + intToString(instructions[i].operands[j]);
        }
        else if(operandTypes[i][j] == CONSTANT) {
          operandString = intToString(instructions[i].operands[j]);
        }
        renderText(xPos + instructionCellWidth + (j + 1) * cellWidth, yPos + (2 + i) * cellHeight, operandString);
      }
    }
  }
}

void View::drawStoreQueue(const int size, const Instruction instructions[], const int reorderBufferIndexes[], 
  const bool validBits[][3])
{
  //specifications
  int xPos = 450;
  int yPos = 220;
  int numOfVerticalCells = size;
  int cellWidth = 40;
  int cellHeight = 20;
  int instructionCellWidth = 50;

  //draw label
  renderText(xPos, yPos, "Store Queue");

  //draw table
  drawTable(xPos, yPos + cellHeight, 1, numOfVerticalCells, cellWidth, cellHeight);
  drawTable(xPos + cellWidth, yPos + cellHeight, 1, numOfVerticalCells, instructionCellWidth, cellHeight);
  drawTable(xPos + cellWidth + instructionCellWidth, yPos + cellHeight, 2, numOfVerticalCells, cellWidth, cellHeight);

  for(int i = 0; i < size; i++) {
    if(reorderBufferIndexes[i] != -1) {
      //draw reorder buffer index
      renderText(xPos, yPos + (1 + i) * cellHeight, intToString(reorderBufferIndexes[i]));
      //draw opcode
      renderText(xPos + cellWidth, yPos + (1 + i) * cellHeight, opcodeToString(instructions[i].opcode));
      //draw operands
      for(int j = 0; j < 2; j++) {
        std::string operandString;
        if(!validBits[i][j]) {
          operandString = "R" + intToString(instructions[i].operands[j]);
        }
        else {
          operandString = intToString(instructions[i].operands[j]);
        }
        renderText(xPos + instructionCellWidth + (1 + j) * cellWidth, yPos + (1 + i) * cellHeight, operandString);
      }
    }
  }
}

void View::drawLoadQueue(const int size, const Instruction instructions[], const int reorderBufferIndexes[], 
  const bool validBits[][3])
{
  //specification
  int xPos = 450;
  int yPos = 120;
  int numOfVerticalCells = size;
  int cellWidth = 40;
  int cellHeight = 20;
  int instructionCellWidth = 50;

  //draw label
  renderText(xPos, yPos, "Load Queue");

  //draw table
  drawTable(xPos, yPos + cellHeight, 1, numOfVerticalCells, cellWidth, cellHeight);
  drawTable(xPos + cellWidth, yPos + cellHeight, 1, numOfVerticalCells, instructionCellWidth, cellHeight);
  drawTable(xPos + cellWidth + instructionCellWidth, yPos + cellHeight, 2, numOfVerticalCells, cellWidth, cellHeight);

  for(int i = 0; i < size; i++) {
    if(reorderBufferIndexes[i] != -1) {
      //draw reorder buffer index
      renderText(xPos, yPos + (1 + i) * cellHeight, intToString(reorderBufferIndexes[i]));
      //draw opcode
      renderText(xPos + cellWidth, yPos + (1 + i) * cellHeight, opcodeToString(instructions[i].opcode));
      //draw operands
      for(int j = 0; j < 2; j++) {
        std::string operandString;
        if(!validBits[i][j]) {
          operandString = "R" + intToString(instructions[i].operands[j]);
        }
        else {
          operandString = intToString(instructions[i].operands[j]);
        }
        renderText(xPos + instructionCellWidth + (1 + j) * cellWidth, yPos + (1 + i) * cellHeight, operandString);
      }
    }
  }
}

void View::drawAlu(const int numALUs, const int* const results, const int* const reorderBufferIndexes) {
  int xPos = 50;
  int yPos = 300;
  int numOfHorizontalCells = 2;
  int numOfVerticalCells = 1;
  int cellWidth = 20;
  int cellHeight = 20;

  for(int i = 0; i < numALUs; i++) {
    renderText(xPos + i * 50, yPos - cellHeight, "ALU : ");
    drawTable(xPos + i * 50, yPos, numOfHorizontalCells, numOfVerticalCells, cellWidth, cellHeight);
    if(reorderBufferIndexes[i] != -1) {
      renderText(xPos + i * 50, yPos, intToString(reorderBufferIndexes[i]));
      renderText(xPos + i * 50 + cellWidth, yPos, intToString(results[i]));
    }
  }
}

void View::drawBranchUnit(const bool successful, const int reorderBufferIndex) {
  int xPos = 250;
  int yPos = 300;
  int numOfHorizontalCells = 2;
  int numOfVerticalCells = 1;
  int cellWidth = 20;
  int cellHeight = 20;

  renderText(xPos, yPos - cellHeight, "Branch Unit : ");
  drawTable(xPos, yPos, numOfHorizontalCells, numOfVerticalCells, cellWidth, cellHeight);
  if(reorderBufferIndex != -1) {
    renderText(xPos, yPos, intToString(reorderBufferIndex));
    renderText(xPos + cellWidth, yPos, intToString(successful));
  }
}

void View::drawReorderBuffer(const int size, const int numFields, const int tailIndex, const int headIndex, const Instruction* const instructions, int** const fields) {
  int xPos = 690;
  int yPos = 10;
  int numOfHorizontalCells = numFields;
  int numOfVerticalCells = size;
  int cellWidth = 20;
  int cellHeight = 20;
  int textCellWidth = 150;

  renderText(xPos, yPos, "Reorder Buffer : ");

  drawTable(xPos, yPos + cellHeight, 1, numOfVerticalCells, cellWidth, cellHeight);
  drawTable(xPos + cellWidth, yPos + cellHeight, 1, numOfVerticalCells, textCellWidth, cellHeight);
  drawTable(xPos + cellWidth + textCellWidth, yPos + cellHeight, numOfHorizontalCells, numOfVerticalCells, cellWidth, cellHeight);

  for(int i = 0; i < size; i++) {
    renderText(xPos, yPos + (i+1) * cellHeight, intToString(i));
    if(fields[i][3] != -1) {
      renderText(xPos + cellWidth, yPos + (i+1) * cellHeight, instructionToString(instructions[i]));
      for(int j = 0; j < numFields; j++) {
        renderText(xPos + textCellWidth + (j+1) * cellWidth, yPos + (i+1) * cellHeight, intToString(fields[i][j]));
      }
    }
  }

  renderText(xPos - 10, yPos + (tailIndex + 1) * cellHeight, "T");
  renderText(xPos - 20, yPos + (headIndex + 1) * cellHeight, "H");
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