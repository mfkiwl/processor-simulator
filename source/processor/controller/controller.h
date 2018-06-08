//===========================
// include guard
#ifndef CONTROLLER_H
#define CONTROLLER_H

//=================================
// included dependencies
#include "../model/instructions.h"
#include "../model/model.h"
#include "../view/view.h"

//===========================
// interface
class Controller {

  public:

  	const int numOfRegisters;
  	const int memorySize;
    const int aluReservationStationSize;
    const int branchUnitReservationStationSize;
    const int loadStoreUnitReservationStationSize;
    const int reorderBufferSize;

    Model model;
    View view;

    Controller(const Instructions instructions, const int numOfRegisters, const int memorySize, 
  const int aluReservationStationSize, const int branchUnitReservationStationSize, 
  const int loadStoreUnitReservationStationSize, const int reorderBufferSize);

    int modelMain(const Instructions instructions);

    int viewmain(const int argc, const char *argv[]);

    void updateView();

    int start(const Instructions instruction);

};

#endif