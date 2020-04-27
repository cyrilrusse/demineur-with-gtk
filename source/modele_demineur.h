/**
 * \file modele_demineur.h
 * \brief Ce fichier contient les déclarations de types et les prototypes des fonctions pour la gestion
 * de l'affichage de l'interface graphique demineur
 * 
 * \author{Randaxhe Martin - Russe Cyril}
 * \date: 24-04-2020
 * 
 */

#ifndef __MODELE_DEMINEUR__
#define __MODELE_DEMINEUR__
#include "type_opaque.h"

/**
 * \fn aleatoire_bombe_et_compteur(Terrain *terrain_de_jeu)
 * \brief places les bombes et incrémente les compteurs à coté
 * 
 * \param terrain_de_jeu La structure contenant toutes les informations du terrain
 * 
 */
void aleatoire_bombe_et_compteur(Terrain *terrain_de_jeu);

/**
 * \fn initialisation_champ_mine(Terrain *terrain_de_jeu)
 * \brief crée un champ de mine avec les bons compteurs
 * 
 * \param terrain_de_jeu La structure contenant toutes les informations du terrain
 * 
 */
void initialisation_champ_mine(Terrain *terrain_de_jeu);

/**
 * \fn verife_bombe_dans_ta_mere(Terrain *terrain_de_jeu, unsigned short ligne, unsigned short colonne)
 * \brief test si on est sur une case vide et si c'est le cas test les cases à coté
 * 
 * \param terrain_de_jeu La structure contenant toutes les informations du terrain
 * \param ligne La ligne où on fait le test
 * \param colonne La colonne où on fait le test 
 * 
 */
void verife_bombe_dans_ta_mere(Terrain *terrain_de_jeu, unsigned short ligne, unsigned short colonne);

/**
 * \fn initialisation_mine_0(Terrain *terrain_de_jeu)
 * \brief initialise toutes les mines du champ de mine à 0
 * 
 * \param terrain_de_jeu La structure contenant toutes les informations du terrain
 * 
 */
void initialisation_mine_0(Terrain *terrain_de_jeu);

#endif