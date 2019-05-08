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
			CreateWindow("BUTTON", "", WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_BITMAP, (ancho/2)-134, 420, 57, 57, ghDialog, (HMENU)IDC_BUTTON_TAKEPHOTO, hIntancePIAD, NULL);
			CreateWindow("BUTTON", "", WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_BITMAP, (ancho / 2) - 67, 420, 57, 57, ghDialog, (HMENU)IDC_BUTTON_PLAYVIDEO, hIntancePIAD, NULL);
			CreateWindow("BUTTON", "", WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_BITMAP, (ancho / 2) , 420, 57, 57, ghDialog, (HMENU)IDC_BUTTON_STOPVIDEO, hIntancePIAD, NULL);
			CreateWindow("BUTTON", "", WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_BITMAP, (ancho / 2) + 67, 420, 57, 57, ghDialog, (HMENU)IDC_BUTTON_RECORDVIDEO, hIntancePIAD, NULL);
			CreateWindow("BUTTON", "Gausiano", WS_VISIBLE | WS_CHILD | BS_CHECKBOX | BS_AUTOCHECKBOX, (ancho / 2) + 250, 10, 80, 10, ghDialog, (HMENU)IDC_CHECK_FILTROGAUS, hIntancePIAD, NULL);
			CreateWindow("BUTTON", "Media Blur", WS_VISIBLE | WS_CHILD | BS_CHECKBOX | BS_AUTOCHECKBOX, (ancho / 2) + 250, 30, 130, 15, ghDialog, (HMENU)IDC_CHECK_FILTROMEDIA, hIntancePIAD, NULL);
			CreateWindow("BUTTON", "Media Pond. Blur", WS_VISIBLE | WS_CHILD | BS_CHECKBOX | BS_AUTOCHECKBOX, (ancho / 2) + 250, 50, 130, 15, ghDialog, (HMENU)IDC_CHECK_FILTROMEDIAP, hIntancePIAD, NULL);
			CreateWindow("BUTTON", "Sustrac. Media", WS_VISIBLE | WS_CHILD | BS_CHECKBOX | BS_AUTOCHECKBOX, (ancho / 2) + 250, 70, 130, 15, ghDialog, (HMENU)IDC_CHECK_FILTROSUBMEDIA, hIntancePIAD, NULL);
			CreateWindow("BUTTON", "Laplaciano", WS_VISIBLE | WS_CHILD | BS_CHECKBOX | BS_AUTOCHECKBOX, (ancho / 2) + 250, 90, 130, 15, ghDialog, (HMENU)IDC_CHECK_FILTROLAPLACIAN, hIntancePIAD, NULL);
			CreateWindow("BUTTON", "Menos Laplaciano", WS_VISIBLE | WS_CHILD | BS_CHECKBOX | BS_AUTOCHECKBOX, (ancho / 2) + 250, 110, 130, 15, ghDialog, (HMENU)IDC_CHECK_FILTROALAPLACIAN, hIntancePIAD, NULL);
			CreateWindow("BUTTON", "Norte", WS_VISIBLE | WS_CHILD | BS_CHECKBOX | BS_AUTOCHECKBOX, (ancho / 2) + 250, 130, 130, 15, ghDialog, (HMENU)IDC_CHECK_FILTRONORTH, hIntancePIAD, NULL);
			CreateWindow("BUTTON", "Este", WS_VISIBLE | WS_CHILD | BS_CHECKBOX | BS_AUTOCHECKBOX, (ancho / 2) + 250, 150, 130, 15, ghDialog, (HMENU)IDC_CHECK_FILTROEAST, hIntancePIAD, NULL);
			CreateWindow("BUTTON", "C Sobel", WS_VISIBLE | WS_CHILD | BS_CHECKBOX | BS_AUTOCHECKBOX, (ancho / 2) + 250, 170, 130, 15, ghDialog, (HMENU)IDC_CHECK_FILTROCSOBEL, hIntancePIAD, NULL);
			CreateWindow("BUTTON", "F Sobel", WS_VISIBLE | WS_CHILD | BS_CHECKBOX | BS_AUTOCHECKBOX, (ancho / 2) + 250, 190, 130, 15, ghDialog, (HMENU)IDC_CHECK_FILTROFSOBEL, hIntancePIAD, NULL);
			CreateWindow("BUTTON", "Blanco y Negro", WS_VISIBLE | WS_CHILD | BS_CHECKBOX | BS_AUTOCHECKBOX, (ancho / 2) + 250, 210, 130, 15, ghDialog, (HMENU)IDC_CHECK_FILTROGRAY, hIntancePIAD, NULL);
			CreateWindow("BUTTON", "Threshold", WS_VISIBLE | WS_CHILD | BS_CHECKBOX | BS_AUTOCHECKBOX, (ancho / 2) + 250, 230, 130, 15, ghDialog, (HMENU)IDC_CHECK_FILTROTHRES, hIntancePIAD, NULL);

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
		}return true;

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

			
		}return true;

		case WM_CTLCOLORBTN:{
			/*if ((HWND)lParam == hButtonGenerar)
			{
				SetBkMode((HDC)wParam, TRANSPARENT);
				SetTextColor((HDC)wParam, RGB(255, 255, 255));
				SetBkColor((HDC)wParam, RGB(30, 30, 30));
				hbrBkgnd = CreateSolidBrush(RGB(30, 30, 30));
				return (INT_PTR)hbrBkgnd;
			}		*/	
		}return true;

		case WM_COMMAND: {
			switch (LOWORD(wParam))
			{	
				
				case WM_DISPLAYCHANGE:{
					ancho = GetSystemMetrics(SM_CXSCREEN);
					alto = GetSystemMetrics(SM_CYSCREEN);
					alto = GetSystemMetrics(SM_CYSCREEN);
					centerElementX(hDisplay);
				}return true;

				case WM_ERASEBKGND: {

				}return true;

				case IDC_BUTTON_GENERAR:{
					
					//imshow("ventana", obturador);
					calcularTamañoIdeal(imgPrueba.size());
					Mat imagePruebaR;
					resize(imgPrueba, imagePruebaR, imagePruebaR.size(), escalaX, escalaY, INTER_AREA);
					
					start = (double)getTickCount();

					for (int i = 0; i < contador; i++){
						if (orden[i] > 0){
							switch (orden[i])
							{
								case MEDIA:{
									filtros::ApliMask(imagePruebaR, dstPrueba, 3, 3, media_mask);
									dstPrueba.copyTo(imagePruebaR);
								}break;
								
								case LAPLACIAN:{
									filtros::ApliMask(imagePruebaR, dstPrueba, 3, 3, laplacian_mask, LAPLACIAN);
									dstPrueba.copyTo(imagePruebaR);
								}break;

								case SUB_MEDIA:{
									filtros::ApliMask(imagePruebaR, dstPrueba, 3, 3, subsMedia_mask, SUB_MEDIA);
									dstPrueba.copyTo(imagePruebaR);
								}break;

								case ALAPLACIAN:{
									filtros::ApliMask(imagePruebaR, dstPrueba, 3, 3, alaplacian_mask, ALAPLACIAN);
									dstPrueba.copyTo(imagePruebaR);
								}break;

								case GAUSIAN:{
									filtros::ApliMask(imagePruebaR, dstPrueba, 3, 3, gausian_mask);
									dstPrueba.copyTo(imagePruebaR);
								}break;

								case MEDIA_POND:{
									filtros::ApliMask(imagePruebaR, dstPrueba, 3, 3, mediaPondered_mask);
									dstPrueba.copyTo(imagePruebaR);
								}break;

								case NORTH:{
									filtros::ApliMask(imagePruebaR, dstPrueba, 3, 3, nort_mask, ALAPLACIAN);
									dstPrueba.copyTo(imagePruebaR);
								}break;

								case EAST:{
									filtros::ApliMask(imagePruebaR, dstPrueba, 3, 3, east_mask, ALAPLACIAN);
									dstPrueba.copyTo(imagePruebaR);
								}break;

								case CSOBEL:{
									filtros::ApliMask(imagePruebaR, dstPrueba, 3, 3, cSobel_mask, LAPLACIAN);
									dstPrueba.copyTo(imagePruebaR);
								}break;

								case FSOBEL:{
									filtros::ApliMask(imagePruebaR, dstPrueba, 3, 3, fSobel_mask, LAPLACIAN);
									dstPrueba.copyTo(imagePruebaR);
								}break;
								
								case GRAY:{
									filtros::toGray(imagePruebaR, dstPrueba);
									dstPrueba.copyTo(imagePruebaR);
								}break;
								
								case THRESH:{
									filtros::threshold(imagePruebaR, dstPrueba, 198, 197);
									dstPrueba.copyTo(imagePruebaR);
								}break;
							}
						}
						
					}
					
					/*if (checkedFiltroGaus){
						filtros::ApliMask(imagePruebaR, dstPrueba, 3, 3, gausian_mask);
						dstPrueba.copyTo(imagePruebaR);
					}*/
				/*	if (checkedFiltroMedia){
						filtros::ApliMask(imagePruebaR, dstPrueba, 3, 3, media_mask);
						dstPrueba.copyTo(imagePruebaR);
					}*/
					/*if (checkedFiltroMediaP){
						filtros::ApliMask(imagePruebaR, dstPrueba, 3, 3, mediaPondered_mask);
						dstPrueba.copyTo(imagePruebaR);
					}*/
					/*if (checkedFiltroSubMedia || checkedFiltroLaplacian || checkedFiltroALaplacian || checkedFiltroNorth || checkedFiltroEast || checkedFiltroCSobel || checkedFiltroFSobel){
						filtros::toGray(imagePruebaR, dstPrueba);
						dstPrueba.copyTo(imagePruebaR);
					}*/
					/*if (checkedFiltroThreshold){
						filtros::threshold(imagePruebaR, dstPrueba, 198, 197);
						dstPrueba.copyTo(imagePruebaR);

					}*/
					/*if (checkedFiltroSubMedia){
						
						filtros::ApliMask(imagePruebaR, dstPrueba, 3, 3, subsMedia_mask,SUB_MEDIA);
						Mat mul = dstPrueba.mul(imagePruebaR);
						mul.copyTo(imagePruebaR);
					}*/
					/*if (checkedFiltroLaplacian){
						
						filtros::ApliMask(imagePruebaR, dstPrueba, 3, 3, laplacian_mask, LAPLACIAN);
						Mat mul = dstPrueba.mul(imagePruebaR);
						mul.copyTo(imagePruebaR);
					}*/
					/*if (checkedFiltroALaplacian){
						
						filtros::ApliMask(imagePruebaR, dstPrueba, 3, 3, alaplacian_mask, ALAPLACIAN);
						Mat mul = dstPrueba.mul(imagePruebaR);
						mul.copyTo(imagePruebaR);
					}*/
					/*if (checkedFiltroNorth){
						
						filtros::ApliMask(imagePruebaR, dstPrueba, 3, 3, nort_mask, ALAPLACIAN);
						Mat mul = dstPrueba.mul(imagePruebaR);
						mul.copyTo(imagePruebaR);
					}*/
					/*if (checkedFiltroEast){
						
						filtros::ApliMask(imagePruebaR, dstPrueba, 3, 3, east_mask, ALAPLACIAN);
						Mat mul = dstPrueba.mul(imagePruebaR);
						mul.copyTo(imagePruebaR);
						
					}*/
					/*if (checkedFiltroCSobel){
						
						filtros::ApliMask(imagePruebaR, dstPrueba, 3, 3, cSobel_mask, LAPLACIAN);
						Mat mul = dstPrueba.mul(imagePruebaR);
						mul.copyTo(imagePruebaR);
					}
					if (checkedFiltroFSobel){
						
						filtros::ApliMask(imagePruebaR, dstPrueba, 3, 3, fSobel_mask, LAPLACIAN);
						Mat mul = dstPrueba.mul(imagePruebaR);
						mul.copyTo(imagePruebaR);
					}*/
					/*if (checkedFiltroGray){
						filtros::toGray(imagePruebaR, dstPrueba);
						dstPrueba.copyTo(imagePruebaR);
						
					}*/
					
					 elapsed = 0;
					elapsed = ((double)getTickCount() - start) / getTickFrequency();
					
					sprintf_s(text, "%f", elapsed);
					
					SendDlgItemMessage(ghDialog, IDC_STATIC_PRINT, WM_SETTEXT, 0, (LPARAM)text);
					HBITMAP image = ConvertCVMatToBMP(dstPrueba);
					SendDlgItemMessage(ghDialog, IDC_DISPLAY, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)image);

				}return true;

				case IDC_BUTTON_PHOTOMODE:{
					SendDlgItemMessage(ghDialog, IDC_STATIC_PRINT, WM_SETTEXT, 0, (LPARAM)"");

					SendDlgItemMessage(ghDialog, IDC_STATIC_PRINT, WM_SETTEXT, 0, (LPARAM)"Jala photo mode");
					mostrarControlesPhotoMode();
					ocultarControlesVideoMode();
				}return true;

				case IDC_BUTTON_VIDEOMODE:{
					SendDlgItemMessage(ghDialog, IDC_STATIC_PRINT, WM_SETTEXT, 0, (LPARAM)"Jala video mode");
					ocultarControlesPhotoMode();
					mostrarControlesVideoMode();
				}return true;

				case IDC_BUTTON_PLAYVIDEO:{
					if (!captureVideo){
						

						if (cam->isOpened()){
							
							captureVideo = true;
						}
							

						
					}
				}return true;

				case IDC_BUTTON_STOPVIDEO:{
					if (captureVideo){
						captureVideo = false;
						takeVideo = false;
						
						if (takeVideo){
							wrt->release();
							wrt->~VideoWriter();
						}
						
						
					}
				}return true;

				case IDC_BUTTON_TAKEPHOTO:{
					if (!takeVideo){
						
						takePhoto = true;

					}
				}return true;

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
						wrt = new VideoWriter(nameVideo, CV_FOURCC('M', 'J', 'P', 'G'), 24, Size(400, 400));
						takeVideo = true;
					}
				}return true;

				//CHECKS--------------------------------------------------------------------
				case IDC_CHECK_FILTROGAUS:{
					checkedFiltroGaus = SendDlgItemMessage(ghDialog, IDC_CHECK_FILTROGAUS, BM_GETCHECK, 0, 0);
					if (checkedFiltroGaus){
						if (contador < 4){
							orden[contador] = GAUSIAN;
							contador++;
						}
					}
					else{
						deleteFiltro(GAUSIAN);
						contador--;
					}
				}return true;
				case IDC_CHECK_FILTROMEDIA:{
					checkedFiltroMedia = SendDlgItemMessage(ghDialog, IDC_CHECK_FILTROMEDIA, BM_GETCHECK, 0, 0);
					if (checkedFiltroMedia){
						if (contador < 4){
							orden[contador] = MEDIA;
							contador++;
						}
					}
					else{
						deleteFiltro(MEDIA);
						contador--;
					}
				}return true;
				case IDC_CHECK_FILTROMEDIAP:{
					checkedFiltroMediaP = SendDlgItemMessage(ghDialog, IDC_CHECK_FILTROMEDIAP, BM_GETCHECK, 0, 0);
					if (checkedFiltroMediaP){
						if (contador < 4){
							orden[contador] = MEDIA_POND;
							contador++;
						}
					}
					else{
						deleteFiltro(MEDIA_POND);
						contador--;
					}
				}return true;
				case IDC_CHECK_FILTROSUBMEDIA:{
					checkedFiltroSubMedia = SendDlgItemMessage(ghDialog, IDC_CHECK_FILTROSUBMEDIA, BM_GETCHECK, 0, 0);
					if (checkedFiltroSubMedia){
						if (contador < 4){
							orden[contador] = SUB_MEDIA;
							contador++;
						}
					}
					else{
						deleteFiltro(SUB_MEDIA);
						contador--;
					}
				}return true;
				case IDC_CHECK_FILTROLAPLACIAN:{
					checkedFiltroLaplacian = SendDlgItemMessage(ghDialog, IDC_CHECK_FILTROLAPLACIAN, BM_GETCHECK, 0, 0);
					if (checkedFiltroLaplacian){
						if (contador < 4){
							orden[contador] = LAPLACIAN;
							contador++;
						}
					}
					else{
						deleteFiltro(LAPLACIAN);
						contador--;
					}
				}return true;
				case IDC_CHECK_FILTROALAPLACIAN:{
					checkedFiltroALaplacian = SendDlgItemMessage(ghDialog, IDC_CHECK_FILTROALAPLACIAN, BM_GETCHECK, 0, 0);
					if (checkedFiltroALaplacian){
						if (contador < 4){
							orden[contador] = ALAPLACIAN;
							contador++;
						}
					}
					else{
						deleteFiltro(ALAPLACIAN);
						contador--;
					}
				}return true;
				case IDC_CHECK_FILTRONORTH:{
					checkedFiltroNorth = SendDlgItemMessage(ghDialog, IDC_CHECK_FILTRONORTH, BM_GETCHECK, 0, 0);
					if (checkedFiltroNorth){
						if (contador < 4){
							orden[contador] = NORTH;
							contador++;
						}
					}
					else{
						deleteFiltro(NORTH);
						contador--;
					}
				}return true;
				case IDC_CHECK_FILTROEAST:{
					checkedFiltroEast = SendDlgItemMessage(ghDialog, IDC_CHECK_FILTROEAST, BM_GETCHECK, 0, 0);
					if (checkedFiltroEast){
						if (contador < 4){
							orden[contador] = EAST;
							contador++;
						}
					}
					else{
						deleteFiltro(EAST);
						contador--;
					}
				}return true;
				case IDC_CHECK_FILTROCSOBEL:{
					checkedFiltroCSobel = SendDlgItemMessage(ghDialog, IDC_CHECK_FILTROCSOBEL, BM_GETCHECK, 0, 0);
					if (checkedFiltroCSobel){
						if (contador < 4){
							orden[contador] = CSOBEL;
							contador++;
						}
					}
					else{
						deleteFiltro(CSOBEL);
						contador--;
					}
				}return true;
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
				}return true;
				case IDC_CHECK_FILTROGRAY:{
					checkedFiltroGray = SendDlgItemMessage(ghDialog, IDC_CHECK_FILTROGRAY, BM_GETCHECK, 0, 0);
					if (checkedFiltroGray){
						if (contador < 4){
							orden[contador] = GRAY;
							contador++;
						}
					}
					else{
						deleteFiltro(GRAY);
						contador--;
					}
				}return true;
				case IDC_CHECK_FILTROTHRES:{
					checkedFiltroThreshold = SendDlgItemMessage(ghDialog, IDC_CHECK_FILTROTHRES, BM_GETCHECK, 0, 0);
					if (checkedFiltroThreshold){
						if (contador < 4){
							orden[contador] = THRESH;
							contador++;
						}
					}
					else{
						deleteFiltro(THRESH);
						contador--;
					}
				}return true;

			}
		}return true;

		case WM_CLOSE: {
			EndDialog(
				ghDialog, // Handle to dialog to end.
				0);
			
		}return true;
		case WM_DESTROY: {
			EndDialog(
				ghDialog, // Handle to dialog to end.
				0);
			DestroyWindow(ghDialog);
		}return true;
		case WM_SIZING:{
			ancho = GetSystemMetrics(SM_CXSCREEN);
			alto = GetSystemMetrics(SM_CYSCREEN);
			alto = GetSystemMetrics(SM_CYSCREEN);
			centerElementX(hDisplay);
		}return true;
	}

	return false;
}


