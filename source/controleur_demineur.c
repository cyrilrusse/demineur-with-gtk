/**
 * \file controleur_demineur.c
 * \brief Ce fichier contient les définitions de types et les fonctions pour la gestion
 * de la logique du demineur
 * 
 * \author{Randaxhe Martin - Russe Cyril}
 * \date: 24-04-2020
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include "modele_demineur.h"
#include "vue_demineur.h"
#include "controleur_demineur.h"

void click_difficulte_debutant(GtkWidget *pButton, gpointer data){
    assert(data!=NULL);

    pButton+=0;
    Data_for_Callback *recup_data = data;
    mode_debutant(get_Terrain(recup_data));
    GtkWidget *pVBox = structure_box(get_fenetre(recup_data), get_Terrain(recup_data));
    gtk_widget_show_all(get_fenetre(recup_data));
    gtk_container_foreach (GTK_CONTAINER (get_fenetre(recup_data)), (GtkCallback) gtk_widget_destroy, NULL);
    gtk_container_add(GTK_CONTAINER(get_fenetre(recup_data)), pVBox);
    gtk_window_resize(GTK_WINDOW(get_fenetre(recup_data)), 300, 340);
    gtk_widget_show_all(get_fenetre(recup_data));
}

void click_difficulte_intermediaire(GtkWidget *pButton, gpointer data){
    assert(data!=NULL);

    pButton+=0;
    Data_for_Callback *recup_data = data;
    mode_intermediaire(get_Terrain(recup_data));
    GtkWidget *pVBox = structure_box(get_fenetre(recup_data), get_Terrain(recup_data));
    gtk_widget_show_all(get_fenetre(recup_data));
    gtk_container_foreach (GTK_CONTAINER (get_fenetre(recup_data)), (GtkCallback) gtk_widget_destroy, NULL);
    gtk_container_add(GTK_CONTAINER(get_fenetre(recup_data)), pVBox);
    gtk_window_resize(GTK_WINDOW(get_fenetre(recup_data)), 600, 640);
    gtk_widget_show_all(get_fenetre(recup_data));
}

void click_difficulte_expert(GtkWidget *pButton, gpointer data){
    assert(data!=NULL);

    pButton+=0;
    Data_for_Callback *recup_data = data;
    mode_expert(get_Terrain(recup_data));
    GtkWidget *pVBox = structure_box(get_fenetre(recup_data), get_Terrain(recup_data));
    gtk_widget_show_all(get_fenetre(recup_data));
    gtk_container_foreach (GTK_CONTAINER (get_fenetre(recup_data)), (GtkCallback) gtk_widget_destroy, NULL);
    gtk_container_add(GTK_CONTAINER(get_fenetre(recup_data)), pVBox);
    gtk_window_resize(GTK_WINDOW(get_fenetre(recup_data)), 1000, 1040);
    gtk_widget_show_all(get_fenetre(recup_data));
}

void click_decouvre_case(GtkWidget *pButton, gpointer data){
    assert(data!=NULL);
    pButton+=0;

    Data_for_Callback *recup_data = data;
    unsigned short *coord = get_coord_boutton(recup_data);
    if(get_Boite_decouverte(get_elem_champ_mine(get_Terrain(recup_data), coord[0], coord[1]))==1)
        return;
    
}


