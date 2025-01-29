## 1. Introduction

Ce projet vise à créer un point d'accès Wi-Fi autonome basé sur un ESP32, capable de fournir une connexion Wi-Fi avec un mot de passe dynamique tout en affichant des statistiques sur un écran TFT. Il permet d'évaluer et de suivre les connexions aux réseaux Wi-Fi environnants.

## 2. Installation et configuration de l'ESP32 et son écran TFT 3.5"

Pour configurer et utiliser un écran TFT 3.5'' avec l'ESP32, suivez les étapes suivantes :

1.  **Télécharger et installer l'Arduino IDE**
    
    -   Disponible ici : [Arduino IDE](https://www.arduino.cc/en/software)
        
2.  **Ajouter le support ESP32 dans l'Arduino IDE**
    
    -   Aller dans `Fichier` > `Préférences` et ajouter cette URL dans `Gestionnaire de cartes` :
        
        ```
        https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
        ```
        
    -   Aller dans `Tools` > `Board` > `Board Manager` et rechercher `ESP32`
        
    -   Installer la version recommandée (ex : 2.0.3)
        
3.  **Configurer l'ESP32 avec les paramètres suivants** :
    
    -   **Board** : `ESP32-WROOM-DA Module`
        
    -   **Upload Speed** : `921600`
        
    -   **CPU Frequency** : `240MHz (WiFi/BT)`
        
    -   **Flash Frequency** : `80MHz`
        
    -   **Flash Mode** : `QIO`
        
    -   **Flash Size** : `4MB (32Mb)`
        
    -   **Partition Scheme** : `Huge APP (3MB No OTA/1MB SPIFFS)`
        
    -   **Core Debug Level** : `None`
        
    -   **Arduino Runs On** : `Core 1`
        
    -   **Events Run On** : `Core 1`
        
4.  **Installer les bibliothèques nécessaires**
    
    -   Télécharger et copier les fichiers des bibliothèques dans le dossier des bibliothèques Arduino (`Sketchbook location` visible dans `Fichier` > `Préférences`)
        
    -   Modifier le fichier `User_Setup.h` de la bibliothèque `TFT_eSPI` pour l’adapter au contrôleur de l’écran 3.5''
        
5.  **Télécharger et ouvrir le programme sur l’ESP32**
    
    -   Choisir la carte ESP32 et ajuster les paramètres dans l'Arduino IDE
        
    -   Sélectionner le port correct
        
    -   Ouvrir le moniteur série, appuyer sur `BOOT` puis `RESET` pour activer le mode flash
        
    -   Cliquer sur `Téléverser` et attendre la fin du téléchargement
        
    -   Redémarrer l'ESP32 pour exécuter le programme
        

## 3. Architecture du Système

Le système est composé de plusieurs composants clés :

-   **ESP32** : Contrôle le point d'accès Wi-Fi et gère les connexions.
    
-   **Écran TFT** : Affiche les informations du réseau et les statistiques en temps réel.
    
-   **Bibliothèques** : Utilisation des bibliothèques WiFi, TFT_eSPI.
    
-   **Système de stockage des mots de passe** : Une liste de mots de passe aléatoires est préchargée et utilisée dynamiquement.
    

## 4. Fonctionnement

### 4.1 Démarrage du Wi-Fi

1.  L'ESP32 crée un **point d'accès Wi-Fi** avec le SSID prédéfini : `wifi_m2dfs_antoine`.
    
2.  Un mot de passe est choisi aléatoirement à partir d'une liste préchargée.
    
3.  L'écran TFT affiche :
    
    -   Le nom du réseau (SSID)
        
    -   Le mot de passe généré
        
    -   Le nombre total de connexions
        
    -   Le nombre de tentatives de connexion
        
    -   Le nombre d'appareils actuellement connectés
        

### 4.2 Mise à jour des statistiques

-   L'ESP32 met à jour les données toutes les 3 secondes.
    
-   L'écran est effacé avant chaque mise à jour.
    
-   Les nouvelles connexions sont enregistrées et affichées en temps réel.
    

### 4.3 Système de connexion Wi-Fi

-   L'ESP32 scanne les réseaux Wi-Fi environnants.
    
-   Il tente de se connecter aux réseaux qui commencent par "wifi_m2dfs_" en testant plusieurs mots de passe.
    
-   Si la connexion réussit, le réseau est ajouté à la liste des connexions validées et affiché sur l'écran TFT.
    

#  Documentation Fonctionnelle

## 1. Connexion au Wi-Fi

1.  **Allumer l'ESP32** : Il génère un **point d'accès Wi-Fi** et affiche ses informations sur l'écran TFT.
    
2.  **Rechercher le réseau Wi-Fi** : Chercher un réseau nommé `wifi_m2dfs_antoine`.
    
3.  **Entrer le mot de passe** affiché sur l'écran TFT.
    
4.  **Se connecter** et utiliser le réseau Wi-Fi.
    

## 2. Fonctionnalités

-   **Création automatique d’un point d’accès Wi-Fi** :
    
    ```
    const char *ssid = "wifi_m2dfs_antoine";
    String password = "randomPassword";
    WiFi.softAP(ssid, password.c_str());
    Serial.println("Point d'accès WiFi démarré");
    ```
    
-   **Génération dynamique d’un mot de passe** :
    
    ```
    std::vector<String> passwords = {"abc123", "xyz789", "pass2024"};
    password = passwords[rand() % passwords.size()];
    ```
    
-   **Affichage en temps réel des statistiques sur un écran TFT** :
    
    ```
    tft.fillScreen(TFT_BLACK);
    tft.drawString("WiFi actif", 10, 10);
    tft.drawString("SSID: " + String(ssid), 10, 40);
    tft.drawString("Password: " + password, 10, 70);
    ```
    

## 3. Instructions de Test

### 3.1 Test de base

1.  Démarrer l'ESP32 et vérifier l'affichage sur l'écran TFT.
    
2.  Chercher et se connecter au **Wi-Fi** généré.
    
3.  Vérifier que le **nombre de connexions** affiché augmente correctement.
    
4.  Redémarrer l’ESP32 et s’assurer qu’un **nouveau mot de passe** est généré.
    

### 3.2 Test avancé

1.  Tester la détection des **réseaux Wi-Fi environnants**.
    
2.  S'assurer que l'ESP32 tente de se connecter à ces réseaux.
    
3.  Vérifier la réussite des connexions et l'affichage sur l'écran TFT.
    

## 7. Fonctionnalités Bonus

-   **Affichage graphique avancé sur l'écran TFT**.
    
-   **Possibilité d'ajouter une interface Bluetooth** pour la gestion des connexions.
    
-   **Enregistrement des statistiques sur une carte SD** pour un suivi plus poussé.