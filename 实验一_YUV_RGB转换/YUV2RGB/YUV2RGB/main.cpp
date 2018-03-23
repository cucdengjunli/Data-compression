#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include<iostream>
#include "yuv2rgb.h"

using namespace std;

#define u_int8_t	unsigned __int8  //8-bit integer
#define u_int		unsigned __int32 //32-bit integer
#define u_int32_t	unsigned __int32
#define FALSE		false
#define TRUE		true

/*
 * yuv2rgb
 * required arg1 should be the input RAW yuv422 file
 * required arg2 should be the output RAW Yrgb444 file
 */ 

int main(int argc, char** argv)
{
	/* variables controlable from command line */
	u_int frameWidth = 352;			/* --width=<uint> */
	u_int frameHeight = 240;		/* --height=<uint> */
	bool flip = TRUE;				/* --flip */
	
	/*cout << frameWidth * frameHeight << endl;
	system("pause");*/
	char* rgbFileName = NULL;
	char* yuvFileName = NULL;

	FILE* rgbFile = NULL;
	FILE* yuvFile = NULL;

	u_int8_t* rgbBuf = NULL;
	u_int8_t* yBuf = NULL;
	u_int8_t* uBuf = NULL;
	u_int8_t* vBuf = NULL;
	u_int8_t* yuvBuf = NULL;
	u_int32_t videoFramesWritten = 0;

	yuvFileName = argv[1];
	rgbFileName = argv[2];

	frameWidth = atoi(argv[3]);//atoi() string to integer
	frameHeight = atoi(argv[4]);
	
	yuvFile = fopen(yuvFileName, "rb");
	if (yuvFile == NULL)
	{
		printf("cannot find yuv file\n");
		exit(1);
	}
	else
	{
		printf("The input yuv file is %s\n", yuvFileName);
	}
	
	rgbFile = fopen(rgbFileName, "wb");
	if (rgbFile == NULL)
	{
		printf("cannot find rgb file\n");
		exit(1);
	}
	else
	{
		printf("The output rgb file is %s\n", rgbFileName);
	}

	rgbBuf = (u_int8_t*)malloc(frameWidth * frameHeight * 3);

	yuvBuf = (u_int8_t*)malloc((u_int)frameWidth * frameHeight * 1.5);

	if (rgbBuf == NULL || yuvBuf == NULL)
	{
		printf("no enought memory\n");
		exit(1);
	}
	//freopen("out.txt", "w", stdout);
	u_int en = frameHeight * frameHeight;
	while (fread(yuvBuf, 1, (u_int)(frameWidth * frameHeight * 1.5), yuvFile)){
		//size_t fread ( void * ptr, size_t size, size_t count, FILE * stream );
		yBuf = yuvBuf;
		uBuf = yuvBuf + (u_int)(frameWidth * frameHeight);
		vBuf = uBuf + (u_int)(frameWidth * frameHeight * 0.25);
		/*for (int i = 0; i < en; i++) {
			cout << "i = " <<i << "yBUf = "<< (int)yBuf[i] << endl;
		}
		for (int i = 0; i < en/4; i++) {
			cout << "i = " << i << "uBUf = " << (int)uBuf[i] << endl;
		}
		for (int i = 0; i < en / 4; i++) {
			cout << "i = " << i << "vBUf = " << (int)vBuf[i] << endl;
		}*/
		system("pause");
		if(YUV2RGB(frameWidth, frameHeight, yBuf, uBuf, vBuf, rgbBuf, flip)){
			printf("error");
			return 0;
		}
		fwrite(rgbBuf, 1, frameWidth * frameHeight * 3, rgbFile);
		printf("\r...%d\n", ++videoFramesWritten);
	}

	printf("\n%u %ux%u video frames written\n", videoFramesWritten, frameWidth, frameHeight);

	/* cleanup */
	free(yuvBuf);
	free(rgbBuf);
	

	fclose(rgbFile);
	fclose(yuvFile);
	system("pause");
	return 0;
}

