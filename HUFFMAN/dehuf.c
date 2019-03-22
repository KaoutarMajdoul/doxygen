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
    char num; /**<nombre des bits outilises pour stocker ce code binaire */
} stockCode;




void lireHeader(FILE * file_input, stockCode codes[]){

    int nombreCodes = (unsigned char)fgetc(file_input);
    int nombreBits, nombreCars, car;
    unsigned char buffer[32];
    
    printf("%d codes trouves.\n", nombreCodes + 1);

    for (int i = 0; i <= nombreCodes; i++){

        nombreCars = 1;
        car = fgetc(file_input);        // lit le caractere
        nombreBits = fgetc(file_input);        // lit nbre des bits utilises pour l'encoder    
        codes[car].num = nombreBits;        // et stocke dans le tableau des codes

        while (nombreCars * 8 < nombreBits){    // trouve le nombre des chars utilises
            nombreCars += 1; 
        }
        
        for (int j = 0; j < nombreCars; j++){    // lit ces chars dans buffer
            buffer[j] = fgetc(file_input);
        }
        
        for (int h = nombreCars -1; h>=0 ;h--){    //pour tous les chars lit, dans l'ordre inverse
            
            for(int k=0; k<8; k++){

                codes[car].code[(k+(nombreCars - h - 1)*8)] = (char) (buffer[h] % 2 )+ 48;
            //printf("%c[%d] => %c\n", car,(k+(nombreCars - h - 1)*8),(buffer[h] % 2 ) + 48);
                buffer[h] /= 2;
            }
        }
    }

    // inverser l'ordre des codes
    char b[256];
    int in = 0;
    for(int i=0;i<256;i++){ 
        if(codes[i].num != 0){
            for(int j=0; j<codes[i].num; j++){
                b[j] = codes[i].code[codes[i].num - j -1];
            }
            for(int j=0; j<codes[i].num; j++){
                codes[i].code[j] = b[j];
            }
        }
    }
}

    /**
        * Decode la partie binaire d'un fichier compresse.
        * Parcours du tableau de structure stockCode, en comptant des codes generes precedament.
        * @param file_input un pointeur vers le fichier compresse.
        * @param file_output un pointeur vers le fichier de destination.
        * @param codes le tableau de structure stockCode ou sont stockes des codes.
        * @param total_num nombre des caracteres, pour savoir si on est deja a l'avant dernier octet de fichier source.
        * @param printit pour choisir entre affichage a la sortie standard, ou dans un fichier
    */
int decode(FILE * file_input, FILE * file_output, stockCode codes[], int total_num, int printit){
    
    unsigned char buffer[264], bf[2];
    
    int deb = 0, fin = 0, octets_ecrits = 0, code = 0;

    while(fread(bf, sizeof(char), 1, file_input) > 0){
    

        for(int b = 7; b >= 0; b--){
            buffer[fin + b] = (bf[0] % 2) + 48;
            bf[0] /= 2;
        }
        fin += 8;
    
        if(ftell(file_input) == total_num - 1){        // si on vient de traiter l'avant dernier
            fread(bf, sizeof(char), 1, file_input);
            fin -= bf[0];                // on saute des zeros a la fin
        }
        
    
        while(trouveCode(codes, buffer, &code, &deb, &fin)){
            //printf("%c, deb:%d, buffer[deb]:%c %d/%d\n", code, deb, buffer[deb], (int)ftell(file_input), total_num);
            if(printit){
                printf("%c", code); fflush(stdout);
            }else{
                fputc(code, file_output);
            }
            octets_ecrits++;
        }
    
        //printf("debug... deb: %d, fin: %d, code: %d, buffer: \n[", deb, fin, code, buffer[0]); fflush(stdout); for(int x = 0; x< fin; x++){ printf("%c", buffer[x]); } printf("]\n");
        
        // decalage
        if(deb > 0){
            for(int i = deb; i < fin; i++){
                buffer[i - deb] = buffer[i];
            }
            fin -= deb;
            deb = 0;
        }
    }
    return octets_ecrits;
}

    /**
        * Cherche un code dans le buffeur.
        * Renvoie 1 et stocke le code ds &code si un code a ete trouve, renvoie 0 sinon.
        * @param codes tableau des codes.
        * @param buffer le buffeur.
        * @param code pointeur pour sauvegarder le caractere trouve .
        * @param deb indice debut de tableau buffeur.
        * @param fin indice fin de tableau buffeur.
    */
