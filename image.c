

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

#define TAILLE_MAX 512
#define INTENSITE_MAX 255
#define RVB_PPM 3


/*********************************************************
 * Fonctions de base : chargement et sauvegarde d'images
 **********************************************************/

int chargeImage(const char * nom_fichier, unsigned char * tableau);
int sauvegardeImage(const char * nom_fichier, unsigned char * tableau);

int chargeImage(const char * nom_fichier, unsigned char * vecteur)
{
	
	int largeur, hauteur;
	FILE * fp;
	unsigned int type;
	int imax, tailleImage, debug;
	char buffer[10];
	
	/* Ouverture du fichier */
	printf("\n");
	if( (fp = fopen(nom_fichier, "rb")) == NULL )
	{
		printf("[Chargement Image] Impossible d'ouvrir le fichier %s\n", nom_fichier);
		return 0;
	}
	printf("[chargeImage] Fichier %s ouvert\n", nom_fichier);


	/* 1- Entête */
	if(fgets(buffer, 3, fp) == NULL)
	{
		printf("[Chargement Image] Erreur de lecture du fichier\n");
		return 0;
	}


	if( (type = buffer[1]-48) != 6 )
	{
		printf("[Chargement Image] L'image n'est pas de type P6 (P%d)\n", type);
	}



	///* 2- On passe les commentaires */
	//do  
	//{
	//	if(fgets(buffer, TAILLE_MAX, fp) == NULL)
	//	{
	//		printf("[chargeImage] Erreur de lecture du fichier\n");
	//		return 0;
	//	}
	//	
	//}
	//while (buffer[0] == '#');

	/* 3- Dimensions */


	if(fscanf(fp, "%d %d\n%d\n", &largeur, &hauteur, &imax) == 0)
	{
		printf("[Chargement Image] Erreur pour acceder aux parametres de l'image\n");
		return 0;
	}
	printf("[Chargement Image] Entete = Image %d x %d pixels, intensite maximale = %d\n", largeur, hauteur, imax);
	tailleImage = TAILLE_MAX * TAILLE_MAX * RVB_PPM;
	
	/* Recuperation des pixels de l'image */
	if( (fread (vecteur, 1, tailleImage, fp)) != tailleImage )
	{
		printf("[Chargement Image] Les pixels n'ont pas ete correctement recuperes\n");
		return 0;
	}
	

	fclose(fp);
	printf("[Chargement Image] Le fichier a ete correctement ferme\n\n");
	return 1;
}


/*
 \brief Fonction pour sauvegarder une image
 \param nom_fichier Le fichier que l'on veut sauvegarder
 \param tableau tableau de char (= endroit où les pixels sont stockes)
 \return 1 si tout c'est bien passe, 0 sinon
 */
int sauvegardeImage(const char * nom_fichier, unsigned char * vecteur)
{

	FILE * fp;
	int tailleImage;
	int largeur=TAILLE_MAX, hauteur=TAILLE_MAX;

	/* Ouverture du fichier */
	printf("\n");	
	if( (fp = fopen(nom_fichier, "wb")) == NULL ){
		printf("[Sauvegarde Image] Impossible d'ouvrir le fichier %s\n", nom_fichier);
		return 0;
	}

	printf("[Sauvegarde Image] Fichier %s ouvert en ecriture\n", nom_fichier);

	if(!fprintf(fp, "P6\n%d %d %d\n", largeur, hauteur, INTENSITE_MAX)){
		printf("[Sauvegarde Image] Erreur pour ecrire les parametres de l'image\n");
		return 0;
	}

	printf("[Sauvegarde Image] Entete = Image %d x %d pixels, intensite maximale = %d\n", largeur, hauteur, INTENSITE_MAX);
	tailleImage = largeur * hauteur * RVB_PPM;

	if( fwrite (vecteur, 1, tailleImage, fp) != tailleImage )
	{
		printf("[Sauvegarde Image] Les pixels n'ont pas ete correctement ecrits\n");
		return 0;
	}
	
	fclose(fp);
	printf("[Sauvegarde Image] Le fichier a ete correctement ferme\n\n");
	return 1;
}

void gris(unsigned char imageDep[TAILLE_MAX][TAILLE_MAX][RVB_PPM], unsigned char imageFin[TAILLE_MAX][TAILLE_MAX][RVB_PPM])
{
	int R,V,B,moy;
	int i,k;
	for(i=0;i<TAILLE_MAX;i++){
		for(k=0;k<TAILLE_MAX;k++){
			R = imageDep[i][k][0];
			V = imageDep[i][k][1];
			B = imageDep[i][k][2];
			moy = (R+V+B)/3;
			imageFin[i][k][0] = moy;
			imageFin[i][k][1] = moy;
			imageFin[i][k][2] = moy;
		}
	}
}

void noirblanc(unsigned char imageDep[TAILLE_MAX][TAILLE_MAX][RVB_PPM], unsigned char imageFin[TAILLE_MAX][TAILLE_MAX][RVB_PPM])
{
	int R,V,B,moy;
	int i,k;
	for(i=0;i<TAILLE_MAX;i++){
		for(k=0;k<TAILLE_MAX;k++){
			R = imageDep[i][k][0];
			V = imageDep[i][k][1];
			B = imageDep[i][k][2];
			moy = (R+V+B)/3;
			if(moy>INTENSITE_MAX/2){
				imageFin[i][k][0] = INTENSITE_MAX ;
				imageFin[i][k][1] = INTENSITE_MAX ;
				imageFin[i][k][2] = INTENSITE_MAX ;
			}
			else{
				imageFin[i][k][0] = 0 ;
				imageFin[i][k][1] = 0 ;
				imageFin[i][k][2] = 0 ;
			}
		}
	}
}


