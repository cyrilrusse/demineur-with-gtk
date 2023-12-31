/**
 * \file vue_demineur.c
 * \brief Ce fichier contient les définitions de types et les fonctions pour la gestion
 * de l'interface graphique de la calculatrice addition.
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

/**
 * \fn popup_close(GtkWidget *pButton, gpointer data)
 * \brief fonction callback de fermeture de la fenetre popup
 * 
 */
static void popup_close(GtkWidget *pButton, gpointer data);

GtkWidget *creation_fenetre(Terrain *terrain){
    assert(terrain!=NULL);

    GtkWidget *pFenetre = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(pFenetre), "Démineur");
    gtk_window_resize(GTK_WINDOW(pFenetre), get_ligne(get_regle(terrain))*35, (get_colonne(get_regle(terrain))*35)+100);
    g_signal_connect(G_OBJECT(pFenetre), "destroy", G_CALLBACK(gtk_main_quit), NULL);
    return pFenetre;
}//fin creation_fenetre

GtkWidget *creation_menus(GtkWidget *pFenetre, Terrain *terrain){
    assert(pFenetre!=NULL && terrain!=NULL);

    //Déclaration des variables
    GtkWidget *barre_menu;

    GtkWidget *menu_partie;
    GtkWidget *item_partie, *item_nouveau, *item_separateur, *item_quitter;

    GtkWidget *menu_niveau;
    GtkWidget *item_niveau, *item_niveau_debutant, *item_niveau_intermediaire, *item_niveau_expert; 

    GtkWidget *menu_aide;
    GtkWidget *item_aide, *item_info_createur;
    
    GtkAccelGroup *accelerateur = NULL;

    //barre de menu
    barre_menu = gtk_menu_bar_new();

    //le menu "Partie"
    menu_partie = gtk_menu_new();
    menu_niveau = gtk_menu_new();
    menu_aide = gtk_menu_new();

    //accélérateur
    accelerateur = gtk_accel_group_new();
    gtk_window_add_accel_group(GTK_WINDOW(pFenetre), accelerateur);
    
    //création des items avec images
    item_partie = gtk_menu_item_new_with_mnemonic("_Partie");
    item_nouveau = gtk_image_menu_item_new_from_stock(GTK_STOCK_NEW, accelerateur);
    item_separateur = gtk_separator_menu_item_new();
    item_quitter = gtk_image_menu_item_new_from_stock(GTK_STOCK_QUIT, accelerateur);

    item_niveau = gtk_menu_item_new_with_mnemonic("_Niveau");
    item_niveau_debutant = gtk_menu_item_new_with_mnemonic("_Débutant");
    item_niveau_intermediaire = gtk_menu_item_new_with_mnemonic("_Intermédiaire");
    item_niveau_expert = gtk_menu_item_new_with_mnemonic("_Expert");

    item_aide = gtk_menu_item_new_with_mnemonic("_Aide");
    item_info_createur = gtk_menu_item_new_with_mnemonic("_A propos");


    gtk_widget_add_accelerator(item_quitter, "activate", accelerateur, GDK_q, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    gtk_widget_add_accelerator(item_nouveau, "activate", accelerateur, GDK_n, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

    //attacher les items
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(item_partie), menu_partie);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_partie), item_nouveau);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_partie), item_separateur);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_partie), item_quitter);
    gtk_menu_shell_append(GTK_MENU_SHELL(barre_menu), item_partie);

    gtk_menu_item_set_submenu(GTK_MENU_ITEM(item_niveau), menu_niveau);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_niveau), item_niveau_debutant);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_niveau), item_niveau_intermediaire);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_niveau), item_niveau_expert);
    gtk_menu_shell_append(GTK_MENU_SHELL(barre_menu), item_niveau);

    gtk_menu_item_set_submenu(GTK_MENU_ITEM(item_aide), menu_aide);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_aide), item_info_createur);
    gtk_menu_shell_append(GTK_MENU_SHELL(barre_menu), item_aide);
    Data_for_Callback *data  = constructeur_Data_for_Callback(terrain, pFenetre);
    //les signaux
    g_signal_connect(G_OBJECT(item_quitter), "activate", G_CALLBACK(gtk_main_quit), NULL);

    g_signal_connect(G_OBJECT(item_niveau_debutant), "activate", G_CALLBACK(click_difficulte_debutant), (gpointer)data);
    g_signal_connect(G_OBJECT(item_niveau_intermediaire), "activate", G_CALLBACK(click_difficulte_intermediaire), (gpointer)data);
    g_signal_connect(G_OBJECT(item_niveau_expert), "activate", G_CALLBACK(click_difficulte_expert), (gpointer)data);

    g_signal_connect(G_OBJECT(item_nouveau), "activate", G_CALLBACK(click_nouvelle_partie), (gpointer)data);

    g_signal_connect(G_OBJECT(item_info_createur), "activate", G_CALLBACK(click_a_propos), NULL);

    return barre_menu;
}//fin creation_menus

