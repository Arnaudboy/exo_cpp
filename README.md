# Record reader

## L'enregistreur de mesures

Le but de ce kata est de faire un code capable de lire et écrire un fichier binaire avec un format inventé pour l'occasion.

Le format vise à enregistrer un scénario de capture de mesures dans le temps.

Chaque mesure est enregistrée avec une valeur et un horodatage (timestamp).

L'horodatage indique pour chaque mesure le temps passé entre le début de l'enregistrement et le moment où la mesure est capturée.

Ce qui donne le format d'une (seule) mesure dans le fichier est:

```
VALEUR;HORODATAGE
```

La valeur est un entier naturel signé car on suppose que l'unité de mesure peut être rendue aussi petite que la précision du capteur.

Par exemple si on a un capteur de température mesurant la température au dixième de degrés près on enregistrera 241 au lieu de 24,1.

Pour permettre de naviguer plus efficacement dans le fichier lors de la lecture, le format de fichier divise les mesures en sections de taille SECTION_SIZE (dans le code on definit `#define SECTION_SIZE 3`).

Chaque section a un en-tête de section qui indique la durée de la section et le nombre de mesures dans la section.

L'en-tête de section ressemble à ce qui suit:

```
SECTION_DURATION, NB_RECORD
```

Toutes les sections sont "pleines" sauf la dernière qui peut ne pas l'être dans le cas où le nombre total de mesures n'est pas un multiple de `SECTION_SIZE`.

Le fichier dispose également d'un en-tête global qui indique le nombre total de mesures et la durée totale de l'enregistrement.

Au global le format de fichier ressemble à ceci:

```
# HEADER
TOTAL_DURATION,TOTAL_SIZE
# SECTION_HEADER
SECTION_DURATION, NB_RECORD
# SECTION DATA
    241;1
    242:2
    241:3...until section size
SECTION_DURATION, NB_RECORD
    251;4
    252:5
    248:6...until section size
# ...REPEAT AS MANY SECTIONS AS WE NEED TO RECORD THE ENTIRE MEASUREMENT SCENARIO
```

Vous trouverez dans le fichier `RecordStruct.h` les définitions de structures que l'on utilise dans ce projet.

## Ce qui est fourni

Le projet contient les fichiers suivants:

- `./cmd_ops.h` une librairie qui permet de lire les arguments de la ligne de commande
- `./main.cpp` lecture des options de la ligne de commande pour savoir si on enregistre ou bien lit un fichier de mesures
- `./FileRecorder.cpp` implémentation de la classe qui permet d'enregistrer les mesures - c'est à vous d'implémenter les méthodes
- `./FileReader.cpp` implémentation de la classe qui permet de lire les mesures - c'est à vous d'implémenter les méthodes
- `./reader.cpp` utilise le `FileReader` pour "rejouer" le scénario de mesures, reaffiche les mesures au même rythme qu'elles ont été enregistrées
- `./recorder.cpp` lance deux threads d'execution: un pour attendre en ligne de commande vos "mesures" un autre qui utilise `FileRecorder` pour enregister

## Votre mission

En première partie vous devez implémenter le code qui lit/écrit le fichier binaire respetant le format expliqué plus haut.

En bonus, il serait très apprécié de faire un code capable de sauter dans le temps lors de la relecture d'un scénario.

Par exemle, si on souhaite lire le fichier depuis la moitié du temps écoulé lors de la mesure on peut lancer la commande suivante:

```sh
./record_reader -p 1 -f nom_du_fichier -j 50 
```

(L'option `-j` "jump" permet de sauter à un endroit voulu défini en pourcentage, dans le fichier)

Dans `FileRecorder.cpp` vous devez mettre à jour l'implémentation des méthodes suivantes:
 - `record_main_header`: enregistre l'en tête du fichier de mesures en début de fichier
 - `record_section_header`: enregistre l'en tête d'une des sections du fichier
 - `record_section_data`: enregistre les données dans le fichier

Dans `FileReader.cpp` vous devez mettre à jour l'implémentation des méthodes suivantes:
- `jump_to_percent` permet de sauter jusqu'à un pourcentage du fichier du point de vue de temps passé. NB: ce qui compte ce n'est pas le nombre de mesures mais leur horodatage.
- `jump_to_section` permet de lire le fichier de mesures à partir d'une section donnée
- `read_measurement` renvoie la mesure suivante du fichier à partir de la dernière lecture. Cette mesure utiliser un buffer `_section_buffer` qui stocke toutes les mesures d'une section. La variable `_measurement_pointer` indique quelle est la mesure suivante que l'on doit lire dans le buffer. Cette logique est implémentée dans la fonction `read_measurement` fournie.
- `read_next_section` permet de lire la section suivante du fichier et de stocker dans le buffer `_section_buffer`
- `read_section_header` permet de lire le header d'une section du fichier
- `read_main_header` permet de lire le header principal

### Indices

Vous n'êtes pas obligé, mais pouvez regarder si ces fonctions de la librairie standard peuvent être utiles:

- [seekg](https://cplusplus.com/reference/istream/istream/seekg/)
- [seekp](https://cplusplus.com/reference/ostream/ostream/seekp/)
- [tellp](https://cplusplus.com/reference/ostream/ostream/tellp/)
- [tellg](https://cplusplus.com/reference/istream/istream/tellg/)

### Règles du jeu

- Vous devez lire et comprendre la logique du code existant ça fait partie du kata.
- Vous êtes libre d'ajouter des méthodes privées dans les classes `FileReader` et `FileRecorder` si vous le jugez nécessaire.
- Si vous sentez que le code sera plus organisé/facile en modifiant la structure proposée, vous pouvez proposer une alternative. Il est cependant de votre responsabilité de livrer un code __lisible__ (au moins mieux que ce qui existe en l'état).


## How to build

```sh
cmake .
cmake --build .
```

For debug build:
```sh
cmake -DCMAKE_BUILD_TYPE=Debug .
cmake --build .
```

## Build and run

To record data
```sh
cmake --build . && ./record_reader -r 1 -f record_file_name
```

To replay record
```sh
cmake --build . && ./record_reader -p 1 -j 0 -f record_file_name
```
