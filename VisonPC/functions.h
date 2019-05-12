#include <windows.h>
#include <string>
#include <iostream>
#include <string.h>
#include<winuser.h>
#include <CommDlg.h>
#include<fstream>
#include <mmsystem.h>
#include <cassert>
#include <d2d1.h>
#include <d2d1helper.h>
#include <wincodec.h>
#include "opencv2/opencv.hpp"
#include <cmath>
#include <math.h>
#include <thread> 
#include "resource.h"
using namespace std;
using namespace cv;

#define M_PI 3.14159265358979323846 
#define Euler 2.718281828

#define MEDIA						500
#define LAPLACIAN					501
#define SUB_MEDIA					502
#define ALAPLACIAN					503
#define GAUSIAN						504
#define MEDIA_POND					505
#define NORTH						506
#define EAST						507
#define CSOBEL						508
#define FSOBEL						509
#define GRAY						510
#define THRESH						512
#define SIMPLE_HITOGRAM				513
#define EXPONENCIAL_HISTOGRAM		514
#define UNIFORM_HISTOGRAM			515


#pragma region VARIABLES GLOBALES
int ancho, alto;
HBITMAP imageBackground;
HBITMAP imageObturador;
HBITMAP iconVideoMode;
HBITMAP iconCamMode;
HBITMAP	iconTakePhoto;
HBITMAP iconPlayVideo;
HBITMAP iconStopVideo;
HBITMAP iconRecordVideo;
float tamanioIdealX = 640;
float tamanioIdealY = 480;
float escalaX = 1;
float escalaY = 1;
int filas=0;
int columnas=0;
Mat imgPrueba;
Mat dstPrueba = Mat(tamanioIdealY, tamanioIdealX, CV_8UC3);
Mat final=Mat(tamanioIdealY, tamanioIdealX, CV_8UC3);
uchar **gausian_mask;
uchar **media_mask;
uchar **mediaPondered_mask;
int **laplacian_mask;
int **subsMedia_mask;
int **alaplacian_mask;
int **nort_mask;
int **east_mask;
int **cSobel_mask;
int **fSobel_mask;
int *histogramR;
int *histogramG;
int *histogramB;


double cdfR[256] = { 0 };
double cdfG[256] = { 0 };
double cdfB[256] = { 0 };
float prkR[256] = { 0 };
float prkG[256] = { 0 };
float prkB[256] = { 0 };
uchar skRnormal[256] = { 0 };
uchar skGnormal[256] = { 0 };
uchar skBnormal[256] = { 0 };
uchar skRexp[256] = { 0 };
uchar skGexp[256] = { 0 };
uchar skBexp[256] = { 0 };
uchar skRuniform[256] = { 0 };
uchar skGuniform[256] = { 0 };
uchar skBuniform[256] = { 0 };
int R=0;
int G=0;
int B=0;
int tonominR = 0;
int tonomaxR = 0;
int tonominG = 0;
int tonomaxG = 0;
int tonominB = 0;
int tonomaxB = 0;

int orden[4] = {0};
int contador;

bool checkedFiltroGaus=false;
bool checkedFiltroMedia = false;
bool checkedFiltroMediaP = false;
bool checkedFiltroLaplacian = false;
bool checkedFiltroSubMedia = false;
bool checkedFiltroALaplacian = false;
bool checkedFiltroNorth = false;
bool checkedFiltroEast = false;
bool checkedFiltroCSobel = false;
bool checkedFiltroFSobel = false;
bool checkedFiltroGray = false;
bool checkedFiltroThreshold = false;
bool checkedFiltroHistogramSimple = false;
bool checkedFiltroHistogramExponencial = false;
bool checkedFiltroHistogramUniforme = false;

float alfa = 0.01;
#pragma endregion

#pragma region VARIABLES DE VENTANAS
HWND ghPIAD = 0;
HINSTANCE hIntancePIAD = 0;
#pragma endregion

#pragma region VARIABLES DE CONTROLES-ghPIAD
static HWND hDisplay = 0;
static HWND hBackground = 0;
static HWND hStaticPrint = 0;
static HWND hButtonGenerar = 0;
static HWND hButtonPhotoMode = 0;
static HWND hButtonVideoMode = 0;
static HWND hButtonTakePhoto = 0;
static HWND hButtonPlayVideo = 0;
static HWND hButtonStopVideo = 0;
static HWND hButtonRecordVideo = 0;
static HWND hComboFiltros = 0;
static HWND hCheckFiltroGaus = 0;
static HWND hCheckFiltroMedia = 0;
static HWND hCheckFiltroSubMedia = 0;
static HWND hCheckFiltroMediaPond = 0;
static HWND hCheckFiltroLaplacian = 0;
static HWND hCheckFiltroALaplacian = 0;
static HWND hCheckFiltroNorth = 0;
static HWND hCheckFiltroEast = 0;
static HWND hCheckFiltroCSobel = 0;
static HWND hCheckFiltroFSobel = 0;
static HWND hCheckFiltroGray = 0;
static HWND hCheckFiltroThreshold = 0;
static HWND hCheckFiltroHitogramNormal = 0;
static HWND hCheckFiltroHistogramExponencial = 0;
static HWND hCheckFiltroHistogramUniform = 0;



#pragma endregion

#pragma region VARIABLES IMAGE VIDEO
VideoCapture *cam;
VideoWriter *wrt;
Mat frame;
bool captureVideo = false;
bool takePhoto = false;
bool takeVideo = false;
HBITMAP videoFrame;
thread *videoCap;
double start=0;
double elapsed = 0;
char text[32] = "";
#pragma endregion

