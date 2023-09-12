#include "include/config.hpp"


void generateFile(std::string filename, unsigned char* content){
            std::string path = ABSOLUTE_PATH + filename;
            std::ofstream file;
            file.open(path);
            file << content;
            file.close();
            chmod(path.c_str(), 0600); //edit file permission to 0600
}

        
void readFileBinary(const std::string& filename, unsigned char*& content) {
    std::string path = ABSOLUTE_PATH + filename;
    std::ifstream file(path, std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "Erreur lors de l'ouverture du fichier : " << path << std::endl;
        return;
    }

    // Initialise la taille du fichier à zéro
    size_t fileSize = 0;

    // Obtient la taille réelle du fichier
    file.seekg(0, std::ios::end);
    fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // Alloue de la mémoire pour le tampon de contenu
    content = new unsigned char[fileSize];

    // Lit le contenu du fichier dans le tampon
    file.read(reinterpret_cast<char*>(content), fileSize);

    // Ferme le fichier
    file.close();
}


void createWindow::togglePassword(GtkWidget *widget, gpointer data) {
    GtkWidget *entry = GTK_WIDGET(data);
    if (gtk_entry_get_visibility(GTK_ENTRY(entry))) {
        gtk_entry_set_visibility(GTK_ENTRY(entry), FALSE);
        gtk_button_set_label(GTK_BUTTON(widget), "Show");
    } else {
        gtk_entry_set_visibility(GTK_ENTRY(entry), TRUE);
        gtk_button_set_label(GTK_BUTTON(widget), "Hide");
    }
}

void createWindow::buttonClicked(GtkWidget *widget, gpointer data) {
    CallbackData* actualData = static_cast<CallbackData*>(data);
    createWindow* window = actualData->windowInstance;
    const gchar *entry_text = gtk_entry_get_text(GTK_ENTRY(actualData->widgetData));

    const char *password = entry_text;
    char hashPassword[DIGEST_SIZE];
    unsigned char aesDerivedKey[AES_KEY_SIZE];
    unsigned char iv[IV_SIZE];
    unsigned char* salt = nullptr;
    unsigned char encryptedHashPassword[272];

    readFileBinary("salt.bin", salt);
    sha3_512((const unsigned char*)password, hashPassword);
    pbkdf2(password, salt, aesDerivedKey);

    generateIv(iv);
    aesEncrypt(reinterpret_cast<unsigned char*>(hashPassword), aesDerivedKey, iv, encryptedHashPassword);
    generateFile("hash.bin", encryptedHashPassword);

    delete[] salt;
    isRegistered = true;
    gtk_widget_destroy(window->createUserwindow);
    gtk_main_quit(); // quitter le main loop de GTK+

    delete actualData; // libérer la mémoire
}



void createWindow::createUser() {
    //GtkWidget *createUserwindow;
    GtkWidget *button;
    GtkWidget *entry;
    GtkWidget *grid;
    GtkWidget *vbox;
    GtkWidget *toggleButton;
    GtkCssProvider *provider, *provider2;

    gtk_init(0, NULL);

    createUserwindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(createUserwindow), 640, 480);
    g_signal_connect(createUserwindow, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(createUserwindow), grid);
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);

    GdkRGBA startColor, endColor;
    gdk_rgba_parse(&startColor, "#A5D6A7");
    gdk_rgba_parse(&endColor, "#1B5E20");

    provider = gtk_css_provider_new();
    gchar *cssData = g_strdup_printf("window { background-image: linear-gradient(to bottom, %s, %s); }",
                                     gdk_rgba_to_string(&startColor), gdk_rgba_to_string(&endColor));
    gtk_css_provider_load_from_data(provider, cssData, -1, NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(provider),
                                              GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_halign(vbox, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(vbox, GTK_ALIGN_CENTER);
    gtk_grid_attach(GTK_GRID(grid), vbox, 1, 1, 1, 1);

    entry = gtk_entry_new();
    gtk_widget_set_size_request(entry, 200, 30); // Ajuster la taille de la zone de texte
    gtk_box_pack_start(GTK_BOX(vbox), entry, FALSE, FALSE, 0);
    gtk_entry_set_visibility(GTK_ENTRY(entry), FALSE); // Masquer le texte entré

    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_end(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

    toggleButton = gtk_button_new_with_label("Show");
    gtk_box_pack_end(GTK_BOX(hbox), toggleButton, FALSE, FALSE, 0);

    button = gtk_button_new_with_label("Create your password");
    gtk_widget_set_size_request(button, 100, 30); // Ajuster la taille du bouton

    provider2 = gtk_css_provider_new();
    cssData = g_strdup_printf("button { background-image: linear-gradient(to bottom, #FF0000, #990000); }");
    gtk_css_provider_load_from_data(provider2, cssData, -1, NULL);
    GtkStyleContext *context = gtk_widget_get_style_context(button);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider2), GTK_STYLE_PROVIDER_PRIORITY_USER);

    gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 0);

    CallbackData* data = new CallbackData;
    data->windowInstance = this;
    data->widgetData = entry; 

    g_signal_connect(toggleButton, "clicked", G_CALLBACK(togglePassword), entry);
    g_signal_connect(button, "clicked", G_CALLBACK(buttonClicked), data);
    g_signal_connect(entry, "activate", G_CALLBACK(buttonClicked), data);
    g_signal_connect_swapped(createUserwindow, "destroy", G_CALLBACK(gtk_widget_destroy), createUserwindow);

    gtk_widget_show_all(createUserwindow);

    gtk_main();
}

