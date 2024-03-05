#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <utilapiset.h>

#define TAILLE 10

void initialiserGrille(char grille[TAILLE][TAILLE]) {
    for (int i = 0; i < TAILLE; i++) {
        for (int j = 0; j < TAILLE; j++) {
            grille[i][j] = '  ';
        }
    }
}

void placerMines(char grille[TAILLE][TAILLE], int nbMines) {
    int count = 0;
    while (count < nbMines) {
        int x = rand() % TAILLE;
        int y = rand() % TAILLE;
        if (grille[x][y] != 'm') {
            grille[x][y] = 'm';
            count++;
        }
    }
}

int compterMines(char grille[TAILLE][TAILLE], int x, int y) {
    int count = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            int newX = x + i;
            int newY = y + j;
            if (newX >= 0 && newX < TAILLE && newY >= 0 && newY < TAILLE && grille[newX][newY] == 'm') {
                count++;
            }
        }
    }
    return count;
}

void afficherGrille(char grille[TAILLE][TAILLE], int montrerMines) {
    for (int i = 0; i < TAILLE; i++) {
        for (int j = 0; j < TAILLE; j++) {
            if (grille[i][j] == 'v') {
                printf("[   ] ");
            } else if (grille[i][j] == 'm' && montrerMines) {
                printf("[ m ] ");
            } else if (grille[i][j] == 'r') {
                if (compterMines(grille, i, j) != 0) {
                    printf("[ %d ] ", compterMines(grille, i, j));
                } else {
                    printf("[   ] ");
                }
            } else if (grille[i][j] == '*') {
                printf("[ * ] ");
            }  else if (grille[i][j] == 'd') {
                printf("[ d ] ");
            }else {
                printf("[%d:%d] ", i, j);
            }
        }
        printf("\n");
    }
    Beep(500, 500);
}

void revelerCellulesVides(char grille[TAILLE][TAILLE], int x, int y) {
    if (x < 0 || x >= TAILLE || y < 0 || y >= TAILLE || grille[x][y] != ' ') {
        return;
    }

    int mines = compterMines(grille, x, y);
    if (mines > 0) {
        grille[x][y] = mines + '0';
    } else {
        grille[x][y] = 'v';  // 'v' for visitee
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                revelerCellulesVides(grille, x + i, y + j);
            }
        }
    }
}

int main() {
    srand(time(NULL));

    char grille[TAILLE][TAILLE];
    int cellulesRestantes;

    do {
        initialiserGrille(grille);

        int difficulte;
        printf("Choisissez la difficulte (1, 2 ou 3) : ");
        scanf("%d", &difficulte);

        int nbMines = 10 * difficulte;
        placerMines(grille, nbMines);

        cellulesRestantes = TAILLE * TAILLE - nbMines;

        afficherGrille(grille, 0);

        while (1) {
            int ligne, colonne;
            char action;
            printf("Choisissez une ligne, une colonne et une action (0-%d) (Format : ligne colonne action (f pour drapeau, u pour de-drapeau, r pour reveler)) : ",
                   TAILLE - 1);
            int resultat = scanf("%d %d %c", &ligne, &colonne, &action);

            if (resultat != 3 || (action != 'f' && action != 'u' && action != 'r')) {
                printf("Entree invalide. Veuillez entrer des coordonnees et une action valides.\n");
                while (getchar() != '\n');
                continue;
            }

            if (action == 'f') {
                if (grille[ligne][colonne] != 'r') {
                    grille[ligne][colonne] = 'd';
                }
            } else if (action == 'u') {
                if (grille[ligne][colonne] == 'd') {
                    grille[ligne][colonne] = ' ';
                }
            } else if (action == 'r') {
                if (grille[ligne][colonne] == 'm') {
                    printf("Partie terminee ! Vous avez touche une mine.\n");
                    grille[ligne][colonne] = '*';
                    afficherGrille(grille, 1);
                    Beep(2000, 1000);
                    break;
                } else if (grille[ligne][colonne] == ' ') {
                    revelerCellulesVides(grille, ligne, colonne);
                    cellulesRestantes--;
                    grille[ligne][colonne] = 'r';
                    if (cellulesRestantes == 0) {
                        printf("Felicitations ! Vous avez degage toutes les cellules non minees.\n");
                        afficherGrille(grille, 1);
                        Beep(2000, 1000);
                        break;
                    }
                }
            }

            afficherGrille(grille, 0);
        }

        char choixRejouer;
        printf("Voulez-vous rejouer ? (o/n) : ");
        scanf(" %c", &choixRejouer);

        if (choixRejouer != 'o' && choixRejouer != 'O') {
            printf("Au revoir ! Fin du programme.\n");
            break;
        }

    } while (1);

    return 0;
}