void negatif(unsigned char imageDep[TAILLE_MAX][TAILLE_MAX][RVB_PPM], unsigned char imageFin[TAILLE_MAX][TAILLE_MAX][RVB_PPM])
{
	int i,k;
	for(i=0;i<TAILLE_MAX;i++){
		for(k=0;k<TAILLE_MAX;k++){
			imageFin[i][k][0] = 255 - imageDep[i][k][0];
			imageFin[i][k][1] = 255 - imageDep[i][k][1];
			imageFin[i][k][2] = 255 - imageDep[i][k][2];
		}
	}
}

void pixelisation(unsigned char imageDep[TAILLE_MAX][TAILLE_MAX][RVB_PPM], unsigned char imageFin[TAILLE_MAX][TAILLE_MAX][RVB_PPM])
{
	int i,k,j,l;
	int R,V,B;
	for(i=0;i<TAILLE_MAX;i+=2){
		for(k=0;k<TAILLE_MAX;k+=2){
			R=0;
			V=0;
			B=0;
			for(j=0;j<=1;j++){
				for(l=0;l<=1;l++){
					R+=imageDep[i+j][k+l][0];
					V+=imageDep[i+j][k+l][1];
					B+=imageDep[i+j][k+l][2];
			

				}
			}	
			R /= 4;
			V /= 4;
			B /= 4;
			for(j=0;j<=1;j++){
				for(l=0;l<=1;l++){
					imageFin[i+j][k+l][0] = R;
					imageFin[i+j][k+l][1] = V;
					imageFin[i+j][k+l][2] = B;
				}
			}	
		}
	}
}

void flou(unsigned char imageDep[TAILLE_MAX][TAILLE_MAX][RVB_PPM], unsigned char imageFin[TAILLE_MAX][TAILLE_MAX][RVB_PPM])
{
	int i,k,j,l,cpt;
	int R,V,B;
	int value;
	printf("\nQuelle est l'intensite du flou ? (Valeur entiere de 1 à 10)\n");
	scanf("%d",&value);
	for(i=0;i<TAILLE_MAX;i++){
		for(k=0;k<TAILLE_MAX;k++){
			R = 0;
			V = 0;
			B = 0;
			cpt = 0;
			
			for(j = i-value ;j <= i+value;j++){
				for(l = k-value; l <= k+value; l++){
					
					if((j>=0) && (j<TAILLE_MAX) && (l>=0) && (l<TAILLE_MAX)){
						R += imageDep[j][l][0];
						V += imageDep[j][l][1];
						B += imageDep[j][l][2];
						cpt++;
					}
				}			
			}	
			R /= cpt;
			V /= cpt;
			B /= cpt;
			imageFin[i][k][0] = R;
			imageFin[i][k][1] = V;
			imageFin[i][k][2] = B;
		}
	}
}


int main ()
{
	int i, k, m;
	char load[50];
	char output[50];
	int choix;
	int end = 1;
	printf("\n\n******************************************\n");
	printf("**         Editeur de photo .ppm        **\n");
	printf("**           Projet en C - IMA3         **\n");
	printf("**  Delobelle Matthieu - Durot Anthony  **\n");
	printf("******************************************\n");
	printf("\nEntrez le nom du ficher a charger :\n");
	scanf("%s",load);

	unsigned char imageDep[TAILLE_MAX][TAILLE_MAX][3];
	int success = chargeImage(load, (unsigned char *)imageDep);

	unsigned char imageFin[TAILLE_MAX][TAILLE_MAX][3];
	if(success){
		while(end){
			printf("Quel effet voulez vous ajouter ?\n");
			printf("\t1 - Gris\n");
			printf("\t2 - Noir et blanc\n");
			printf("\t3 - Negatif\n");
			printf("\t4 - Pixelisation\n");
			printf("\t5 - Flou (Intensite Reglable)\n");
			scanf("%d",&choix);

			switch(choix){
				case 1:
					gris(imageDep, imageFin);
					break;
				case 2:
					noirblanc(imageDep, imageFin);
					break;
				case 3:
					negatif(imageDep, imageFin);
					break;
				case 4:
					pixelisation(imageDep, imageFin);
					break;
				case 5:
					flou(imageDep, imageFin);
					break;	
			}

			for(i=0;i<TAILLE_MAX;i++){
				for(k=0;k<TAILLE_MAX;k++){
					for(m=0; m< RVB_PPM; m++){
					imageDep[i][k][m] = imageFin[i][k][m];
					}		
				}
			}

			printf("Souhaitez vous appliquer un autre effet ?\n\t0 - Non\n\t1 - Oui\n");
			scanf("%d",&end);
		}
	


		printf("\nEntrez le nom du fichier de sortie :\n");
		scanf("%s",output);
		

		sauvegardeImage(output, (unsigned char *)imageFin);
	}
	return 0;
}

