#include "include/config.hpp"

#define DATABASE_PATH ABSOLUTE_PATH "database.db"

void createTables(sqlite3 *db) {
    const char *createTableSQL = "CREATE TABLE IF NOT EXISTS users ("
                              "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                              "username TEXT NOT NULL,"
                              "password TEXT NOT NULL,"
                              "link TEXT NOT NULL);";


    int rc = sqlite3_exec(db, createTableSQL, 0, 0, 0);
    if (rc != SQLITE_OK) {
        std::cerr << "Erreur lors de la création de la table : " << sqlite3_errmsg(db) << std::endl;
    } 
}

extern bool haveSession = false;

void verifyFileExistance() {
    isRegistered = true;
    // Vérifiez si le répertoire ABSOLUTE_PATH existe, sinon, créez-le
    struct stat dirStat;
    if (stat(ABSOLUTE_PATH, &dirStat) != 0) {
        const mode_t permissions = S_IRUSR | S_IWUSR | S_IXUSR;
        if (mkdir(ABSOLUTE_PATH, permissions) == -1) {
            std::cerr << "Erreur lors de la création du répertoire : " << strerror(errno) << std::endl;
            return;
        }
        std::cout << "Répertoire créé avec succès : " << ABSOLUTE_PATH << std::endl;
    }

    // Vérifiez si le fichier DATABASE_PATH existe, sinon, créez-le
    std::ifstream file(DATABASE_PATH);
    if (!file) {
        //std::cout << "Fichier de base de données introuvable, création en cours" << std::endl;
        sqlite3 *db;
        int rc = sqlite3_open(DATABASE_PATH, &db);
        if (rc) {
            std::cerr << "Erreur lors de la création du fichier de base de données : " << sqlite3_errmsg(db) << std::endl;
            sqlite3_close(db);
            return;
        }

        createTables(db);

        sqlite3_close(db);
    }
    file.close();

    // Vérifiez si le fichier de sel (salt.bin) existe, sinon, créez-le
    std::ifstream saltFile(ABSOLUTE_PATH "salt.bin");
    if (!saltFile) {
        //std::cout << "Fichier de salt introuvable, création en cours" << std::endl;
        unsigned char salt[16];
        generateSalt(salt);

        std::ofstream saltOut(ABSOLUTE_PATH "salt.bin", std::ios::out | std::ios::binary);
        if (saltOut) {
            saltOut.write(reinterpret_cast<const char *>(salt), sizeof(salt));
            saltOut.close();

            // Modifier les permissions du fichier salt.bin à 0600
            chmod(ABSOLUTE_PATH "salt.bin", S_IRUSR | S_IWUSR);
        } else {
            std::cerr << "Erreur lors de la création du fichier de salt (salt.bin)" << std::endl;
        }
    }
    saltFile.close();

    std::ifstream hashFile(ABSOLUTE_PATH "hash.bin");
    if(!hashFile){
        isRegistered = false;
    }
    
}