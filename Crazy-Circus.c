#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX 3
#define MAX_PLAYERS 10
#define MAX_STATES 24

// STRUCTURES

typedef struct {
    char* bleu[MAX];
    int nb_bleu;
    char* rouge[MAX];
    int nb_rouge;
} GameState;

typedef struct {
    char nom[50];
    int score;
    int elimine;
} Player;

// GLOBAL

char* animaux[MAX];
int nb_animaux = 0;

char* commandes[5];
int nb_cmds = 0;

GameState states[MAX_STATES];
int nb_states = 0;


// UTILS

void erreur(char* msg){
    printf("%s\n", msg);
    exit(1);
}

int equals(GameState* a, GameState* b){
    if(a->nb_bleu != b->nb_bleu || a->nb_rouge != b->nb_rouge) return 0;

    for(int i=0;i<a->nb_bleu;i++)
        if(strcmp(a->bleu[i], b->bleu[i])) return 0;

    for(int i=0;i<a->nb_rouge;i++)
        if(strcmp(a->rouge[i], b->rouge[i])) return 0;

    return 1;
}

void copy(GameState* d, GameState* s){
    *d = *s;
}

// PILES

void push(char* s[], int* n, char* v){
    for(int i=*n;i>0;i--) s[i]=s[i-1];
    s[0]=v;
    (*n)++;
}

char* pop(char* s[], int* n){
    if(*n==0) return NULL;
    char* v=s[0];
    for(int i=0;i<*n-1;i++) s[i]=s[i+1];
    (*n)--;
    return v;
}

void rotate(char* s[], int n){
    if(n<=1) return;
    char* last = s[n-1];
    for(int i=n-1;i>0;i--) s[i]=s[i-1];
    s[0]=last;
}

// COMMANDES

int autorisee(char* cmd){
    for(int i=0;i<nb_cmds;i++)
        if(strcmp(cmd,commandes[i])==0) return 1;
    return 0;
}

void apply_cmd(GameState* g, char* cmd){

    if(strcmp(cmd,"KI")==0){
        char* v = pop(g->bleu,&g->nb_bleu);
        if(v) push(g->rouge,&g->nb_rouge,v);
    }

    else if(strcmp(cmd,"LO")==0){
        char* v = pop(g->rouge,&g->nb_rouge);
        if(v) push(g->bleu,&g->nb_bleu,v);
    }

    else if(strcmp(cmd,"SO")==0){
        char* b = pop(g->bleu,&g->nb_bleu);
        char* r = pop(g->rouge,&g->nb_rouge);
        if(b) push(g->rouge,&g->nb_rouge,b);
        if(r) push(g->bleu,&g->nb_bleu,r);
    }

    else if(strcmp(cmd,"NI")==0) rotate(g->bleu,g->nb_bleu);
    else if(strcmp(cmd,"MA")==0) rotate(g->rouge,g->nb_rouge);
}

int apply_seq(GameState* res, GameState* start, char* seq){

    copy(res,start);

    for(int i=0;i<strlen(seq);i+=2){

        char cmd[3]={seq[i],seq[i+1],0};

        if(!autorisee(cmd)) return 0;

        apply_cmd(res,cmd);
    }
    return 1;
}

// CFG

void load_cfg(){

    FILE* f=fopen("crazy.cfg","r");
    if(!f) erreur("Erreur fichier cfg");

    char line[256];

    fgets(line,256,f);
    char* tok=strtok(line," \n");
    while(tok){
        animaux[nb_animaux++]=strdup(tok);
        tok=strtok(NULL," \n");
    }

    if(nb_animaux<2) erreur("Animaux invalides");

    fgets(line,256,f);
    tok=strtok(line," \n");
    while(tok){
        commandes[nb_cmds++]=strdup(tok);
        tok=strtok(NULL," \n");
    }

    if(nb_cmds<3) erreur("Commandes invalides");

    fclose(f);
}

// GENERATION ETATS

