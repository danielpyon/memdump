#include <iostream>
#include <unistd.h>

#include "dump.h"
#include "address.h"

#define ROWS 10
#define COLS 20

Dump::Dump(const Glib::RefPtr<Gtk::Application>& app)
: m_box(Gtk::ORIENTATION_VERTICAL), proc_handler(*this) {
    menuw = nullptr;
    addrw = nullptr;

    set_size_request(1210, 500);
    set_resizable(false);
    add(m_box);

    /// toolbar
    // define the actions:
    m_refActionGroup = Gio::SimpleActionGroup::create();
    m_refActionGroup->add_action("select",
        sigc::mem_fun(*this, &Dump::on_action_select_process));
    m_refActionGroup->add_action("addr",
        sigc::mem_fun(*this, &Dump::on_action_select_addr));

    insert_action_group("actions", m_refActionGroup);

    // define how the actions are presented in the menus and toolbars:
    m_refBuilder = Gtk::Builder::create();

    app->set_accel_for_action("actions.select", "<Primary>p");
    app->set_accel_for_action("actions.addr", "<Primary>a");

    Glib::ustring ui_info =
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
        "<!-- Generated with glade 3.22.1 -->"
        "<interface>"
        "<requires lib=\"gtk+\" version=\"3.20\"/>"
        "<object class=\"GtkToolbar\" id=\"toolbar\">"
        "    <property name=\"visible\">True</property>"
        "    <property name=\"can_focus\">False</property>"
        "    <child>"
        "    <object class=\"GtkToolButton\" id=\"select\">"
        "        <property name=\"action_name\">actions.select</property>"
        "        <property name=\"visible\">True</property>"
        "        <property name=\"can_focus\">False</property>"
        "        <property name=\"is_important\">True</property>"
        "        <property name=\"label\" translatable=\"yes\">Select Process</property>"
        "        <property name=\"use_underline\">True</property>"
        "        <property name=\"stock_id\">gtk-add</property>"
        "    </object>"
        "    <packing>"
        "        <property name=\"expand\">False</property>"
        "        <property name=\"homogeneous\">True</property>"
        "    </packing>"
        "    </child>"
        "    <child>"
        "    <object class=\"GtkToolButton\" id=\"addr\">"
        "        <property name=\"action_name\">actions.addr</property>"
        "        <property name=\"visible\">True</property>"
        "        <property name=\"can_focus\">False</property>"
        "        <property name=\"is_important\">True</property>"
        "        <property name=\"label\" translatable=\"yes\">Select Address</property>"
        "        <property name=\"use_underline\">True</property>"
        "        <property name=\"stock_id\">gtk-edit</property>"
        "    </object>"
        "    <packing>"
        "        <property name=\"expand\">False</property>"
        "        <property name=\"homogeneous\">True</property>"
        "    </packing>"
        "    </child>"

        "</object>"
        "</interface>";

    try {
        m_refBuilder->add_from_string(ui_info);
    } catch(const Glib::Error& ex) {
        std::cerr << "Building toolbar failed: " <<  ex.what();
    }

    Gtk::Toolbar* toolbar = nullptr;
    m_refBuilder->get_widget("toolbar", toolbar);
    if (!toolbar)
        g_warning("GtkToolbar not found");
    else
        m_box.pack_start(*toolbar, Gtk::PACK_SHRINK);
    /// end toolbar

    m_ScrolledWindow.set_border_width(10);
    m_ScrolledWindow.set_policy(Gtk::POLICY_ALWAYS, Gtk::POLICY_ALWAYS);
    m_box.pack_start(m_ScrolledWindow);

    m_Grid.set_row_spacing(10);
    m_Grid.set_column_spacing(10);
    m_ScrolledWindow.add(m_Grid);

    // default mem
    for (int i = 0; i < COLS; i++) {
        for (int j = 0; j < ROWS; j++) {
            auto pButton = Gtk::make_managed<Gtk::ToggleButton>("--");
            m_Grid.attach(*pButton, i, j);
        }
    }

    show_all_children();
}

Dump::~Dump() { }

void Dump::menu_win_close() {
    delete menuw;
    menuw = nullptr;
}

void Dump::addr_win_close() {
    delete addrw;
    addrw = nullptr;
}

void Dump::ProcessHandler::on_process_selection(pid_t pid) {
    std::cout << "user selected pid " << pid << std::endl;

    parent.mt.reset(new memlib::MemoryTool(pid));
    parent.proc.reset(new memlib::Process(pid));

    // set title to process name
    parent.set_title(parent.proc->GetName());

    // get the first mapped address in virtual memory
    auto vmmap(parent.proc->GetVMMap());
    char* addr = vmmap->front().start;
    std::cout << "viewing: " << std::hex << (void*)addr << std::endl;
    delete vmmap;

    // start examining memory
    if (!parent.curr_conn.empty()) {
        parent.curr_conn.disconnect();
    }

    sigc::slot<bool()> slot = sigc::bind(sigc::mem_fun(parent,
        &Dump::poll_memory), addr);
    parent.curr_conn = Glib::signal_timeout().connect(slot, 1000);
}

void Dump::ProcessHandler::on_address_selection(char* addr) {
    std::cout << "user selected address " << std::hex << (void*)addr << std::endl;
    if (!parent.curr_conn.empty()) {
        parent.curr_conn.disconnect();
    }
    sigc::slot<bool()> slot = sigc::bind(sigc::mem_fun(parent,
        &Dump::poll_memory), addr);
    parent.curr_conn = Glib::signal_timeout().connect(slot, 1000);
}

bool Dump::poll_memory(char* addr) {
    std::cout << "polling memory..." << std::endl;
    char* data;
    if (addr == nullptr || !mt->ReadMem(addr, COLS * ROWS, &data)) {
        // just set empty cells
        for (int i = 0; i < COLS; i++) {
            for (int j = 0; j < ROWS; j++) {
                Gtk::ToggleButton* widget = static_cast<Gtk::ToggleButton*>(
                    m_Grid.get_child_at(i, j)
                );
                widget->set_label("--");
            }
        }
    } else {
        // ReadMem succeeded
        for (int i = 0; i < COLS; i++) {
            for (int j = 0; j < ROWS; j++) {
                Gtk::ToggleButton* widget = static_cast<Gtk::ToggleButton*>(
                    m_Grid.get_child_at(i, j)
                );
                char buf[32];
                sprintf(buf, "%x", data[i + j * ROWS] & 0xff);
                widget->set_label(Glib::ustring(buf));
            }
        }
        delete data;
    }
    return true;
}

void Dump::on_action_select_process() {
    if (menuw) {
        delete menuw;
        menuw = nullptr;
    }

    menuw = new Menu;
    menuw->signal_hide().connect(sigc::mem_fun(*this, &Dump::menu_win_close));
    menuw->signal_process_selection().connect(sigc::mem_fun(this->proc_handler,
        &Dump::ProcessHandler::on_process_selection));
    menuw->show();
}

void Dump::on_action_select_addr() {
    if (addrw) {
        delete addrw;
        addrw = nullptr;
    }

    if (!proc) {
        return;
    }

    addrw = new Address(proc);

    addrw->signal_hide().connect(sigc::mem_fun(*this, &Dump::addr_win_close));
    addrw->signal_address_selection().connect(sigc::mem_fun(this->proc_handler,
        &Dump::ProcessHandler::on_address_selection));
    addrw->show();
}
