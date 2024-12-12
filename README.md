ESP32 WiFi Scanner and Connector
Description
Ce projet utilise un ESP32 équipé d'un écran TFT tactile pour scanner, afficher et tenter de se connecter à des réseaux WiFi spécifiques. Les principales fonctionnalités incluent :

Le scan continu des réseaux WiFi disponibles, filtrant uniquement ceux dont le SSID commence par wifi_m2dfs_.
L'affichage des réseaux filtrés sur un écran tactile.
La tentative de connexion à ces réseaux en utilisant des mots de passe aléatoires provenant d'une liste prédéfinie.
La possibilité de naviguer entre deux écrans :
Un écran principal montrant les instructions générales.
Un écran de scan listant les réseaux filtrés.
Fonctionnalités
Scan continu en arrière-plan : Les réseaux WiFi sont scannés toutes les 10 secondes même lorsque l'utilisateur est sur l'écran principal.
Filtrage des réseaux : Seuls les SSID commençant par wifi_m2dfs_ sont pris en compte.
Connexion automatique : Chaque réseau est testé avec un mot de passe aléatoire de la liste jusqu'à trouver une combinaison valide.
Écran tactile : L'utilisateur peut naviguer entre les écrans avec des interactions tactiles.
