#pragma once 

#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include "config.hpp"
#include <string>
#include <fstream>
#include <gtk/gtk.h>

void generateFile(std::string filename, unsigned char* content);
void readFileBinary(const std::string& filename, unsigned char*& content);

class createWindow {
    private:
        bool connected = false;
        GtkWidget *createUserwindow;

    public:
        struct CallbackData {
            createWindow* windowInstance;
            GtkWidget* widgetData;
        };

        static void togglePassword(GtkWidget *widget, gpointer data);
        static void buttonClicked(GtkWidget *widget, gpointer data);
        void createUser();
};


class loginWindow {
    private:
        bool connected = false;

    public:
        GtkWidget* errorLabel = nullptr;

        static void togglePassword(GtkWidget *widget, gpointer data);
        struct CallbackData {
            loginWindow* self;
            GtkWidget *entry;
            GtkWidget *errorLabel;
        };
        static void buttonClicked(GtkWidget *widget, gpointer data);
        void connectUser();
};

#endif