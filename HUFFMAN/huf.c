#include <stdio.h>



    /**
        * Structure d'un noeud d'arbre.
    */
typedef struct Noeud {
    int pere, /**< indice du pere*/ fg, /**< indice du fils gauche*/ fd; /**< indice du fils droite */
    double freq; /**< frequence d'un caractere dans le fichier source */
    char code; /**< un bit, 0 ou 1 */
} noeud;

    /**
        * Structure qui stocke un code.
    */
typedef struct Code {
    char code[256]; /**< tableau des chars pour stocker le code. 256 chars maximum. */
    char num; /**<nombre des bits utilises pour stocker ce code binaire */
} stockCode;


    /**
        * Initialisation d'un arbre.
        * Parcours de tableau de structures noeud, de 0 a stop - 1.
        * Les membres pere, fg et fd sont intialises a -1.
        * Le membre freq est initialise a 0.0.
        * @param arbre le tableau de structures noeud - l'arbre a traiter.
        * @param stop l'indice minimale qui ne va pas etre traite.
    */
    
void initArbre(noeud arbre[], int stop){

    for(int i=0;i<stop;i++){
        arbre[i].freq = 0.0;
        arbre[i].pere = -1;
        arbre[i].fg = -1;
        arbre[i].fd = -1;
    }
}


    /**
        * Initialisation du tableau des codes.
        * Parcours de tableau de structures stockCode, de 0 a stop - 1.
        * Le membre num est initialise a 0.
        * Le premier element du membre code est initialise a '0'.
        * @param codes le tableau de structures stockCode a traiter.
        * @param stop l'indice minimale qui ne va pas etre traite.
    */
void initCode(stockCode codes[], int stop){

    for(int i=0;i<stop;i++){
        codes[i].num = 0;
        codes[i].code[0] = '0';
    }
}


    /**
        * Calcule les frequences a partir de nombre d'occurences.
        * Parcours de tableau de structures noeud, en divisant son nombre d'occurences (stocke dans membre freq), par nombre total de caracteres.
        * @param arbre le tableau de structures noeud - l'arbre.
        * @param total nombre total de caracteres.
    */
void occur2Freq(noeud arbre[], int total){
    int i = 0;
    for(i=0;i<256;i++){
        if(arbre[i].freq != 0.0){
            arbre[i].freq = arbre[i].freq / total;
        }
    }
}


    /**
        * Recherche du noeud minimal dans l'arbre.
        * Parcours de tableau de structures noeud, en cherchant le noeud avec la frequence minimale.
        * @param arbre le tableau de structures noeud - l'arbre.
        * @param max indice maximale a regarder.
    */
int minimum(noeud arbre[], int max){
    int indice = -1;
    float freq = 1.0;
    while(max>=0){
        if((arbre[max].pere == -1) && (arbre[max].freq != 0.0) && (arbre[max].freq <= freq)){
            indice = max;
            freq = arbre[max].freq;
        }
        max--;
    }
    return indice;
}


    /**
        * Construction de l'arbre.
        * Parcours de tableau de structures noeud et etablit des liaisons pere-fils. Renvoie l'indice root de l'arbre.
        * @param arbre le tableau de structures noeud - l'arbre.
        * @param indice_nn indice de tableau arbre ou placer le premier noeud genere.
    */
int consArbre(noeud arbre[], int indice_nn){
    /* CONSTRUIT UNE ARBRE */
    
    int minG, minD;
    
    while((minimum(arbre, (indice_nn - 1)) != -1)){
        // FILS GAUCHE
        minG = minimum(arbre, (indice_nn - 1));
        arbre[minG].pere = indice_nn;
        arbre[indice_nn].fg = minG;

        // FILS DROITE
        minD = minimum(arbre, (indice_nn - 1));
        if(minD == -1){
            break;
        }
        arbre[minD].pere = indice_nn;
        arbre[indice_nn].fd = minD;

        // SOMME DES FREQS
        arbre[indice_nn].freq = arbre[minG].freq + arbre[minD].freq;
        
        indice_nn++;
    }
    
    return (indice_nn -1);
}


    /**
        * Construction recursive des codes.
        * Parcours de tableau de structures noeud en attribuant 0 a fils gauche et 1 a fils droit.
        * @param arbre le tableau de structure noeud - l'arbre.
        * @param code le tableau de structure stockCode pour stocker des codes.
        * @param indice indice root de l'arbre.
        * @param profondeur de la reccurence - pour savoir le longeur du code genere.
    */
