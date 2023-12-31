/**
 * \file modele_demineur.c
 * \brief Ce fichier contient les définitions de types et les fonctions pour la gestion
 * de l'interface graphique de la calculatrice addition.
 * 
 * \author{Randaxhe Martin - Russe Cyril}
 * \date: 24-04-2020
 * 
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>

#include "modele_demineur.h"
#include "vue_demineur.h"
#include "controleur_demineur.h"
#include "type_opaque.h"

#define NBR_MINE_DEBUTANT 10
#define TEMPS_DEBUTANT 60
#define NBR_LIGNE_DEBUTANT 10
#define NBR_COLONNE_DEBUTANT 10

#define NBR_MINE_INTERMEDIAIRE 40
#define TEMPS_INTERMEDIAIRE 180
#define NBR_LIGNE_INTERMEDIAIRE 20
#define NBR_COLONNE_INTERMEDIAIRE 20

#define NBR_MINE_EXPERT 90
#define TEMPS_EXPERT 300
#define NBR_LIGNE_EXPERT 30
#define NBR_COLONNE_EXPERT 30

static void actualise_compteur_autour_de_bombe(Terrain *terrain, unsigned short i, unsigned short j, unsigned short ligne_max, unsigned short colonne_max);

static int nombre_drapeau_autour(Terrain *terrain, unsigned short i, unsigned short j);

void nouvelle_partie(Terrain *terrain){
    assert(terrain!=NULL);

    Regle *regle = get_regle(terrain);

    initialisation_0(terrain);
    aleatoire_bombe_et_compteur(terrain);
    set_Boite_deja_decouverte(regle, 0);
    set_win(regle, 0);
    Initialisation_timer(terrain);
}//fin nouvelle_partie

void initialisation_0(Terrain *terrain){
    assert(terrain != NULL);
    unsigned short colonne_max = get_colonne(get_regle(terrain)), ligne_max = get_ligne(get_regle(terrain));

    for (int i = 0; i < ligne_max; i++){
        for (int j = 0; j < colonne_max; j++){
            set_Boite_decouverte(get_elem_champ_mine(terrain, i, j), 0);
            set_mine(get_elem_champ_mine(terrain, i, j), 0);
        }
    }
}//fin initialisation_0

void aleatoire_bombe_et_compteur(Terrain *terrain){
    assert(terrain != NULL);
    srand(time(NULL));
    unsigned short nbr_mine = get_nombre_mine(get_regle(terrain));
    unsigned short colonne_max = get_colonne(get_regle(terrain));
    unsigned short ligne_max = get_ligne(get_regle(terrain)), i, j;

    for(unsigned short k = 0; k < nbr_mine;){
        i = rand()%ligne_max;
        j = rand()%colonne_max;

        if(get_mine(get_elem_champ_mine(terrain, i, j)) >= 0){
            set_mine(get_elem_champ_mine(terrain, i, j), -1);
            actualise_compteur_autour_de_bombe(terrain, i, j, ligne_max-1, colonne_max-1);
            k++;
        }
    }
}//fin aleatoire_bombe_et_compteur

void decouvre_boite(Terrain *terrain, unsigned int ligne, unsigned int colonne){
    assert(terrain != NULL);
    
    if(!(get_Boite_decouverte(get_elem_champ_mine(terrain, ligne, colonne))) && get_mine(get_elem_champ_mine(terrain, ligne, colonne)) > 0){
        set_Boite_deja_decouverte(get_regle(terrain), get_Boite_deja_decouverte(get_regle(terrain)) + 1);
        set_Boite_decouverte(get_elem_champ_mine(terrain, ligne, colonne), 1);
        if(get_Boite_deja_decouverte(get_regle(terrain))+get_nombre_mine(get_regle(terrain))==get_ligne(get_regle(terrain))*get_colonne(get_regle(terrain))){
            set_win(get_regle(terrain), 1);
            charge_image_bouton(get_bouton_new_game(terrain), -7);
            decouvre_bombe(terrain);
        }
        
        charge_image_bouton(get_bouton(terrain, ligne, colonne), get_mine(get_elem_champ_mine(terrain, ligne, colonne)));
    }
    else if (!(get_Boite_decouverte(get_elem_champ_mine(terrain, ligne, colonne))) && !(get_mine(get_elem_champ_mine(terrain, ligne, colonne)))){
        charge_image_bouton(get_bouton(terrain, ligne, colonne), 0);
        set_Boite_deja_decouverte(get_regle(terrain), get_Boite_deja_decouverte(get_regle(terrain)) + 1);
        set_Boite_decouverte(get_elem_champ_mine(terrain, ligne, colonne), 1);
        unsigned short colonne_max = get_ligne(get_regle(terrain)) - 1;
        unsigned short ligne_max = get_colonne(get_regle(terrain)) - 1;
        if(ligne == 0){
            decouvre_boite(terrain, ligne+1, colonne);
            if(colonne == 0){
                decouvre_boite(terrain, ligne, colonne+1);
                decouvre_boite(terrain, ligne + 1, colonne + 1);
            }
            else if(colonne == colonne_max){
                decouvre_boite(terrain, ligne, colonne-1);
                decouvre_boite(terrain, ligne + 1, colonne - 1);
            }
            else{
                decouvre_boite(terrain, ligne, colonne-1);
                decouvre_boite(terrain, ligne + 1, colonne - 1);
                decouvre_boite(terrain, ligne, colonne + 1);
                decouvre_boite(terrain, ligne + 1, colonne + 1);
            }
        }
        else if (ligne == ligne_max){
            decouvre_boite(terrain, ligne - 1, colonne);
            if (colonne == 0){
                decouvre_boite(terrain, ligne, colonne+1);
                decouvre_boite(terrain, ligne - 1, colonne + 1);
            }
            else if (colonne == colonne_max){
                decouvre_boite(terrain, ligne, colonne-1);
                decouvre_boite(terrain, ligne - 1, colonne - 1);
            }
            else{
                decouvre_boite(terrain, ligne, colonne-1);
                decouvre_boite(terrain, ligne - 1, colonne - 1);
                decouvre_boite(terrain, ligne, colonne+1);
                decouvre_boite(terrain, ligne - 1, colonne + 1);
            }
        }
        else{
            decouvre_boite(terrain, ligne-1, colonne);
            decouvre_boite(terrain, ligne+1, colonne);
            if(colonne == 0){
                decouvre_boite(terrain, ligne - 1, colonne + 1);
                decouvre_boite(terrain, ligne, colonne+1);
                decouvre_boite(terrain, ligne + 1, colonne + 1);
            }
            else if(colonne == colonne_max){
                decouvre_boite(terrain, ligne - 1, colonne - 1);
                decouvre_boite(terrain, ligne, colonne-1);
                decouvre_boite(terrain, ligne + 1, colonne - 1);
            }
            else{
                decouvre_boite(terrain, ligne - 1, colonne - 1);
                decouvre_boite(terrain, ligne, colonne-1);
                decouvre_boite(terrain, ligne + 1, colonne - 1);
                decouvre_boite(terrain, ligne - 1, colonne + 1);
                decouvre_boite(terrain, ligne, colonne+1);
                decouvre_boite(terrain, ligne + 1, colonne + 1);
            }
        }
    }
    else if (!(get_Boite_decouverte(get_elem_champ_mine(terrain, ligne, colonne))) && get_mine(get_elem_champ_mine(terrain, ligne, colonne)) == -1){
        set_win(get_regle(terrain), -1);
        decouvre_bombe(terrain);//affiche toutes les bombes mais laisse les drapeaux corrects
        charge_image_bouton(get_bouton(terrain, ligne, colonne), -3);//affiche la bomb qui a explosé en rouge
        charge_image_bouton(get_bouton_new_game(terrain), -6);//affiche bonhomme mort sur bouton nouvelle partie
    }
}//fin decouvre_boite

static void actualise_compteur_autour_de_bombe(Terrain *terrain, unsigned short i, unsigned short j, unsigned short ligne_max, unsigned short colonne_max){
    assert(terrain != NULL);

    if(i == 0){
        if (get_mine(get_elem_champ_mine(terrain, i+1, j)) != -1)
            set_mine(get_elem_champ_mine(terrain, i+1, j), get_mine(get_elem_champ_mine(terrain, i+1, j)) + 1);

        if (j == 0){
            if (get_mine(get_elem_champ_mine(terrain, i, j+1)) != -1)
                set_mine(get_elem_champ_mine(terrain, i, j+1), get_mine(get_elem_champ_mine(terrain, i, j+1)) + 1);
            if (get_mine(get_elem_champ_mine(terrain, i+1, j+1)) != -1)
                set_mine(get_elem_champ_mine(terrain, i+1, j+1), get_mine(get_elem_champ_mine(terrain, i+1, j+1)) + 1);
        }
        else if(j == colonne_max){
            if (get_mine(get_elem_champ_mine(terrain, i+1,j-1)) != -1)
                set_mine(get_elem_champ_mine(terrain, i+1, j-1), get_mine(get_elem_champ_mine(terrain, i+1, j-1)) + 1);
            if (get_mine(get_elem_champ_mine(terrain, i, j-1)) != -1)
                set_mine(get_elem_champ_mine(terrain, i, j-1), get_mine(get_elem_champ_mine(terrain, i, j-1)) + 1);
        }
        else{
            if (get_mine(get_elem_champ_mine(terrain, i, j+1)) != -1)
                set_mine(get_elem_champ_mine(terrain, i, j+1), get_mine(get_elem_champ_mine(terrain, i, j+1)) + 1);
            if (get_mine(get_elem_champ_mine(terrain, i+1, j+1)) != -1)
                set_mine(get_elem_champ_mine(terrain, i+1, j+1), get_mine(get_elem_champ_mine(terrain, i+1, j+1)) + 1);
            if (get_mine(get_elem_champ_mine(terrain, i+1, j-1)) != -1)
                set_mine(get_elem_champ_mine(terrain, i+1, j-1), get_mine(get_elem_champ_mine(terrain, i+1, j-1)) + 1);
            if (get_mine(get_elem_champ_mine(terrain, i, j-1)) != -1)
                set_mine(get_elem_champ_mine(terrain, i, j-1), get_mine(get_elem_champ_mine(terrain, i, j-1)) + 1);
        }  
    }
    else if(i == ligne_max){
        if (get_mine(get_elem_champ_mine(terrain, i-1, j)) != -1)
            set_mine(get_elem_champ_mine(terrain, i-1, j), get_mine(get_elem_champ_mine(terrain, i-1, j)) + 1);

        if (j == 0){
            if (get_mine(get_elem_champ_mine(terrain, i, j+1)) != -1)
                set_mine(get_elem_champ_mine(terrain, i, j+1), get_mine(get_elem_champ_mine(terrain, i, j+1)) + 1);
            if (get_mine(get_elem_champ_mine(terrain, i-1, j+1)) != -1)
                set_mine(get_elem_champ_mine(terrain, i-1, j+1), get_mine(get_elem_champ_mine(terrain, i-1, j+1)) + 1);
        }
        else if (j == colonne_max){
            if (get_mine(get_elem_champ_mine(terrain, i-1, j-1)) != -1)
                set_mine(get_elem_champ_mine(terrain, i-1, j-1), get_mine(get_elem_champ_mine(terrain, i-1, j-1)) + 1);
            if (get_mine(get_elem_champ_mine(terrain, i, j-1)) != -1)
                set_mine(get_elem_champ_mine(terrain, i, j-1), get_mine(get_elem_champ_mine(terrain, i, j-1)) + 1);
        }
        else{
            if (get_mine(get_elem_champ_mine(terrain, i, j+1)) != -1)
                set_mine(get_elem_champ_mine(terrain, i, j+1), get_mine(get_elem_champ_mine(terrain, i, j+1)) + 1);
            if (get_mine(get_elem_champ_mine(terrain, i-1, j+1)) != -1)
                set_mine(get_elem_champ_mine(terrain, i-1, j+1), get_mine(get_elem_champ_mine(terrain, i-1, j+1)) + 1);
            if (get_mine(get_elem_champ_mine(terrain, i-1, j-1)) != -1)
                set_mine(get_elem_champ_mine(terrain, i-1, j-1), get_mine(get_elem_champ_mine(terrain, i-1, j-1)) + 1);
            if (get_mine(get_elem_champ_mine(terrain, i, j-1)) != -1)
                set_mine(get_elem_champ_mine(terrain, i, j-1), get_mine(get_elem_champ_mine(terrain, i, j-1)) + 1);
        }
    }
    else{
        if (get_mine(get_elem_champ_mine(terrain, i-1, j)) != -1)
            set_mine(get_elem_champ_mine(terrain, i-1, j), get_mine(get_elem_champ_mine(terrain, i-1, j)) + 1);
        if (get_mine(get_elem_champ_mine(terrain, i+1, j)) != -1)
            set_mine(get_elem_champ_mine(terrain, i+1, j), get_mine(get_elem_champ_mine(terrain, i+1, j)) + 1);
        if(j == 0){
            if (get_mine(get_elem_champ_mine(terrain, i-1, j+1)) != -1)
                set_mine(get_elem_champ_mine(terrain, i-1, j+1), get_mine(get_elem_champ_mine(terrain, i-1, j+1)) + 1);
            if (get_mine(get_elem_champ_mine(terrain, i, j+1)) != -1)
                set_mine(get_elem_champ_mine(terrain, i, j+1), get_mine(get_elem_champ_mine(terrain, i, j+1)) + 1);
            if (get_mine(get_elem_champ_mine(terrain, i+1, j+1)) != -1)
                set_mine(get_elem_champ_mine(terrain, i+1, j+1), get_mine(get_elem_champ_mine(terrain, i+1, j+1)) + 1);
        }
        else if(j == colonne_max){
            if (get_mine(get_elem_champ_mine(terrain, i-1, j-1)) != -1)
                set_mine(get_elem_champ_mine(terrain, i-1, j-1), get_mine(get_elem_champ_mine(terrain, i-1, j-1)) + 1);
            if (get_mine(get_elem_champ_mine(terrain, i, j-1)) != -1)
                set_mine(get_elem_champ_mine(terrain, i, j-1), get_mine(get_elem_champ_mine(terrain, i, j-1)) + 1);
            if (get_mine(get_elem_champ_mine(terrain, i+1, j-1)) != -1)
                set_mine(get_elem_champ_mine(terrain, i+1, j-1), get_mine(get_elem_champ_mine(terrain, i+1, j-1)) + 1);
        }
        else{
            if (get_mine(get_elem_champ_mine(terrain, i-1, j-1)) != -1)
                set_mine(get_elem_champ_mine(terrain, i-1, j-1), get_mine(get_elem_champ_mine(terrain, i-1, j-1)) + 1);
            if (get_mine(get_elem_champ_mine(terrain, i, j-1)) != -1)
                set_mine(get_elem_champ_mine(terrain, i, j-1), get_mine(get_elem_champ_mine(terrain, i, j-1)) + 1);
            if (get_mine(get_elem_champ_mine(terrain, i+1, j-1)) != -1)
                set_mine(get_elem_champ_mine(terrain, i+1, j-1), get_mine(get_elem_champ_mine(terrain, i+1, j-1)) + 1);
            if (get_mine(get_elem_champ_mine(terrain, i-1, j+1)) != -1)
                set_mine(get_elem_champ_mine(terrain, i-1, j+1), get_mine(get_elem_champ_mine(terrain, i-1, j+1)) + 1);
            if (get_mine(get_elem_champ_mine(terrain, i, j+1)) != -1)
                set_mine(get_elem_champ_mine(terrain, i, j+1), get_mine(get_elem_champ_mine(terrain, i, j+1)) + 1);
            if (get_mine(get_elem_champ_mine(terrain, i+1, j+1)) != -1)
                set_mine(get_elem_champ_mine(terrain, i+1, j+1), get_mine(get_elem_champ_mine(terrain, i+1, j+1)) + 1);
        }
    }

}//fin actualise_compteur_autour_de_bombe

void mode_debutant(Terrain *terrain){
    assert(terrain!=NULL);

    destructeur_champ_mine(get_champ_mine(terrain), get_ligne(get_regle(terrain)), get_colonne(get_regle(terrain)));
    set_ligne(get_regle(terrain), NBR_LIGNE_DEBUTANT);
    set_colonne(get_regle(terrain), NBR_COLONNE_DEBUTANT);
    set_temps(get_regle(terrain), TEMPS_DEBUTANT);
    set_nombre_mine(get_regle(terrain), NBR_MINE_DEBUTANT);
    set_champ_mine(terrain, constructeur_champ_mine(get_ligne(get_regle(terrain)), get_colonne(get_regle(terrain))));
    nouvelle_partie(terrain);
}//fin mode_debutant

void mode_intermediaire(Terrain *terrain){
    assert(terrain!=NULL);

    destructeur_champ_mine(get_champ_mine(terrain), get_ligne(get_regle(terrain)), get_colonne(get_regle(terrain)));
    set_ligne(get_regle(terrain), NBR_LIGNE_INTERMEDIAIRE);
    set_colonne(get_regle(terrain), NBR_COLONNE_INTERMEDIAIRE);
    set_temps(get_regle(terrain), TEMPS_INTERMEDIAIRE);
    set_nombre_mine(get_regle(terrain), NBR_MINE_INTERMEDIAIRE);
    set_champ_mine(terrain, constructeur_champ_mine(get_ligne(get_regle(terrain)), get_colonne(get_regle(terrain))));
    nouvelle_partie(terrain);
}//fin mode_intermediaire

void mode_expert(Terrain *terrain){
    assert(terrain!=NULL);
    
    destructeur_champ_mine(get_champ_mine(terrain), get_ligne(get_regle(terrain)), get_colonne(get_regle(terrain)));
    set_ligne(get_regle(terrain), NBR_LIGNE_EXPERT);
    set_colonne(get_regle(terrain), NBR_COLONNE_EXPERT);
    set_temps(get_regle(terrain), TEMPS_EXPERT);
    set_nombre_mine(get_regle(terrain), NBR_MINE_EXPERT);
    set_champ_mine(terrain, constructeur_champ_mine(get_ligne(get_regle(terrain)), get_colonne(get_regle(terrain))));
    nouvelle_partie(terrain);
}//fin mode_expert

void decouvre_bombe(Terrain *terrain){
    unsigned int derniere_coord_bombe_devoilee[2] = {0};
    unsigned short colonne = get_colonne(get_regle(terrain));
    for(unsigned short i=0; i<get_nombre_mine(get_regle(terrain)); i++){
        while(get_mine(get_elem_champ_mine(terrain, derniere_coord_bombe_devoilee[0], derniere_coord_bombe_devoilee[1]))!=-1){
            derniere_coord_bombe_devoilee[1]++;
            if(derniere_coord_bombe_devoilee[1]==colonne){
                derniere_coord_bombe_devoilee[1]=0;
                derniere_coord_bombe_devoilee[0]++;
            }
            if(get_Boite_decouverte(get_elem_champ_mine(terrain, derniere_coord_bombe_devoilee[0], derniere_coord_bombe_devoilee[1]))==-1)
                charge_image_bouton(get_bouton(terrain, derniere_coord_bombe_devoilee[0], derniere_coord_bombe_devoilee[1]), -2);
        }
        if(get_win(get_regle(terrain))==-1 && get_Boite_decouverte(get_elem_champ_mine(terrain, derniere_coord_bombe_devoilee[0], derniere_coord_bombe_devoilee[1]))!=-1)//si perdu 
            charge_image_bouton(get_bouton(terrain, derniere_coord_bombe_devoilee[0], derniere_coord_bombe_devoilee[1]), -1);
        else
            charge_image_bouton(get_bouton(terrain, derniere_coord_bombe_devoilee[0], derniere_coord_bombe_devoilee[1]), -4);
        derniere_coord_bombe_devoilee[1]++;
        if(derniere_coord_bombe_devoilee[1]==colonne){
            derniere_coord_bombe_devoilee[1]=0;
            derniere_coord_bombe_devoilee[0]++;
        }
        if(get_Boite_decouverte(get_elem_champ_mine(terrain, derniere_coord_bombe_devoilee[0], derniere_coord_bombe_devoilee[1]))==-1)
            charge_image_bouton(get_bouton(terrain, derniere_coord_bombe_devoilee[0], derniere_coord_bombe_devoilee[1]), -2);
    }
}//fin decouvre_bombe

int verifie_correspondance_nombre_drapeau_nombre_mine(Terrain *terrain, unsigned short i, unsigned short j){
    if(nombre_drapeau_autour(terrain, i, j)==get_mine(get_elem_champ_mine(terrain, i, j)))
        return 1;
    else
        return 0;
}//fin verifie_correspondance_nombre_drapeau_nombre_mine

static int nombre_drapeau_autour(Terrain *terrain, unsigned short i, unsigned short j){
    int nombre_drapeau=0;
    unsigned short ligne = get_ligne(get_regle(terrain)), colonne = get_colonne(get_regle(terrain));

    if(i!=0 && j!=0){
        if(get_Boite_decouverte(get_elem_champ_mine(terrain, i-1, j-1))==-1)
            nombre_drapeau++;
    }
    if(i!=0){
        if(get_Boite_decouverte(get_elem_champ_mine(terrain, i-1, j))==-1)
            nombre_drapeau++;
    }
    if(i!=0 && j!=colonne-1){
        if (get_Boite_decouverte(get_elem_champ_mine(terrain, i-1, j+1))==-1)
         nombre_drapeau++;
    }
    if(j!=0){
        if (get_Boite_decouverte(get_elem_champ_mine(terrain, i, j-1))==-1)
            nombre_drapeau++;
    }
    if(j!=colonne-1){
        if (get_Boite_decouverte(get_elem_champ_mine(terrain, i, j+1))==-1)
            nombre_drapeau++;
    }
    if(i!=ligne-1 && j!=0){
        if (get_Boite_decouverte(get_elem_champ_mine(terrain, i+1, j-1))==-1)
            nombre_drapeau++;
    }
    if(i!=ligne-1){
        if (get_Boite_decouverte(get_elem_champ_mine(terrain, i+1, j))==-1)
            nombre_drapeau++;
    }
    if(i!=ligne-1 && j!=colonne-1){
        if (get_Boite_decouverte(get_elem_champ_mine(terrain, i+1, j+1))==-1)
            nombre_drapeau++;
    }

    return nombre_drapeau;
}//fin nombre_drapeau_autour

void Initialisation_timer(Terrain *terrain){
    assert(terrain!=NULL);
    char texte[4];
    Timer *timer = get_timer(terrain);
    GtkWidget *pLabel = get_label_timer(timer);
    Regle *regle = get_regle(terrain);

    sprintf(texte,"%d", get_temps(regle));
    
    set_temps_restant(timer, get_temps(get_regle(terrain)));
    set_timer_lance(timer, 0);
    gtk_label_set_text(GTK_LABEL(pLabel), texte);
    
}//fin Initialisation_timer

void demarre_timer(Terrain *terrain){
    assert(terrain!=NULL);

    g_timeout_add(1000, tic, (gpointer)terrain);
}

gint tic(gpointer data){
    assert(data!=NULL);

    Terrain *terrain = data;
    Timer *timer = get_timer(terrain);
    if(get_temps_restant(timer)>0 && get_win(get_regle(terrain))==0 && get_timer_lance(timer)==1)
        g_timeout_add(1000, tic, (gpointer)terrain);
    else
        return 0;

    set_temps_restant(timer, get_temps_restant(timer)-1);
    maj_timer(timer);

    if(get_temps_restant(timer)==0){
        set_win(get_regle(terrain), -1);
        decouvre_bombe(terrain);
        charge_image_bouton(get_bouton_new_game(terrain), -6);
    }

    return 0;
}