#pragma region PROTORIPOS DE FUNCIONES
void reziseWindow(int an,int al);
auto ConvertCVMatToBMP(Mat frame)->HBITMAP;
void cargarImagenes();
void calcularTamañoIdeal(Size tamaño);
void centerElementX(HWND element);
void changeBackgroundColor(Mat image, uchar r, uchar g, uchar b);
void ocultarControlesPhotoMode();
void mostrarControlesPhotoMode();
void toGray(Mat& image,Mat& gray);
void toSepia(Mat& src,Mat& dst);
void pixelate(Mat& src,Mat& dst,int pixel_size);
uchar** GenerateGausianMask(int x,int y, float sigma);
uchar** GenerateMediaMask(int x,int y);
uchar** GenerateMediaPonderedMask(int x, int y,int p);
uchar** GenerateSubMediaMask(int x, int y);
void ApliMask(Mat& src,Mat& dst,int x,int y,uchar** mask);
#pragma endregion



HICON mainicon;

#pragma region PROTOTIPOS DE CALLBACKS
LRESULT CALLBACK EditDlgProc(HWND ghDialog, UINT mensaje, WPARAM wParam, LPARAM lParam);
#pragma endregion


#pragma region CLASE FILTROS
static class filtros{
public:

	filtros(){

	}

	static void threshold(Mat& src, Mat&dst, int limint_max, int limit_min){
		for (int i = 0; i < src.rows; i++)
		{
			Vec3b* imgrow = src.ptr<Vec3b>(i);
			Vec3b* grayrow = dst.ptr<Vec3b>(i);

			for (int j = 0; j < src.cols; j++)
			{
				uchar B = imgrow[j][0];
				uchar G = imgrow[j][1];
				uchar R = imgrow[j][2];
				int val = (B + G + R) / 3;
				if (val>limint_max){
					grayrow[j][0] = 255;
					grayrow[j][1] = 255;
					grayrow[j][2] = 255;
				}
				else if (val<limit_min){
					grayrow[j][0] = 0;
					grayrow[j][1] = 0;
					grayrow[j][2] = 0;
				}
			}
		}
	}

	static void toGray(Mat& image, Mat& gray){
		for (int i = 0; i < image.rows; i++)
		{
			Vec3b* imgrow = image.ptr<Vec3b>(i);
			Vec3b* grayrow = gray.ptr<Vec3b>(i);

			for (int j = 0; j < image.cols; j++)
			{
				uchar B = imgrow[j][0];
				uchar G = imgrow[j][1];
				uchar R = imgrow[j][2];

				grayrow[j][0] = (B + G + R) / 3;
				grayrow[j][1] = (B + G + R) / 3;
				grayrow[j][2] = (B + G + R) / 3;

			}
		}
	}

	static void toSepia(Mat& src, Mat& dst){
		for (int i = 0; i < src.rows; i++)
		{
			Vec3b* imgrow = src.ptr<Vec3b>(i);
			Vec3b* grayrow = dst.ptr<Vec3b>(i);

			for (int j = 0; j < src.cols; j++)
			{
				int B = (imgrow[j][2] * .272) + (imgrow[j][1] * .534) + (imgrow[j][0] * .131);
				int G = (imgrow[j][2] * .349) + (imgrow[j][1] * .686) + (imgrow[j][0] * .168);
				int R = (imgrow[j][2] * .349) + (imgrow[j][1] * .686) + (imgrow[j][0] * .168);

				if (B > 255) {
					B = 255;
				}
				if (G > 255) {
					G = 255;
				}
				if (R > 255) {
					R = 255;
				}
				grayrow[j][2] = R;
				grayrow[j][1] = G;
				grayrow[j][0] = B;

			}
		}
	}

	static void pixelate(Mat& src, Mat& dst, int pixel_size) {
		try {
			// crear cv::Mat de salida, de igual tamano que la imagen src
			dst.create(src.rows, src.cols, src.type());

			Rect rect;

			for (int r = 0; r < src.rows; r += pixel_size)
			{
				for (int c = 0; c < src.cols; c += pixel_size)
				{
					rect.x = c;
					rect.y = r;
					rect.width = c + pixel_size < src.cols ? pixel_size : src.cols - c;
					rect.height = r + pixel_size < src.rows ? pixel_size : src.rows - r;

					// obtener el color promedio del area indicada
					Scalar color = mean(Mat(src, rect));

					// pintar el area indicada con el color obtenido
					rectangle(dst, rect, color, CV_FILLED);
				}
			}
		}
		catch (cv::Exception &ex) {
			cout << ex.what() << endl;
		}
	}

	static uchar** GenerateGausianMask(int x, int y, float sigma) {
		double **M = new double*[x];
		for (int i = 0; i < y; i++)
			M[i] = new double[x];

		uchar **MR = new uchar*[x];
		for (int i = 0; i < y; i++)
			MR[i] = new uchar[x];

		for (int i = 0; i < x; i++) {
			for (int j = 0; j < y; j++)
			{
				double potencia = (-(pow(i, 2)) + (-(pow(j, 2)))) / pow(sigma, 2);
				double result = pow(Euler, potencia) / (2 * M_PI)*(pow(sigma, 2));
				M[i][j] = result;
			}
		}

		double minResult;

		for (int i = 0; i < x; i++) {
			for (int j = 0; j < y; j++)
			{
				if ((i + 1)<x && (j + 1)<y) {
					if (M[i + 1][j + 1]<M[i][j]) {
						minResult = M[i + 1][j + 1];
					}
				}
			}
		}

		for (int i = 0; i < x; i++) {
			for (int j = 0; j < y; j++)
			{
				MR[i][j] = (int)(M[i][j] / minResult);

			}
		}

		return MR;
	}

