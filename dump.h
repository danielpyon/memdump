#ifndef MEMDUMP_DUMP_H
#define MEMDUMP_DUMP_H

#include <gtkmm.h>
#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <sigc++/sigc++.h>

#include "menu.h"

class Dump : public Gtk::Window {
  public:
    Dump(const Glib::RefPtr<Gtk::Application>& app);
    virtual ~Dump();

    // handle process selection
    class ProcessHandler : public sigc::trackable {
      public:
        ProcessHandler(Dump& x): parent(x) { }
        void on_process_selection(pid_t pid);
      private:
        Dump& parent;
    };

  protected:
    void on_button_clicked();
    void menu_win_close();

    void on_action_select_process();

    Gtk::ScrolledWindow m_ScrolledWindow;
    Gtk::Grid m_Grid;
    Gtk::Box m_box;

    Glib::RefPtr<Gtk::Builder> m_refBuilder;
    Glib::RefPtr<Gio::SimpleActionGroup> m_refActionGroup;

    Menu* menuw;
    ProcessHandler proc_handler;
};

#endif
