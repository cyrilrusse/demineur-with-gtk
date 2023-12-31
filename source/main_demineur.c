/**
 * \file main.c
 * \brief Ce fichier contient la fonction main()
 * \author{Randaxhe Martin - Russe Cyril}
 * \date 27-04-2020
 * 
 */


#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#include "modele_demineur.h"
#include "vue_demineur.h"
#include "controleur_demineur.h"
#include "type_opaque.h"

#define NBR_MINE_DEFAULT 10
#define TEMPS_DEFAULT 60
#define NBR_LIGNE_DEFAULT 10
#define NBR_COLONNE_DEFAULT 10


int main(int argc, char **argv){
    char *optstring = "l:h:t:m:H";
    Regle *recup_arg = constructeur_Regle();
    int val, recup, recup_nbr_mine = -1;
    
    //Initialisation des règles du jeu aux valeurs par défauts
    set_ligne(recup_arg, NBR_LIGNE_DEFAULT);
    set_colonne(recup_arg, NBR_COLONNE_DEFAULT);
    set_temps(recup_arg, TEMPS_DEFAULT);
    set_nombre_mine(recup_arg, NBR_MINE_DEFAULT);

    //Récupération des options et modification des règles si respect des limites
    while ((val = getopt(argc, argv, optstring)) != EOF){
        switch (val){
        case 'l':
            recup = atoi(optarg);
            if(recup>=10 && recup<=30)
                set_ligne(recup_arg, (unsigned short)recup);
            break;
        case 'h':
            recup = atoi(optarg);
            if(recup>=10 && recup<=30)
                set_colonne(recup_arg, (unsigned short)recup);
            break;
        case 't':
            recup = atoi(optarg);
            if(recup>=60 && recup<=600)
                set_temps(recup_arg, (unsigned short)recup);
            break;
        case 'm':
            recup_nbr_mine = atoi(optarg);
            break;
        case 'H':
            printf("usage : ./demineur [-l <nombre ligne>] [-h <nombre colonne>] [-t <temps>] [-m <nombre mine>] [-H]\n\n");
            printf("\tl\tChoisir le nombre de ligne du demineur (10(default) - 30)\n");
            printf("\th\tChoisir le nombre de colonne du demineur (10(default) - 30)\n");
            printf("\tt\tChoisir le temps max pour résoudre le démineur (60(default) - 600)(secondes)\n");
            printf("\tm\tChoisir le nombre de mine (10(default) - l*h)\n");
            printf("\tH\thelp\n\n");
            printf("Si une option ne respecte pas les valeurs max, celle-ci sera automatique établie à sa valeur par défaut.\n");
            destructeur_Regle(recup_arg);
            return EXIT_SUCCESS;

        default:
            break;
        }
    }
    /*Modification du nombre de mine après switch pour pouvoir vérifier 
    qu'il respecte les dimensions dans le cas où le nombre de mines ait 
    été donné avant celles-ci*/
    if(recup_nbr_mine>=10 && (unsigned short)recup_nbr_mine<=(get_colonne(recup_arg)*get_ligne(recup_arg)))
        set_nombre_mine(recup_arg, (unsigned short)recup_nbr_mine);

    //Initialisation du terrain et du champ de mine
    Terrain *terrain = constructeur_Terrain(get_ligne(recup_arg), get_colonne(recup_arg), get_temps(recup_arg), get_nombre_mine(recup_arg));
    if (terrain==NULL){
        destructeur_Regle(recup_arg);
        return EXIT_FAILURE;
    }
    Timer *timer = constructeur_Timer(get_temps(recup_arg));
    destructeur_Regle(recup_arg);
    if (timer==NULL){
        destructeur_Terrain(terrain);
        return EXIT_FAILURE;
    }

    GtkWidget *pFenetre;
    GtkWidget *pVBox;
    GtkWidget *pTab_button[900];
    GtkWidget *pButton_new_game = gtk_button_new();
    GtkWidget *pLabel_timer = gtk_label_new(NULL);

    set_timer_label(timer, pLabel_timer);
    set_timer(terrain, timer);

    nouvelle_partie(terrain);

    gtk_init(&argc, &argv);

    //Initialisation de la fenetre et affichage
    pFenetre = creation_fenetre(terrain);
    pVBox = structure_box(pFenetre, terrain, pTab_button, pButton_new_game);
    gtk_container_add(GTK_CONTAINER(pFenetre), pVBox);
    gtk_widget_show_all(pFenetre);

    gtk_main();

    destructeur_Terrain(terrain);

    return EXIT_SUCCESS;
}//fin programme