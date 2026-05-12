#include "gtk_ui.h"

#include <string.h>
#include "game.h"
#include "minimax.h"

typedef enum {
    MATCH_HUMAN_VS_HUMAN = 0,
    MATCH_HUMAN_VS_COMPUTER = 1
} MatchMode;

typedef struct {
    GtkWidget *window;
    GtkWidget *status_label;
    GtkWidget *score_label;
    GtkWidget *mode_combo;
    GtkWidget *symbol_combo;
    GtkWidget *new_match_button;
    GtkWidget *reset_score_button;
    GtkWidget *cells[3][3];
    Board *board;
    MatchMode mode;
    char current_player;
    char human_symbol;
    char computer_symbol;
    gboolean match_finished;
    int x_wins;
    int o_wins;
    int draws;
} TicTacToeApp;

typedef struct {
    TicTacToeApp *app;
    int row;
    int col;
} CellData;

static const char *APP_CSS =
    "window {"
    "  background-image: linear-gradient(135deg, #eadfce, #d8c3a5);"
    "}"
    "#hero-panel, #control-panel, #board-panel {"
    "  background: rgba(255, 248, 240, 0.92);"
    "  border-radius: 22px;"
    "  border: 1px solid #cdb79e;"
    "}"
    "#title-label {"
    "  color: #1d3557;"
    "  font-size: 30px;"
    "  font-weight: 700;"
    "}"
    "#status-label {"
    "  background: #fff8ee;"
    "  color: #9a3412;"
    "  border-radius: 14px;"
    "  border: 1px solid #d9c0ab;"
    "  padding: 12px 14px;"
    "  font-weight: 600;"
    "}"
    "#score-label {"
        "  color: #243b53;"
        "  font-weight: 600;"
    "}"
    "#guide-label {"
    "  color: #52606d;"
    "  background: #f6ede2;"
    "  border: 1px solid #dbc7b1;"
    "  border-radius: 16px;"
    "  padding: 14px;"
    "  line-height: 1.5;"
    "}"
    "button, combobox box {"
    "  font-size: 14px;"
    "}"
    "button.action-button {"
    "  background: #ead8c1;"
    "  color: #1d3557;"
    "  border-radius: 12px;"
    "  border: 1px solid #c49d73;"
    "  padding: 10px 14px;"
    "  font-weight: 600;"
    "}"
    "button.action-button:hover {"
    "  background: #ddc3a2;"
    "}"
    "button.cell-button {"
    "  min-height: 120px;"
    "  min-width: 120px;"
    "  background: #fffaf4;"
    "  color: #14213d;"
    "  border-radius: 18px;"
    "  border: 1px solid #ccb89e;"
    "  font-size: 34px;"
    "  font-weight: 700;"
    "}"
    "button.cell-button:hover {"
    "  background: #f2e2cf;"
    "  border-color: #b87942;"
    "}"
    "button.cell-button:disabled {"
    "  background: #fcf2e5;"
    "}"
    "button.cell-x {"
    "  color: #b45309;"
    "}"
    "button.cell-o {"
    "  color: #1d4ed8;"
    "}"
    "label.section-label {"
    "  color: #3e4c59;"
    "  font-weight: 700;"
    "}";

static void app_refresh_board(TicTacToeApp *app);
static void app_refresh_status(TicTacToeApp *app, const char *message);
static void app_start_match(TicTacToeApp *app);
static void app_finish_match(TicTacToeApp *app, const char *message);
static gboolean app_mode_is_ai(const TicTacToeApp *app);
static void app_advance_turn(TicTacToeApp *app);
static void app_run_ai_turn(TicTacToeApp *app);
static gboolean app_schedule_ai_turn(gpointer user_data);
static void app_update_cell_style(GtkWidget *button, char value);

static void app_apply_css(void) {
    GtkCssProvider *provider;
    GdkScreen *screen;

    provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, APP_CSS, -1, NULL);

    screen = gdk_screen_get_default();
    gtk_style_context_add_provider_for_screen(
        screen,
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    g_object_unref(provider);
}

static GtkWidget *app_create_panel(const char *name) {
    GtkWidget *frame;

    frame = gtk_frame_new(NULL);
    gtk_widget_set_name(frame, name);
    gtk_frame_set_shadow_type(GTK_FRAME(frame), GTK_SHADOW_NONE);

    return frame;
}

static void app_update_scoreboard(TicTacToeApp *app) {
    char buffer[128];

    g_snprintf(
        buffer,
        sizeof(buffer),
        "Score   X: %d   O: %d   Draws: %d",
        app->x_wins,
        app->o_wins,
        app->draws);

    gtk_label_set_text(GTK_LABEL(app->score_label), buffer);
}