int trouveCode(stockCode codes[], unsigned char buffer[], int *code, int *deb, int *fin){

    int trouve, nbr;
    
    for(int i = 0; i < 256; i++){ 
    
        if(codes[i].num != 0){
        
            trouve = 1;
            nbr = i;
            
            for(int j = 0; j < codes[i].num; j++){
            
                if((*(deb) + j) == *(fin) || (codes[i].code[j] != buffer[*(deb) + j])){
                    trouve = 0;
                    break;
                }
            }
            
            if(trouve){ break; }
        }
    }

    if(trouve){
        *deb += codes[nbr].num;
        *code = nbr;
        return 1;
    }else{
        return 0;
    }
}

    /**
        * Affiche un arbre.
        * Affiche un tableau des structures noeud, de 0 a max, sur la sortie standard.
        * @param arbre le tableau des structures noeud a afficher.
        * @param max indice maximale a afficher.
    */
void printTArbre(noeud arbre[], int max){
    printf("\nAffichage de l'arbre:\n\n");
    for(int i=0;i<=max;i++){ 
        if(arbre[i].freq != 0.0){
            if(i<=126){ printf("%d) [%c] pere:%d, fg:%d, fd:%d, frequence:%f, code: %c\n", i, i, arbre[i].pere, arbre[i].fg, arbre[i].fd, arbre[i].freq, arbre[i].code); }
            else{ printf("%d) pere:%d, fg:%d, fd:%d, frequence:%f, code: %c\n", i,  arbre[i].pere, arbre[i].fg, arbre[i].fd, arbre[i].freq, arbre[i].code); }
        }
    }
}


    /**
        * Affiche un tableau des codes.
        * Affiche un tableau de structure stockCode sur la sortie standard.
        * @param codes le tableau de structure stockCode ou sont stockes des codes.
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

    stockCode codes[256];

    unsigned long int total_chars = 0, octets_ecrits = 0;
    unsigned char buff[3];
    
    FILE* file_input, *file_output;

    int ver = 0, ver_pos2 = 8; 

    if(n >= 2){ 
            printf("\n DEHUF.C");
            
            if(file_input = fopen(argv[1], "r")){
    
                initCode(codes, 256);
                
                // calcule la taille du fichier...
                while(fread(buff, sizeof(char), 1, file_input) > 0){ total_chars++; }; rewind(file_input);
                
                printf("\nInitialisation des codes.... ");
                lireHeader(file_input, codes);
                printTCodes(codes);
                
                printf("\nDecodage....\n"); fflush(stdout);
                
                if(n>=3){
                    
                    if(file_output = fopen(argv[2], "w")){
                    
                        octets_ecrits += decode(file_input, file_output, codes, total_chars, 0);
                        printf("\nOK\n\nTaille de fichier decode (%s): %d bytes (%f %%)\n\n", argv[2], (int)octets_ecrits, (octets_ecrits/(float)total_chars)*100);
                        fclose(file_output);
                    }else{
                        printf("On n'a pas pu creer un fichier '%s'\n", argv[2]);
                    }
                }else{
                
                    octets_ecrits += decode(file_input, file_output, codes, total_chars, 1);
                    printf("\nOK\n\nTaille de output: %d bytes (%f %%)\n\n", (int)octets_ecrits, (octets_ecrits/(float)total_chars)*100);
                }
            }else{
                printf("\nError. Le fichier '%s' ne peut pas etre ouvert.\n", argv[1]);
            }
//////////////////////////////////////////////////////////////////////////////////////////////////////////// DECODAGE - FIN
    }else{
        
        printf("Utilisation: %s <fichier source> [<fichier destination>]\n\n", argv[0]);
    }
    return 0;
}