void genereCodes(noeud arbre[], stockCode codes[], int indice, int profondeur){
    if(arbre[indice].fg == -1 || arbre[indice].fd == -1){ // feuille
        sauvegardeCode(arbre, codes, indice, profondeur);
        //printf("sauvegarde: indice %d, profondeur %d", indice, profondeur);
    }else{
        arbre[arbre[indice].fg].code = '0';
        arbre[arbre[indice].fd].code = '1';
        genereCodes(arbre, codes, arbre[indice].fg, profondeur + 1);
        genereCodes(arbre, codes, arbre[indice].fd, profondeur + 1);
    }
}


    /**
        * Sauvegarder un code.
        * Parcours de l'arbre dans le sens de feuille, en suvagardant des 0 et 1 dans le tableau des structures stockCode.
        * @param arbre le tableau de structure noeud - l'arbre.
        * @param code le tableau de structure stockCode pour stocker des codes.
        * @param indice feuille de l'arbre.
        * @param profondeur de la reccurence - le longeur du code genere.
    */
void sauvegardeCode(noeud arbre[], stockCode codes[], int indice, int profondeur){

    int i = profondeur - 1, j = indice;
    
    // le cas d'un seul caractere:
    if(profondeur == 0){
        codes[arbre[indice + 1].fg].code[0] = '0';
        codes[arbre[indice + 1].fg].num = 1;
    }else{
    
        // suite des bits dans l'orde inverse
        while (arbre[j].pere != -1 && i >= 0)
        {
            codes[indice].code[i] = arbre[j].code;
            j = arbre[j].pere;
            i--;
        }
    
        // nombre des bits utilises - profondeur de la reccurence
        codes[indice].num = profondeur;
    }
}

    /**
        * Ecrire 0 ou 1 dans le buffeur.
        * Parcours de l'arbre en sauvegardant des 0 et 1 dans le tableau de structure stockCode.
        * @param file_output le pointeur vers un fichier ouvert pour l'ecriture.
        * @param glob_buff le buffeur a ecrire dans le fichier.
        * @param glob_counter stocke le nombre des "bits" ecrits dans buffeur.
        * @param x 0 ou 1 a ecrire dans le buffeur.
    */
void ecritUnBit(FILE *file_output, unsigned char *glob_buff, int *glob_counter, int x){
    
    *glob_buff *= 2;
    if(x==1){ 
        *glob_buff += 1; 
    }
    *(glob_counter) += 1;
    //printf("%d",x);
    
    if(*(glob_counter) == 8){
        fputc(*glob_buff, file_output);
        *(glob_counter) = 0;
    }
}


    /**
        * Ecrit la partie d'Entete dans le fichier compresse.
        * On fait la chose suivante:
            *    - sur un char on stocke le nombre total de codes outilises,
            *    - pour chaque caractere qui apparait dans le fichier de source:
            *         -# sur un char on stocke le caractere lui meme,
            *         -# sur un char on stocke un certain nombre X des bits outilises pour stocker le code binaire du caractere,
            *         -# sur X chars on stocke le code binaire du caractere (on ajoute des zeros au debut).
        * @param file_output le pointeur vers un fichier ouvert pour l'ecriture.
        * @param codes le tableau de structure stockCode ou sont stockes des codes.
        * @param glob_buff le buffeur a ecrire dans le fichier.
        * @param glob_counter stocke le nombre des "bits" ecrits dans buffeur.
    */
