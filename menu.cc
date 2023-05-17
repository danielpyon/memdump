#include <iostream>

#include "menu.h"
#include "memlib.h"

Menu::Menu() {
    set_resizable(false);

    set_title("Select a process");
    set_border_width(50);

    add(m_box);

    // create the Tree model:
    m_refTreeModel = Gtk::ListStore::create(m_Columns);
    m_Combo.set_model(m_refTreeModel);

    // fill the ComboBox's Tree Model:
    std::vector<std::unique_ptr<memlib::Process>> procs;
    std::vector<pid_t>* pids(memlib::get_all_pids());

    std::size_t middle = pids->size() / 2;

    auto it(pids->begin());
    for (std::size_t i = 0; i < pids->size(); i++) {
        pid_t pid = *it;

        procs.push_back(
            std::make_unique<memlib::Process>(
                memlib::Process(pid)
            )
        );

        Gtk::TreeModel::Row row = *(m_refTreeModel->append());
        row[m_Columns.m_col_pid] = pid;
        row[m_Columns.m_col_name] = procs.back()->GetName();

        if (i == middle) {
            // only set first row to active
            m_Combo.set_active(row);
        }

        it++;
    }

    // free memory
    procs.clear();
    delete pids;

    // add the model columns to the Combo (which is a kind of view),
    // rendering them in the default way:
    m_Combo.pack_start(m_Columns.m_col_pid);
    m_Combo.pack_start(m_Columns.m_col_name);
    m_Combo.pack_start(m_cell);

    // add the ComboBox to the window.
    // add(m_Combo);
    m_box.pack_start(m_Combo);

    // connect signal handler:
    m_Combo.signal_changed().connect( sigc::mem_fun(*this, &Menu::on_combo_changed) );
    show_all_children();

    // show button
    m_button = Gtk::Button("Select");
    m_button.signal_clicked().connect(sigc::mem_fun(*this,
        &Menu::on_button_clicked));
    m_box.pack_start(m_button);
    // add(m_button);
    m_box.pack_start(m_button);
    m_button.show();
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
    auto row = get_selected_row();
    if (row) {
        pid_t pid = static_cast<pid_t>(row[m_Columns.m_col_pid]);
        Glib::ustring name = row[m_Columns.m_col_name];

        std::cout << " PID=" << pid << ", name=" << name << std::endl;
    }
}

void Menu::on_button_clicked() {
    auto row = get_selected_row();
    if (row) {
        pid_t pid = static_cast<pid_t>(row[m_Columns.m_col_pid]);
        m_signal_process_selection.emit(pid);
    }
}

Menu::pid_signal_t Menu::signal_process_selection() {
  return m_signal_process_selection;
}

Gtk::TreeModel::Row Menu::get_selected_row() {
    Gtk::TreeModel::iterator iter = m_Combo.get_active();
    return *iter;
}
