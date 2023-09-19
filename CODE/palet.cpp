#include "image_ppm.h"
#include <math.h>
#include <vector>
#include <float.h>
#include <random>

struct RGB
{
	unsigned char r,g,b;
	int centroid = -1;
};

float distance(RGB c1, RGB c2)
{
	float rsquare, gsquare, bsquare;
	rsquare = pow((c1.r - c2.r), 2);
	gsquare = pow((c1.g - c2.g), 2);
	bsquare = pow((c1.b - c2.b), 2);
	return sqrt((rsquare + gsquare + bsquare));
}

std::vector<RGB> generateRandomClasses(OCTET* ImgIn, int nbClasses, int nTaille)
{
	std::default_random_engine generator;
	generator.seed(std::random_device()());
  	std::uniform_int_distribution<int> distribution(0,nTaille);
	std::vector<RGB> classes(nbClasses);
	int index = 0;
	int previous_p = -3;
	while(index < nbClasses)
	{
		int p = distribution(generator); //random pixel index
		// if((p-previous_p) < 3) continue;
		RGB color = {ImgIn[p], ImgIn[p+1], ImgIn[p+2]};
		classes[index] = color;
		previous_p = p;
		index++; 
	}
	return classes;
}

void fill(OCTET* in, OCTET gray, int fromI, int fromJ, int toI, int toJ, int nH, int nW)
{
	for (int i = fromI; i < toI; ++i)
	{
		for (int j = fromJ; j < toJ; ++j)
		{
			in[i*nW+j] = gray;
		}
	}
}


void generatePalet(OCTET* palet, int nW, int nH, std::vector<RGB> &means)
{
	int nbClasses = means.size();
	int n = nW*nH*3;
	int index = 0;
	for (int i = 0; i < nbClasses; ++i)
	{
		if(index >= n) break;
		palet[index] = means[i].r;
		palet[index+1] = means[i].g;
		palet[index+2] = means[i].b;
		index+=3;
	}
	
}


RGB sum(RGB c1, RGB c2)
{
	unsigned char r,g,b;
	r = (unsigned char) c1.r + c2.r;
	g = (unsigned char) c1.g + c2.g;
	b = (unsigned char) c1.b + c2.b;
	printf("%u + %u = %u\n", c1.r, c2.r, r);
	return {r,g,b};
}

RGB divide(RGB c, int scalar)
{
	unsigned char r,g,b;
	r = (unsigned char) c.r/scalar;
	g = (unsigned char) c.g/scalar;
	b = (unsigned char) c.b/scalar;
	return {r,g,b};
}

void convert(OCTET* ImgIn, int nW, int nH, std::vector<RGB> &out)
{
	int n = nW * nH;
	int index;
	for (int i = 0; i < n; i++)
	{
		index = i * 3;
		RGB c = {ImgIn[index], ImgIn[index+1], ImgIn[index+2]};
		out[i] = c;
	}
}


void convert(std::vector<RGB>&in, OCTET* ImgOut)
{
	int n = in.size();
	int index;
	for (int i = 0; i <n ; i++)
	{
		index = i*3;
		ImgOut[index] = in[i].r;
		ImgOut[index+1] = in[i].g;
		ImgOut[index+2] = in[i].b;
	}
}

std::vector<RGB> kMean(std::vector<RGB> &ImgIn, int k, std::vector<RGB> &classes, float threshold)
{
	size_t changed = 0;
	size_t n = ImgIn.size();
	std::vector<RGB> means(k);
	for (size_t i = 0; i < n; ++i)
	{
		float min = FLT_MAX;
		int index = -1;
		for (int j = 0; j < k; ++j)
		{
			float dist = distance(ImgIn[i], classes[j]);
			if(dist < min) 
			{
				min = dist;
				index = j;
			}
		}
		if(ImgIn[i].centroid != index && index != -1)
		{
			changed ++;
			ImgIn[i].centroid = index;
		}

	}
	float percentage = (float)(changed/n)*100;
	for (int i = 0; i < k; ++i)
	{
		int avgR, avgG, avgB;
		avgR = 0; avgG = 0; avgB = 0;
		int total = 0;
		for (size_t j = 0; j < n; ++j)
		{
			if(ImgIn[j].centroid != i) continue;
			total ++;
			avgR += ImgIn[j].r;
			avgG += ImgIn[j].g;
			avgB += ImgIn[j].b;
			// average = sum(average, ImgIn[j]);
		}
		if(total != 0) 
		{
			avgR /= total;
			avgG /= total;
			avgB /= total;
			RGB average = {(unsigned char) avgR, (unsigned char) avgG, (unsigned char) avgB};
			means[i] = average; 
		}
		else
		{
			means[i] = classes[i];
		}
	}
	if(percentage <= threshold) return means;
	return kMean(ImgIn, k, means, threshold);

}


int main(int argc, char* argv[])
{
	char cNomImgLue[250], cNomImgEcrite[250], cNomPalette[250], cNomIndexes[250];
	int nH, nW, nTaille, nbClasses;

	if (argc != 6) 
	{
		printf("Usage: ImageIn.ppm ImageOut.ppm nbClasses palet.ppm indexes.pgm\n"); 
		exit (1) ;
	}

	sscanf (argv[1],"%s",cNomImgLue) ;
	sscanf (argv[2],"%s",cNomImgEcrite);
	sscanf (argv[4],"%s",cNomPalette);
	sscanf (argv[5],"%s",cNomIndexes);
	sscanf (argv[3], "%d", &nbClasses);
	
	
	OCTET *ImgIn, *ImgOut;

	lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
	nTaille = nH * nW;
	int nTaille3 = nTaille * 3;


	std::vector<RGB> ImgInRGB = std::vector<RGB> (nTaille);
	allocation_tableau(ImgIn, OCTET, nTaille3);
	lire_image_ppm(cNomImgLue, ImgIn, nH * nW);
	allocation_tableau(ImgOut, OCTET, nTaille3);

	std::vector<RGB> classes = generateRandomClasses(ImgIn, nbClasses, nTaille3);

	convert (ImgIn, nW, nH, ImgInRGB);
	std::vector<RGB> means = kMean(ImgInRGB, nbClasses, classes, 0.01);

	OCTET* palet;
	allocation_tableau(palet, OCTET, nbClasses*3);
	int length = floor(sqrt(nbClasses));
	generatePalet(palet, length, length, means);
	
	OCTET* indexes;
	allocation_tableau(indexes, OCTET, nTaille);
	for (int i = 0; i < nTaille; ++i)
	{
		indexes[i] = ImgInRGB[i].centroid*255;
	}

	for (int i = 0; i < nTaille; ++i)
	{
		ImgInRGB[i] = means[ImgInRGB[i].centroid];
	}
	convert(ImgInRGB, ImgOut);


	ecrire_image_ppm(cNomPalette, palet, length, length);
	ecrire_image_pgm(cNomIndexes, indexes, nH, nW);
	ecrire_image_ppm(cNomImgEcrite, ImgOut,  nH, nW);
	free(ImgIn); 
	return 1;
}