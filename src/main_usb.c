#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <unistd.h>
#include "define.h"
#include "ftd2xx.h"
#include "commande.h"
#include "releve.h"
#include "regulation.h"
#include "consigne.h"

/**
 * Programme principal USB
 * Lecture des températures via USB -> Régulation PID -> Envoi commande chauffage via USB
*/
int main(void) {
    printf("=== Demarrage du Programme de Regulation USB ===\n\n");
    
    // Variables
    temp_t temperatures;
    temperatures.interieure = 20.0;
    temperatures.exterieure = 15.0;
    float cmd = 0.0;
    float csgn = 0.0;
    pid PID;
    PID.erreur_precedente = 0.0;
    PID.integrale = 0.0;
    PID.iteration = 0;
    
    FT_STATUS ftStatus;
    
    // ===== ÉTAPE 1 : LISTER ET OUVRIR UNE SEULE CONNEXION USB =====
    printf("Etape 1: Detection et connexion au device FTDI\n");
    printf("-" "-----\n");
    
    // Afficher tous les devices disponibles
    printf("\nDevices FTDI disponibles:\n");
    commande_list_devices();
    
    // Initialiser une SEULE connexion (device 0) pour la commande
    printf("\nOuverture connexion COMMANDE (device 0)...\n");
    ftStatus = commande_init(0);
    if (ftStatus != FT_OK) {
        printf("ERREUR: Impossible d'ouvrir la connexion FTDI\n");
        return EXIT_FAILURE;
    }
    
    printf("===== Connexion USB etablie avec succes =====\n\n");
    
    // ===== ÉTAPE 2 : BOUCLE PRINCIPALE DE RÉGULATION =====
    printf("Etape 2: Boucle de regulation PID\n");
    printf("-" "-----\n\n");
    
    int iteration = 0;
    const int MAX_ITERATIONS = 10;  // Limiter à 10 itérations pour les tests
    
    while (iteration < MAX_ITERATIONS) {
        iteration++;
        
        printf("\n=== Iteration %d ===\n", iteration);
        
        // --- Lecture de la consigne ---
        csgn = consigne(temperatures.interieure);
        printf("Consigne: %.1f°C\n", csgn);
        
        // --- Lecture des températures via USB ---
        printf("Lecture des donnees USB...\n");
        temperatures = releve_read();
        printf("T_interieure: %.1f°C, T_exterieure: %.1f°C\n", 
               temperatures.interieure, temperatures.exterieure);
        
        // --- Calcul de la régulation PID ---
        printf("Calcul regulation PID...\n");
        cmd = regulation(2, csgn, temperatures.interieure, &PID);
        printf("Commande calculee: %.1f%%\n", cmd);
        
        // --- Envoi de la commande via USB ---
        printf("Envoi commande via USB...\n");
        float cmd_result = commande(cmd);
        if (cmd_result < 0) {
            printf("ERREUR: Echec envoi commande\n");
        }
        
        // --- Attendre avant la prochaine itération ---
        printf("Attente 1 seconde avant prochaine iteration...\n");
        Sleep(1000);  // 1 seconde
    }
    
    // ===== ÉTAPE 3 : FERMETURE =====
    printf("\n===== Fin de la simulation ===\n");
    printf("Fermeture de la connexion...\n");
    
    commande_close();
    
    printf("Au revoir!\n");
    return EXIT_SUCCESS;
}
