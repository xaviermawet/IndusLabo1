#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

#define CLS         printf("\033[H\033[2J");
#define HOME_CURSOR printf("\033[H");
#define FREE_BUFF   while ((c = getchar()) != '\n' && c != EOF);
#define UNUSED(x) (void)(x)

#define BLACK               30
#define RED                 31
#define GREEN               32
#define YELLOW              33
#define BLUE                34
#define MAGENTA             35
#define CYAN                36
#define WHITE               37
#define COLOR(id)           printf("\033[%sm",id)
#define GREENPRINTF(msg)    printf ("\33[32m%s\33[00m", msg)
#define REDPRINTF(msg)      printf ("\33[31m%s\33[00m", msg)

#define FALSE   0
#define TRUE    1

typedef short bool;

/*--------------------------------------------------------------------------*
 * Récupère un nombre entier saisi au clavier
 *--------------------------------------------------------------------------*
 * Return:
 *    x         la valeur entrée par l'utilisateur
 *    0         si erreur
 *--------------------------------------------------------------------------*/
int getNumber(void);

/*--------------------------------------------------------------------------*
 * Génère un nombre aléatoire compris entre deux bornes
 *--------------------------------------------------------------------------*
 * Paramètres:
 *    min       borne inférieure
 *    max       borne supérieure
 * Return:
 *    x         le nombre aléatoire généré
 *--------------------------------------------------------------------------*/
int getRandomNumber(int min, int max);

/*--------------------------------------------------------------------------*
 * Lit le tampon d'entrée
 *--------------------------------------------------------------------------*
 * Paramètres:
 *    string    chaîne dans laquelle les données seront sauvées
 *    length    nombre de caractères pouvant être sauvés ('\0' compris)
 *--------------------------------------------------------------------------*/
void read_stdin(char *string, short length);

/*--------------------------------------------------------------------------*
 * Vérifie si l'utilisateur a appuyé sur une touche
 *--------------------------------------------------------------------------*
 * Return:
 *    TRUE      Si une touche à été enfoncée
 *    FALSE     si aucune touche n'a été enfoncée
 *--------------------------------------------------------------------------*/
bool kbhit(void);

/*--------------------------------------------------------------------------*
 * Renvoie un caractère lu au clavier sans écho à l’écran
 *--------------------------------------------------------------------------*
 * Return:
 *    int   le code asci du caractère entré
 *--------------------------------------------------------------------------*/
int mygetch (void);

/*--------------------------------------------------------------------------*
 * Sauve et modifie la configuration du terminal pour masquer la saisie
 * au clavier
 *--------------------------------------------------------------------------*
 * Paramètre:
 *    prev  structure dans laquelle l'ancienne configuration sera sauvée
 *--------------------------------------------------------------------------*/
int reconfigurerTerminal(struct termios* prev);

/*--------------------------------------------------------------------------*
 * Restaure la configuration du terminal
 *--------------------------------------------------------------------------*
 * Paramètre:
 *    prev  la configuration a appliquer au terminal
 *--------------------------------------------------------------------------*/
int restaurerTerminal(const struct termios* prev);

#endif /* __COMMON_H__ */
