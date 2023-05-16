#include <iostream>

#include "menu.h"
#include "memlib.h"

Menu::Menu() {
    set_title("Select a process");
    set_border_width(250);

    //Create the Tree model:
    //m_refTreeModel = Gtk::TreeStore::create(m_Columns);
    m_refTreeModel = Gtk::ListStore::create(m_Columns);
    m_Combo.set_model(m_refTreeModel);

    //Fill the ComboBox's Tree Model:
    std::vector<std::unique_ptr<memlib::Process>> procs;
    std::vector<pid_t>* pids(memlib::get_all_pids());
    bool is_first_row = true;

    for (const auto& pid : *pids) {
        procs.push_back(
            std::make_unique<memlib::Process>(
                memlib::Process(pid)
            )
        );

        Gtk::TreeModel::Row row = *(m_refTreeModel->append());
        row[m_Columns.m_col_pid] = pid;
        row[m_Columns.m_col_name] = procs.back()->GetName();

        if (is_first_row) {
            // only set first row to active
            m_Combo.set_active(row);
            is_first_row = false;
        }
    }

    // free memory
    procs.clear();
    delete pids;

    //Add the model columns to the Combo (which is a kind of view),
    //rendering them in the default way:
    m_Combo.pack_start(m_Columns.m_col_pid);
    m_Combo.pack_start(m_Columns.m_col_name);
    m_Combo.pack_start(m_cell);

    //Add the ComboBox to the window.
    add(m_Combo);

    //Connect signal handler:
    m_Combo.signal_changed().connect( sigc::mem_fun(*this, &Menu::on_combo_changed) );

    show_all_children();
}

Menu::~Menu() {
    Gtk::TreeIter iList = m_refTreeModel->get_iter("0");
    while (iList) {
        std::cout << (*iList)[m_Columns.m_col_name] << std::endl;
        ++iList;
    }
    m_refTreeModel->clear();
}

void Menu::on_combo_changed() {
    Gtk::TreeModel::iterator iter = m_Combo.get_active();
    if(iter) {
        Gtk::TreeModel::Row row = *iter;
        if(row) {
            int id = row[m_Columns.m_col_pid];
            Glib::ustring name = row[m_Columns.m_col_name];
            std::cout << " ID=" << id << ", name=" << name << std::endl;
        }
    }
    else
        std::cout << "invalid iter" << std::endl;
}
