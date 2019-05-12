#include "functions.h"



#pragma comment(lib, "winmm.lib")

 int x=3;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, PSTR cmdLine, int nCmdShow)
{
	mainicon = LoadIcon(hInst, "icon.ico");
	cargarImagenes();

	

	ghPIAD = CreateDialog(
		hInst, // Application instance.
		MAKEINTRESOURCE(IDD_PIAD), // Dialog resource ID.
		0, // Parent window--null for no parent.
		(DLGPROC)EditDlgProc);

	
	

	ShowWindow(ghPIAD, SW_MAXIMIZE);

	


	centerElementX(hDisplay);
	     centerElementX(hStaticPrint,-240,240);


	

	MSG mensaje;
	ZeroMemory(&mensaje, sizeof(MSG));
	while (GetMessage(&mensaje, 0, 0, 0))
	{
		// Is the message a dialog message? If so the function
		// IsDialogMessage will return true and then dispatch
		// the message to the dialog window procedure.
		// Otherwise, we process as the message as normal.
		if (ghPIAD == 0 || !IsDialogMessage(ghPIAD, &mensaje))
		{
			TranslateMessage(&mensaje);
			DispatchMessage(&mensaje);
		}

	}

	return (int)mensaje.wParam;
}

LRESULT CALLBACK EditDlgProc(HWND ghDialog, UINT mensaje, WPARAM wParam, LPARAM lParam)
{
	static HBRUSH hbrBkgnd;
	switch (mensaje)
	{
		case WM_INITDIALOG: {
			cam = new VideoCapture(0);
			captureVideo = true;
			thread hilo(getVideo, ghDialog, IDC_DISPLAY);
			videoCap = &hilo;
			videoCap->detach();
			

			ancho = GetSystemMetrics(SM_CXSCREEN);
			alto = GetSystemMetrics(SM_CYSCREEN);
			alto = GetSystemMetrics(SM_CYSCREEN);

			hBackground = GetDlgItem(ghDialog, IDC_BACKGROUND);
			hDisplay= GetDlgItem(ghDialog, IDC_DISPLAY);
			hStaticPrint = GetDlgItem(ghDialog, IDC_STATIC_PRINT);
			hButtonGenerar = GetDlgItem(ghDialog, IDC_BUTTON_GENERAR);
			//CreateWindow("BUTTON", "", WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_BITMAP, 375, 10, 105, 105, ghDialog, (HMENU)IDC_BUTTON_PHOTOMODE, hIntancePIAD, NULL);
			//CreateWindow("BUTTON", "", WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_BITMAP, 375, 120, 105, 105, ghDialog, (HMENU)IDC_BUTTON_VIDEOMODE, hIntancePIAD, NULL);
			CreateWindow("BUTTON", "", WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_BITMAP, (ancho/2)-134, 520, 57, 57, ghDialog, (HMENU)IDC_BUTTON_TAKEPHOTO, hIntancePIAD, NULL);
			CreateWindow("BUTTON", "", WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_BITMAP, (ancho / 2) - 67, 520, 57, 57, ghDialog, (HMENU)IDC_BUTTON_PLAYVIDEO, hIntancePIAD, NULL);
			CreateWindow("BUTTON", "", WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_BITMAP, (ancho / 2) , 520, 57, 57, ghDialog, (HMENU)IDC_BUTTON_STOPVIDEO, hIntancePIAD, NULL);
			CreateWindow("BUTTON", "", WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_BITMAP, (ancho / 2) + 67, 520, 57, 57, ghDialog, (HMENU)IDC_BUTTON_RECORDVIDEO, hIntancePIAD, NULL);
			CreateWindow("BUTTON", "Gausiano", WS_VISIBLE | WS_CHILD | BS_CHECKBOX | BS_AUTOCHECKBOX, (ancho / 2) + 330, 10, 80, 10, ghDialog, (HMENU)IDC_CHECK_FILTROGAUS, hIntancePIAD, NULL);
			CreateWindow("BUTTON", "Media Blur", WS_VISIBLE | WS_CHILD | BS_CHECKBOX | BS_AUTOCHECKBOX, (ancho / 2) + 330, 30, 130, 15, ghDialog, (HMENU)IDC_CHECK_FILTROMEDIA, hIntancePIAD, NULL);
			CreateWindow("BUTTON", "Media Pond. Blur", WS_VISIBLE | WS_CHILD | BS_CHECKBOX | BS_AUTOCHECKBOX, (ancho / 2) + 330, 50, 130, 15, ghDialog, (HMENU)IDC_CHECK_FILTROMEDIAP, hIntancePIAD, NULL);
			CreateWindow("BUTTON", "Sustrac. Media", WS_VISIBLE | WS_CHILD | BS_CHECKBOX | BS_AUTOCHECKBOX, (ancho / 2) + 330, 70, 130, 15, ghDialog, (HMENU)IDC_CHECK_FILTROSUBMEDIA, hIntancePIAD, NULL);
			CreateWindow("BUTTON", "Laplaciano", WS_VISIBLE | WS_CHILD | BS_CHECKBOX | BS_AUTOCHECKBOX, (ancho / 2) + 330, 90, 130, 15, ghDialog, (HMENU)IDC_CHECK_FILTROLAPLACIAN, hIntancePIAD, NULL);
			CreateWindow("BUTTON", "Menos Laplaciano", WS_VISIBLE | WS_CHILD | BS_CHECKBOX | BS_AUTOCHECKBOX, (ancho / 2) + 330, 110, 130, 15, ghDialog, (HMENU)IDC_CHECK_FILTROALAPLACIAN, hIntancePIAD, NULL);
			CreateWindow("BUTTON", "Norte", WS_VISIBLE | WS_CHILD | BS_CHECKBOX | BS_AUTOCHECKBOX, (ancho / 2) +330, 130, 130, 15, ghDialog, (HMENU)IDC_CHECK_FILTRONORTH, hIntancePIAD, NULL);
			CreateWindow("BUTTON", "Este", WS_VISIBLE | WS_CHILD | BS_CHECKBOX | BS_AUTOCHECKBOX, (ancho / 2) + 330, 150, 130, 15, ghDialog, (HMENU)IDC_CHECK_FILTROEAST, hIntancePIAD, NULL);
			CreateWindow("BUTTON", "C Sobel", WS_VISIBLE | WS_CHILD | BS_CHECKBOX | BS_AUTOCHECKBOX, (ancho / 2) + 330, 170, 130, 15, ghDialog, (HMENU)IDC_CHECK_FILTROCSOBEL, hIntancePIAD, NULL);
			CreateWindow("BUTTON", "F Sobel", WS_VISIBLE | WS_CHILD | BS_CHECKBOX | BS_AUTOCHECKBOX, (ancho / 2) + 330, 190, 130, 15, ghDialog, (HMENU)IDC_CHECK_FILTROFSOBEL, hIntancePIAD, NULL);
			CreateWindow("BUTTON", "Blanco y Negro", WS_VISIBLE | WS_CHILD | BS_CHECKBOX | BS_AUTOCHECKBOX, (ancho / 2) + 330, 210, 130, 15, ghDialog, (HMENU)IDC_CHECK_FILTROGRAY, hIntancePIAD, NULL);
			CreateWindow("BUTTON", "Threshold", WS_VISIBLE | WS_CHILD | BS_CHECKBOX | BS_AUTOCHECKBOX, (ancho / 2) + 330, 230, 130, 15, ghDialog, (HMENU)IDC_CHECK_FILTROTHRES, hIntancePIAD, NULL);
			CreateWindow("BUTTON", "Ecualización Hist. Normal", WS_VISIBLE | WS_CHILD | BS_CHECKBOX | BS_AUTOCHECKBOX, (ancho / 2) + 330, 250, 200, 15, ghDialog, (HMENU)IDC_CHECK_FILTRONORMALHISTOGRAM, hIntancePIAD, NULL);
			CreateWindow("BUTTON", "Ecualización Hist. Exp.", WS_VISIBLE | WS_CHILD | BS_CHECKBOX | BS_AUTOCHECKBOX, (ancho / 2) + 330, 270, 200, 15, ghDialog, (HMENU)IDC_CHECK_FILTROEXPHISTOGRAM, hIntancePIAD, NULL);
			CreateWindow("BUTTON", "Ecualización Hist. Uniforme", WS_VISIBLE | WS_CHILD | BS_CHECKBOX | BS_AUTOCHECKBOX, (ancho / 2) + 330, 290, 200, 15, ghDialog, (HMENU)IDC_CHECK_FILTROUNIHISTOGRAM, hIntancePIAD, NULL);

			hButtonPhotoMode = GetDlgItem(ghDialog, IDC_BUTTON_PHOTOMODE);
			hButtonVideoMode = GetDlgItem(ghDialog, IDC_BUTTON_VIDEOMODE);
			hButtonTakePhoto = GetDlgItem(ghDialog, IDC_BUTTON_TAKEPHOTO);
			hButtonPlayVideo = GetDlgItem(ghDialog, IDC_BUTTON_PLAYVIDEO);
			hButtonStopVideo = GetDlgItem(ghDialog, IDC_BUTTON_STOPVIDEO);
			hButtonRecordVideo = GetDlgItem(ghDialog, IDC_BUTTON_RECORDVIDEO);
			hCheckFiltroGaus = GetDlgItem(ghDialog, IDC_CHECK_FILTROGAUS);
			hCheckFiltroMedia = GetDlgItem(ghDialog, IDC_CHECK_FILTROMEDIA);
			hCheckFiltroMediaPond = GetDlgItem(ghDialog, IDC_CHECK_FILTROMEDIAP);
			hCheckFiltroSubMedia = GetDlgItem(ghDialog, IDC_CHECK_FILTROSUBMEDIA);
			hCheckFiltroLaplacian= GetDlgItem(ghDialog, IDC_CHECK_FILTROLAPLACIAN);
			hCheckFiltroALaplacian = GetDlgItem(ghDialog, IDC_CHECK_FILTROALAPLACIAN);
			hCheckFiltroNorth = GetDlgItem(ghDialog, IDC_CHECK_FILTRONORTH);
			hCheckFiltroEast = GetDlgItem(ghDialog, IDC_CHECK_FILTROEAST);
			hCheckFiltroCSobel = GetDlgItem(ghDialog, IDC_CHECK_FILTROCSOBEL);
			hCheckFiltroFSobel = GetDlgItem(ghDialog, IDC_CHECK_FILTROFSOBEL);
			hCheckFiltroGray = GetDlgItem(ghDialog, IDC_CHECK_FILTROGRAY);
			hCheckFiltroThreshold = GetDlgItem(ghDialog, IDC_CHECK_FILTROTHRES);
			hCheckFiltroHitogramNormal = GetDlgItem(ghDialog, IDC_CHECK_FILTRONORMALHISTOGRAM);
			hCheckFiltroHistogramExponencial = GetDlgItem(ghDialog, IDC_CHECK_FILTROEXPHISTOGRAM);
			hCheckFiltroHistogramUniform = GetDlgItem(ghDialog, IDC_CHECK_FILTROUNIHISTOGRAM);

						
			CheckDlgButton(hCheckFiltroGaus, 1, BST_CHECKED);
			//hComboFiltros = CreateWindow(L"COMBOBOX", L"", CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE, 900, 10, 100, 20, ghDialog, (HMENU)IDC_COMBO_FILTROS, hIntancePIAD, NULL);

			

			//SendDlgItemMessage(ghDialog, IDC_BUTTON_VIDEOMODE, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)iconVideoMode);
			//SendMessage(hButtonPhotoMode, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)iconCamMode);
			SendMessage(hButtonTakePhoto, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)iconTakePhoto);
			SendMessage(hButtonPlayVideo, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)iconPlayVideo);
			SendMessage(hButtonStopVideo, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)iconStopVideo);
			SendMessage(hButtonRecordVideo, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)iconRecordVideo);


			

			SendDlgItemMessage(ghDialog, IDC_DISPLAY, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)imageObturador);
			SendDlgItemMessage(ghDialog, IDC_BACKGROUND, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)imageBackground);
			SendMessage(ghDialog, WM_SETICON, ICON_SMALL, (LPARAM)mainicon);
			SendMessage(ghDialog, WM_SETICON, ICON_BIG, (LPARAM)mainicon);
			POINT pos = {10,10};
			moveElement(hButtonGenerar,pos);

			//ocultarControlesPhotoMode();
			//ocultarControlesVideoMode();
			
			filtros f;
			
			gausian_mask = filtros::GenerateGausianMask(3, 3, 9);
			filtros::GenerateSubMediaMask(3,3);
			filtros::GenerateMediaMask(3,3);
			filtros::GenerateLaplacianMask();
			filtros::GenerateALaplacianMask();
			filtros::GenerateNorthMask();
			filtros::GenerateEastMask();
			filtros::GenerateCSobelMask();
			filtros::GenerateFSobelMask();
			filtros::GenerateMediaPMask(3, 3, 4);
			/*std::ofstream file;
			file.open("cout.txt");
			
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++)
				{
					char t[4];
					_itoa_s(alaplacian_mask[i][j], t, 10);
					file.write(t,4);
				}
			}
			file.close();*/
		}break;

		case WM_CTLCOLORSTATIC:{
			if ((HWND)lParam == GetDlgItem(ghDialog, IDC_STATIC_PRINT) )
			{
				SetBkMode((HDC)wParam, TRANSPARENT);
				SetTextColor((HDC)wParam, RGB(255, 255, 255));
				hbrBkgnd = CreateSolidBrush(RGB(30, 30, 30));
				return (INT_PTR)hbrBkgnd;
			}
			if ((HWND)lParam == GetDlgItem(ghDialog, IDC_CHECK_FILTROGAUS))
			{
				SetBkMode((HDC)wParam, TRANSPARENT);
				SetTextColor((HDC)wParam, RGB(255, 255, 255));
				SetBkColor((HDC)wParam, RGB(30, 30, 30));
				hbrBkgnd = CreateSolidBrush(RGB(30, 30, 30));
				return (INT_PTR)hbrBkgnd;
			}
			if ((HWND)lParam == GetDlgItem(ghDialog, IDC_CHECK_FILTROMEDIA))
			{
				SetBkMode((HDC)wParam, TRANSPARENT);
				SetTextColor((HDC)wParam, RGB(255, 255, 255));
				SetBkColor((HDC)wParam, RGB(30, 30, 30));
				hbrBkgnd = CreateSolidBrush(RGB(30, 30, 30));
				return (INT_PTR)hbrBkgnd;
			}
			if ((HWND)lParam == GetDlgItem(ghDialog, IDC_CHECK_FILTROMEDIAP))
			{
				SetBkMode((HDC)wParam, TRANSPARENT);
				SetTextColor((HDC)wParam, RGB(255, 255, 255));
				SetBkColor((HDC)wParam, RGB(30, 30, 30));
				hbrBkgnd = CreateSolidBrush(RGB(30, 30, 30));
				return (INT_PTR)hbrBkgnd;
			}
			if ((HWND)lParam == GetDlgItem(ghDialog, IDC_CHECK_FILTROSUBMEDIA))
			{
				SetBkMode((HDC)wParam, TRANSPARENT);
				SetTextColor((HDC)wParam, RGB(255, 255, 255));
				SetBkColor((HDC)wParam, RGB(30, 30, 30));
				hbrBkgnd = CreateSolidBrush(RGB(30, 30, 30));
				return (INT_PTR)hbrBkgnd;
			}
			if ((HWND)lParam == GetDlgItem(ghDialog, IDC_CHECK_FILTROLAPLACIAN))
			{
				SetBkMode((HDC)wParam, TRANSPARENT);
				SetTextColor((HDC)wParam, RGB(255, 255, 255));
				SetBkColor((HDC)wParam, RGB(30, 30, 30));
				hbrBkgnd = CreateSolidBrush(RGB(30, 30, 30));
				return (INT_PTR)hbrBkgnd;
			}
			if ((HWND)lParam == GetDlgItem(ghDialog, IDC_CHECK_FILTROALAPLACIAN))
			{
				SetBkMode((HDC)wParam, TRANSPARENT);
				SetTextColor((HDC)wParam, RGB(255, 255, 255));
				SetBkColor((HDC)wParam, RGB(30, 30, 30));
				hbrBkgnd = CreateSolidBrush(RGB(30, 30, 30));
				return (INT_PTR)hbrBkgnd;
			}
			if ((HWND)lParam == GetDlgItem(ghDialog, IDC_CHECK_FILTRONORTH))
			{
				SetBkMode((HDC)wParam, TRANSPARENT);
				SetTextColor((HDC)wParam, RGB(255, 255, 255));
				SetBkColor((HDC)wParam, RGB(30, 30, 30));
				hbrBkgnd = CreateSolidBrush(RGB(30, 30, 30));
				return (INT_PTR)hbrBkgnd;
			}
			if ((HWND)lParam == GetDlgItem(ghDialog, IDC_CHECK_FILTROEAST))
			{
				SetBkMode((HDC)wParam, TRANSPARENT);
				SetTextColor((HDC)wParam, RGB(255, 255, 255));
				SetBkColor((HDC)wParam, RGB(30, 30, 30));
				hbrBkgnd = CreateSolidBrush(RGB(30, 30, 30));
				return (INT_PTR)hbrBkgnd;
			}
			if ((HWND)lParam == GetDlgItem(ghDialog, IDC_CHECK_FILTROCSOBEL))
			{
				SetBkMode((HDC)wParam, TRANSPARENT);
				SetTextColor((HDC)wParam, RGB(255, 255, 255));
				SetBkColor((HDC)wParam, RGB(30, 30, 30));
				hbrBkgnd = CreateSolidBrush(RGB(30, 30, 30));
				return (INT_PTR)hbrBkgnd;
			}
			if ((HWND)lParam == GetDlgItem(ghDialog, IDC_CHECK_FILTROFSOBEL))
			{
				SetBkMode((HDC)wParam, TRANSPARENT);
				SetTextColor((HDC)wParam, RGB(255, 255, 255));
				SetBkColor((HDC)wParam, RGB(30, 30, 30));
				hbrBkgnd = CreateSolidBrush(RGB(30, 30, 30));
				return (INT_PTR)hbrBkgnd;
			}
			if ((HWND)lParam == GetDlgItem(ghDialog, IDC_CHECK_FILTROGRAY))
			{
				SetBkMode((HDC)wParam, TRANSPARENT);
				SetTextColor((HDC)wParam, RGB(255, 255, 255));
				SetBkColor((HDC)wParam, RGB(30, 30, 30));
				hbrBkgnd = CreateSolidBrush(RGB(30, 30, 30));
				return (INT_PTR)hbrBkgnd;
			}
			if ((HWND)lParam == GetDlgItem(ghDialog, IDC_CHECK_FILTROTHRES))
			{
				SetBkMode((HDC)wParam, TRANSPARENT);
				SetTextColor((HDC)wParam, RGB(255, 255, 255));
				SetBkColor((HDC)wParam, RGB(30, 30, 30));
				hbrBkgnd = CreateSolidBrush(RGB(30, 30, 30));
				return (INT_PTR)hbrBkgnd;
			}

			if ((HWND)lParam == GetDlgItem(ghDialog, IDC_CHECK_FILTRONORMALHISTOGRAM))
			{
				SetBkMode((HDC)wParam, TRANSPARENT);
				SetTextColor((HDC)wParam, RGB(255, 255, 255));
				SetBkColor((HDC)wParam, RGB(30, 30, 30));
				hbrBkgnd = CreateSolidBrush(RGB(30, 30, 30));
				return (INT_PTR)hbrBkgnd;
			}

			if ((HWND)lParam == GetDlgItem(ghDialog, IDC_CHECK_FILTROEXPHISTOGRAM))
			{
				SetBkMode((HDC)wParam, TRANSPARENT);
				SetTextColor((HDC)wParam, RGB(255, 255, 255));
				SetBkColor((HDC)wParam, RGB(30, 30, 30));
				hbrBkgnd = CreateSolidBrush(RGB(30, 30, 30));
				return (INT_PTR)hbrBkgnd;
			}

			if ((HWND)lParam == GetDlgItem(ghDialog, IDC_CHECK_FILTROUNIHISTOGRAM))
			{
				SetBkMode((HDC)wParam, TRANSPARENT);
				SetTextColor((HDC)wParam, RGB(255, 255, 255));
				SetBkColor((HDC)wParam, RGB(30, 30, 30));
				hbrBkgnd = CreateSolidBrush(RGB(30, 30, 30));
				return (INT_PTR)hbrBkgnd;
			}


			
		}break;

		case WM_COMMAND: {
			switch (LOWORD(wParam))
			{	
				
				case WM_DISPLAYCHANGE:{
					ancho = GetSystemMetrics(SM_CXSCREEN);
					alto = GetSystemMetrics(SM_CYSCREEN);
					alto = GetSystemMetrics(SM_CYSCREEN);
					centerElementX(hDisplay);
					centerElementX(hStaticPrint,-240,240);

				}break;

				case IDC_BUTTON_PHOTOMODE:{
					SendDlgItemMessage(ghDialog, IDC_STATIC_PRINT, WM_SETTEXT, 0, (LPARAM)"");

					SendDlgItemMessage(ghDialog, IDC_STATIC_PRINT, WM_SETTEXT, 0, (LPARAM)"Jala photo mode");
					mostrarControlesPhotoMode();
					ocultarControlesVideoMode();
				}break;

				case IDC_BUTTON_VIDEOMODE:{
					SendDlgItemMessage(ghDialog, IDC_STATIC_PRINT, WM_SETTEXT, 0, (LPARAM)"Jala video mode");
					ocultarControlesPhotoMode();
					mostrarControlesVideoMode();
				}break;

				case IDC_BUTTON_PLAYVIDEO:{
					if (!captureVideo){
						

						if (cam->isOpened()){
							
							captureVideo = true;
						}
							

						
					}
				}break;

				case IDC_BUTTON_STOPVIDEO:{
					if (captureVideo){
						captureVideo = false;
						takeVideo = false;
						
						if (takeVideo){
						
							wrt->release();
							wrt->~VideoWriter();
						}
						
						
					}
				}break;

				case IDC_BUTTON_TAKEPHOTO:{
					if (!takeVideo){
						
						takePhoto = true;

					}
				}break;

				case IDC_BUTTON_RECORDVIDEO:{
					time_t t = time(0);
					tm* now = localtime(&t);
					char anio[4] = "";
					char mes[2] = "";
					char dia[2] = "";
					char hour[2] = "";
					char min[2] = "";
					char seg[2] = "";

					_itoa(now->tm_year+1900, anio, 10);
					_itoa(now->tm_mon + 1, mes, 10);
					_itoa(now->tm_mday, dia, 10);
					_itoa(now->tm_hour, hour, 10);
					_itoa(now->tm_min, min, 10);
					_itoa(now->tm_sec, seg, 10);

					char nameVideo[32] = "";
					strcpy_s(nameVideo, "video_");
					strcat_s(nameVideo, anio);
					strcat_s(nameVideo, mes);
					strcat_s(nameVideo, dia);
					strcat_s(nameVideo, hour);
					strcat_s(nameVideo, min);
					strcat_s(nameVideo, seg);

					strcat_s(nameVideo, ".avi");

					
					if (!takePhoto && takeVideo==false){
						wrt = new VideoWriter(nameVideo, CV_FOURCC('M', 'J', 'P', 'G'), 24, Size(640, 480));
						takeVideo = true;
					}
				}break;

				//CHECKS--------------------------------------------------------------------
				case IDC_CHECK_FILTROGAUS:{
					checkedFiltroGaus = SendDlgItemMessage(ghDialog, IDC_CHECK_FILTROGAUS, BM_GETCHECK, 0, 0);
					if (checkedFiltroGaus){
						if (contador < 4){
							orden[contador] = GAUSIAN;
							contador++;
						}
					}else{
						deleteFiltro(GAUSIAN);
						contador--;
					}
				}break;

				case IDC_CHECK_FILTROMEDIA:{
					checkedFiltroMedia = SendDlgItemMessage(ghDialog, IDC_CHECK_FILTROMEDIA, BM_GETCHECK, 0, 0);
					if (checkedFiltroMedia){
						if (contador < 4){
							orden[contador] = MEDIA;
							contador++;
						}
					}else{
						deleteFiltro(MEDIA);
						contador--;
					}
				}break;

				case IDC_CHECK_FILTROMEDIAP:{
					checkedFiltroMediaP = SendDlgItemMessage(ghDialog, IDC_CHECK_FILTROMEDIAP, BM_GETCHECK, 0, 0);
					if (checkedFiltroMediaP){
						if (contador < 4){
							orden[contador] = MEDIA_POND;
							contador++;
						}
					}else{
						deleteFiltro(MEDIA_POND);
						contador--;
					}
				}break;

				case IDC_CHECK_FILTROSUBMEDIA:{
					checkedFiltroSubMedia = SendDlgItemMessage(ghDialog, IDC_CHECK_FILTROSUBMEDIA, BM_GETCHECK, 0, 0);
					if (checkedFiltroSubMedia){
						if (contador < 4){
							orden[contador] = SUB_MEDIA;
							contador++;
						}
					}else{
						deleteFiltro(SUB_MEDIA);
						contador--;
					}
				}break;

				case IDC_CHECK_FILTROLAPLACIAN:{
					checkedFiltroLaplacian = SendDlgItemMessage(ghDialog, IDC_CHECK_FILTROLAPLACIAN, BM_GETCHECK, 0, 0);
					if (checkedFiltroLaplacian){
						if (contador < 4){
							orden[contador] = LAPLACIAN;
							contador++;
						}
					}else{
						deleteFiltro(LAPLACIAN);
						contador--;
					}
				}break;

				case IDC_CHECK_FILTROALAPLACIAN:{
					checkedFiltroALaplacian = SendDlgItemMessage(ghDialog, IDC_CHECK_FILTROALAPLACIAN, BM_GETCHECK, 0, 0);
					if (checkedFiltroALaplacian){
						if (contador < 4){
							orden[contador] = ALAPLACIAN;
							contador++;
						}
					}else{
						deleteFiltro(ALAPLACIAN);
						contador--;
					}
				}break;

				case IDC_CHECK_FILTRONORTH:{
					checkedFiltroNorth = SendDlgItemMessage(ghDialog, IDC_CHECK_FILTRONORTH, BM_GETCHECK, 0, 0);
					if (checkedFiltroNorth){
						if (contador < 4){
							orden[contador] = NORTH;
							contador++;
						}
					}else{
						deleteFiltro(NORTH);
						contador--;
					}
				}break;

				case IDC_CHECK_FILTROEAST:{
					checkedFiltroEast = SendDlgItemMessage(ghDialog, IDC_CHECK_FILTROEAST, BM_GETCHECK, 0, 0);
					if (checkedFiltroEast){
						if (contador < 4){
							orden[contador] = EAST;
							contador++;
						}
					}else{
						deleteFiltro(EAST);
						contador--;
					}
				}break;

				case IDC_CHECK_FILTROCSOBEL:{
					checkedFiltroCSobel = SendDlgItemMessage(ghDialog, IDC_CHECK_FILTROCSOBEL, BM_GETCHECK, 0, 0);
					if (checkedFiltroCSobel){
						if (contador < 4){
							orden[contador] = CSOBEL;
							contador++;
						}
					}else{
						deleteFiltro(CSOBEL);
						contador--;
					}
				}break;

				case IDC_CHECK_FILTROFSOBEL:{
					checkedFiltroFSobel = SendDlgItemMessage(ghDialog, IDC_CHECK_FILTROFSOBEL, BM_GETCHECK, 0, 0);
					if (checkedFiltroFSobel){
						if (contador < 4){
							orden[contador] = FSOBEL;
							contador++;
						}
					}
					else{
						deleteFiltro(FSOBEL);
						contador--;
					}
				}break;
				case IDC_CHECK_FILTROGRAY:{
					checkedFiltroGray = SendDlgItemMessage(ghDialog, IDC_CHECK_FILTROGRAY, BM_GETCHECK, 0, 0);
					if (checkedFiltroGray){
						if (contador < 4){
							orden[contador] = GRAY;
							contador++;
						}
					}else{
						deleteFiltro(GRAY);
						contador--;
					}
				}break;

				case IDC_CHECK_FILTROTHRES:{
					checkedFiltroThreshold = SendDlgItemMessage(ghDialog, IDC_CHECK_FILTROTHRES, BM_GETCHECK, 0, 0);
					if (checkedFiltroThreshold){
						if (contador < 4){
							orden[contador] = THRESH;
							contador++;
					
							
						}
					}else{
						deleteFiltro(THRESH);
						contador--;
					}
				}break;

				case IDC_CHECK_FILTRONORMALHISTOGRAM:{
					checkedFiltroHistogramSimple = SendDlgItemMessage(ghDialog, IDC_CHECK_FILTRONORMALHISTOGRAM, BM_GETCHECK, 0, 0);
					if (checkedFiltroHistogramSimple){
						if (contador < 4){
							orden[contador] = SIMPLE_HITOGRAM;
							contador++;


						}
					}
					else{
						deleteFiltro(SIMPLE_HITOGRAM);
						contador--;
					}
				}break;

				case IDC_CHECK_FILTROEXPHISTOGRAM:{
					checkedFiltroHistogramExponencial = SendDlgItemMessage(ghDialog, IDC_CHECK_FILTROEXPHISTOGRAM, BM_GETCHECK, 0, 0);
					if (checkedFiltroHistogramExponencial){
						if (contador < 4){
							orden[contador] = EXPONENCIAL_HISTOGRAM;
							contador++;


						}
					}
					else{
						deleteFiltro(EXPONENCIAL_HISTOGRAM);
						contador--;
					}
				}break;

				case IDC_CHECK_FILTROUNIHISTOGRAM:{
					checkedFiltroHistogramUniforme = SendDlgItemMessage(ghDialog, IDC_CHECK_FILTROUNIHISTOGRAM, BM_GETCHECK, 0, 0);
					if (checkedFiltroHistogramUniforme){
						if (contador < 4){
							orden[contador] = UNIFORM_HISTOGRAM;
							contador++;
						}
					}
					else{
						deleteFiltro(UNIFORM_HISTOGRAM);
						contador--;
					}
				}break;



			}
		}break;

		case WM_CLOSE: {
			EndDialog(
				ghDialog, // Handle to dialog to end.
				0);
			cam->release();
		}break;
		case WM_DESTROY: {
			EndDialog(
				ghDialog, // Handle to dialog to end.
				0);
			DestroyWindow(ghDialog);
		}break;
		case WM_SIZING:{
			ancho = GetSystemMetrics(SM_CXSCREEN);
			alto = GetSystemMetrics(SM_CYSCREEN);
			alto = GetSystemMetrics(SM_CYSCREEN);
			centerElementX(hDisplay);
			     centerElementX(hStaticPrint,-240,240);

		}break;
	}

	return false;
}


