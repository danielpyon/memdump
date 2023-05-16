#ifndef MEMDUMP_MENU_H
#define MEMDUMP_MENU_H

#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/liststore.h>

class Menu : public Gtk::Window {

  public:
    Menu();
    virtual ~Menu();

  protected:
    void on_button_clicked();
    void on_combo_changed();

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
};

#endif
