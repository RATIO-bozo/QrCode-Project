#ifndef INTERFACE_H
#define INTERFACE_H


#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include "../encode/encode.h"

/*
struct fct1
{
    GtkWidget * entry;
    GtkWidget * label;
    GtkWidget * encode_image;

};

void generate(GtkButton * button, gpointer data);

void createwindow(GtkApplication * app, gpointer data);
*/

int app(int argumentcount, char * argument[]);


#endif