GtkWidget *structure_box(GtkWidget *pFenetre, Terrain *terrain, GtkWidget **pButton, GtkWidget *pButton_new_game){
    assert(pFenetre!=NULL && terrain!=NULL);
    
    
    char texte_nbr_mine[4];
    int nombre_ligne = get_ligne(get_regle(terrain)), nombre_colonne = get_colonne(get_regle(terrain));
   
    //Initialisation box
    GtkWidget *pVBox = gtk_vbox_new(TRUE, 0);
    GtkWidget *pHBox_info = gtk_hbox_new(TRUE, 0);
    GtkWidget *pHBox_champ_mine[get_ligne(get_regle(terrain))];

    //Initialisation boutons
    gtk_widget_set_size_request(pButton_new_game, 35, 35);
    pButton_new_game = gtk_button_new();

    //Initialisation labels
    sprintf(texte_nbr_mine, "%hu", get_nombre_mine(get_regle(terrain)));
    GtkWidget *pLabel_nbr_mine = gtk_label_new(texte_nbr_mine);

    //Initialisation barre menu
    GtkWidget *pBarre_menu = creation_menus(pFenetre, terrain);
    

    gtk_box_pack_start(GTK_BOX(pVBox), pBarre_menu, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(pVBox), pHBox_info, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(pHBox_info), pLabel_nbr_mine, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(pHBox_info), pButton_new_game, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(pHBox_info), get_label_timer(get_timer(terrain)), TRUE, TRUE, 0);

    
    unsigned int tab_coord[2];
    for (int i = 0; i < nombre_ligne; i++){
        pHBox_champ_mine[i]=gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(pVBox), pHBox_champ_mine[i], TRUE, TRUE, 0);
        for(int j=0; j < nombre_colonne; j++){
            pButton[(i*nombre_colonne)+j]=gtk_button_new();
            gtk_widget_set_size_request(pButton[(i*nombre_colonne)+j], 20, 20);
            gtk_box_pack_start(GTK_BOX(pHBox_champ_mine[i]), pButton[(i*nombre_colonne)+j], TRUE, TRUE, 0);
            Data_for_Callback *data = constructeur_Data_for_Callback(terrain, pFenetre);
            tab_coord[0]=i;
            tab_coord[1]=j;
            set_coord_boutton(data, tab_coord);
            gtk_widget_set_events(pButton[(i*nombre_colonne)+j], GDK_BUTTON_PRESS_MASK);
            g_signal_connect(G_OBJECT(pButton[(i*nombre_colonne)+j]), "button_press_event", G_CALLBACK(click_decouvre_case), (gpointer)data);
        }
    }
    set_tableau_bouton(terrain, pButton);
    set_bouton_new_game(terrain, pButton_new_game);

    charge_image_bouton(get_bouton_new_game(terrain), -5);

     //Signaux
    Data_for_Callback *data_nouvelle_partie = constructeur_Data_for_Callback(terrain, pFenetre);
    g_signal_connect(G_OBJECT(pButton_new_game), "clicked", G_CALLBACK(click_nouvelle_partie), (gpointer)data_nouvelle_partie);

    return pVBox;
}//fin structure_box