	static void GenerateMediaMask(int x, int y){
		media_mask = new uchar*[x];
		for (int i = 0; i < y; i++)
			media_mask[i] = new uchar[x];

		for (int i = 0; i < x; i++) {
			for (int j = 0; j < y; j++)
			{
				media_mask[i][j] = 1;
			}
		}
	}

	static void GenerateMediaPMask(int x, int y, int pondered){
		mediaPondered_mask = new uchar*[x];
		for (int i = 0; i < y; i++)
			mediaPondered_mask[i] = new uchar[x];

		for (int i = 0; i < x; i++) {
			for (int j = 0; j < y; j++)
			{
				if (i == i / 2 && j == j / 2){
					mediaPondered_mask[i][j] = pondered;
				}
				else{
					mediaPondered_mask[i][j] = 1;
				}

			}
		}
	}

	static void GenerateSubMediaMask(int xs, int ys){

		int fila_dinamica = xs;
		int columna_dinamica = ys;

		subsMedia_mask = (int **)malloc(ys);
		int i = 0;
		while (i < columna_dinamica) {
			subsMedia_mask[i] = new int[xs];
			i++;
		}


		for (int i = 0; i < xs; i++) {
			for (int j = 0; j < ys; j++)
			{
				if (i == (xs / 2) && j == (ys / 2))
					subsMedia_mask[i][j] = 8;
				else
					subsMedia_mask[i][j] = -1;

			}
		}

	}

	static void GenerateLaplacianMask(){
		laplacian_mask = new int*[3];
		for (int i = 0; i < 3; i++)
			laplacian_mask[i] = new int[3];


		laplacian_mask[0][0] = 0;
		laplacian_mask[0][1] = 1;
		laplacian_mask[0][2] = 0;

		laplacian_mask[1][0] = 1;
		laplacian_mask[1][1] = -4;
		laplacian_mask[1][2] = 1;

		laplacian_mask[2][0] = 0;
		laplacian_mask[2][1] = 1;
		laplacian_mask[2][2] = 0;
	}

	static void GenerateALaplacianMask(){
		alaplacian_mask = new int*[3];
		for (int i = 0; i < 3; i++)
			alaplacian_mask[i] = new int[3];


		alaplacian_mask[0][0] = 0;
		alaplacian_mask[0][1] = -1;
		alaplacian_mask[0][2] = 0;

		alaplacian_mask[1][0] = -1;
		alaplacian_mask[1][1] = 5;
		alaplacian_mask[1][2] = -1;

		alaplacian_mask[2][0] = 0;
		alaplacian_mask[2][1] = -1;
		alaplacian_mask[2][2] = 0;
	}

	static void GenerateNorthMask(){
		nort_mask = new int*[3];
		for (int i = 0; i < 3; i++)
			nort_mask[i] = new int[3];


		nort_mask[0][0] = 1;
		nort_mask[0][1] = 1;
		nort_mask[0][2] = 1;

		nort_mask[1][0] = 1;
		nort_mask[1][1] = 2;
		nort_mask[1][2] = 1;

		nort_mask[2][0] = -1;
		nort_mask[2][1] = -1;
		nort_mask[2][2] = -1;
	}

	static void GenerateEastMask(){
		east_mask = new int*[3];
		for (int i = 0; i < 3; i++)
			east_mask[i] = new int[3];


		east_mask[0][0] = -1;
		east_mask[0][1] = 1;
		east_mask[0][2] = 1;

		east_mask[1][0] = -1;
		east_mask[1][1] = 2;
		east_mask[1][2] = 1;

		east_mask[2][0] = -1;
		east_mask[2][1] = 1;
		east_mask[2][2] = 1;
	}

	static void GenerateCSobelMask(){
		cSobel_mask = new int*[3];
		for (int i = 0; i < 3; i++)
			cSobel_mask[i] = new int[3];


		cSobel_mask[0][0] = -1;
		cSobel_mask[0][1] = 0;
		cSobel_mask[0][2] = 1;

		cSobel_mask[1][0] = -2;
		cSobel_mask[1][1] = 0;
		cSobel_mask[1][2] = 2;

		cSobel_mask[2][0] = -1;
		cSobel_mask[2][1] = 0;
		cSobel_mask[2][2] = 1;
	}

	static void GenerateFSobelMask(){
		fSobel_mask = new int*[3];
		for (int i = 0; i < 3; i++)
			fSobel_mask[i] = new int[3];


		fSobel_mask[0][0] = -1;
		fSobel_mask[0][1] = -2;
		fSobel_mask[0][2] = -1;

		fSobel_mask[1][0] = 0;
		fSobel_mask[1][1] = 0;
		fSobel_mask[1][2] = 0;

		fSobel_mask[2][0] = 1;
		fSobel_mask[2][1] = 2;
		fSobel_mask[2][2] = 1;
	}

	

	

	

