#ifndef MEMDUMP_MENU_H
#define MEMDUMP_MENU_H

#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/liststore.h>
#include <sigc++/sigc++.h>
#include "memlib.h"

class Menu : public Gtk::Window {
  public:
    Menu();
    virtual ~Menu();
    typedef sigc::signal<void, pid_t> pid_signal_t;
    pid_signal_t signal_process_selection();

  protected:
    pid_signal_t m_signal_process_selection;

    void on_button_clicked();
    void on_combo_changed();
    Gtk::TreeModel::Row get_selected_row();

    class ModelColumns : public Gtk::TreeModel::ColumnRecord {
      public:
        ModelColumns()
        { add(m_col_pid); add(m_col_name); }

        Gtk::TreeModelColumn<int> m_col_pid;
        Gtk::TreeModelColumn<Glib::ustring> m_col_name;
    };

    ModelColumns m_Columns;

    Gtk::ComboBox m_Combo;
    Gtk::CellRendererText m_cell;
    Glib::RefPtr<Gtk::ListStore> m_refTreeModel;
    Gtk::Button m_button;
    Gtk::Box m_box;
};

#endif
