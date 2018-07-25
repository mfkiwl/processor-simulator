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

  //private member variables
  public:

    Model model;
    View view;

  //public functions
  public:

    Controller(const Instructions instructions);

    int modelMain(const Instructions instructions);

    int viewmain(const int argc, const char *argv[]);

    void updateView();

    int start(const Instructions instruction);

};

#endif