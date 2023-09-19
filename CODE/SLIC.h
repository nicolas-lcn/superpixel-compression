#ifndef SLIC_H
#define SLIC_H

#include <cmath>
#include <cstdio>
#include <vector>
#include <float.h>
#include <algorithm>
#include <iostream>

#include "image_ppm.h"
#include "Conversion.h"
#include "ColorSpace.h"

using namespace std;

struct CIELAB
{
	/*pixel in CIELAB*/
	double l,a,b,x,y;

	CIELAB(){l,a,b,x,y=0;}
	CIELAB(double _l, double _a, double _b, double _x, double _y){l=_l;a=_a;b=_b;x=_x;y=_y;}

	void print(){printf("{%f, %f, %f, %f, %f}\n", l,a,b,x,y);}
};

ColorSpace::Lab convert(OCTET pR, OCTET pG, OCTET pB)
{
	ColorSpace::Rgb rgb((double)pR, (double)pG, (double)pB);
	ColorSpace::Lab lab;
	rgb.To<ColorSpace::Lab>(&lab);
	return lab;
}

ColorSpace::Rgb convert(double l, double a, double b)
{
	ColorSpace::Lab lab(l,a,b);
	ColorSpace::Rgb rgb;
	lab.To<ColorSpace::Rgb>(&rgb);
	return rgb;
}

/*
Converts OCTET* Img (in RGB color space) using ColorSpace lib onto array of structs CIELAB to ease next computations
*/
void convert(OCTET* ImgIn, std::vector<CIELAB> &out, int nW, int nH)
{
    for (int i = 0; i<(nW*nH*3); i+=3)
    {
        int indexI = floor((i/3)/nW); int indexJ = (i/3) - indexI*nW;
        ColorSpace::Lab p = convert(ImgIn[i], ImgIn[i+1], ImgIn[i+2]);
        CIELAB c = {p.l, p.a, p.b, indexI , indexJ};
        out[indexI * nW + indexJ] = c;
    }
}

/*
Converts CIELAB Image into OCTET* Image (RGB space) to display
*/
void reverseConvert(std::vector<CIELAB> &in, OCTET* ImgOut)
{
	int n = (int) in.size();
	int index = 0;
	for (int i = 0; i < n; ++i)
	{
		index = i*3;
		ColorSpace::Rgb p = convert(in[i].l, in[i].a, in[i].b);
		ImgOut[index] = (unsigned char) p.r;
		ImgOut[index+1] = (unsigned char) p.g;
		ImgOut[index+2] = (unsigned char) p.b;
	}

}



float gradientNorm(OCTET* ImgIn, int i, int j, int nW, int nH)
{
	
	ColorSpace::Lab p,pX,pY;
	int x,y;
	if(i<nH-1) 
	{
		p = convert(ImgIn[i*nW*3], ImgIn[i*nW*3+1], ImgIn[i*nW*3+2]);
		pX = convert(ImgIn[(i-nW)*nW*3], ImgIn[(i-nW)*nW*3 + 1], ImgIn[(i-nW)*nW*3 + 2]);
	
		x = (p.l - pX.l) + 
		(p.a - pX.a) + 
		(p.b - pX.b);
	}
	else x = 0;
	if(j<nW-1)
	{
		p = convert(ImgIn[i*nW*3], ImgIn[i*nW*3+1], ImgIn[i*nW*3+2]);
		pY = convert(ImgIn[(i+3)*nW*3], ImgIn[(i+3)*nW*3 + 1], ImgIn[(i+3)*nW*3 + 2]);
		y = (p.l - pY.l) + 
		(p.a - pY.a) + 
		(p.b - pY.b);
	} 
	else y = 0;
	return sqrt(x*x + y*y);
}


void moveClusters(OCTET* ImgIn, std::vector<CIELAB> &C, int S, int nW, int nH)
{
	int o = 0;
	for (int i = 0; i < nH; i+=S)
	{
		for (int j = 0; j < nW; j+=S)
		{
			float minG = FLT_MAX;
			int minX,minY;
			for (int k = i; k < i+S; ++k)
			{
				for (int l = j; l < j+S; ++l)
				{
					float norm = gradientNorm(ImgIn, k,l,nW,nH);
					if(norm <= minG)
					{
						minG = norm;
						minX = k; minY = l;
					}
				}
			}
			C[o].x = minX; C[o].y = minY;
			o++;
		}
	}
}

/*
Create and retrieves clusters C (vector of 5-uples size k) from Img
*/
void initClusters(OCTET* ImgIn, std::vector<CIELAB> &C, int k, int nW, int nH)
{
	int nTaille = nW * nH; 
	int nTaille3 = nTaille * 3;
	
	moveClusters(ImgIn, C, (int) sqrt(nTaille/k), nW, nH);
	for (int i = 0; i < k; ++i)
	{
		int index = C[i].x * nW + C[i].y;
		ColorSpace::Lab p = convert(ImgIn[index], ImgIn[index+1], ImgIn[index+2]);
		C[i].l = p.l;
		C[i].a = p.a;
		C[i].b = p.b;
	}
}

