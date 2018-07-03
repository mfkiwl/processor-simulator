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

//=================================
// interface

class View {
 
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

    //processor information
    const int numOfRegisters;
    const int memorySize;
    const int aluReservationStationSize;
    const int branchUnitReservationStationSize;
    const int loadStoreUnitReservationStationSize;
    const int reorderBufferSize;

  public:

    //Constructor
    View(const int numOfRegisters, const int memorySize, const int aluReservationStationSize, 
        const int branchUnitReservationStationSize, const int loadStoreUnitReservationStationSize, 
        const int reorderBufferSize);

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

    void drawRegisterFile(const int* const registerValues, const int* const scoreBoard);

    void drawMemory(const int* const memoryValues);

    void drawProcessorStats(const int numOfInstructionsExecuted, const int numOfClockCycles, const float numOfInstructionsExecutedPerCycle);

    void drawPC(const int programCounter);

    void drawFetchUnit(const Instruction currentInstruction);

    void drawDecodeIssueUnit(const Instruction instruction);

    void drawAluReservationStation(const Instruction* const instructions, const int* const reorderBufferIndexes);

    void drawBranchUnitReservationStation(const Instruction* const instructions, const int* const reorderBufferIndexes);

    void drawLoadStoreUnitReservationStation(const Instruction* const loadQueueInstructions, const int* const loadQueueReorderBufferIndexes, const int* const loadQueueAges, const Instruction* const storeQueueInstructions, const int* const storeQueueReorderBufferIndexes, const int* const storeQueueAges);

    void drawAlu(const int result, const int reorderBufferIndex);

    void drawBranchUnit(const bool successful, const int reorderBufferIndex);

    void drawReorderBuffer(const Instruction* const instructions, int** const fields);

    void clearScreen();

    void updateScreen();

    //============================================
    // getter functions

    int hasQuit() const;
};

#endif