	static void EcualizacionSimple(Mat& src){

		histogramR = new int[255]{0};
		histogramG = new int[255]{0};
		histogramB = new int[255]{0};
		//Sacar el histograma
		for (int i = 0; i < src.rows; i++){
			Vec3b* color = src.ptr<Vec3b>(i);
			for (int j = 0; j < src.cols; j++){
				histogramR[color[j][2]]++;
				histogramG[color[j][1]]++;
				histogramB[color[j][0]]++;

			}
		}
		// sacar el cdf
		int acumuladoR = 0;
		int acumuladoG = 0;
		int acumuladoB = 0;

		for (int i = 0; i < 256; i++){
			acumuladoR += histogramR[i];
			acumuladoG += histogramG[i];
			acumuladoB += histogramB[i];

			cdfR[i] = acumuladoR;
			cdfG[i] = acumuladoG;
			cdfB[i] = acumuladoB;

		}
		//sacamos el prk
		double cantPixel = src.rows*src.cols;
		for (int i = 0; i < 256; i++){
			prkR[i] = cdfR[i] / cantPixel;
			prkG[i] = cdfG[i] / cantPixel;
			prkB[i] = cdfB[i] / cantPixel;
		}
		//sacamos el sk
		for (int i = 0; i < 256; i++){
			skRnormal[i] = prkR[i] * 255;
			skGnormal[i] = prkG[i] * 255;
			skBnormal[i] = prkB[i] * 255;
		}
	}

	static void AplyHistogramSimple(Mat& src,Mat& dst){
		EcualizacionSimple(src);
		for (int r = 0; r < src.rows; r++){
			Vec3b* color = src.ptr<Vec3b>(r);
			Vec3b* aply = dst.ptr<Vec3b>(r);
			for (int c = 0; c < src.cols; c++){
				R = color[c][2];
				G = color[c][1];
				B = color[c][0];

				aply[c][2] = skRnormal[R];
				aply[c][1] = skGnormal[G];
				aply[c][0] = skBnormal[B];
			}
		}
	}
	
	 
	static void EcualizacionExponencial(Mat& src){

		histogramR = new int[256]{0};
		histogramG = new int[256]{0};
		histogramB = new int[256]{0};
		//Sacar el histograma
		for (int i = 0; i < 480; i++){
			Vec3b* color = src.ptr<Vec3b>(i);
			for (int j = 0; j < 640; j++){
				histogramR[color[j][2]]++;
				histogramG[color[j][1]]++;
				histogramB[color[j][0]]++;

			}
		}
		// sacar el cdf
		double acumuladoR = 0;
		double acumuladoG = 0;
		double acumuladoB = 0;

		for (int i = 0; i < 256; i++){
			acumuladoR += histogramR[i];
			acumuladoG += histogramG[i];
			acumuladoB += histogramB[i];

			cdfR[i] = acumuladoR;
			cdfG[i] = acumuladoG;
			cdfB[i] = acumuladoB;

		}
		//sacamos el prk
		double cantPixel = src.rows*src.cols;
		for (int i = 0; i < 256; i++){
			if ((cdfR[i] / cantPixel)==1){
				prkR[i] = 0.9;
			}
			else{
				prkR[i] = cdfR[i] / cantPixel;
			}

			if ((cdfG[i] / cantPixel) == 1){
				prkG[i] = 0.9;
			}
			else{
				prkG[i] = cdfG[i] / cantPixel;

			}
			if ((cdfB[i] / cantPixel) == 1){
				prkB[i] = 0.9;
			}
			else{
				prkB[i] = cdfB[i] / cantPixel;
			}
		}

		//sacamos el tono minimo y el maximo
		

		bool minR = false, maxR = false;
		bool minG = false, maxG = false;
		bool minB = false, maxB = false;

		for (int i = 0; i < 256; i++){
			if (histogramR[i] != 0 && !minR){
				tonominR = i;
				minR = true;
			}
			if (histogramG[i] != 0 && !minG){
				tonominG = i;
				minG = true;
			}
			if (histogramB[i] != 0 && !minB){
				tonominB = i;
				minB = true;
			}	
			if (minR && minG && minB){
				break;
			}
		}

		/*for (int i = 255; i > 0; i--){
			if (histogramR[i] != 0 && !maxR){
				tonomaxR = i;
				maxR = true;
			}
			if (histogramG[i] != 0 && !maxG){
				tonomaxG = i;
				maxG = true;
			}
			if (histogramB[i] != 0 && !maxB){
				tonomaxB = i;
				maxB = true;
			}
		}*/
		//sacamos el sk
		for (int i = 0; i < 256; i++){
			
			skRexp[i] = tonominR - (1 / alfa)*(log(1 - prkR[i]));
			skGexp[i] = tonominG - (1 / alfa)*(log(1 - prkG[i]));
			skBexp[i] = tonominB - (1 / alfa)*(log(1 - prkB[i]));
		}
	}

