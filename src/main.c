/**
 * main.c
 * GTK application entry point for Tic-Tac-Toe with Minimax AI
 */

#include <gtk/gtk.h>
#include "gtk_ui.h"

int main(int argc, char *argv[]) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("com.fop2.tictactoe", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate_tic_tac_toe_ui), NULL);

    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;

}
