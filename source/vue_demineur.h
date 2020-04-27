/**
 * \file vue_demineur.h
 * \brief Ce fichier contient les déclarations de types et les prototypes des fonctions pour la gestion
 * des entrées de l'utilisateur
 * 
 * \author{Randaxhe Martin - Russe Cyril}
 * \date: 24-04-2020
 * 
 */

#ifndef __VUE_DEMINEUR__
#define __VUE_DEMINEUR__

#include "type_opaque.h"

GtkWidget *creation_fenetre(void);

GtkWidget *creation_menus(GtkWidget *pFenetre);

GtkWidget *structure_box(GtkWidget *pFenetre, Terrain *terrain);

#endif