	static void AplyHistogramExponencial(Mat& src, Mat& dst){
		EcualizacionExponencial(src);
		for (int r = 0; r < src.rows; r++){
			Vec3b* color = src.ptr<Vec3b>(r);
			Vec3b* aply = dst.ptr<Vec3b>(r);
			for (int c = 0; c < src.cols; c++){
				R = color[c][2];
				G = color[c][1];
				B = color[c][0];

				aply[c][2] = skRexp[R];
				aply[c][1] = skGexp[G];
				aply[c][0] = skBexp[B];
			}
		}
	}
	static void EcualizacionUniform(Mat& src){

		histogramR = new int[256]{0};
		histogramG = new int[256]{0};
		histogramB = new int[256]{0};
		//Sacar el histograma
		for (int i = 0; i < 480; i++){
			Vec3b* color = src.ptr<Vec3b>(i);
			for (int j = 0; j < 640; j++){
				histogramR[color[j][2]]++;
				histogramG[color[j][1]]++;
				histogramB[color[j][0]]++;

			}
		}
		// sacar el cdf
		double acumuladoR = 0;
		double acumuladoG = 0;
		double acumuladoB = 0;

		for (int i = 0; i < 256; i++){
			acumuladoR += histogramR[i];
			acumuladoG += histogramG[i];
			acumuladoB += histogramB[i];

			cdfR[i] = acumuladoR;
			cdfG[i] = acumuladoG;
			cdfB[i] = acumuladoB;

		}
		//sacamos el prk
		double cantPixel = src.rows*src.cols;
		for (int i = 0; i < 256; i++){
			if ((cdfR[i] / cantPixel) == 1){
				prkR[i] = 0.9;
			}
			else{
				prkR[i] = cdfR[i] / cantPixel;
			}

			if ((cdfG[i] / cantPixel) == 1){
				prkG[i] = 0.9;
			}
			else{
				prkG[i] = cdfG[i] / cantPixel;

			}
			if ((cdfB[i] / cantPixel) == 1){
				prkB[i] = 0.9;
			}
			else{
				prkB[i] = cdfB[i] / cantPixel;
			}
		}

		//sacamos el tono minimo y el maximo


		bool minR = false, maxR = false;
		bool minG = false, maxG = false;
		bool minB = false, maxB = false;

		for (int i = 0; i < 256; i++){
			if (histogramR[i] != 0 && !minR){
				tonominR = i;
				minR = true;
			}
			if (histogramG[i] != 0 && !minG){
				tonominG = i;
				minG = true;
			}
			if (histogramB[i] != 0 && !minB){
				tonominB = i;
				minB = true;
			}
			if (minR && minG && minB){
				break;
			}
		}

		for (int i = 255; i > 0; i--){
			if (histogramR[i] != 0 && !maxR){
			tonomaxR = i;
			maxR = true;
			}
			if (histogramG[i] != 0 && !maxG){
			tonomaxG = i;
			maxG = true;
			}
			if (histogramB[i] != 0 && !maxB){
			tonomaxB = i;
			maxB = true;
			}
		}
		//sacamos el sk
		for (int i = 0; i < 256; i++){

			skRuniform[i] = ((tonomaxR-tonominR)*prkR[i])+tonominR;
			skGuniform[i] = ((tonomaxG-tonominG)*prkG[i])+tonominG;
			skBuniform[i] = ((tonomaxB-tonominB)*prkB[i])+tonominB;
		}
	}

	static void AplyHistogramUniforme(Mat& src, Mat& dst){
		EcualizacionUniform(src);
		for (int r = 0; r < src.rows; r++){
			Vec3b* color = src.ptr<Vec3b>(r);
			Vec3b* aply = dst.ptr<Vec3b>(r);
			for (int c = 0; c < src.cols; c++){
				R = color[c][2];
				G = color[c][1];
				B = color[c][0];

				aply[c][2] = skRuniform[R];
				aply[c][1] = skGuniform[G];
				aply[c][0] = skBuniform[B];
			}
		}
	}

	static void ApliMask(Mat& src, Mat& dst, int x, int y, uchar** mask) {
		int bResult = 0, gResult = 0, rResult = 0;
		int xn = (x / 2 != 0) ? x / 2 : 1;
		int yn = (y / 2 != 0) ? y / 2 : 1;
		int div = 0;

		for (int i = 0; i < x; i++) {
			for (int j = 0; j < y; j++)
			{
				div += mask[i][j];
			}
		}

		//COMO EN ESTA MASCARA NO SE HACE NINGUN CALCULO EXTRA PASAMOS LOS VALORES QUE DEBERIAN DE SER PARA SACAR LA MEDIA
		for (int r = 0; r < src.rows; r++)
		{
			Vec3b* color = src.ptr<Vec3b>(r);
			Vec3b* grayrow = dst.ptr<Vec3b>(r);
			for (int c = 0; c < src.cols; c++)
			{

				for (int i = -xn; i < xn + 1; i++) {
					for (int j = -yn; j < yn + 1; j++) {
						if (((c + i) <= src.cols - 1 && (r + j) <= src.rows - 1) && ((c + i) >= 0 && (r + j) >= 0)) {
							uchar b = (color + j)[c + i][0] * mask[i + xn][j + yn];
							uchar g = (color + j)[c + i][1] * mask[i + xn][j + yn];
							uchar r = (color + j)[c + i][2] * mask[i + xn][j + yn];
							bResult += (int)b;
							gResult += (int)g;
							rResult += (int)r;
						}
						else {
							bResult += 0;
							gResult += 0;
							rResult += 0;
						}
					}
				}
				bResult /= div;
				gResult /= div;
				rResult /= div;
				if (bResult > 255) {
					bResult = 255;
				}
				if (gResult > 255) {
					gResult = 255;
				}
				if (rResult > 255) {
					rResult = 255;
				}
				//Vec3b col(bResult, gResult, rResult);

				grayrow[c][0] = bResult;
				grayrow[c][1] = gResult;
				grayrow[c][2] = rResult;

				/*dst.at<Vec3b>(Point(c, r))[0] = bResult;
				dst.at<Vec3b>(Point(c, r))[1] = gResult;
				dst.at<Vec3b>(Point(c, r))[2] = rResult;*/
			}
		}



	}

