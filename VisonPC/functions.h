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

#define MEDIA				1
#define LAPLACIAN			2
#define SUB_MEDIA			3
#define ALAPLACIAN			4
#define GAUSIAN				5
#define MEDIA_POND			6
#define NORTH				7
#define EAST				8
#define CSOBEL				9
#define FSOBEL				10
#define GRAY				11
#define THRESH				12


#pragma region VARIABLES GLOBALES
int ancho, alto;
HBITMAP imageBackground;
HBITMAP imageObturador;
HBITMAP iconVideoMode;
HBITMAP iconCamMode;
HBITMAP	iconTakePhoto;
HBITMAP iconPlayVideo;
HBITMAP iconStopVideo;
float tamanioIdealX = 400;
float tamanioIdealY = 400;
float escalaX = 1;
float escalaY = 1;
int filas=0;
int columnas=0;
Mat imgPrueba;
Mat dstPrueba = Mat(tamanioIdealX, tamanioIdealY, CV_8UC3);
Mat final=Mat(tamanioIdealX, tamanioIdealY, CV_8UC3);
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
int histogram[255];
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
		Size siz = Size(640, 480);
		Mat dest;
		calcularTamañoIdeal(siz);
		while (true){
			if (captureVideo){
				start = (double)getTickCount();
				cam ->read( frame);

				resize(frame, dest, dest.size(), escalaX, escalaY, INTER_AREA);
				for (int i = 0; i < contador; i++){
					if (orden[i] > 0){
						switch (orden[i])
						{
						case MEDIA:{
							filtros::ApliMask(dest, final, 3, 3, media_mask);
							final.copyTo(dest);
						}break;

						case LAPLACIAN:{
							filtros::ApliMask(dest, final, 3, 3, laplacian_mask, LAPLACIAN);
							final.copyTo(dest);
						}break;

						case SUB_MEDIA:{
							filtros::ApliMask(dest, final, 3, 3, subsMedia_mask, SUB_MEDIA);
							final.copyTo(dest);
						}break;

						case ALAPLACIAN:{
							filtros::ApliMask(dest, final, 3, 3, alaplacian_mask, ALAPLACIAN);
							final.copyTo(dest);
						}break;

						case GAUSIAN:{
							filtros::ApliMask(dest, final, 3, 3, gausian_mask);
							final.copyTo(dest);
						}break;

						case MEDIA_POND:{
							filtros::ApliMask(dest, final, 3, 3, mediaPondered_mask);
							final.copyTo(dest);
						}break;

						case NORTH:{
							filtros::ApliMask(dest, final, 3, 3, nort_mask, ALAPLACIAN);
							final.copyTo(dest);
						}break;

						case EAST:{
							filtros::ApliMask(dest, final, 3, 3, east_mask, ALAPLACIAN);
							final.copyTo(dest);
						}break;

						case CSOBEL:{
							filtros::ApliMask(dest, final, 3, 3, cSobel_mask, LAPLACIAN);
							final.copyTo(dest);
						}break;

						case FSOBEL:{
							filtros::ApliMask(dest, final, 3, 3, fSobel_mask, LAPLACIAN);
							final.copyTo(dest);
						}break;

						case GRAY:{
							filtros::toGray(dest, final);
							final.copyTo(dest);
						}break;

						case THRESH:{
							filtros::threshold(dest, final, 198, 197);
							final.copyTo(dest);
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
					imwrite(namefoto, dest);
					takePhoto = false;
				}

				if (takeVideo){
					wrt->write(dest);
				}

				SendDlgItemMessage(ghDialog, IDC_STATIC_PRINT, WM_SETTEXT, 0, (LPARAM)text);
				videoFrame = ConvertCVMatToBMP(dest);
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




