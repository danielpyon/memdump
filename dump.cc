#include <iostream>
#include "dump.h"

Dump::Dump()
: m_button("Select a process"), proc_handler(*this) {
    menuw = nullptr;

    set_size_request(1200, 500);
    set_resizable(false);
    add(m_box);

    m_button.signal_clicked().connect(sigc::mem_fun(*this,
        &Dump::on_button_clicked));

    m_box.pack_start(m_button);
    m_button.show();

    m_ScrolledWindow.set_border_width(10);
    m_ScrolledWindow.set_policy(Gtk::POLICY_ALWAYS, Gtk::POLICY_ALWAYS);
    m_box.pack_start(m_ScrolledWindow);

    m_Grid.set_row_spacing(10);
    m_Grid.set_column_spacing(10);
    m_ScrolledWindow.add(m_Grid);

    // default mem
    for(int i = 0; i < 64; i++) {
        for(int j = 0; j < 64; j++) {
            auto pButton = Gtk::make_managed<Gtk::ToggleButton>("--");
            m_Grid.attach(*pButton, i, j);
        }
    }

    show_all_children();
}

Dump::~Dump() {
    
}

void Dump::on_button_clicked() {
    if (menuw) {
        // render memory
        return;
    }

    std::cout << "clicked!" << std::endl;

    menuw = new Menu;
    menuw->signal_hide().connect(sigc::mem_fun(*this, &Dump::menu_win_close));
    menuw->signal_process_selection().connect(sigc::mem_fun(this->proc_handler,
        &Dump::ProcessHandler::on_process_selection));
    menuw->show();
}

void Dump::menu_win_close() {
    
}

void Dump::ProcessHandler::on_process_selection(pid_t pid) {
    std::cout << "user selected pid " << pid << std::endl;
}
