#include <unordered_map>
#include <string>
#include <vector>


#include "image_ppm.h"
#include "Conversion.h"
#include "ColorSpace.h"


std::vector<std::pair<OCTET, int>> RLEComponent(OCTET* Comp, int nW, int nH)
{
	std::vector<std::pair<OCTET, int>> compressedComp;
	int j,k, count;
	for (int i = 0; i < nH; ++i)
	{
		j = 0;
		while(j<nW)
		{
			OCTET val = Comp[i*nW+j];
			count = 1;
			k = j+1;
			while(k<nW && Comp[i*nW+k] == val)
			{
				count++; k++;
			}
			compressedComp.push_back(std::make_pair(val, count));
			j=k;
		}
	}
	return compressedComp;
}

void BilineardownscaleRGB(OCTET* ImgIn, OCTET* ImgOut, int nW, int nH, float factor)
{
    int w = static_cast<int>(nW * factor);
    int h = static_cast<int>(nH * factor);

    for (int i = 0; i < h; ++i)
    {
        float y = i / factor;
        int y1 = static_cast<int>(y);
        int y2 = y1 + 1;
        float yFraction = y - y1;

        for (int j = 0; j < w; ++j)
        {
            float x = j / factor;
            int x1 = static_cast<int>(x);
            int x2 = x1 + 1;
            float xFraction = x - x1;

            for (int k = 0; k < 3; ++k)
            {
                float topLeft = ImgIn[(y1 * nW + x1) * 3 + k];
                float topRight = ImgIn[(y1 * nW + x2) * 3 + k];
                float bottomLeft = ImgIn[(y2 * nW + x1) * 3 + k];
                float bottomRight = ImgIn[(y2 * nW + x2) * 3 + k];

                float top = topLeft * (1 - xFraction) + topRight * xFraction;
                float bottom = bottomLeft * (1 - xFraction) + bottomRight * xFraction;

                float value = top * (1 - yFraction) + bottom * yFraction;
                int avg = static_cast<int>(value);
                ImgOut[(i * w + j) * 3 + k] = static_cast<unsigned char>(avg);
            }
        }
    }
}

void NNdownscaleRGB(OCTET* ImgIn, OCTET* ImgOut, int nW, int nH, float factor)
{
    int w = static_cast<int>(nW * factor);
    int h = static_cast<int>(nH * factor);

    for (int i = 0; i < h; ++i)
    {
        int y = static_cast<int>(i / factor);

        for (int j = 0; j < w; ++j)
        {
            int x = static_cast<int>(j / factor);

            for (int k = 0; k < 3; ++k)
            {
                ImgOut[(i * w + j) * 3 + k] = ImgIn[(y * nW + x) * 3 + k];
            }
        }
    }
}

void NNupsample(OCTET* ImgIn, OCTET* ImgOut, int nW, int nH, int factor)
{
    int w = nW * factor;
    int h = nH * factor;

    for (int i = 0; i < h; ++i)
    {
        int y = i / factor;
        for (int j = 0; j < w; ++j)
        {
            int x = j / factor;
            for (int k = 0; k < 3; ++k)
            {
                ImgOut[(i * w + j) * 3 + k] = ImgIn[(y * nW + x) * 3 + k];
            }
        }
    }
}

void quantify(OCTET* ImgIn, OCTET* ImgOut, int nW, int nH, float factor)
{
	int w = (int) (nW * factor);
	int h = (int) (nH * factor);
	for (int i = 0; i < nH; i++) {
	    for (int j = 0; j < nW; j++) {

	        // calculate the corresponding pixel in the downsampled image
	        int x = (j / factor);
	        int y = (i / factor);

	        // calculate the four nearest pixels in the downscaled image
	        int topLeft = ImgIn[y * w + x];
	        int topRight = ImgIn[y * w + x + 1];
	        int bottomLeft = ImgIn[(y + 1) * w + x];
	        int bottomRight = ImgIn[(y + 1) * w + x + 1];

	        // calculate the fractional distances between the target pixel and the nearest pixels
	        float xFraction = static_cast<float>(x) / 2 - x;
	        float yFraction = static_cast<float>(y) / 2 - y;

	        // perform bilinear interpolation
	        int topAvg = topLeft * (1 - xFraction) + topRight * xFraction;
	        int bottomAvg = bottomLeft * (1 - xFraction) + bottomRight * xFraction;
	        int averageValue = topAvg * (1 - yFraction) + bottomAvg * yFraction;

	        // store the interpolated pixel value in the upscaled image
	        ImgOut[y * w + x] = averageValue;
	    }
	}

}