#include <WiFi.h>
#include "TFT_eSPI.h"
#include "esp_wifi.h"
#include <vector>
#include <cstdlib>
#include <ctime>
#include <map>

// Charger le dictionnaire de mots de passe
std::vector<String> passwords = {
    "kxzvaajn", "nulebozl", "sylmukyv", "fzxhzodw", "mnhfwylw", "xhnvtwdd", "jrulvtsp", "ochdamfa", "plaluafr", "ovnkkilc", "oizuxgas",
    "tjrdbrav", "pklzocye", "vtapalxp", "rtagjdnv", "frvuyeoe", "xghgxava", "rgolvnna", "yfxeqlny", "aewazmzb", "shsiejiu", "iuvmohlb",
    "cjvxdfsl", "qpikserq", "utacmzqp", "ooxevhdn", "sxthtwxj", "llrwdkcx", "ddudifar", "zphrewdm", "mpdkrxev", "updhshlc", "tllzdwkl",
    "riyypzdz", "oemqckth", "tffobpzd", "gtffxvfi", "txjtdwqy", "jfiqxhca", "sgypghxn", "juixjinr", "aowreqdu", "swjeitqz", "igjautba",
    "aiyqeglh", "oswyyore", "xptdcttd", "pcprabke", "kmqzndiw", "dpqzamtm", "dyzcslls", "navqcfvd", "anowzypk", "suahetlj", "yfhccclh",
    "qbmdivqr", "qblucjky", "dguzkxel", "ylpyzgjf", "kbfcdujc", "ctuqkrdr", "ajevmebu", "skmcsqfz", "ffzlwiio", "aftglkvf", "xxdgtzlw",
    "cvlfsunn", "bhdvmvku", "nfqzutau", "ggxbhrho", "qyfalgmh", "tcxkbhrn", "qpogobwf", "cllvemtd", "vthbmclx", "mygnkqqa", "whfrjrlf",
    "gwfeqkgf", "fkvgcrjw", "lezddzaj", "xvczgqaa", "iekrpdyj", "ebkftonw", "eoudylkl", "ebxoxxfm", "janmvljn", "xfudqipe", "yuyfhefw",
    "irxkfjbq", "kwdvygqs", "yvhsdunb", "qhqzskuo", "nkcuqjjx", "treyrebr", "iubgkumg", "iqeblqrp", "awllfhdq", "ywbvnsvp", "eqivzdwz",
    "feskhshm", "fytbaolk", "lnqguosc", "mplxzpwm", "vsedeanw", "rvcnmtqq", "fsptuhkw", "pmjuvole", "fxwnbwxl", "rnkilsjh", "uqpkhdhl",
    "pqzxizwu", "lfgpxewt", "wbsvxglw", "adkfvisy", "moslxmvo", "caiongpk", "mdfldeyy", "fsacirgb", "seosmnio", "zjafdgsg", "xjhbsqbr",
    "emjtockn", "ymajxpiu", "nijknifq", "hbkawhxp", "dxpygtws", "pkkkfsqc", "yfgydefu", "ocnbsmfu", "hqxndvfm", "mpzxmzmc", "ynbxnvcb",
    "yvmmciwz", "nycmnnfk", "foitwmhx", "wslaloll", "obsotlwf", "qcejxtcc", "jytvottv", "yhgoeojj", "nqvxubel", "zekupkif", "iuxizxgu",
    "ksfdaahq", "rfdyxagk", "aodiivkl", "nbomkfft", "gzjubrrt", "xkzfllsy", "zwnghvsp", "lbdnrijp", "tztubmsb", "tmnfepsp", "dvhmvvak",
    "qldcakpn", "gaykchmm", "viulztoy", "rgppnxmt", "gxhxirmi", "nkrhfpvl", "cnuvswpy", "qitnplny", "wrjcumvb", "fmksndas", "cwotawuz",
    "lzkfjavt", "deshqjlv", "hfcrpmqj", "stgtdpmk", "nealxloj", "pqwygeet", "uuvqbkxw", "gyxuyioo", "rqkuzleq", "czuaaire", "pyxcaiqc",
    "exxysayd", "jajnriij", "nchoqudf", "uzexsnfm", "jajuagec", "rnutbptb", "ogvbnogl", "emwuzqan", "kcgzpxfo", "fhgjzrxe", "nrncgdsx",
    "cywwydif", "lyuzcaqv", "iwzzufyn", "qbfjmlfi", "tdnsshfc", "kssrcayx", "ifykxdys", "bazausxn", "wahbnioc", "jvnqahyb", "jngbwrke",
    "nobrqqda", "qjoehftw",

    // Ajoutez ici le reste des mots de passe fournis dans le fichier passwords.txt
};

// Définir les informations du point d'accès
const char *ssid = "wifi_m2dfs_antoine";
String password = "";

// Variables pour stocker le nombre de connexions et les adresses MAC des clients
int totalConnections = 0;
int totalConnectionsEver = 0;
int totalAttempts = 0;
int currentConnections = 0;
String connectedMACs = "";
std::map<String, int> macAttempts;   // Compter le nombre de tentatives par adresse MAC
std::map<String, bool> macConnected; // Statut de connexion pour chaque adresse MAC

// Initialiser l'écran TFT
TFT_eSPI tft = TFT_eSPI();
bool onMainScreen = true;

uint16_t x = 0 , y = 0; 

void setup()
{
  Serial.begin(115200);
  srand(esp_random());

  // Sélectionner un mot de passe au hasard
  password = passwords[rand() % passwords.size()];

  // Initialiser l'écran TFT
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  // Démarrer le point d'accès Wi-Fi avec le mot de passe aléatoire
  WiFi.softAP(ssid, password.c_str());
  Serial.println("Point d'accès WiFi démarré");

  // Afficher l'écran principal
  displayMainScreen();
}

