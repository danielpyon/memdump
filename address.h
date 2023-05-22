#ifndef MEMDUMP_ADDRESS_H
#define MEMDUMP_ADDRESS_H

#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <sigc++/sigc++.h>
#include <gtkmm.h>
#include "memlib.h"

class Address : public Gtk::Window {
  public:
    Address(std::shared_ptr<memlib::Process> proc);
    virtual ~Address();
    typedef sigc::signal<void, char*> pid_signal_t;
    pid_signal_t signal_address_selection();

  protected:
    pid_signal_t m_signal_address_selection;

    void on_button_clicked();

    Gtk::Button m_button;

    Gtk::Paned m_HPaned;
    Gtk::Paned m_VPaned;
    Gtk::ScrolledWindow m_ScrolledWindow;
    Gtk::TextView m_TextView;
    Glib::RefPtr<Gtk::TextBuffer> m_refTextBuffer;
};

#endif
