#include <gtkmm/application.h>

#include "dump.h"
#include "memlib.h"

int main (int argc, char *argv[]) {
    auto app = Gtk::Application::create(argc, argv, "memdump");
    Dump dump;
    return app->run(dump);
}