void swap(char** a, char** b){
    char* t=*a; *a=*b; *b=t;
}

void permute(char* arr[], int l, int r){
    if(l==r){
        // répartitions possibles (2/1 et 1/2)
        GameState s1 = {{arr[0],arr[1]},2,{arr[2]},1};
        GameState s2 = {{arr[0]},1,{arr[1],arr[2]},2};

        states[nb_states++] = s1;
        states[nb_states++] = s2;
        return;
    }

    for(int i=l;i<=r;i++){
        swap(&arr[l],&arr[i]);
        permute(arr,l+1,r);
        swap(&arr[l],&arr[i]);
    }
}

// AFFICHAGE

void print_stack(char* s[], int n){
    for(int i=0;i<n;i++) printf("%s ",s[i]);
    printf("\n");
}

void print_state(GameState* s){
    print_stack(s->bleu,s->nb_bleu);
    print_stack(s->rouge,s->nb_rouge);
}

void print_cmds(){
    printf("KI (B -> R) | LO (B <- R) | SO (B <-> R) | NI (B ^) | MA (R ^)\n");
}

// JEU

int restants(Player p[], int n){
    int c=0;
    for(int i=0;i<n;i++) if(!p[i].elimine) c++;
    return c;
}

Player* last(Player p[], int n){
    for(int i=0;i<n;i++) if(!p[i].elimine) return &p[i];
    return NULL;
}

void reset(Player p[], int n){
    for(int i=0;i<n;i++) p[i].elimine=0;
}

// MAIN

int main(int argc, char* argv[]){

    if(argc<3) erreur("Joueurs insuffisants");

    Player p[MAX_PLAYERS];
    int n=argc-1;

    for(int i=0;i<n;i++){
        strcpy(p[i].nom,argv[i+1]);
        p[i].score=0;

        for(int j=0;j<i;j++)
            if(strcmp(p[i].nom,p[j].nom)==0)
                erreur("Noms dupliqués");
    }

    load_cfg();

    // générer états
    char* base[3]={animaux[0],animaux[1],animaux[2]};
    permute(base,0,2);

    srand(time(NULL));

    int used[MAX_STATES]={0};

    print_cmds();

    for(int t=0;t<MAX_STATES;t++){

        int i;
        do{i=rand()%nb_states;}while(used[i]);
        used[i]=1;

        int j;
        do{j=rand()%nb_states;}while(j==i);

        GameState start=states[i];
        GameState goal=states[j];

        print_state(&start);
        printf("---- ---- ==> ---- ----\n");
        print_state(&goal);

        reset(p,n);

        while(1){

            char nom[50], seq[100];
            scanf("%s %s",nom,seq);

            int id=-1;
            for(int k=0;k<n;k++)
                if(strcmp(nom,p[k].nom)==0) id=k;

            if(id==-1){
                printf("Joueur inconnu\n");
                continue;
            }

            if(p[id].elimine){
                printf("%s ne peut pas jouer\n",nom);
                continue;
            }

            GameState res;

            if(!apply_seq(&res,&start,seq)){
                printf("Sequence invalide\n");
                continue;
            }

            if(equals(&res,&goal)){
                p[id].score++;
                printf("%s gagne un point\n",nom);
                break;
            } else {
                p[id].elimine=1;
                printf("Mauvaise sequence -- %s elimine\n",nom);
            }

            if(restants(p,n)==1){
                Player* w=last(p,n);
                w->score++;
                printf("%s gagne\n",w->nom);
                break;
            }
        }
    }

    // tri
    for(int i=0;i<n;i++)
        for(int j=i+1;j<n;j++)
            if(p[j].score>p[i].score ||
              (p[j].score==p[i].score && strcmp(p[j].nom,p[i].nom)<0)){
                Player tmp=p[i]; p[i]=p[j]; p[j]=tmp;
            }

    for(int i=0;i<n;i++)
        printf("%s %d\n",p[i].nom,p[i].score);

    return 0;
}
