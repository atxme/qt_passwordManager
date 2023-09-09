# Nom de l'exécutable
EXECUTABLE = Pegasus.exe

# Dossiers source et inclusion
SRC_DIR = /home/kali/qt_passwordManager/src
OBJ_DIR = /home/kali/qt_passwordManager/src
INCLUDE_DIR = .

# Compilateur et options de compilation
CC = g++
CFLAGS = -I$(INCLUDE_DIR)
LDFLAGS = -lssl -lcrypto -lsqlite3

# Liste des fichiers source (ajoutez tous vos fichiers source ici)
SOURCES = $(wildcard $(SRC_DIR)/*.c) $(wildcard $(SRC_DIR)/*.cpp)

# Liste des fichiers objet générés à partir des fichiers source
OBJECTS = $(SOURCES:.c=.o)
OBJECTS := $(OBJECTS:.cpp=.o)

all: $(EXECUTABLE) post_build

post_build:
	rm -f $(SRC_DIR)/*.o

# Règle de création de l'exécutable
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXECUTABLE) $(LDFLAGS)
	@echo "L'exécutable $(EXECUTABLE) a été créé avec succès."

# Règle de compilation des fichiers source en fichiers objet
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c $< -o $@ $(CFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) -c $< -o $@ $(CFLAGS)

# Nettoyage des fichiers objets et de l'exécutable
clean:
	rm -f $(SRC_DIR)/*.o $(EXECUTABLE)


