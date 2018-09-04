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

    void drawTable(const int xPos, const int yPos, const int noOfHorizontalCells, const int noOfVerticalCells, 
      const int cellWidth, const int cellHeight);

    void drawTextCell(const int xPos, const int yPos, const int width, const int height, const std::string text, 
      const int xOffset, const int yOffset);

    void drawRegisterFile(const int numRegisters, const int registerValues[], const int renameTable[]);

    void drawMemory(const int memorySize, const int memoryValues[]);

    void drawProcessorStats(const int numOfInstructionsExecuted, const int numOfClockCycles, 
      const float numOfInstructionsExecutedPerCycle);

    void drawPC(const int programCounter);

    void drawFetchUnit(const int issueWindowSize, const Instruction instructions[]);

    void drawDecodeIssueUnit(const int issueWindowSize, const Instruction instructions[], 
      const int reorderBufferIndexes[]);

    void drawAluReservationStation(const int reservationStationSize, const Instruction instructions[], 
      const int reorderBufferIndexes[], const OperandType operandTypes[][3]);

    void drawBranchUnitReservationStation(const int reservationStationSize, const Instruction instructions[], 
      const int reorderBufferIndexes[], const OperandType OperandTypes[][3]);

    void drawStoreQueue(const int size, const Instruction instructions[], const int reorderBufferIndexes[], 
      const OperandType operandTypes[][3]);

    void drawLoadQueue(const int size, const Instruction instructions[], const int reorderBufferIndexes[], 
      const bool validBits[][3]);

    void drawAlu(const int numALUs, const int results[], const int reorderBufferIndexes[]);

    void drawBranchUnit(const bool successful, const int reorderBufferIndex);

    void drawReorderBuffer(const int size, const int tailIndex, const int headIndex, 
      const Instruction instructions[], int fields[][ReorderBufferIndex::COUNT]);

    void clearScreen();

    void updateScreen();

  //getters and setters
  public:

    int hasQuit() const;
};

#endif