void displayMainScreen()
{
  tft.fillScreen(TFT_BLACK);
  tft.drawString("WiFi démarré", 10, 10);

  // Afficher l'adresse MAC de l'ESP32
  String macAddress = WiFi.softAPmacAddress();
  tft.drawString("MAC ESP: " + macAddress, 10, 40);

  // Afficher le mot de passe choisi
  tft.drawString("Password: " + password, 10, 60);

  // Afficher le nombre initial de connexions
  tft.drawString("Total connexions: " + String(totalConnections), 10, 90);
  tft.drawString("Total connexions ever: " + String(totalConnectionsEver), 10, 120);
  tft.drawString("Total tentatives: " + String(totalAttempts), 10, 150);
  tft.drawString("Clients actuels: " + String(currentConnections), 10, 180);
}

void scanAndAttemptConnection()
{
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.drawString("Scanning WiFi...", 10, 10);

  // Scanner les réseaux Wi-Fi
  int n = WiFi.scanNetworks();
  if (n == 0)
  {
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.drawString("No WiFi networks found!", 10, 40);
    Serial.println("No WiFi networks found!");
  }
  else
  {
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    Serial.println("WiFi networks found:");
    std::vector<String> filteredSSIDs;

    for (int i = 0; i < n; i++)
    {
      String ssid = WiFi.SSID(i);
      if (ssid.startsWith("wifi_m2dfs_")) {
        filteredSSIDs.push_back(ssid);
      }
    }

    if (filteredSSIDs.empty()) {
      tft.setTextColor(TFT_RED, TFT_BLACK);
      tft.drawString("No matching WiFi networks!", 10, 40);
      Serial.println("No matching WiFi networks!");
      return;
    }

    // Afficher les réseaux filtrés
    for (size_t i = 0; i < filteredSSIDs.size() && i < 10; i++)
    {
      tft.drawString(String(i + 1) + ": " + filteredSSIDs[i], 10, 40 + (i * 20));
    }

    // Essayer de se connecter à chaque réseau avec un mot de passe sélectionné aléatoirement
    while (!passwords.empty()) {
      String randomPassword = passwords[rand() % passwords.size()];
      Serial.println("Trying password: " + randomPassword);
      Serial.println("-------------------------------");

      for (const String& ssid : filteredSSIDs) {
        Serial.println("Trying to connect to: " + ssid);
        Serial.println("-------------------------------");
        if (WiFi.status() == WL_CONNECTED)
        {
          WiFi.disconnect();
          delay(100);
        }

        WiFi.begin(ssid.c_str(), randomPassword.c_str());

        // Attendre pour voir si la connexion réussit
        int attempts = 5;
        while (WiFi.status() != WL_CONNECTED && attempts > 0)
        {
          delay(1000);
          attempts--;
        }

        if (WiFi.status() == WL_CONNECTED)
        {
          Serial.println("Connected to: " + ssid + " with password: " + randomPassword);
          tft.fillScreen(TFT_BLACK);
          tft.setTextColor(TFT_GREEN, TFT_BLACK);
          tft.drawString("Connected to:", 10, 10);
          tft.drawString(ssid, 10, 40);
          tft.drawString("Password:", 10, 70);
          tft.drawString(randomPassword, 10, 100);

          // Déconnexion avant de passer au réseau suivant
          WiFi.disconnect();
          delay(1000);
          return;
        }
        else
        {
          Serial.println("Failed to connect to: " + ssid);
          Serial.println("-------------------------------");
        }
      }

      // Retirer le mot de passe essayé pour éviter de le réutiliser
      passwords.erase(std::remove(passwords.begin(), passwords.end(), randomPassword), passwords.end());
    }
  }

  // Nettoyer les réseaux Wi-Fi scannés pour la prochaine utilisation
  WiFi.scanDelete();
}

void loop()
{
  // Gérer les changements d'écran
  if (tft.getTouch(&x, &y)) // Vérifier si l'écran tactile est pressé
  {
    if (onMainScreen)
    {
      onMainScreen = false;
      scanAndAttemptConnection();
    }
    else
    {
      onMainScreen = true;
      displayMainScreen();
    }
    delay(300); // Éviter les appuis multiples rapides
  }

  if (onMainScreen)
  {
    // Code pour la mise à jour de l'écran principal
    int numClients = WiFi.softAPgetStationNum();
    if (numClients != totalConnections)
    {
      totalConnections = numClients;
      currentConnections = numClients;
      if (numClients > totalConnectionsEver)
      {
        totalConnectionsEver = numClients;
      }
      connectedMACs = "";

      // Mettre à jour les informations sur l'écran principal
      tft.fillRect(10, 90, 220, 20, TFT_BLACK); // Effacer la zone avant de réécrire
      tft.drawString("Total connexions: " + String(totalConnections), 10, 90);
      tft.fillRect(10, 120, 220, 20, TFT_BLACK); // Effacer la zone avant de réécrire
      tft.drawString("Total connexions ever: " + String(totalConnectionsEver), 10, 120);
      tft.fillRect(10, 150, 220, 20, TFT_BLACK); // Effacer la zone avant de réécrire
      tft.drawString("Total tentatives: " + String(totalAttempts), 10, 150);
      tft.fillRect(10, 180, 220, 20, TFT_BLACK); // Effacer la zone avant de réécrire
      tft.drawString("Clients actuels: " + String(currentConnections), 10, 180);
    }
  }
}
