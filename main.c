#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

#define MOT_DEFAULT "CATAMARAN"
#define NB_TOUR 10

void waitFor (unsigned int secs) {
    unsigned int retTime = time(0) + secs;   // Get finishing time.
    while (time(0) < retTime);               // Loop until it arrives.
} // Fin waitFor

int doRand(int startVal, int endVal){// fonction doRand modifiée
    waitFor(0.05);
    srand(time(NULL)*rand());
    if(startVal == 0){
        return (rand() % (endVal +1));
    }else{
        return (rand() % ((endVal + startVal -1)) + startVal);
    }
} // Fin doRand

void initATrouver(char *mot,int *progress,int motLength){
    int i,j;
    for (i=0;i<motLength;i++){
        progress[i]=1;
    }
}

void printResult(char *mot,int *progress,int motLength){

    int i;

    for (i=0;i<motLength;i++){
        if (progress[i]==0){
            printf("%c",mot[i]);
        }else{
            printf("*");
        }
    }
    printf("\n");

}

int verifLettre(char *mot,int *progress,int motLength,char lettre){// si mauvaise réponse retourne 1
    int i,isGood;
    isGood=1;
    for (i=0;i<motLength;i++){
        if(lettre == mot[i]){
            progress[i]=0;
            isGood=0;
        }
    }
    return isGood;
}

int tourjeu(char *mot,int *progress,int motLength){// si mauvaise reponse retourne 1 (1 essai en moins)
    //Variables
    char lettre=' ';
    //Execution
    printf("Quel est le mot secret ? ");
    printResult(mot,progress,motLength);

    while((int)lettre < 65 || (int)lettre < 97 && (int)lettre > 90 || (int)lettre > 122){// test de validité
        fflush(stdin);
        printf("proposez une lettre : ");
        scanf("%c",&lettre);
    }
    if((int)lettre >= 97 || (int)lettre <= 122){// convertir de min à MAJ
        lettre-=32;
    }

    return verifLettre(mot,progress,motLength,lettre);
}

int toFind(int *progress,int motLength){
    int i,left;
    left=0;
    for (i=0;i<motLength;i++){
        left+=progress[i];
    }
    return left;
}

void crateDicoIfNotExist(){
    FILE* dico = NULL;
    dico = fopen("dico.txt","r");
    if(dico == NULL){
        fclose(dico);
        printf("Creation du fichier dictionnaire ... \n");
        dico = fopen("dico.txt","w");
        fprintf(dico,"%s\n",MOT_DEFAULT);
        printf("done \n\n");
    }
    fclose(dico);
}

int getNbWord(){
    FILE* dico = NULL;
    char mot[30];
    int nbMot = 0;
    dico = fopen("dico.txt","r");
    while (fscanf(dico,"%30s",mot)==1){
        nbMot++;
    }
    return nbMot;
}

char* getWordFromDico(int nb){
    FILE* dico = NULL;
    char mot[30];
    int nbMot = 0;
    int i;
    dico = fopen("dico.txt","r");
    for(i=0;i<nb;i++){
    fscanf(dico,"%30s",mot);
    }
    return mot;
}

int main()
{
    // Declaration des variables
    int motLength,tour;
    FILE* dico = NULL;
    char mot[]=MOT_DEFAULT;

    dico = fopen("dico.txt","r");
    if(dico != NULL){
        strcpy(mot,getWordFromDico(doRand(1,getNbWord())));

    }
    fclose(dico);


    //initialisation des variables
    motLength = strlen(mot);
    int progress[motLength];
    tour = NB_TOUR;

    //Execution !
    printf("Bienvenue dans le jeu du Pendu !\n\n");

    initATrouver(mot,progress,motLength);// Initialisation

    while (toFind(progress,motLength)>0 && tour>0){// Tant que toutes les lettres ne sont pas trouvées
        printf("Il vous reste %d coups a jouer\n",tour);
        tour-=tourjeu(mot,progress,motLength);
        getchar();
        fflush(stdin);
    }

    if (tour>0){
        printf("\nBravo vous avez gagne ! le mot etait bien : ");
    }else{
        printf("\nVous avez perdu ! le mot etait : ");
    }
    printf("%s\n",mot);

    return 0;
    }