static void app_update_cell_style(GtkWidget *button, char value) {
    GtkStyleContext *context;

    context = gtk_widget_get_style_context(button);
    gtk_style_context_remove_class(context, "cell-x");
    gtk_style_context_remove_class(context, "cell-o");

    if (value == 'X') {
        gtk_style_context_add_class(context, "cell-x");
    } else if (value == 'O') {
        gtk_style_context_add_class(context, "cell-o");
    }
}

static void app_refresh_board(TicTacToeApp *app) {
    int row;
    int col;

    for (row = 0; row < 3; ++row) {
        for (col = 0; col < 3; ++col) {
            char value;
            char text[2];

            value = get_cell(app->board, row, col);
            text[0] = value == ' ' ? '\0' : value;
            text[1] = '\0';

            gtk_button_set_label(GTK_BUTTON(app->cells[row][col]), text);
            app_update_cell_style(app->cells[row][col], value);
            gtk_widget_set_sensitive(
                app->cells[row][col],
                !app->match_finished && value == ' ');
        }
    }

    app_update_scoreboard(app);
}

static void app_refresh_status(TicTacToeApp *app, const char *message) {
    gtk_label_set_text(GTK_LABEL(app->status_label), message);
}

static gboolean app_mode_is_ai(const TicTacToeApp *app) {
    return app->mode == MATCH_HUMAN_VS_COMPUTER;
}

static void app_advance_turn(TicTacToeApp *app) {
    app->current_player = app->current_player == 'X' ? 'O' : 'X';
}

static void app_finish_match(TicTacToeApp *app, const char *message) {
    app->match_finished = TRUE;
    app_refresh_board(app);

    if (message != NULL) {
        char buffer[196];
        g_snprintf(buffer, sizeof(buffer), "%s Click New Match to play again.", message);
        app_refresh_status(app, buffer);
    }
}

static void app_handle_post_move(TicTacToeApp *app, char actor) {
    if (check_win(app->board, actor)) {
        if (actor == 'X') {
            app->x_wins++;
        } else {
            app->o_wins++;
        }

        if (app_mode_is_ai(app)) {
            if (actor == app->human_symbol) {
                app_finish_match(app, "You won the round.");
            } else {
                app_finish_match(app, "The AI won this round.");
            }
        } else {
            char winner_message[64];
            g_snprintf(winner_message, sizeof(winner_message), "Player %c won the round.", actor);
            app_finish_match(app, winner_message);
        }
        return;
    }

    if (is_board_full(app->board)) {
        app->draws++;
        app_finish_match(app, "Draw. The board is full.");
        return;
    }

    app_advance_turn(app);

    if (app_mode_is_ai(app)) {
        if (app->current_player == app->computer_symbol) {
            app_refresh_status(app, "The AI is evaluating the best move...");
            g_timeout_add(220, app_schedule_ai_turn, app);
        } else {
            char status[48];
            g_snprintf(status, sizeof(status), "Your turn as %c.", app->human_symbol);
            app_refresh_status(app, status);
        }
    } else {
        char status[48];
        g_snprintf(status, sizeof(status), "Player %c's turn.", app->current_player);
        app_refresh_status(app, status);
    }
}

static void app_run_ai_turn(TicTacToeApp *app) {
    Move best_move;

    if (app->match_finished || !app_mode_is_ai(app) || app->current_player != app->computer_symbol) {
        return;
    }

    best_move = find_best_move(app->board, app->computer_symbol, app->human_symbol);
    if (best_move.row == -1 || best_move.col == -1) {
        app->draws++;
        app_finish_match(app, "Draw. No more moves were available.");
        return;
    }

    make_move(app->board, best_move.row, best_move.col, app->computer_symbol);
    app_refresh_board(app);
    app_handle_post_move(app, app->computer_symbol);
}

static gboolean app_schedule_ai_turn(gpointer user_data) {
    TicTacToeApp *app;

    app = user_data;
    app_run_ai_turn(app);
    return G_SOURCE_REMOVE;
}

static void app_start_match(TicTacToeApp *app) {
    int mode_index;
    int symbol_index;
    char status[96];

    mode_index = gtk_combo_box_get_active(GTK_COMBO_BOX(app->mode_combo));
    symbol_index = gtk_combo_box_get_active(GTK_COMBO_BOX(app->symbol_combo));

    app->mode = mode_index == 0 ? MATCH_HUMAN_VS_HUMAN : MATCH_HUMAN_VS_COMPUTER;
    app->human_symbol = symbol_index == 0 ? 'X' : 'O';
    app->computer_symbol = app->human_symbol == 'X' ? 'O' : 'X';
    app->current_player = 'X';
    app->match_finished = FALSE;

    clear_board(app->board);
    gtk_widget_set_sensitive(app->symbol_combo, app_mode_is_ai(app));
    app_refresh_board(app);

    if (app_mode_is_ai(app)) {
        g_snprintf(status, sizeof(status), "Match against the AI. You are playing as %c.", app->human_symbol);
        app_refresh_status(app, status);
        if (app->current_player == app->computer_symbol) {
            app_refresh_status(app, "The AI starts this match. Preparing move...");
            g_timeout_add(260, app_schedule_ai_turn, app);
        }
    } else {
        app_refresh_status(app, "Local two-player match. X starts.");
    }
}

