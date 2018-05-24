//===========================
// include guard
#ifndef CONTROLLER_H
#define CONTROLLER_H

//=================================
// included dependencies
#include "model/processor.h"
#include "view/view.h"
#include "model/instructions.h"

//===========================
// interface
class Controller {

  public:

    Processor processor;
    View view;

    Controller(Instructions instructions);

    int processorMain(Instructions instructions);

    int viewmain(int argc, char *argv[]);

};

#endif