int charge_image_bouton(GtkWidget *pButton, int numero_image){
    assert(pButton!=NULL);

    GdkPixbuf *pb_temp, *emeline_la_boss;
    GtkWidget *image;
    // 1. Charger l’image et la redimensionner (20*20 pixels)
    switch(numero_image){
        case 0://chiffre 0
            pb_temp = gdk_pixbuf_new_from_file("images/0.bmp", NULL);
            break;
        case 1://chiffre 1
            pb_temp = gdk_pixbuf_new_from_file("images/1.bmp", NULL);
            break;
        case 2://chiffre 2
            pb_temp = gdk_pixbuf_new_from_file("images/2.bmp", NULL);
            break;
        case 3://chiffre 3
            pb_temp = gdk_pixbuf_new_from_file("images/3.bmp", NULL);
            break;
        case 4://chiffre 4
            pb_temp = gdk_pixbuf_new_from_file("images/4.bmp", NULL);
            break;
        case 5://chiffre 5
            pb_temp = gdk_pixbuf_new_from_file("images/5.bmp", NULL);
            break;
        case 6://chiffre 6
            pb_temp = gdk_pixbuf_new_from_file("images/6.bmp", NULL);
            break;
        case 7://chiffre 7
            pb_temp = gdk_pixbuf_new_from_file("images/7.bmp", NULL);
            break;
        case 8://chiffre 8
            pb_temp = gdk_pixbuf_new_from_file("images/8.bmp", NULL);
            break;
        case -1://bombe
            pb_temp = gdk_pixbuf_new_from_file("images/Bomb1.bmp", NULL);
            break;
        case -2://bombe barrée -> si lorsque perdu car cliqué sur bombe, un drapeau se trouvait sur une case sans bombe
            pb_temp = gdk_pixbuf_new_from_file("images/Bomb3.bmp", NULL);
            break;
        case -3://bombe rouge -> celle qui a explosée
            pb_temp = gdk_pixbuf_new_from_file("images/Bomb2.bmp", NULL);
            break;
        case -4://drapeau
            pb_temp = gdk_pixbuf_new_from_file("images/flag.png", NULL);
            break;
        case -5://image new_game defaut
            pb_temp = gdk_pixbuf_new_from_file("images/Bonhomme1.bmp", NULL);
            break;
        case -6://image new_game perdu
            pb_temp = gdk_pixbuf_new_from_file("images/Bonhomme3.bmp", NULL);
            break;
        case -7://image new_game gagné
            pb_temp = gdk_pixbuf_new_from_file("images/Bonhomme4.bmp", NULL);
            break;
        default:
            return -1;
            break;
    }

    if(pb_temp == NULL)
    {
        printf(" Erreur de chargement de l’image qu'on tente d'afficher bordel de merde!\n");
        return -1;
    }
    emeline_la_boss = gdk_pixbuf_scale_simple(pb_temp, 20, 20, GDK_INTERP_NEAREST);

    // 2. Placer l’image
    image = gtk_image_new_from_pixbuf(emeline_la_boss);
    gtk_button_set_image(GTK_BUTTON(pButton), image);

    return 0; 
} // fin charge_image_bouton()

void reinitialise_image_bouton(Terrain *terrain){
    assert(terrain!=NULL);

    unsigned int ligne = get_ligne(get_regle(terrain)), colonne = get_colonne(get_regle(terrain));
    
    for(unsigned int i=0; i<ligne; i++){
        for(unsigned int j=0; j<colonne; j++)
            gtk_button_set_image(GTK_BUTTON(get_bouton(terrain, i, j)), NULL);
    }
}//fin reinitialise_image_bouton

void fenetre_pop_up_a_propos(void){
    GtkWidget *pPopup = gtk_window_new(GTK_WINDOW_POPUP);
    GtkWidget *pVBox = gtk_vbox_new(TRUE, 0);
    GtkWidget *pHBox = gtk_hbox_new(TRUE, 0);
    GtkWidget *pLabel = gtk_label_new("Demineur\nCréateur : Randaxhe Martin & Russe Cyril\nINFO0030 -- 30/04/19");
    gtk_label_set_justify(GTK_LABEL(pLabel), GTK_JUSTIFY_CENTER);
    GtkWidget *pButton_ok = gtk_button_new_with_label("ok");

    gtk_window_set_default_size(GTK_WINDOW(pPopup), 400, 150);
    gtk_window_move(GTK_WINDOW(pPopup), 550, 420);
    gtk_box_pack_start(GTK_BOX(pVBox), pLabel, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(pVBox), pHBox, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(pHBox), pButton_ok, TRUE, TRUE, 0);

    gtk_container_add(GTK_CONTAINER(pPopup), pVBox);

    g_signal_connect(G_OBJECT(pButton_ok), "clicked", G_CALLBACK(popup_close), (gpointer)pPopup);

    gtk_widget_show_all(pPopup);
}//fin fenetre_pop_up_a_propos

static void popup_close(GtkWidget *pButton, gpointer data){
    GtkWidget *recup_data = data;
    pButton = pButton;//inutile, mais sinon warning "unused variable à la compilation"
    gtk_widget_destroy(recup_data);
}//fin popup_close

void maj_timer(Timer *timer){
    assert(timer!=NULL);
    char texte[4];

    GtkWidget *label = get_label_timer(timer);
    sprintf(texte, "%d", get_temps_restant(timer));
    gtk_label_set_text(GTK_LABEL(label), texte);
}