	static void ApliMask(Mat& src, Mat& dst, int x, int y, int** mask, int op = 0) {
		int bResult = 0, gResult = 0, rResult = 0;
		int xn = (x / 2 != 0) ? x / 2 : 1;
		int yn = (y / 2 != 0) ? y / 2 : 1;
		int div = 0;
		for (int i = 0; i < x; i++) {
			for (int j = 0; j < y; j++)
			{
				div += mask[i][j];
			}
		}

		if (op == LAPLACIAN){
			div = 1;
		}

		if (op == ALAPLACIAN){
			div = 1;
		}


		if (op == SUB_MEDIA){
			div = 9;
		}



		//COMO EN ESTA MASCARA NO SE HACE NINGUN CALCULO EXTRA PASAMOS LOS VALORES QUE DEBERIAN DE SER PARA SACAR LA MEDIA
		for (int r = 0; r < src.rows; r++)
		{
			Vec3b* color = src.ptr<Vec3b>(r);
			Vec3b* grayrow = dst.ptr<Vec3b>(r);
			for (int c = 0; c < src.cols; c++)
			{

				for (int i = -xn; i < xn + 1; i++) {
					for (int j = -yn; j < yn + 1; j++) {
						if (((c + i) <= src.cols - 1 && (r + j) <= src.rows - 1) && ((c + i) >= 0 && (r + j) >= 0)) {
							int b = (color + j)[c + i][0] * mask[i + xn][j + yn];
							int g = (color + j)[c + i][1] * mask[i + xn][j + yn];
							int r = (color + j)[c + i][2] * mask[i + xn][j + yn];
							bResult += (int)b;
							gResult += (int)g;
							rResult += (int)r;
						}
						else {
							bResult += 0;
							gResult += 0;
							rResult += 0;
						}
					}
				}

				bResult /= div;
				gResult /= div;
				rResult /= div;

				if (bResult > 255) {
					bResult = 255;
				}
				if (gResult > 255) {
					gResult = 255;
				}
				if (rResult > 255) {
					rResult = 255;
				}

				if (bResult < 0) {
					bResult = 0;
				}
				if (gResult < 0) {
					gResult = 0;
				}
				if (rResult < 0) {
					rResult = 0;
				}


				//Vec3b col(bResult, gResult, rResult);

				grayrow[c][0] = bResult;
				grayrow[c][1] = gResult;
				grayrow[c][2] = rResult;

				/*dst.at<Vec3b>(Point(c, r))[0] = bResult;
				dst.at<Vec3b>(Point(c, r))[1] = gResult;
				dst.at<Vec3b>(Point(c, r))[2] = rResult;*/
			}
		}



	}

	static void ApliMask(Mat& src, Mat& dst, int x, int y, int** mask, int** mask2) {
		int bResult = 0, gResult = 0, rResult = 0;
		int xn = (x / 2 != 0) ? x / 2 : 1;
		int yn = (y / 2 != 0) ? y / 2 : 1;
		int div = 1;





		//COMO EN ESTA MASCARA NO SE HACE NINGUN CALCULO EXTRA PASAMOS LOS VALORES QUE DEBERIAN DE SER PARA SACAR LA MEDIA
		for (int r = 0; r < src.rows; r++)
		{
			Vec3b* color = src.ptr<Vec3b>(r);
			Vec3b* grayrow = dst.ptr<Vec3b>(r);
			for (int c = 0; c < src.cols; c++)
			{

				for (int i = -xn; i < xn + 1; i++) {
					for (int j = -yn; j < yn + 1; j++) {
						if (((c + i) <= src.cols - 1 && (r + j) <= src.rows - 1) && ((c + i) >= 0 && (r + j) >= 0)) {
							int val1 = mask[i + xn][j + yn];
							int val2 = mask2[i + xn][j + yn];
							int b = (color + j)[c + i][0] * sqrt((val1*val1) + (val2*val2));
							int g = (color + j)[c + i][1] * sqrt((val1*val1) + (val2*val2));
							int r = (color + j)[c + i][2] * sqrt((val1*val1) + (val2*val2));



							bResult += (int)b;
							gResult += (int)g;
							rResult += (int)r;
						}
						else {
							bResult += 0;
							gResult += 0;
							rResult += 0;
						}
					}
				}

				bResult /= div;
				gResult /= div;
				rResult /= div;

				if (bResult > 255) {
					bResult = 255;
				}
				if (gResult > 255) {
					gResult = 255;
				}
				if (rResult > 255) {
					rResult = 255;
				}

				if (bResult < 0) {
					bResult = 0;
				}
				if (gResult < 0) {
					gResult = 0;
				}
				if (rResult < 0) {
					rResult = 0;
				}


				//Vec3b col(bResult, gResult, rResult);

				grayrow[c][0] = bResult;
				grayrow[c][1] = gResult;
				grayrow[c][2] = rResult;

				/*dst.at<Vec3b>(Point(c, r))[0] = bResult;
				dst.at<Vec3b>(Point(c, r))[1] = gResult;
				dst.at<Vec3b>(Point(c, r))[2] = rResult;*/
			}
		}



	}



};

#pragma endregion


