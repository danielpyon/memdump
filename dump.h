#ifndef MEMDUMP_DUMP_H
#define MEMDUMP_DUMP_H

#include <gtkmm/button.h>
#include <gtkmm/window.h>

#include "menu.h"

class Dump : public Gtk::Window {

  public:
    Dump();
    virtual ~Dump();

  protected:
    void on_button_clicked();
    void hello_win_close();
    Gtk::Button m_button;
    Menu* menuw;
};

#endif
