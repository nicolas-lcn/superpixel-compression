#include "include/Compressor.h"
#include <fstream>
#include <vector>
#include <utility>

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

	OCTET *ImgIn;

	lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
	nTaille = nH * nW;

	int nTaille3 = nTaille * 3;
	allocation_tableau(ImgIn, OCTET, nTaille3);
	lire_image_ppm(cNomImgLue, ImgIn, nH * nW);

	OCTET* R, *G, *B;
	allocation_tableau(R, OCTET, nTaille);
	allocation_tableau(G, OCTET, nTaille);
	allocation_tableau(B, OCTET, nTaille);
	planR(R, ImgIn, nTaille);
	planV(G, ImgIn, nTaille);
	planB(B, ImgIn, nTaille);

	std::vector<std::pair<OCTET,int>> compressedR = RLEComponent(R, nW, nH);
	std::vector<std::pair<OCTET,int>> compressedG = RLEComponent(G, nW, nH);
	std::vector<std::pair<OCTET,int>> compressedB = RLEComponent(B, nW, nH);

	std::vector<std::pair<OCTET,int>> compressedImg;
	compressedImg.insert(compressedImg.end(), compressedR.begin(), compressedR.end());
	compressedImg.insert(compressedImg.end(), compressedG.begin(), compressedG.end());
	compressedImg.insert(compressedImg.end(), compressedB.begin(), compressedB.end());

	std::string delimiter = ",";
	std::string compressedImgString = "";

	// Open a binary file for writing
	std::ofstream outFile(cNomImgEcrite, std::ios::binary);

	// Write the compressed image data to the file
	for (const auto& pair : compressedImg) {
	    outFile.write(reinterpret_cast<const char*>(&pair.first), sizeof(pair.first));
	    outFile.write(reinterpret_cast<const char*>(&pair.second), sizeof(pair.second));
	}

	// Close the file
	outFile.close();
	return 0;
}