static void on_new_match_clicked(GtkButton *button, gpointer user_data) {
    TicTacToeApp *app;
    (void)button;

    app = user_data;
    app_start_match(app);
}

static void on_reset_score_clicked(GtkButton *button, gpointer user_data) {
    TicTacToeApp *app;
    (void)button;

    app = user_data;
    app->x_wins = 0;
    app->o_wins = 0;
    app->draws = 0;
    app_refresh_board(app);
    app_refresh_status(app, "Score reset. Ready for a new set of matches.");
}

static void on_mode_changed(GtkComboBox *combo_box, gpointer user_data) {
    TicTacToeApp *app;
    (void)combo_box;

    app = user_data;
    app_start_match(app);
}

static void on_symbol_changed(GtkComboBox *combo_box, gpointer user_data) {
    TicTacToeApp *app;
    (void)combo_box;

    app = user_data;
    if (app_mode_is_ai(app)) {
        app_start_match(app);
    }
}

static void on_cell_clicked(GtkButton *button, gpointer user_data) {
    CellData *cell;
    TicTacToeApp *app;

    (void)button;
    cell = user_data;
    app = cell->app;

    if (app->match_finished) {
        return;
    }

    if (app_mode_is_ai(app) && app->current_player != app->human_symbol) {
        return;
    }

    if (!make_move(app->board, cell->row, cell->col, app->current_player)) {
        app_refresh_status(app, "That cell is already occupied. Choose another one.");
        return;
    }

    app_refresh_board(app);
    app_handle_post_move(app, app->current_player);
}

static void on_window_destroy(GtkWidget *widget, gpointer user_data) {
    TicTacToeApp *app;
    (void)widget;

    app = user_data;
    free_board(app->board);
    g_free(app);
}

static GtkWidget *create_labeled_combo(const char *label_text, GtkWidget **combo_out) {
    GtkWidget *box;
    GtkWidget *label;
    GtkWidget *combo;

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    label = gtk_label_new(label_text);
    combo = gtk_combo_box_text_new();

    gtk_widget_set_halign(label, GTK_ALIGN_START);
    gtk_style_context_add_class(gtk_widget_get_style_context(label), "section-label");
    gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), combo, FALSE, FALSE, 0);

    *combo_out = combo;
    return box;
}