int sauvegardeHeader(FILE * file_output, stockCode codes[], unsigned char *glob_buff, int *glob_counter){

    int reste = 0, ecrit = 8;
    
    printf("%d codes sauvegardes. \n", fputc(compteCodes(codes) - 1, file_output) + 1);      // le premier octet stocke nombre des codes
    
    for(int j=0; j<256; j++){
        if(codes[j].num != 0){
        
            fputc(j, file_output);         // stocke le caractere
            fputc(codes[j].num, file_output); // stocke nombre des bits pris par le code
            ecrit += 16;
            
            reste = 0;                    
            while (reste < codes[j].num){ reste += 8; }    // compter combien de zeros ajouter au debut
            reste -= codes[j].num;
            
            //printf("%c[%d]: \t[", j, codes[j].num);
            
            for (int h = 0; h < reste; h++){
                ecritUnBit(file_output, glob_buff, glob_counter, 0);
                //printf("0");
                ecrit++;
            }
            
            //printf("]");

            for (int i = 0; i < codes[j].num; i++){
                ecritUnBit(file_output, glob_buff, glob_counter, (codes[j].code[i] - 48));
                //printf("%d", codes[j].code[i] - 48);
                ecrit++;
            }
            //printf("\n");
        }
    }
    return ecrit/8;
}

    /**
        * Ecrit la partie binaire du fichier compresse.
        * Parcours de fichier de source, et pour chaque caractere, on stocke le code correspandant dans le fichier destination
        * @param file_input le pointeur vers le fichier source.
        * @param file_output le pointeur vers le fichier destination.
        * @param codes le tableau de structure stockCode ou sont stockes des codes.
        * @param glob_buff le buffeur a ecrire dans le fichier.
        * @param glob_counter stocke le nombre des "bits" ecrits dans buffeur.
    */
int encode(FILE * file_input, FILE * file_output, stockCode codes[], unsigned char *glob_buff, int *glob_counter){
    /* lit le source, et encode dans la destination */
    
    int i, ecrit = 0;
    unsigned char reste = 0, buff[2], car;
    
    rewind(file_input);
    
    while(fread(buff, sizeof(char), 1, file_input) > 0){
        
        car = buff[0];
        
        for (i = 0; i < codes[car].num; i++)
        {
            ecritUnBit(file_output, glob_buff, glob_counter, (codes[car].code[i] - 48));
        
            ecrit++;
        }
    }
    
    // pour les bits restant
    if (*(glob_counter) > 0){
        reste = 8 - *(glob_counter);
        for(int i=0;i<reste;i++)
        {
            ecritUnBit(file_output, glob_buff, glob_counter, 0);
            ecrit++;
        }
    }
    // sauvegarder dans le dernier octet nombre des zeros ajoutes
    fputc(reste, file_output);
    
    return ecrit/8 + 1;
}


    /**
        * Compte des codes.
        * Parcours du tableau des structures stockCode, en comptant des codes generes precedament.
        * @param codes le tableau de structure stockCode ou sont stockes des codes.
    */

int compteCodes(stockCode codes[]){
    int occur = 0;
    for(int i=0;i<256;i++){
        if(codes[i].num != 0){occur++;}
    }
    return occur;
}

    /**
        * Compte la longeur moyenne des codes.
        * Parcours du tableau de structure stockCode, en comptant la moyenne de la longeur des codes.
        * @param codes le tableau de structure stockCode ou sont stockes des codes.
    */

float moyCodes(stockCode codes[]){
    int occur = 0;
    float moy = 0.0;
    for(int i=0;i<256;i++){
        if(codes[i].num != 0){
            moy += codes[i].num;
            occur++;
        }
    }
    return moy/occur;
}


    /**
        * Affiche un arbre.
        * Affiche un tableau des structures noeud, de 0 a max, sur la sortie standard.
        * @param arbre le tableau des structures noeud a afficher.
        * @param max indice maximale a afficher.
    */