#pragma region FUNCIONES
	void reziseWindow(int an, int al) {

		SetWindowPos(
			ghPIAD,
			NULL,
			0,
			0,
			an,
			al,
			SWP_NOMOVE
			);
	}

	void centerElementX(HWND element){

		RECT r;

		GetWindowRect(element, &r); //get window rect of control relative to screen
		POINT pt = { r.left, r.top }; //new point object using rect x, y
		int width = 0;
		width = r.right - r.left;
		int px = 0;
		px = (ancho - width) / 2;
		ScreenToClient(ghPIAD, &pt);
		MoveWindow(element, px, pt.y + 10, r.right - r.left, r.bottom - r.top, TRUE);
	}

	void centerElementX(HWND element,int movex,int movey){

		RECT r;

		GetWindowRect(element, &r); //get window rect of control relative to screen
		POINT pt = { r.left, r.top }; //new point object using rect x, y
		int width = 0;
		width = r.right - r.left;
		int px = 0;
		px = (ancho - width) / 2;
		ScreenToClient(ghPIAD, &pt);
		MoveWindow(element, px+movex, pt.y + movey, r.right - r.left, r.bottom - r.top, TRUE);
	}

	void moveElement(HWND element,POINT pt){
		RECT r;
		GetWindowRect(element, &r); //get window rect of control relative to screen
		ScreenToClient(ghPIAD, &pt);
		MoveWindow(element, pt.x, pt.y + 10, r.right - r.left, r.bottom - r.top, TRUE);
	}

	auto ConvertCVMatToBMP(Mat frame) -> HBITMAP
	{
		auto convertOpenCVBitDepthToBits = [](const int32_t value)
		{
			auto regular = 0u;

			switch (value)
			{
			case CV_8U:
			case CV_8S:
				regular = 8u;
				break;

			case CV_16U:
			case CV_16S:
				regular = 16u;
				break;

			case CV_32S:
			case CV_32F:
				regular = 32u;
				break;

			case CV_64F:
				regular = 64u;
				break;

			default:
				regular = 0u;
				break;
			}

			return regular;
		};

		auto imageSize = frame.size();
		assert(imageSize.width && "invalid size provided by frame");
		assert(imageSize.height && "invalid size provided by frame");

		if (imageSize.width && imageSize.height)
		{
			BITMAPINFOHEADER headerInfo;
			ZeroMemory(&headerInfo, sizeof(headerInfo));

			headerInfo.biSize = sizeof(headerInfo);
			headerInfo.biWidth = imageSize.width;
			headerInfo.biHeight = -(imageSize.height); // negative otherwise it will be upsidedown
			headerInfo.biPlanes = 1;// must be set to 1 as per documentation frame.channels();

			const auto bits = convertOpenCVBitDepthToBits(frame.depth());
			headerInfo.biBitCount = frame.channels() * bits;

			BITMAPINFO bitmapInfo;
			ZeroMemory(&bitmapInfo, sizeof(bitmapInfo));

			bitmapInfo.bmiHeader = headerInfo;
			bitmapInfo.bmiColors->rgbBlue = 0;
			bitmapInfo.bmiColors->rgbGreen = 0;
			bitmapInfo.bmiColors->rgbRed = 0;
			bitmapInfo.bmiColors->rgbReserved = 0;

			auto dc = GetDC(nullptr);
			assert(dc != nullptr && "Failure to get DC");
			auto bmp = CreateDIBitmap(dc,
				&headerInfo,
				CBM_INIT,
				frame.data,
				&bitmapInfo,
				DIB_RGB_COLORS);
			assert(bmp != nullptr && "Failure creating bitmap from captured frame");

			return bmp;
		}
		else
		{
			return nullptr;
		}
	}

	void cargarImagenes() {
		//Mat background = imread("background4.jpg");
		imgPrueba=imread("patron.jpg");
		Mat obturador = imread("obturador.jpg");
		Mat iconVideo = imread("icons/videoIcon2.jpg");
		Mat iconCam = imread("icons/camIcon2.jpg");
		Mat iconTake = imread("icons/iconObturador.jpg");
		Mat iconPlay = imread("icons/iconPlay.jpg");
		Mat iconStop = imread("icons/iconStop.jpg");
		Mat iconRecord = imread("icons/iconRecord.jpg");

		Mat background = Mat(720, 1420, CV_8UC3);

		Mat obturadorR;
		changeBackgroundColor(background, 30, 30, 30);

		calcularTamañoIdeal(obturador.size());
		resize(obturador, obturadorR, obturadorR.size(), escalaX, escalaY, INTER_AREA);

		imageBackground = ConvertCVMatToBMP(background);
		imageObturador = ConvertCVMatToBMP(obturadorR);	
		iconVideoMode = ConvertCVMatToBMP(iconVideo);
		iconCamMode = ConvertCVMatToBMP(iconCam);
		iconTakePhoto = ConvertCVMatToBMP(iconTake);
		iconPlayVideo = ConvertCVMatToBMP(iconPlay);
		iconStopVideo = ConvertCVMatToBMP(iconStop);
		iconRecordVideo = ConvertCVMatToBMP(iconRecord);

	}

	void calcularTamañoIdeal(Size tamaño){
		escalaX = (tamanioIdealX / tamaño.width);
		escalaY = (tamanioIdealY / tamaño.height);
	}

	void changeBackgroundColor(Mat image, uchar r, uchar g, uchar b){
		for (int i = 0; i < image.rows; i++)
		{
			Vec3b* imgrow = image.ptr<Vec3b>(i);


			for (int j = 0; j < image.cols; j++)
			{
				imgrow[j][0] = r;
				imgrow[j][1] = g;
				imgrow[j][2] = b;
			}
		}
	}

	void ocultarControlesPhotoMode(){
		ShowWindow(hButtonTakePhoto, 0);
	}

	void mostrarControlesPhotoMode(){
		ShowWindow(hButtonTakePhoto, 1);
	}

	void ocultarControlesVideoMode(){
		ShowWindow(hButtonPlayVideo, 0);
		ShowWindow(hButtonStopVideo, 0);
	}

	void mostrarControlesVideoMode(){
		ShowWindow(hButtonPlayVideo, 1);
		ShowWindow(hButtonStopVideo, 1);
	}

	void getVideo(HWND ghDialog,int id){
		//Size siz = Size(640, 480);
		
		//calcularTamañoIdeal(siz);
		while (true){
			
			if (captureVideo){
				if (contador>4){
					contador = 4;
					contador = 4;
					contador = 4;

				}





				start = (double)getTickCount();
				cam ->read( frame);

				//resize(frame, dest, dest.size(), escalaX, escalaY, INTER_AREA);
				
				
				for (int i = 0; i < contador; i++){
					if (orden[i] > 0){
						switch (orden[i])
						{
							case MEDIA:{
								filtros::ApliMask(frame, final, 3, 3, media_mask);
								final.copyTo(frame);
							}break;

							case LAPLACIAN:{
								filtros::ApliMask(frame, final, 3, 3, laplacian_mask, LAPLACIAN);
								final.copyTo(frame);
							}break;

							case SUB_MEDIA:{
								filtros::ApliMask(frame, final, 3, 3, subsMedia_mask, SUB_MEDIA);
								final.copyTo(frame);
							}break;

							case ALAPLACIAN:{
								filtros::ApliMask(frame, final, 3, 3, alaplacian_mask, ALAPLACIAN);
								final.copyTo(frame);
							}break;

							case GAUSIAN:{
								filtros::ApliMask(frame, final, 3, 3, gausian_mask);
								final.copyTo(frame);
							}break;

							case MEDIA_POND:{
								filtros::ApliMask(frame, final, 3, 3, mediaPondered_mask);
								final.copyTo(frame);
							}break;

							case NORTH:{
								filtros::ApliMask(frame, final, 3, 3, nort_mask, ALAPLACIAN);
								final.copyTo(frame);
							}break;

							case EAST:{
								filtros::ApliMask(frame, final, 3, 3, east_mask, ALAPLACIAN);
								final.copyTo(frame);
							}break;

							case CSOBEL:{
								filtros::ApliMask(frame, final, 3, 3, cSobel_mask, LAPLACIAN);
								final.copyTo(frame);
							}break;

							case FSOBEL:{
								filtros::ApliMask(frame, final, 3, 3, fSobel_mask, LAPLACIAN);
								final.copyTo(frame);
							}break;

							case GRAY:{
								filtros::toGray(frame, final);
								final.copyTo(frame);
							}break;

							case THRESH:{
								filtros::threshold(frame, final, 198, 197);
								final.copyTo(frame);								
							}break;

							case SIMPLE_HITOGRAM:{
								filtros::AplyHistogramSimple(frame, final);
								final.copyTo(frame);
							}break;

							case EXPONENCIAL_HISTOGRAM:{
								filtros::AplyHistogramExponencial(frame, final);
								final.copyTo(frame);
							}break;

							case UNIFORM_HISTOGRAM:{
								filtros::AplyHistogramUniforme(frame, final);
								final.copyTo(frame);
							}break;
						}
					}

				}
				
				elapsed = 0;
				elapsed = 1 / (((double)getTickCount() - start) / getTickFrequency());

				sprintf_s(text, "%f", elapsed);
				

				if (takePhoto){
					time_t t = time(0);
					tm* now = localtime(&t);
					char anio[4] = "";
					char mes[2] = "";
					char dia[2] = "";
					char hour[2] = "";
					char min[2] = "";
					char seg[2] = "";

					_itoa(now->tm_year + 1900, anio, 10);
					_itoa(now->tm_mon + 1, mes, 10);
					_itoa(now->tm_mday, dia, 10);
					_itoa(now->tm_hour, hour, 10);
					_itoa(now->tm_min, min, 10);
					_itoa(now->tm_sec, seg, 10);

					char namefoto[32] = "";
					strcpy_s(namefoto, "image_");
					strcat_s(namefoto, anio);
					strcat_s(namefoto, mes);
					strcat_s(namefoto, dia);
					strcat_s(namefoto, hour);
					strcat_s(namefoto, min);
					strcat_s(namefoto, seg);

					strcat_s(namefoto, ".jpg");
					imwrite(namefoto, frame);
					takePhoto = false;
					Sleep(500);
				}

				if (takeVideo && wrt!=NULL){
					wrt->write(frame);
				}

				SendDlgItemMessage(ghDialog, IDC_STATIC_PRINT, WM_SETTEXT, 0, (LPARAM)text);
				videoFrame = ConvertCVMatToBMP(frame);
				SendDlgItemMessage(ghDialog, id, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)videoFrame);
			}
		}
	}
	

	void deleteFiltro(int filtro){
		for (int i = 0; i < 4; i++)
		{
			if (orden[i]==filtro){
				if (i < 3){
					orden[i] = 0;
					for (int j = i; j < 4;j++){
						
						if (j < 3){
							orden[j] = orden[j+1];

						}
						else{
							orden[j] = 0;

						}
					}
					
				}
				else{
					orden[i] = 0;
				}
				break;

			}
		}
	}

#pragma endregion




