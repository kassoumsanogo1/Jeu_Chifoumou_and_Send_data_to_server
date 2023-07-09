#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 12345

char* play_round(char* player_choice) {
    char* server_choice;
    char* result;
    int random_num;

    // Générer le choix du serveur aléatoirement
    srand(time(NULL));
    random_num = rand() % 3;
    if (random_num == 0) {
        server_choice = "pierre";
    } else if (random_num == 1) {
        server_choice = "feuille";
    } else {
        server_choice = "ciseau";
    }

    // Comparer le choix du joueur avec le choix du serveur pour déterminer le résultat
    if (strcmp(player_choice, server_choice) == 0) {
        result = "égalité";
    } else if ((strcmp(player_choice, "pierre") == 0 && strcmp(server_choice, "ciseau") == 0) || (strcmp(player_choice, "feuille") == 0 && strcmp(server_choice, "pierre") == 0) || (strcmp(player_choice, "ciseau") == 0 && strcmp(server_choice, "feuille") == 0)) {
        result = "joueur";
    } else {
        result = "serveur";
    }

    // Créer une chaîne de caractères contenant le résultat intermédiaire
    char* intermediate_result = malloc(strlen("jeu A: :: B:  Resultat A|B gagne") + strlen(player_choice) + strlen(server_choice) + strlen(result));
    sprintf(intermediate_result, "jeu A: %s :: B: %s : Resultat %s|%s gagne", player_choice, server_choice, result, result == "joueur" ? "serveur" : "joueur");

    return intermediate_result;
}

int main(int argc, char const *argv[]) {
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char* welcome_message = "Bienvenue au jeu de Chifoumi! Êtes-vous prêt à jouer? (oui/non)\n";
    char* prompt_message = "Entrez votre choix (pierre/feuille/ciseau):\n";
    char* goodbye_message = "Merci d'avoir joué! Appuyez sur une touche pour quitter.\n";
    int num_rounds = 3;
    int player_wins = 0;
    int server_wins = 0;

    // Créer le socket serveur
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Configurer le socket serveur
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Lier le socket serveur à l'adresse et au port
    if (bind(server_fd, ((struct sockaddr *)&address, sizeof(address))<0) {
	perror("bind failed");
	exit(EXIT_FAILURE);
	}
	
	// Mettre le socket serveur en écoute
if (listen(server_fd, 3) < 0) {
    perror("listen");
    exit(EXIT_FAILURE);
}

printf("En attente de connexions...\n");

// Accepter une connexion
if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
    perror("accept");
    exit(EXIT_FAILURE);
}

// Envoyer un message de bienvenue au joueur
send(new_socket, welcome_message, strlen(welcome_message), 0);

// Boucle principale du jeu
while (true) {
    // Recevoir la réponse du joueur
    valread = read(new_socket, buffer, 1024);
    buffer[valread] = '\0';

    // Vérifier si le joueur souhaite jouer
    if (strcmp(buffer, "oui\n") == 0) {
        // Initialiser le compteur de victoires pour chaque joueur
        player_wins = 0;
        server_wins = 0;

        // Boucle pour jouer le nombre de manches spécifié
        for (int i = 0; i < num_rounds; i++) {
            // Demander au joueur son choix
            send(new_socket, prompt_message, strlen(prompt_message), 0);
            valread = read(new_socket, buffer, 1024);
            buffer[valread] = '\0';

            // Jouer un tour et envoyer le résultat intermédiaire au joueur
            char* intermediate_result = play_round(buffer);
            send(new_socket, intermediate_result, strlen(intermediate_result), 0);

            // Mettre à jour le compteur de victoires pour chaque joueur
            if (strstr(intermediate_result, "joueur gagne")) {
                player_wins++;
            } else if (strstr(intermediate_result, "serveur gagne")) {
                server_wins++;
            }

            // Libérer la mémoire allouée pour la chaîne de caractères intermédiaire
            free(intermediate_result);
        }

        // Envoyer le résultat final au joueur
        char* final_result;
        if (player_wins == server_wins) {
            final_result = "égalité";
        } else if (player_wins > server_wins) {
            final_result = "joueur";
        } else {
            final_result = "serveur";
        }
        char final_result_message[strlen(final_result) + strlen(" a gagné\n")];
        sprintf(final_result_message, "%s a gagné\n", final_result);
        send(new_socket, final_result_message, strlen(final_result_message), 0);

        // Demander au joueur s'il souhaite rejouer
        send(new_socket, "Voulez-vous rejouer? (oui/non)\n", strlen("Voulez-vous rejouer? (oui/non)\n"), 0);
    } else {
        // Le joueur a choisi de ne pas jouer, terminer la connexion
        send(new_socket, goodbye_message, strlen(goodbye_message), 0);
        break;
    }
}

// Fermer le socket
close(new_socket);
close(server_fd);

return 0;

