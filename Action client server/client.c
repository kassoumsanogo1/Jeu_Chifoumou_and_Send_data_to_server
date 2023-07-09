
Code Client:

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

// Fonction pour lire une chaîne de caractères depuis l'entrée standard
void read_string(char* buffer, int size) {
fgets(buffer, size, stdin);
}

int main(int argc, char const *argv[]) {
// Vérifier que l'adresse IP et le port ont été spécifiés en ligne de commande
if (argc < 3) {
printf("Utilisation: %s adresse_IP port\n", argv[0]);
exit(EXIT_FAILURE);
}

// Récupérer l'adresse IP et le port depuis la ligne de commande
char* ip_address = (char*)argv[1];
int port = atoi(argv[2]);

// Créer un socket pour se connecter au serveur
int sock = 0;
if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("\n Erreur : la création du socket a échoué \n");
    return EXIT_FAILURE;
}

// Configurer l'adresse du serveur
struct sockaddr_in serv_addr;
serv_addr.sin_family = AF_INET;
serv_addr.sin_port = htons(port);

// Convertir l'adresse IP depuis une chaîne de caractères vers une structure d'adresse
if(inet_pton(AF_INET, ip_address, &serv_addr.sin_addr)<=0) {
    printf("\n Erreur : l'adresse IP invalide \n");
    return EXIT_FAILURE;
}

// Se connecter au serveur
if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    printf("\n Erreur : la connexion au serveur a échoué \n");
    return EXIT_FAILURE;
}

// Recevoir un message de bienvenue du serveur
char buffer[1024] = {0};
int valread = read(sock, buffer, 1024);
printf("%s\n", buffer);

// Boucle principale du jeu
while (true) {
    // Demander au joueur s'il souhaite jouer
    printf("Voulez-vous jouer? (oui/non)\n");
    char answer[1024];
    read_string(answer, 1024);

    // Envoyer la réponse au serveur
    send(sock, answer, strlen(answer), 0);

    // Vérifier si le joueur souhaite jouer
    if (strcmp(answer, "oui\n") == 0) {
        // Boucle pour jouer le nombre de manches spécifié
        for (int i = 0; i < num_rounds; i++) {
            // Recevoir la demande du serveur pour le choix du joueur
            valread = read(sock, buffer, 1024);
            printf("%s\n", buffer);

            // Demander au joueur son choix
            char choice[1024];
            read_string(choice, 1024);

            // Envoyer le choix du joueur au serveur
            send(sock, choice, strlen(choice), 0);

            // Recevoir le résultat intermédiaire du serveur
            valread = read(sock, buffer, 1024);
            printf("%s\n", buffer);
        }

        // Recevoir le résultat final du serveur
        valread = read(sock, buffer, 1024);
        printf("%s\n", buffer);

        // Demander au joueur s'il souhaite rejouer
        valread = read(sock, buffer, 1024);
        printf("%s\n", buffer);
        read_string(answer, 1024);
        send(sock,
		        // Vérifier si le joueur souhaite rejouer
        if (strcmp(answer, "oui\n") != 0) {
            break;
        }
    } else {
        // Le joueur ne veut pas jouer, terminer la connexion
        break;
    }
	}

	// Terminer la connexion
	printf("Déconnexion...\n");
	close(sock);

return EXIT_SUCCESS;
}