void printTArbre(noeud arbre[], int max){
    printf("\n ARBRE :\n\n");
    for(int i=0;i<=max;i++){ 
        if(arbre[i].freq != 0.0){
            if(i<=126){ 
                printf("%d) [%c] pere:%d, fg:%d, fd:%d, frequence:%f, code: %c\n", i, i, arbre[i].pere, arbre[i].fg, arbre[i].fd, arbre[i].freq, arbre[i].code); }
            else{ 
                printf("%d) pere:%d, fg:%d, fd:%d, frequence:%f, code: %c\n", i,  arbre[i].pere, arbre[i].fg, arbre[i].fd, arbre[i].freq, arbre[i].code); }
        }
    }
}


    /**
        * Affiche un tableau des codes.
        * Affiche un tableau de structure stockCode sur la sortie standard.
        * @param code le tableau de structure stockCode ou sont stockes des codes.
    */
void printTCodes(stockCode codes[]){
    printf("\nAffichage des codes:\n\n");
    for(int i=0;i<256;i++){ 
        if(codes[i].num != 0){
            if(i<=126){ printf("%d) [%c] => ", i, i); }
            else{ printf("%d)     => ", i); }
            for(int j=0; j < codes[i].num; j++){
                printf("%c", codes[i].code[j], codes[i].code[j]);
            }
            printf(", code sur %d bits\n", codes[i].num);
        }
    }
}






int main(int n, char **argv){

    // variables
    noeud arbre[512];    // tableau des noeds
    stockCode codes[256];    // tableau des codes binaires pour des caracteres

    unsigned long int total_chars = 0, octets_ecrits = 0;
    
    FILE* file_input, *file_output;
    
    unsigned char buff[3], glob_buff = 0;
    int racine, glob_counter = 0;

    
    
    if(n >= 3){

        printf("\n HUFF.C ");
        
        if(file_input = fopen(argv[1], "r")){
            
            // INITIALISATION DES STRUCTURES
            initArbre(arbre, 512);
            initCode(codes, 256);

            // lire des chars & compter nbs d'occurences
            while(fread(buff, sizeof(char), 1, file_input) > 0){
                arbre[(int)buff[0]].freq++;
                total_chars++;
            }
            
            // calculer la frequences a partir des nombres d'occurences
            occur2Freq(arbre, total_chars); 
            
            // CONTRUCTION DE L'ARBRE
            racine = consArbre(arbre, 256);
            
            
            // GENERATION DES CODES
            genereCodes(arbre, codes, racine, 0);

            printTArbre(arbre, racine); 
            printTCodes(codes);

            printf("\n\nLongeur moyenne de codage: %f\n", moyCodes(codes));
            printf("Encodage.... "); fflush(stdout);
            
            if(file_output = fopen(argv[2], "w")){
                
                octets_ecrits += sauvegardeHeader(file_output, codes, &glob_buff, &glob_counter);
                octets_ecrits += encode(file_input, file_output, codes, &glob_buff, &glob_counter);
                
                    printf("\nTaille originelle : %d (%s); taille compressee : %d (%s); gain : %f%% !\n\n", (int)total_chars, argv[1], (int)octets_ecrits, argv[2], 100 - (octets_ecrits/(float)total_chars)*100);
                    //affiche la taille originelle du fichier compressé, son chemin, la nouvelle taille après compression et le gain total de la compression
                fclose(file_output);
            }else{
                printf("On n'a pas pu creer un fichier '%s'\n", argv[2]);
            }
            
            fclose(file_input);
        }else{
        
            printf("\nError. Le fichier '%s' ne peut pas etre ouvert.\n", argv[1]);
        }

    }else{
        
        printf("Utilisation: %s <fichier source> <fichier destination>\n\n", argv[0]);
    }
    return 0;
}



