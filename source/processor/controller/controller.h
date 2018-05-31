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

    Model model;
    View view;

    Controller(Instructions instructions);

    int modelMain(Instructions instructions);

    int viewmain(int argc, char *argv[]);

    int start(Instructions instruction);

};

#endif