#include <iostream>
#include "dump.h"

Dump::Dump()
: m_button("Select a process"), proc_handler(*this) {
    set_border_width(300);

    m_button.signal_clicked().connect(sigc::mem_fun(*this,
        &Dump::on_button_clicked));

    add(m_button);
    menuw = nullptr;
    m_button.show();
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