void loginWindow::togglePassword(GtkWidget *widget, gpointer data) {
    GtkWidget *entry = GTK_WIDGET(data);
    if (gtk_entry_get_visibility(GTK_ENTRY(entry))) {
        gtk_entry_set_visibility(GTK_ENTRY(entry), FALSE);
        gtk_button_set_label(GTK_BUTTON(widget), "Show");
    } else {
        gtk_entry_set_visibility(GTK_ENTRY(entry), TRUE);
        gtk_button_set_label(GTK_BUTTON(widget), "Hide");
    }
}

void loginWindow::buttonClicked(GtkWidget *widget, gpointer data) {
    CallbackData* callbackData = static_cast<CallbackData*>(data);
    GtkWidget *entry = callbackData->entry;
    GtkWidget *errorLabel = callbackData->errorLabel;

    const gchar *entry_text;
    entry_text = gtk_entry_get_text(GTK_ENTRY(entry));
    const char *password = entry_text;
    char hashPassword[DIGEST_SIZE];
    unsigned char aesDerivedKey[AES_KEY_SIZE];
    unsigned char iv[IV_SIZE];
    unsigned char* salt = nullptr;
    unsigned char* encryptedHashPassword = nullptr;
    unsigned char* hash = nullptr;

    hash = new unsigned char[272]();

    readFileBinary("salt.bin", salt);
    readFileBinary("hash.bin", encryptedHashPassword);

    pbkdf2(password, salt, aesDerivedKey);
    aesDecrypt(reinterpret_cast< unsigned char*>(encryptedHashPassword), aesDerivedKey,hash);
    sha3_512((const unsigned char*)password, hashPassword);
    

    if (memcmp(hashPassword, hash, DIGEST_SIZE) == 0) {
        isLogged = true;
        gtk_main_quit(); // quitter le main loop de GTK+
        return;
    }


    if (callbackData->self->connected) {
        gtk_main_quit(); // quitter le main loop de GTK+
        return;
    } else {
        const gchar *errorMessage = "Mot de passe incorrect";
        gtk_label_set_text(GTK_LABEL(errorLabel), errorMessage);
        sleep(1);
        // Utilisez CSS pour définir la couleur au lieu de gtk_widget_override_color
    }

    delete [] salt;
    delete [] encryptedHashPassword;
    delete [] hash;

    // Réinitialiser le texte de l'entrée
    gtk_entry_set_text(GTK_ENTRY(entry), "");
}

void loginWindow::connectUser(){
    CallbackData data = {};
    data.self = this;

    gtk_init(0, NULL);

    GtkWidget *connectUserwindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(connectUserwindow), 640, 480);
    g_signal_connect(connectUserwindow, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(connectUserwindow), grid);
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);

    GdkRGBA startColor, endColor;
    gdk_rgba_parse(&startColor, "#A5D6A7");
    gdk_rgba_parse(&endColor, "#1B5E20");

    GtkCssProvider *provider = gtk_css_provider_new();
    gchar *cssData = g_strdup_printf("window { background-image: linear-gradient(to bottom, %s, %s); }",
                                        gdk_rgba_to_string(&startColor), gdk_rgba_to_string(&endColor));
    gtk_css_provider_load_from_data(provider, cssData, -1, NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(provider),
                                                GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_halign(vbox, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(vbox, GTK_ALIGN_CENTER);
    gtk_grid_attach(GTK_GRID(grid), vbox, 1, 1, 1, 1);

    GtkWidget *entry = gtk_entry_new();
    gtk_widget_set_size_request(entry, 200, 30);
    gtk_box_pack_start(GTK_BOX(vbox), entry, FALSE, FALSE, 0);
    gtk_entry_set_visibility(GTK_ENTRY(entry), FALSE);

    data.entry = entry;

    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_end(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

    GtkWidget *toggleButton = gtk_button_new_with_label("Show");
    gtk_box_pack_end(GTK_BOX(hbox), toggleButton, FALSE, FALSE, 0);

    GtkWidget *button = gtk_button_new_with_label("Login");
    gtk_widget_set_size_request(button, 100, 30);

    GtkCssProvider *provider2 = gtk_css_provider_new();
    cssData = g_strdup_printf("button { background-image: linear-gradient(to bottom, #FF0000, #990000); }");
    gtk_css_provider_load_from_data(provider2, cssData, -1, NULL);
    GtkStyleContext *context = gtk_widget_get_style_context(button);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider2), GTK_STYLE_PROVIDER_PRIORITY_USER);

    gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 0);

    GtkWidget *errorLabel = gtk_label_new(NULL);
    gtk_box_pack_end(GTK_BOX(vbox), errorLabel, FALSE, FALSE, 0);

    data.errorLabel = errorLabel;

    g_signal_connect(toggleButton, "clicked", G_CALLBACK(togglePassword), data.entry);
    g_signal_connect(button, "clicked", G_CALLBACK(buttonClicked), &data);
    g_signal_connect(entry, "activate", G_CALLBACK(buttonClicked), &data);
    g_signal_connect_swapped(connectUserwindow, "destroy", G_CALLBACK(gtk_widget_destroy), connectUserwindow);

    gtk_widget_show_all(connectUserwindow);

    gtk_main();
}





