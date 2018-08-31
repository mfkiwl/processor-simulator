//=================================
// include guard
#ifndef VIEW_H
#define VIEW_H

//=================================
//forward declared dependencies
class SDL_Window;
class SDL_Renderer;
class SDL_Texture;
class SDL_Texture;
class Instruction;


//=================================
// included dependencies
#include <string>
#include <SDL2/SDL_ttf.h>

#include "lTexture.h"
#include "../model/constants.h"

//=================================
// interface

class View {

  //private member variables
  private:
 
	//Screen dimension constants
    const int SCREEN_WIDTH;
    const int SCREEN_HEIGHT;

    //The window we'll be rendering to
    SDL_Window* gWindow;

    //The window renderer
    SDL_Renderer* gRenderer;

    //Globally used font
    TTF_Font* gFont;

    //Rendered texture
    LTexture gTextTexture;

    //Main loop flag
	bool quit;

  //public functions
  public:

    //Constructor
    View();

    //Starts up SDL and creates window
    bool init();

    //Loads media
    bool loadMedia();

    //Frees media and shuts down SDL
    void close();

    void renderText(const int x, const int y, const std::string text);

    //handle events
    void eventHandler();

    void drawTable(const int xPos, const int yPos, const int noOfHorizontalCells, const int noOfVerticalCells, const int cellWidth, const int cellHeight);

    void drawTextCell(const int xPos, const int yPos, const int width, const int height, const std::string text, const int xOffset, const int yOffset);

    void drawRegisterFile(const int numRegisters, const int* const registerValues, const int* const renameTable);

    void drawMemory(const int memorySize, const int* const memoryValues);

    void drawProcessorStats(const int numOfInstructionsExecuted, const int numOfClockCycles, const float numOfInstructionsExecutedPerCycle);

    void drawPC(const int programCounter);

    void drawFetchUnit(const int issueWindowSize, const Instruction* const instructions);

    void drawDecodeIssueUnit(const int issueWindowSize, const Instruction* const instructions, const int* const reorderBufferIndexes);

    void drawAluReservationStation(const int reservationStationSize, const Instruction* const instructions, 
      const int* const reorderBufferIndexes, OperandType** const operandTypes);

    void drawBranchUnitReservationStation(const int reservationStationSize, const Instruction* const instructions, 
      const int* const reorderBufferIndexes, bool** const validBits);

    void drawStoreQueue(const int size, const Instruction* const instructions, 
      const int* const reorderBufferIndexes, bool** const validBits);

    void drawLoadQueue(const int size, const Instruction* const instructions, 
      const int* const reorderBufferIndexes, bool** const validBits);

    void drawAlu(const int numALUs, const int* const results, const int* const reorderBufferIndex);

    void drawBranchUnit(const bool successful, const int reorderBufferIndex);

    void drawReorderBuffer(const int size, const int numFields, const int tailIndex, const int headIndex, 
      const Instruction* const instructions, int** const fields);

    void clearScreen();

    void updateScreen();

  //getters and setters
  public:

    int hasQuit() const;
};

#endif