/*
 * Compute color and spatial distance according to distance calculation described in the article
 * returns a float value of distance
 * param S = sqrt(ntaille/k)
 * param m also allows us to weigh the
 * relative importance between color similarity and spatial proximity.
 * When m is large, spatial proximity is more important and the
 * resulting superpixels are more compact (i.e., they have a lower
 * area to perimeter ratio). When m is small, the resulting superpixels
 * adhere more tightly to image boundaries, but have less regular
 * size and shape. When using the CIELAB color space, m can be in
 * the range [1, 40].
 */
float distance(CIELAB &i, CIELAB &j, int S, int m)
{
	float dc, ds, D;
	/*color euclidian distance*/
	float squared_dc = pow((j.l - i.l), 2) + pow((j.a - i.a), 2) + pow((j.b - i.b), 2);
	dc = sqrt(squared_dc);

	/*spatial euclidian distance */
	float squared_ds = pow((j.x - i.x), 2) + pow((j.y - i.y), 2);
	ds = sqrt(squared_ds);

	float squared_D = squared_dc + pow(ds/(float)S, 2) * m*m;
	D = sqrt(squared_D);
	return D;
}

void updateClusters(std::vector<CIELAB> &Img, std::vector<CIELAB> &C, int* labels, int k, float &E)
{
	int n = Img.size();
	double total, avgL, avgA, avgB, avgX, avgY;
	for (int i = 0; i < k; ++i)
	{
		total = avgL = avgA = avgB = avgX = avgY = 0;
		for (int j = 0; j < n; ++j)
		{
			if(labels[j] != i) continue;
			CIELAB p = Img[j];
			avgL += p.l;
			avgA += p.a;
			avgB += p.b;
			avgX += p.x;
			avgY += p.y;
			total ++;
		}
		if(total != 0)
		{
			avgL/=total; avgA/=total; avgB/=total; avgX/=total; avgY/=total;
			CIELAB average = {avgL, avgA, avgB, avgX, avgY};
			int prevX = C[i].x; int prevY = C[i].y;
			C[i] = average;

			E = sqrt((avgX - prevX) * (avgX - prevX) + (avgY - prevY) * (avgY - prevY));
		}
	}
}

void execute(char* cNomImgLue, char* cNomImgEcrite, int k, int m)
{
	int nH, nW, nTaille;

	OCTET *ImgIn, *ImgOut;

	lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
	nTaille = nH * nW;

	int nTaille3 = nTaille * 3;
	allocation_tableau(ImgIn, OCTET, nTaille3);
	lire_image_ppm(cNomImgLue, ImgIn, nH * nW);
	allocation_tableau(ImgOut, OCTET, nTaille3);
	/*Initialisation*/
	int S = (int) sqrt(nTaille/k);

	std::vector<CIELAB> imgLAB(nTaille);

	int* labels = new int[nTaille];
	float* distances = new float[nTaille];
	for (int i = 0; i < nTaille; ++i)
	{
		imgLAB[i] = {0,0,0,0,0};
		labels[i] = -1;
		distances[i] = INFINITY;
	}
	std::vector<CIELAB> C(k);
	for(int i = 0; i < k; i++){
	    C.emplace_back(CIELAB{});
	}

	std::vector<std::vector<ColorSpace::Rgb>> superpixels(k);


/*
	for(int i = 0 ; i < k ; i++) {
		CIELAB a = new CIELAB();	
		C.push_back(a);
	}
*/
	//cout<<"Test 1"<<endl;
	initClusters(ImgIn, C, k, nW, nH);
	//cout<<"Test 2"<<endl;
	convert(ImgIn, imgLAB, nW, nH);
	int c_x, c_y, index;


	int iteration = 0;
	float E;
	do
	{
		/* for each cluster C[i] */
		for (int c_i = 0; c_i < k; ++c_i)
		{
			c_x = C[c_i].x; c_y = C[c_i].y;
			for(int i = std::max(0, c_x-2*S); i < std::min(nH, c_x+2*S); i++)
			{
				for (int j = std::max(0, c_y-2*S); j < std::min(nW, c_y+2*S); ++j)
				{
					index = i*nW+j;
					float D = distance(C[c_i], imgLAB[index], S, m);
					if(D < distances[index])
					{
						distances[index] = D;
						labels[index] = c_i;
					}
				}
			}
		}
		updateClusters(imgLAB, C, labels, k, E);
		//cout<<"Test 3"<<endl;
		iteration ++;
		
	}while(E > 0.1f && iteration < 10);

	
	/*
	for (int i = 0; i < nTaille; ++i)
	{
		cout<<"labels de "<<i<<" = "<<labels[i]<<endl;
	}
	*/
	//cout<<"Test 3"<<endl;
	for (int i = 0; i < nTaille; ++i)
	{
		imgLAB[i] = C[labels[i]];
	}
	//cout<<"Test 4"<<endl;
	reverseConvert(imgLAB, ImgOut);
	//cout<<"Test 5"<<endl;
	ecrire_image_ppm(cNomImgEcrite, ImgOut,  nH, nW);
	free(ImgIn);free(ImgOut);
	delete[] labels;
	delete[] distances;
	//cout<<"Test 6"<<endl;
}

#endif //SLIC_H