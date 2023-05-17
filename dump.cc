#include <iostream>
#include "dump.h"

Dump::Dump()
: m_button("Select a process") {
    set_border_width(250);

    m_button.signal_clicked().connect(sigc::mem_fun(*this,
        &Dump::on_button_clicked));

    add(m_button);
    menuw = nullptr;
    m_button.show();
}

Dump::~Dump() {
    
}

void Dump::on_button_clicked() {
    if(menuw) {
        // render memory
        return;
    }

    std::cout << "clicked!" << std::endl;

    menuw = new Menu;
    menuw->signal_hide().connect(sigc::mem_fun(*this, &Dump::menu_win_close));
    menuw->show();
}

void Dump::menu_win_close() {
    
}
