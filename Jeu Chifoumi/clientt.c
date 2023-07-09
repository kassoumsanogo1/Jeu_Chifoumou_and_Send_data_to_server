#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};

    // Vérification des arguments
    if (argc != 2) {
        printf("Usage: %s <adresse IP du serveur>\n", argv[0]);
        return -1;
    }

    // Création de la socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Erreur de création de la socket\n");
        return -1;
    }

    // Initialisation de la structure serv_addr
    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);

    // Conversion de l'adresse IP en format binaire
    if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0) {
        printf("Adresse IP invalide\n");
        return -1;
    }

    // Connexion au serveur
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("Erreur de connexion au serveur\n");
        return -1;
    }

    // Boucle principale de jeu
    while (1) {
        printf("Choisissez pierre, feuille ou ciseau : ");
        fgets(buffer, 1024, stdin);

        // Envoi du choix au serveur
        send(sock, buffer, strlen(buffer), 0);

        // Réception du résultat intermédiaire
        memset(buffer, 0, sizeof(buffer));
        read(sock, buffer, 1024);
        printf("%s\n", buffer);

        // Demande au joueur s'il souhaite rejouer
        printf("Voulez-vous rejouer ? (oui/non) : ");
        memset(buffer, 0, sizeof(buffer));
        fgets(buffer, 1024, stdin);

        // Envoi de la réponse au serveur
        send(sock, buffer, strlen(buffer), 0);

        // Si le joueur ne veut pas rejouer, fermeture de la connexion
        if (strcmp(buffer, "non\n") == 0) {
            close(sock);
            break;
        }
    }

    return 0;
}
