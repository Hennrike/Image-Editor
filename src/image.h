#ifndef IMAGE_H
#define IMAGE_H

#include "bmp.h"

#include <stdio.h>
#include <stdlib.h>


LONG width;
LONG height;
struct RGBTRIPLE **data;


void clear() {
    for (int i = 0; i < height; i++) {
        free(data[i]);
        data[i] = NULL;
    }
    free(data);
    data = NULL;

    width = 0;
    height = 0;
}


int read(const char *filename) {
    FILE *inptr;

    inptr = fopen(filename, "rb");
    if(inptr == NULL) {
        return 1;
    }

    struct BITMAPFILEHEADER bf;
    fread(&bf, sizeof(struct BITMAPFILEHEADER), 1, inptr);

    struct BITMAPINFOHEADER bi;
    fread(&bi, sizeof(struct BITMAPINFOHEADER), 1, inptr);

    width = bi.biWidth;
    height = bi.biHeight;

    int padding = (4 - width * sizeof(struct RGBTRIPLE) % 4) % 4;

    int rowSize = width * sizeof(struct RGBTRIPLE) + padding;

    BYTE *row = malloc(rowSize * sizeof(BYTE));
    data = malloc(height * sizeof(struct RGBTRIPLE *));
    for (int i = 0; i < height; i++) {
        data[i] = malloc(width * sizeof(struct RGBTRIPLE));
        fread(row, sizeof(BYTE), rowSize, inptr);
        for (int j = 0; j < width; j++) {
            data[i][j] = (struct RGBTRIPLE){
                row[j * 3],
                row[j * 3 + 1],
                row[j * 3 + 2]
            };
        }
    }

    fclose(inptr);
    free(row);

    return 0;
}


int write(const char *filename) {
    FILE *outptr;

    outptr = fopen(filename, "wb");
    if (outptr == NULL) {
        return 1;
    }

    int padding = (4 - width * sizeof(struct RGBTRIPLE) % 4) % 4;
    DWORD imgSize = height * (width * sizeof(struct RGBTRIPLE) + padding);

    struct BITMAPFILEHEADER bf;
    bf.bfType = 0x4D42;
    bf.bfSize = sizeof(struct BITMAPFILEHEADER) + sizeof(struct BITMAPINFOHEADER) + imgSize;
    bf.bfReserved1 = 0;
    bf.bfReserved2 = 0;
    bf.bfOffBits = sizeof(struct BITMAPFILEHEADER) + sizeof(struct BITMAPINFOHEADER);
    fwrite(&bf, sizeof(struct BITMAPFILEHEADER), 1, outptr);

    struct BITMAPINFOHEADER bi;
    bi.biSize = sizeof(struct BITMAPINFOHEADER);
    bi.biWidth = width;
    bi.biHeight = height;
    bi.biPlanes = 1;
    bi.biBitCount = 24;
    bi.biCompression = 0;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;
    fwrite(&bi, sizeof(struct BITMAPINFOHEADER), 1, outptr);

    for (int i = 0; i < height; i++) {
        fwrite(data[i], sizeof(struct RGBTRIPLE), width, outptr);

        if (padding > 0) {
            for (int j = 0; j < padding; j++) {
                fputc(0x00, outptr);
            }
        }
    }

    fclose(outptr);

    return 0;
}

#endif
