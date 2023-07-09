#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>

#define PORT 8080

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *pierre = "pierre";
    char *feuille = "feuille";
    char *ciseau = "ciseau";
    int joueurA = 0, joueurB = 0;
    int rounds = 0;

    // Création du socket serveur
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        printf("Erreur lors de la création du socket\n");
        return -1;
    }

    // Configuration des informations de connexion pour le serveur
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Attachement du socket au port 8080
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        printf("L'attachement du socket a échoué\n");
        return -1;
    }

    // Mise en écoute du socket pour les connexions entrantes
    if (listen(server_fd, 3) < 0) {
        printf("Erreur lors de la mise en écoute du socket\n");
        return -1;
    }

    // Boucle infinie pour accepter les connexions entrantes
    while(1) {
        // Attente de la connexion d'un client
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
            printf("Erreur lors de la connexion du client\n");
            return -1;
        }

        // Boucle de jeu
        while(1) {
            // Lecture du choix du joueur A
            memset(buffer, 0, sizeof(buffer));
            read(new_socket, buffer, 1024);

            // Génération aléatoire du choix du joueur B
            srand(time(NULL));
            int choixB = rand() % 3;

            // Analyse du choix du joueur A et comparaison avec celui du joueur B
            if (strcmp(buffer, pierre) == 0) {
                if (choixB == 0) {
                    // Egalité
                    send(new_socket, "Jeu 1: A : pierre :: B : pierre : Egalite", strlen("Jeu 1: A : pierre :: B : pierre : Egalite"), 0);
                }
                else if (choixB == 1) {
                    // Joueur B gagne
                    send(new_socket, "Jeu 1: A : pierre :: B : feuille : B gagne", strlen("Jeu 1: A : pierre :: B : feuille : B gagne"), 0);
                    joueurB++;
                }
                else if (choixB == 2) {
                    // Joueur A gagne
                    send(new_socket, "Jeu 1: A : pierre :: B : ciseau : A gagne", strlen("Jeu 1: A : pierre :: B : ciseau : A gagne"), 0);
                    joueurA++;
                }
            }
            else if (strcmp(buffer, feuille) == 0) {
			
			           
                if (choixB == 0) {
                    // Joueur A gagne
                    send(new_socket, "Jeu 1: A : feuille :: B : pierre : A gagne", strlen("Jeu 1: A : feuille :: B : pierre : A gagne"), 0);
                    joueurA++;
                }
                else if (choixB == 1) {
                    // Egalité
                    send(new_socket, "Jeu 1: A : feuille :: B : feuille : Egalite", strlen("Jeu 1: A : feuille :: B : feuille : Egalite"), 0);
                }
                else if (choixB == 2) {
                    // Joueur B gagne
                    send(new_socket, "Jeu 1: A : feuille :: B : ciseau : B gagne", strlen("Jeu 1: A : feuille :: B : ciseau : B gagne"), 0);
                    joueurB++;
                }
            }
            else if (strcmp(buffer, ciseau) == 0) {
                if (choixB == 0) {
                    // Joueur B gagne
                    send(new_socket, "Jeu 1: A : ciseau :: B : pierre : B gagne", strlen("Jeu 1: A : ciseau :: B : pierre : B gagne"), 0);
                    joueurB++;
                }
                else if (choixB == 1) {
                    // Joueur A gagne
                    send(new_socket, "Jeu 1: A : ciseau :: B : feuille : A gagne", strlen("Jeu 1: A : ciseau :: B : feuille : A gagne"), 0);
                    joueurA++;
                }
                else if (choixB == 2) {
                    // Egalité
                    send(new_socket, "Jeu 1: A : ciseau :: B : ciseau : Egalite", strlen("Jeu 1: A : ciseau :: B : ciseau : Egalite"), 0);
                }
            }

            // Incrémentation du nombre de manches
            rounds++;

            // Vérification du nombre de manches jouées
            if (rounds >= 3) {
                // Envoi du résultat final
                if (joueurA > joueurB) {
                    send(new_socket, "A a gagné", strlen("A a gagné"), 0);
                }
                else if (joueurB > joueurA) {
                    send(new_socket, "B a gagné", strlen("B a gagné"), 0);
                }
                else {
                    send(new_socket, "Egalite", strlen("Egalite"), 0);
                }

                // Demande au joueur s'il souhaite rejouer
                memset(buffer, 0, sizeof(buffer));
                read(new_socket, buffer, 1024);
                if (strcmp(buffer, "non") == 0) {
                    // Fermeture de la connexion
                    close(new_socket);
                    break;
                }
                else {
                    // Réinitialisation des variables de jeu
                    rounds = 0;
                    joueurA = 0;
                    joueurB = 0;
                }
            }
        }
    }

    return 0;
}

               
