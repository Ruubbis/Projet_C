

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

int min(int a, int b, int c){
	int res = a;
	if(b<res){
		res = b;	
	}
	if(c<res){
		res = c;	
	}
	return res;
}

int max(int a, int b, int c){
	int res = a;
	if(b>res){
		res = b;	
	}
	if(c>res){
		res = c;	
	}
	return res;
}


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

int sauvegardeImageReduite(const char * nom_fichier, unsigned char * vecteur)
{

	FILE * fp;
	int tailleImage;
	int largeur=TAILLE_MAX/2, hauteur=TAILLE_MAX/2;

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
	int i,k,j,l,cpt,p,m;
	int R,V,B;
	int value,N;
	printf("\nQuelle est l'intensite du flou ? (Valeur entiere de 1 à 10)\n");
	scanf("%d",&value);
	printf("\nCombien de passage voulez vous faire ? (Valeur entiere de 1 à 10)\n");
	scanf("%d",&N);
	for(p=0;p<N;p++){
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
		for(i=0;i<TAILLE_MAX;i++){
			for(k=0;k<TAILLE_MAX;k++){
				for(m=0; m< RVB_PPM; m++){
					imageDep[i][k][m] = imageFin[i][k][m];
				}		
			}
		}	
	}
}




void redimensionnement(unsigned char imageDep[TAILLE_MAX][TAILLE_MAX][RVB_PPM], unsigned char imageFin[TAILLE_MAX/2][TAILLE_MAX/2][RVB_PPM])
{
	int i,k,j,l,cpt,p,m;
	int R,V,B;
	int value=1;
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
			imageFin[i/2][k/2][0] = R;
			imageFin[i/2][k/2][1] = V;
			imageFin[i/2][k/2][2] = B;
		}
	}	
}


void rgb2tsv(unsigned char imageDep[TAILLE_MAX][TAILLE_MAX][RVB_PPM], int imageTSV[TAILLE_MAX][TAILLE_MAX][RVB_PPM])
{
	int i,k,clr;
	int R,V,B;
	int m, M;
	int t, s ,v;
	for(i=0;i<TAILLE_MAX;i+=1){
		for(k=0;k<TAILLE_MAX;k+=1){
			R = imageDep[i][k][0];
			V = imageDep[i][k][1];
			B = imageDep[i][k][2];
			m = min(R,V,B);
			M = max(R,V,B);	
			
			if(M==m){
				t = 0;			
			}
			else if(M == R){
				t = (60 * (V - B) / (M - m) + 360)%360;
			}
			else if(M == V){
				t = 60 * (B - R) / (M - m) + 120;
			}
			else if(M == B){
				t = 60 * (R - V) / (M - m) + 240;
			}
			
	
			if(M!=0){
				s = (1 - (double)m/(double)M)*255;
			}			
			else{
				s = 0;
			}
			v = M;
			printf("(%d;%d)--- R = %d, V = %d, B = %d => M = %d m = %d=> t = %d, s = %d, v = %d\n",i,k,R,V,B,M,m,t,s,v);

			imageTSV[i][k][0] = t;
			imageTSV[i][k][1] = s;
			imageTSV[i][k][2] = v;
		}
	}
}



void tsv2rgb(int imageTSV[TAILLE_MAX][TAILLE_MAX][RVB_PPM], unsigned char imageFin[TAILLE_MAX][TAILLE_MAX][RVB_PPM])
{
	int i,k,clr;
	int R,V,B;
	int m, M;
	int N,P,Q,U;
	double F;
	int t,s,v;
	for(i=0;i<TAILLE_MAX;i+=1){
		for(k=0;k<TAILLE_MAX;k+=1){
			
			t = imageTSV[i][k][0];
			s = imageTSV[i][k][1];
			v = imageTSV[i][k][2];


			N = (t/60)%6;
			F = (double)t/60 - N;

			P = (int)((double)(v) * (1.0 - (double)s/255));
			Q = (int)((double)(v) * (1.0 - (F*(double)s/255)));
			U = (int)((double)(v) * (1.0 - (1.0-F) * (double)s/255));

			switch(N){
				case 0:
					R = v;
					V = U;
					B = P;
					break;
				case 1:
					R = Q;
					V = v;
					B = P;
					break;
						
				case 2:
					R = P;
					V = v;
					B = U;
					break;
				case 3:
					R = P;
					V = Q;
					B = v;
					break;
				case 4:
					R = U;
					V = P;
					B = v;
					break;
				case 5:
					R = v;
					V = P;
					B = Q;
					break;							
			}
			
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
	int redim = 0;
	int convert = 0;
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

	unsigned char imageFinReduite[TAILLE_MAX/2][TAILLE_MAX/2][3]; //Creation d'un tableau de taille N/2 N/2

	int imageTSV[TAILLE_MAX][TAILLE_MAX][3]; //Creation d'un tableau d'entier car en TSV T > 255 donc ne rentre pas dans un unsigned char


	if(success){
		while(end){
			printf("Quel effet voulez vous ajouter ?\n");
			printf("\t1 - Gris\n");
			printf("\t2 - Noir et blanc\n");
			printf("\t3 - Negatif\n");
			printf("\t4 - Pixelisation\n");
			printf("\t5 - Flou (Intensite Reglable)\n");
			printf("\t6 - Redimensionnement\n");
			printf("\t7 - Effet de type TSV");
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
				case 6:
					redimensionnement(imageDep, imageFinReduite);
					redim = 1;
					end = 0;
					break;
				case 7:
					rgb2tsv(imageDep, imageTSV);
					tsv2rgb(imageTSV, imageFin);
					convert = 1;
					end = 0;
					break;
			}
			if(redim==0){
				for(i=0;i<TAILLE_MAX;i++){
					for(k=0;k<TAILLE_MAX;k++){
						for(m=0; m< RVB_PPM; m++){
						imageDep[i][k][m] = imageFin[i][k][m];
						}		
					}
				}
				if(convert==0){
					printf("Souhaitez vous appliquer un autre effet ?\n\t0 - Non\n\t1 - Oui\n");
					scanf("%d",&end);
				}
			}

		}
	


		printf("\nEntrez le nom du fichier de sortie :\n");
		scanf("%s",output);
		
		if(redim==0){
			sauvegardeImage(output, (unsigned char *)imageFin);
		}
		else{
			sauvegardeImageReduite(output, (unsigned char *)imageFinReduite);
		}
	}
	return 0;
}

