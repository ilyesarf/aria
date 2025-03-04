#include <SDL/SDL.h>//inclure SDL.h  pour permettre au programme d'être portable sur tous les systèmes.
//.................//Pour manipuler des images ayants des types autre que bmp
//.................//Pour manipuler de l’audio.
//.................//Pour manipuler des textes

int main ( int argc, char** argv )
{

SDL_Surface *ecran;//Déclaration d'une variable pointeure sur surface:  pour l'écran,
//..................//Déclaration d'une variable pointeure sur surface: pour l'image 
///................../Déclaration des variables pointeurs sur surfaces:  pour le texte
//Déclaration des variables de type entier pour la boucle de jeu 
int quitter = 1;
//Déclaration d'un compteur pour la mise à jour aprés un événement
//..................
//Déclaration des positions d'affichage : une pour l'image et une pour le texte
//..................
//..................
//Déclaration du police
//..................
//Déclaration de la couleur
//..................
//Déclaration de l'événment 
SDL_Event event;
//Déclaration d'une chaine de caractère
//..................
//Déclaration d'une variable pointeur sur l'audio pour le son continue
//..................
//Initialisation des sous systhème de SDL vidéo, son et texte
SDL_Init( SDL_INIT_VIDEO) ;
//..................
//..................

	/* initialisation de SDL_Video */
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("Echec d'initialisation de SDL : %s\n", SDL_GetError());
		return 1;
	
	}
	
	else
	printf("Bonjour le monde, SDL est initialisé avec succès.\n");
	/* creation d'une fenetre avec une zone d'affichage de 600*430 pixels et qui pemet d'afficher des image couleur*/
	ecran = SDL_SetVideoMode(600, 430,  32,SDL_HWSURFACE | SDL_DOUBLEBUF);
	if ( ecran == NULL )
	{
		fprintf(stderr, "Echec de creation de la fenetre de 300*300: %s.\n", SDL_GetError());
		return 1;
	}
	
	//téléchargement de l'image palestine.jpg
	//..................
		/*if (image== NULL) {
			return ;
		}*/
	//Position de l'image palestine.jpg
	//..................
	//..................
	//..................
	//..................	
	//position du texte
	//..................
	//..................
       // couleur du texte noire
        //..................
        //..................
        //..................
	//téléchargement du police
        //..................
        //Télécharger la musique
	//..................
	//Lancer la musique avant la boucle du jeu
      //..................
      
      /* instructions f et g du workshop */

	while(quitter) 
	{
	// Activité 2 du workshop  Afficher l'image palestine.jpg
	//..................
	//Lecture de l'événement	
	SDL_PollEvent(&event); 
	// Selon le type de l'événement
	switch (event.type)
         { 
        // exit if the window is closed
		case SDL_QUIT: quitter = 0;
        	break;
        //Lecture d'un événement par clavier
        	
        		
       //Si on clique sur le bouton echap
           		
        	break;
        //lecture d'un événement clique bouton souris
         	
                		
           	
	 	break;
	 }
	//if(..........)
                {	//écriture du texte We Will NEVER forget \n We will NEVER stop sharing
		//..................
                //Affichage du texte 
               //..................
       		}
          //Refraichir l'écran
	//..................

	}
	
		//Fermeture des sous-systèmes audio et texte, libération de la mémoire des ressources images, texte et son.
	
	return 0;
}

