#include "SLIC.h"

int main(int argc, char const *argv[])
{
	char cNomImgLue[250], cNomImgEcrite[250];
	int k,m;

	if (argc != 5) 
	{
		printf("Usage: ImageInRGB.ppm ImageOutSegmentée.ppm K m\n"); 
		exit (1) ;
	}

	sscanf (argv[1],"%s",cNomImgLue) ;
	sscanf (argv[2],"%s",cNomImgEcrite);
	sscanf (argv[3],"%d", &k);
	sscanf (argv[4],"%d", &m);

	execute(cNomImgLue, cNomImgEcrite, k, m);
	return 0;
}