void activate_tic_tac_toe_ui(GtkApplication *app, gpointer user_data) {
    TicTacToeApp *state;
    GtkWidget *window;
    GtkWidget *root;
    GtkWidget *hero_panel;
    GtkWidget *hero_box;
    GtkWidget *body_box;
    GtkWidget *control_panel;
    GtkWidget *control_box;
    GtkWidget *board_panel;
    GtkWidget *board_grid;
    GtkWidget *title_label;
    GtkWidget *guide_label;
    int row;
    int col;

    (void)user_data;

    app_apply_css();

    state = g_new0(TicTacToeApp, 1);
    state->board = create_board();
    if (state->board == NULL) {
        g_free(state);
        return;
    }

    window = gtk_application_window_new(app);
    state->window = window;
    gtk_window_set_title(GTK_WINDOW(window), "Tic-Tac-Toe Studio");
    gtk_window_set_default_size(GTK_WINDOW(window), 920, 660);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    root = gtk_box_new(GTK_ORIENTATION_VERTICAL, 22);
    gtk_container_set_border_width(GTK_CONTAINER(root), 24);
    gtk_container_add(GTK_CONTAINER(window), root);

    hero_panel = app_create_panel("hero-panel");
    hero_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_container_set_border_width(GTK_CONTAINER(hero_box), 24);
    gtk_container_add(GTK_CONTAINER(hero_panel), hero_box);

    title_label = gtk_label_new("Tic-Tac-Toe Studio");
    state->status_label = gtk_label_new("");
    state->score_label = gtk_label_new("");
    guide_label = gtk_label_new(
        "How to play:\n"
        "Place three of the same symbol in a row, column, or diagonal.\n"
        "Choose a match mode, pick who starts, and click any empty cell to make a move.");

    gtk_widget_set_name(title_label, "title-label");
    gtk_widget_set_name(state->status_label, "status-label");
    gtk_widget_set_name(state->score_label, "score-label");
    gtk_widget_set_name(guide_label, "guide-label");

    gtk_widget_set_halign(title_label, GTK_ALIGN_START);
    gtk_widget_set_halign(state->status_label, GTK_ALIGN_FILL);
    gtk_widget_set_halign(state->score_label, GTK_ALIGN_START);
    gtk_widget_set_halign(guide_label, GTK_ALIGN_FILL);
    gtk_label_set_xalign(GTK_LABEL(title_label), 0.0f);
    gtk_label_set_xalign(GTK_LABEL(state->status_label), 0.0f);
    gtk_label_set_xalign(GTK_LABEL(state->score_label), 0.0f);
    gtk_label_set_xalign(GTK_LABEL(guide_label), 0.0f);
    gtk_label_set_line_wrap(GTK_LABEL(guide_label), TRUE);

    gtk_box_pack_start(GTK_BOX(hero_box), title_label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hero_box), state->status_label, FALSE, FALSE, 8);
    gtk_box_pack_start(GTK_BOX(hero_box), state->score_label, FALSE, FALSE, 2);

    body_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 22);

    control_panel = app_create_panel("control-panel");
    control_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 14);
    gtk_container_set_border_width(GTK_CONTAINER(control_box), 22);
    gtk_container_add(GTK_CONTAINER(control_panel), control_box);
    gtk_widget_set_size_request(control_panel, 270, -1);

    gtk_box_pack_start(
        GTK_BOX(control_box),
        create_labeled_combo("Match mode", &state->mode_combo),
        FALSE,
        FALSE,
        0);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(state->mode_combo), "Human vs Human");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(state->mode_combo), "Human vs Computer");
    gtk_combo_box_set_active(GTK_COMBO_BOX(state->mode_combo), 1);

    gtk_box_pack_start(
        GTK_BOX(control_box),
        create_labeled_combo("Player symbol", &state->symbol_combo),
        FALSE,
        FALSE,
        0);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(state->symbol_combo), "X starts");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(state->symbol_combo), "O starts");
    gtk_combo_box_set_active(GTK_COMBO_BOX(state->symbol_combo), 0);

    state->new_match_button = gtk_button_new_with_label("New Match");
    state->reset_score_button = gtk_button_new_with_label("Reset Score");
    gtk_style_context_add_class(gtk_widget_get_style_context(state->new_match_button), "action-button");
    gtk_style_context_add_class(gtk_widget_get_style_context(state->reset_score_button), "action-button");

    gtk_box_pack_start(GTK_BOX(control_box), state->new_match_button, FALSE, FALSE, 4);
    gtk_box_pack_start(GTK_BOX(control_box), state->reset_score_button, FALSE, FALSE, 0);

    gtk_box_pack_end(GTK_BOX(control_box), guide_label, FALSE, FALSE, 0);

    board_panel = app_create_panel("board-panel");
    board_grid = gtk_grid_new();
    gtk_container_set_border_width(GTK_CONTAINER(board_grid), 18);
    gtk_grid_set_row_spacing(GTK_GRID(board_grid), 14);
    gtk_grid_set_column_spacing(GTK_GRID(board_grid), 14);
    gtk_container_add(GTK_CONTAINER(board_panel), board_grid);

    for (row = 0; row < 3; ++row) {
        for (col = 0; col < 3; ++col) {
            CellData *cell_data;

            state->cells[row][col] = gtk_button_new_with_label("");
            gtk_style_context_add_class(
                gtk_widget_get_style_context(state->cells[row][col]),
                "cell-button");

            cell_data = g_new0(CellData, 1);
            cell_data->app = state;
            cell_data->row = row;
            cell_data->col = col;

            g_object_set_data_full(
                G_OBJECT(state->cells[row][col]),
                "cell-data",
                cell_data,
                g_free);

            g_signal_connect(state->cells[row][col], "clicked", G_CALLBACK(on_cell_clicked), cell_data);
            gtk_grid_attach(GTK_GRID(board_grid), state->cells[row][col], col, row, 1, 1);
        }
    }

    gtk_box_pack_start(GTK_BOX(body_box), control_panel, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(body_box), board_panel, TRUE, TRUE, 0);

    gtk_box_pack_start(GTK_BOX(root), hero_panel, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(root), body_box, TRUE, TRUE, 0);

    g_signal_connect(state->new_match_button, "clicked", G_CALLBACK(on_new_match_clicked), state);
    g_signal_connect(state->reset_score_button, "clicked", G_CALLBACK(on_reset_score_clicked), state);
    g_signal_connect(state->mode_combo, "changed", G_CALLBACK(on_mode_changed), state);
    g_signal_connect(state->symbol_combo, "changed", G_CALLBACK(on_symbol_changed), state);
    g_signal_connect(window, "destroy", G_CALLBACK(on_window_destroy), state);

    app_start_match(state);
    gtk_widget_show_all(window);
}
