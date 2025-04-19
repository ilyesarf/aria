#include "map.h"

void initialiser_map (map *m,SDL_Surface *screen,personne *p)
{
	m->map=IMG_Load("mini_backgg.png");
	m->pos_map.x = 1000;// equiv (*m).pos_map.x=1000;
	m->pos_map.y = 100;
	m->mini_perso=SDL_CreateRGBSurface(SDL_HWSURFACE|SDL_DOUBLEBUF,10,10,32,0,0,0,0);//hedhy taaml mouraba fil mini map ali houwa l perso 
	SDL_FillRect(m->mini_perso,NULL,SDL_MapRGB(screen->format,0,255,0));// lel couleur 
	m->perso_pos_map.x=0;
	m->perso_pos_map.y=0;

}


void perso_map(map *m,personne *p)
{
	m->perso_pos_map.x=p->pos_perso.x/10+m->pos_map.x;
	m->perso_pos_map.y=p->pos_perso.y/10+10+m->pos_map.y;// besh l position ta" l mini perso iji kad kad maa l perso lekbir 3al axe y 
}



void affiche_map(map *m , SDL_Surface *screen,personne *p)
{
	SDL_BlitSurface(m->map, NULL, screen,&m->pos_map);
	SDL_BlitSurface(m->mini_perso, NULL, screen,&m->perso_pos_map);
}













void sauvegarder(int score,char nomjoueur[],char nomfichier[])
{
	FILE *f,*fnom;
   	int id;
char nom[255];
	f=fopen(nomfichier, "a+");
	fnom=fopen("testnom.txt", "a+");
	if(f==NULL){
        printf("Erreur lors de l'ouverture d'un fichier");
	
       }
	if(fnom==NULL){
        printf("Erreur lors de l'ouverture d'un fichier");
       }
 	fprintf(f, "%d\n", score);
	while( fscanf(fnom, "%d %s\n",&id,&nom)!=EOF)
	{ 
		printf("%d \n",id);
	}	
	fprintf(fnom,"%d %s\n",id+1,nomjoueur);
	
	fclose(f);
	fclose(fnom);
}

void meilleur (char nomfichier[],int score,char nomjoueur[])
{
	FILE *f,*fnom;
    char nom[255];
    int age;
	int id;
 	char c;
	int t[50];//tab score
	int i=0,s=0;
	char* tnom[50];
    f = fopen("test.txt", "r");
 fnom = fopen("testnom.txt", "r");
    if(f==NULL)
	{
        printf("Erreur lors de l'ouverture d'un fichier");
        exit(1);
    }
	if(fnom==NULL)
	{
        printf("Erreur lors de l'ouverture d'un fichier");
        exit(1);
    }
while(fscanf(f,"%d\n",&t[i])==1)
{
	i++;
}	
for(int j=0;j<i;j++) //testt
 	{printf("score : %d\n", t[j]);
}
int meilleur=t[0],d=1;
int x=1;
for(int h=1;h<i;h++)
{	x++;
	if(t[h]>meilleur)
	{	d=h+1;
		meilleur=t[h];
		//printf("%d",d);
	}
	
}
int z=1;
char nomjoueurmeilleur[4];

while( fscanf(fnom, "%d %s\n",&id,&nom)!=EOF)
{
	printf("%s\n",nom);

	if(id==d)
	{
		strcpy(nomjoueurmeilleur,nom);
		//printf("test jeouer: %s",nomjoueurmeilleur);
		break;
	}

}


printf("meilleur score: %d",meilleur);

printf("meilleur jeouer: %s",nomjoueurmeilleur);
    	fclose(f);
	 fclose(fnom);
}
