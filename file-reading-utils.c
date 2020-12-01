#include "file-reading-utils.h"

char *get_by_ref(int ref) {
    FILE *fp;
    char *line_cpy, *line = NULL;
    size_t len = 0;

    //ouverture du fichier
    if ((fp = fopen("fichier-exemple.txt", "r")) == NULL)
        return "Couldn't open file";

    //on parcours ligne par ligne le fichier
    while ((getline(&line, &len, fp)) != -1) {
        //on fait une copie de la ligne car strtok() modifie la chaine de caractères
        line_cpy = strdup(line);
        if (atoi(strtok(line, "#")) == ref)
            return line_cpy;
    }

    free(line);
    return "Reference not found\n";
}

void get_by_keyword(char **book_list, char *keyword) {
    FILE *fp;
    char buffer[1024], *line = NULL;
    size_t len = 0;

    //ouverture du fichier
    if ((fp = fopen("fichier-exemple.txt", "r")) == NULL) {
        *book_list = "Couldn't open file";
        return;
    }

    //on remplis le buffer de 0
    bzero(buffer, sizeof(buffer));

    //on parcours ligne par ligne le fichier
    while ((getline(&line, &len, fp)) != -1) {
        //si le mot clé se trouve dans la ligne, on le concatène au buffer
        if (strstr(line, keyword)) {
            strcat(buffer, line);
        }
    }

    free(line);

    //si le buffer est vide
    if (buffer[0] == '\0')
        *book_list = "Keyword not found\n";
    else
        *book_list = strdup(buffer); //copie de buffer car il s'agit d'une variable locale

    return;
}
