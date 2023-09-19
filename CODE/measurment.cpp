#include <cstdio>


#include "./include/image_ppm.h"

int main(int argc, char const *argv[])
{
	char cNomImgLue[250], cNomImgEcrite[250];

	if (argc != 3) 
	{
		printf("Usage: ImageInRGB.ppm downscaledImage.ppm\n"); 
		exit (1) ;
	}

	sscanf (argv[1],"%s",cNomImgLue) ;
	sscanf (argv[2],"%s",cNomImgEcrite);
	
	int nH, nW, nTaille;

	OCTET *ImgIn0, *ImgIn1;

	lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
	nTaille = nH * nW;

	int nTaille3 = nTaille * 3;
	allocation_tableau(ImgIn0, OCTET, nTaille3);
	lire_image_ppm(cNomImgLue, ImgIn0, nH * nW);
	allocation_tableau(ImgIn1, OCTET, nTaille3);
	lire_image_ppm(cNomImgEcrite, ImgIn1, nH * nW);
	

	printf("%f\n", psnr(ImgIn1, ImgIn0, nTaille3));
	

	free(ImgIn0);
	free(ImgIn1);
	return 0;
}