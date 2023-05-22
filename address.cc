#include "address.h"
#include "memlib.h"

template <typename T>
static std::string to_string(const T& value);

Address::Address(std::shared_ptr<memlib::Process> proc)
: m_HPaned(Gtk::ORIENTATION_HORIZONTAL), m_VPaned(Gtk::ORIENTATION_VERTICAL) {
    m_HPaned.set_position(500);

    set_title("Select an address");
    set_resizable(false);
    set_border_width(10);
    set_default_size(600, 400);

    add(m_HPaned);

    // text
    m_ScrolledWindow.add(m_TextView);
    m_ScrolledWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
    m_HPaned.add1(m_ScrolledWindow);

    // button
    m_button = Gtk::Button("Select");
    m_button.signal_clicked().connect(sigc::mem_fun(*this,
        &Address::on_button_clicked));
    m_button.show();

    // add stuff to vpane
    m_VPaned.add1(m_button);
    // m_VPaned.add2();
    // TODO: add Entry
    // https://developer-old.gnome.org/gtkmm-tutorial/3.24/sec-text-entry.html.en

    // add vpane to hpane
    m_HPaned.add2(m_VPaned);

    m_refTextBuffer = Gtk::TextBuffer::create();
    m_refTextBuffer->set_text(to_string(*proc));
    m_TextView.set_buffer(m_refTextBuffer);

    show_all_children();
}

Address::~Address() { }

void Address::on_button_clicked() {
    /*
    m_signal_address_selection.emit(someaddr);
    close();
    */
    close();
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
