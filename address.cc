#include <vector>
#include "address.h"
#include "memlib.h"

template <typename T>
static std::string to_string(const T& value);

Address::Address(std::shared_ptr<memlib::Process> proc)
: m_HPaned(Gtk::ORIENTATION_HORIZONTAL), m_VPaned(Gtk::ORIENTATION_VERTICAL) {
    this->proc = proc;
    vmmap = proc->GetVMMap();

    m_VPaned.set_position(430);

    set_title("Select an address");
    set_resizable(false);
    set_border_width(10);
    set_default_size(800, 500);

    add(m_VPaned);

    // text
    m_ScrolledWindow.add(m_TextView);
    m_ScrolledWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
    m_VPaned.add1(m_ScrolledWindow);

    // button
    m_button = Gtk::Button("Select");
    m_button.signal_clicked().connect(sigc::mem_fun(*this,
        &Address::on_button_clicked));
    m_button.show();

    // add stuff to hpane
    m_Entry.set_max_length(20);
    m_Entry.signal_activate().connect(sigc::mem_fun(*this,
        &Address::on_button_clicked));
    m_HPaned.add1(m_Entry);

    m_HPaned.add2(m_button);
    m_HPaned.set_position(700);

    // add hpane to vpane
    m_VPaned.add2(m_HPaned);

    m_refTextBuffer = Gtk::TextBuffer::create();
    m_refTextBuffer->set_text(to_string(*proc));
    m_TextView.set_buffer(m_refTextBuffer);
  
    show_all_children();
}

Address::~Address() {
    delete vmmap;
}

void Address::on_button_clicked() {
    char* addr;

    try {
        addr = (char*)std::stoul(m_Entry.get_text(), nullptr, 0);

        bool valid = false;
        for (const auto& e: *vmmap) {
            if (addr >= e.start || addr <= e.end) {
                valid = true;
                break;
            }
        }

        if (valid) {
            m_signal_address_selection.emit(addr);
            close();
        }
    } catch(const std::invalid_argument& ia) {
        std::cerr << "Invalid argument: " << ia.what() << std::endl;
    }
}

Address::pid_signal_t Address::signal_address_selection() {
    return m_signal_address_selection;
}

template <typename T>
static std::string to_string(const T& value) {
    std::ostringstream ss;
    ss << value;
    return ss.str();
}
