#include "stdafx.h"
#include<iostream>
#include "./gdal/gdal_priv.h"
#pragma comment(lib, "gdal_i.lib")
using namespace std;

int main()
{
	GDALDataset* poSrcDs;
	GDALDataset* poDstDs;
	int imgXlen, imgYlen, bandNum;
	char* srcPath = "pic.jpg";
	char* dstPath = "ans.tif";
	GByte* buffTmp;
	GByte* copyTmp;
	int StartX[2] = { 300 ,500};
	int StartY[2] = { 300 ,500};
	int tmpXlen[2] = { 100,50 };
	int tmpYlen[2] = { 50,100 };
	int value[2] = { 255, 0 };
	/*ע������*/
	GDALAllRegister();
	poSrcDs = (GDALDataset*)GDALOpenShared(srcPath, GA_ReadOnly);
	/*��ȡ��ȣ��߶ȺͲ�����*/
	imgXlen = poSrcDs->GetRasterXSize();
	imgYlen = poSrcDs->GetRasterYSize();
	bandNum = poSrcDs->GetRasterCount();
	buffTmp = (GByte*)CPLMalloc(tmpXlen[0] *tmpYlen[0]*sizeof(GByte));
	copyTmp = (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte));
	poDstDs = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(dstPath, imgXlen, imgYlen, bandNum, GDT_Byte, NULL);
	/*����ͼ��*/
	for (int i = 0; i < bandNum; i++) {
		poSrcDs->GetRasterBand(i + 1)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, copyTmp, imgXlen, imgYlen, GDT_Byte, 0, 0);
		poDstDs->GetRasterBand(i + 1)->RasterIO(GF_Write, 0, 0, imgXlen, imgYlen, copyTmp, imgXlen, imgYlen, GDT_Byte, 0, 0);
	}
	for (int i = 0; i < 2; i++) {
		buffTmp = (GByte*)CPLMalloc(tmpXlen[i] * tmpYlen[i] * sizeof(GByte));
		/*����������ֵ*/
		for (int j = 0; j < tmpYlen[i]; j++) {
			for (int k = 0; k < tmpXlen[i]; k++) {
				buffTmp[j*tmpXlen[i] + k] = (GByte)value[i];
			}
		}
		/*�滻*/
		for (int j = 0; j < bandNum; j++) {
			poDstDs->GetRasterBand(j + 1)->RasterIO(GF_Write, StartX[i], StartY[i], tmpXlen[i], tmpYlen[i], buffTmp, tmpXlen[i], tmpYlen[i], GDT_Byte, 0, 0);
		}
		CPLFree(buffTmp);
	}
	/*����ڴ�*/
	GDALClose(poSrcDs);
	GDALClose(poDstDs);
    return 0;
}

