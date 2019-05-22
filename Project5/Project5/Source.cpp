#include <Windows.h>
#include "resource.h"
#include <fstream>
#include <stdio.h>
#include <string.h>

using namespace std;

struct cita
{
	int		id;
	char	Nombre[80];
	char	Mascota[80];
	char	Especie[80];
	char	Telefono[20];
	char	Motivo[300];
	char	Fecha[11];
	char	Hora[9];
	char	Costo[200];
	bool	genero;
	//char textocont[200];

	cita	*prev = 0;
	cita	*sig = 0;

};

//Globales
cita *inicio = 0, *ultimo = 0;
cita AUX;

//Variables Globales
HWND ghDlg = 0;
HINSTANCE _hInst;
int _show = 0;
char file[80] = "Citas.txt";
bool aceptado = 0;
//bool lineas = true;
int k = 0;

char _arch_especies[] = "Especies.txt";
char _arch_DR[] = "InfoDR.txt";
char _pic[MAX_PATH] = "C:\\Users\\alber\\source\\repos\\Proyecto Progra A\\Proyecto Progra A\\Prueba.bmp";
char _folder[MAX_PATH] = ""; //
//char szFileName[MAX_PATH] = "";

//HELPME


//FUNCIONES  Dialogos 
BOOL CALLBACK ProcDialogo1(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam);
BOOL CALLBACK ProcDialogo2(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam);
BOOL CALLBACK ProcDialogo3(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam);
BOOL CALLBACK ProcDialogo4(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam);
BOOL CALLBACK ProcDialogo5(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam);
BOOL CALLBACK ProcDialogo6(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam);


//Funciones
void LlenaEspecies(HWND objeto, UINT mensaje, char *file);
void AgregaNodo();
void LeeArchivo();
void GuardarArchivo();
char *ConvierteFecha(char*Fecha);
void llenalista(HWND Dlg);
void validarCita(HWND Dlg);
void eliminarNodo();
void llenalistaFecha(HWND Dlg, int fecha);
void BuscarNodo(HWND Dlg, char *text);
void BuscarMascota(HWND Dlg, char *text);
//void MostarLista(HWND objeto, UINT mensa);
//void EscribirDatos();
//void LeeDatos();
//void BuscarNodo();
void PonerImagen(HWND dlg, WPARAM IDC, char *imagen, int m, int n);
//void leerDoctor(HWND objeto, UINT mensaje, char *NombreDeDocumento);


int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, PSTR cmd, int show)
{
	//GetCurrentDirectory(MAX_PATH, _folder);//
	//MessageBox(0, _folder, "folder", MB_ABORTRETRYIGNORE); //
	_hInst = hInst;
	_show = show;
	ghDlg = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOGO),
		0, ProcDialogo1);
	ShowWindow(ghDlg, show);
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	while (GetMessage(&msg, 0, 0, 0))
	{
		if (ghDlg == 0 || !IsDialogMessage(ghDlg, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return (int)msg.wParam;
}

//Dialogo principal..
BOOL CALLBACK ProcDialogo1(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam)
{
	//static HBITMAP bmp1, bmp2;
	//bmp1 = (HBITMAP)SendDlgItemMessage(Dlg, IDC_STATIC1,
		//STM_GETIMAGE, IMAGE_BITMAP, 0);
	//bmp2 = (HBITMAP)LoadImage(NULL, "C:\\Users\\alber\\source\\repos\\Proyecto Progra A\\Proyecto Progra A\\Prueba.bmp",
		//IMAGE_BITMAP, 50, 70, LR_LOADFROMFILE);

	//CC
	HDC hdcStatic = (HDC)wParam;
	static HBRUSH  hbrBkgnd = CreateSolidBrush(RGB(186, 201, 200));
	//
	static HWND hCte = 0;


	switch (Mensaje)
	{

		//Cambiar color
	case WM_CTLCOLORDLG:
		return(INT_PTR)hbrBkgnd;

	case WM_CTLCOLORSTATIC:
		SetTextColor(hdcStatic, RGB(0, 0, 0));
		SetBkColor(hdcStatic, RGB(186, 201, 200));
		return(INT_PTR)hbrBkgnd;
		//
	case WM_INITDIALOG:
		char fectem[12];
		int fecha;
		SendDlgItemMessage(Dlg, IDC_DATETIMEPICKER1, WM_GETTEXT, WPARAM(12), (LPARAM)fectem);
		strcpy(fectem, ConvierteFecha(fectem));
		fecha = atoi(fectem);

		PonerImagen(Dlg, IDC_STATIC1, _pic, 50, 70);
		LeeArchivo();
	
		
		llenalistaFecha(Dlg, fecha);

		{
			hCte = GetDlgItem(Dlg, IDC_EDIT1);
			//PonerImagen(Dlg, IDC_STATIC1, _pic, 83, 108);

		//SendDlgItemMessage(
		//	Dlg,
		//	IDC_STATIC1,
		//	STM_SETIMAGE,
		//	IMAGE_BITMAP,
		//	(LPARAM)bmp2);

			//llenalista(Dlg);

			return true;
		}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{

			//MENU
		case ID_INICIO_SALIR:

		{
			if (MessageBox(Dlg, "¿Desea guardar los datos?",
				"Salir",
				MB_YESNO |
				MB_ICONASTERISK | MB_DEFBUTTON1) == IDYES)
			{
				MessageBox(Dlg, "Datos Guardaddos", "Sin perdida de datos", MB_OK | MB_ICONINFORMATION);
				GuardarArchivo();
				
				PostQuitMessage(0);
			}
			else {
				//MessageBox(Dlg, "No se guardo nada..", "informacion", MB_OK | MB_ICONEXCLAMATION);
				PostQuitMessage(0);
			}
			//Guardar 
			return true;
		}


		case ID_INICIO_PROXIMAS:
		{
			DialogBox(_hInst, MAKEINTRESOURCE(IDD_DIALOGO5), Dlg, ProcDialogo5);
			return true;
		}

		case ID_CITAS_AGREGARCITA:
		{
			DialogBox(_hInst, MAKEINTRESOURCE(IDD_DIALOGO2), Dlg, ProcDialogo2);
			return true;
		}

		case ID_CITAS_MODIFICARCITA:
		{
			DialogBox(_hInst, MAKEINTRESOURCE(IDD_DIALOGO2), Dlg, ProcDialogo2);
			return true;
		}

		case ID_BUSCAR_CITA:
		{
			DialogBox(_hInst, MAKEINTRESOURCE(IDD_DIALOGO3), Dlg, ProcDialogo3);
			return true;

		}
		case ID_AYUDA_INFORMACIONDELDOCTOR:
		{
			DialogBox(_hInst, MAKEINTRESOURCE(IDD_DIALOGO4), Dlg, ProcDialogo4);
			return true;

		}
	

		//

		//BOTONES
		case IDC_BUTTONAC:
			DialogBox(_hInst, MAKEINTRESOURCE(IDD_DIALOGO2), Dlg, ProcDialogo2);
			return true;

		case IDC_BUTTONBUS:
			DialogBox(_hInst, MAKEINTRESOURCE(IDD_DIALOGO3), Dlg, ProcDialogo3);
			return true;

		case IDC_BUTTONDOC:
			DialogBox(_hInst, MAKEINTRESOURCE(IDD_DIALOGO4), Dlg, ProcDialogo4);
			return true;
		}
		return true;
	}
	case WM_CLOSE:
	{
		PostQuitMessage(0);
	}
	case WM_DESTROY:
	{

		return true;
	}
	}
	return false;
}

//Agregar Cita
BOOL CALLBACK ProcDialogo2(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam)
{
	//CC
	HDC hdcStatic = (HDC)wParam;
	static HBRUSH  hbrBkgnd = CreateSolidBrush(RGB(186, 201, 200));
	//

	static HWND hComboEspecie = 0;
	switch (Mensaje)
	{
		//Cambiar color
	case WM_CTLCOLORDLG:
	{
		return(INT_PTR)hbrBkgnd;
	}

	case WM_CTLCOLORSTATIC:
	{
		SetTextColor(hdcStatic, RGB(0, 0, 0));
		SetBkColor(hdcStatic, RGB(186, 201, 200));
		return(INT_PTR)hbrBkgnd;
	}
	//

	case WM_INITDIALOG:
	{
		hComboEspecie = GetDlgItem(Dlg, IDC_COMBO1);
		LlenaEspecies(GetDlgItem(Dlg, IDC_COMBO1), CB_ADDSTRING, _arch_especies);
		return true;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
			//MENU
		case ID_INICIO_SALIR:
		{

			MessageBox(Dlg, "¿Seguro que quieres salir?", "Salir", MB_YESNO);
			PostQuitMessage(0);
			//Guardar 
			return true;
		}



		case ID_INICIO_PROXIMAS:
		{
			DialogBox(_hInst, MAKEINTRESOURCE(IDD_DIALOGO5), Dlg, ProcDialogo5);
			return true;
		}

		case ID_CITAS_AGREGARCITA:
		{
			DialogBox(_hInst, MAKEINTRESOURCE(IDD_DIALOGO2), Dlg, ProcDialogo2);
			return true;
		}

		case ID_CITAS_MODIFICARCITA:
		{
			DialogBox(_hInst, MAKEINTRESOURCE(IDD_DIALOGO2), Dlg, ProcDialogo2);
			return true;
		}

		case ID_BUSCAR_CITA:
		{
			DialogBox(_hInst, MAKEINTRESOURCE(IDD_DIALOGO3), Dlg, ProcDialogo3);
			return true;

		}
		case ID_AYUDA_INFORMACIONDELDOCTOR:
		{
			DialogBox(_hInst, MAKEINTRESOURCE(IDD_DIALOGO4), Dlg, ProcDialogo4);
			return true;

		}
		


		//


		case IDC_AGREGAR:
		{
			SendDlgItemMessage(Dlg, IDC_NCliente, WM_GETTEXT, sizeof(AUX.Nombre), (LPARAM)AUX.Nombre);

			//strcpy(AUX.Fecha, ConvierteFecha(AUX.Fecha));
			SendDlgItemMessage(Dlg, IDC_NMascota, WM_GETTEXT, sizeof(AUX.Nombre), (LPARAM)AUX.Mascota);
			SendDlgItemMessage(Dlg, IDC_Telefono, WM_GETTEXT, sizeof(AUX.Nombre), (LPARAM)AUX.Telefono);

			SendDlgItemMessage(Dlg, IDC_COMBO1, WM_GETTEXT, sizeof(AUX.Nombre), (LPARAM)AUX.Especie);
			SendDlgItemMessage(Dlg, IDC_Costo, WM_GETTEXT, sizeof(AUX.Nombre), (LPARAM)AUX.Costo);

			SendDlgItemMessage(Dlg, IDC_Motivo, WM_GETTEXT, sizeof(AUX.Nombre), (LPARAM)AUX.Motivo);
			SendDlgItemMessage(Dlg, IDC_Fecha, WM_GETTEXT, sizeof(AUX.Nombre), (LPARAM)AUX.Fecha);
			SendDlgItemMessage(Dlg, IDC_Hora, WM_GETTEXT, sizeof(AUX.Nombre), (LPARAM)AUX.Hora);


			validarCita(Dlg);

			if (aceptado == true) {




				SendDlgItemMessage(Dlg, IDC_NCliente, WM_SETTEXT, sizeof(AUX.Nombre), (LPARAM)"");
				SendDlgItemMessage(Dlg, IDC_NMascota, WM_SETTEXT, sizeof(AUX.Nombre), (LPARAM)"");
				SendDlgItemMessage(Dlg, IDC_Telefono, WM_SETTEXT, sizeof(AUX.Nombre), (LPARAM)"");
				SendDlgItemMessage(Dlg, IDC_COMBO1, WM_SETTEXT, sizeof(AUX.Nombre), (LPARAM)"");
				SendDlgItemMessage(Dlg, IDC_Motivo, WM_SETTEXT, sizeof(AUX.Nombre), (LPARAM)"");
				SendDlgItemMessage(Dlg, IDC_Costo, WM_SETTEXT, sizeof(AUX.Nombre), (LPARAM)"");


			}




		}


		return true;

		case IDC_CANCELAR:
		{
			EndDialog(Dlg, 0);
			return true;
		}

		}
		return true;
	}
	case WM_CLOSE:
	{
		EndDialog(Dlg, 0);
	}
	case WM_DESTROY:
	{
		return true;
	}
	}
	return false;
}

//Buscar
BOOL CALLBACK ProcDialogo3(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam)
{
	//CC
	HDC hdcStatic = (HDC)wParam;
	static HBRUSH  hbrBkgnd = CreateSolidBrush(RGB(186, 201, 200));
	//

	static HWND hComboEspecie = 0;
	switch (Mensaje)
	{
		//Cambiar color
	case WM_CTLCOLORDLG:
		return(INT_PTR)hbrBkgnd;

	case WM_CTLCOLORSTATIC:
		SetTextColor(hdcStatic, RGB(0, 0, 0));
		SetBkColor(hdcStatic, RGB(186, 201, 200));
		return(INT_PTR)hbrBkgnd;
		//

	case WM_INITDIALOG:
	{
		llenalista(Dlg);
		return true;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
			//MENU
		case ID_INICIO_SALIR:
			MessageBox(Dlg, "¿Seguro que quieres salir?", "Salir", MB_YESNO);
			PostQuitMessage(0);
			//Guardar 
			return true;


		case ID_INICIO_PROXIMAS:
			DialogBox(_hInst, MAKEINTRESOURCE(IDD_DIALOGO5), Dlg, ProcDialogo5);
			return true;

		case ID_CITAS_AGREGARCITA:
			DialogBox(_hInst, MAKEINTRESOURCE(IDD_DIALOGO2), Dlg, ProcDialogo2);
			return true;

		case ID_CITAS_MODIFICARCITA:
			DialogBox(_hInst, MAKEINTRESOURCE(IDD_DIALOGO2), Dlg, ProcDialogo2);
			return true;

		case ID_BUSCAR_CITA:
			DialogBox(_hInst, MAKEINTRESOURCE(IDD_DIALOGO3), Dlg, ProcDialogo3);
			return true;

		case ID_AYUDA_INFORMACIONDELDOCTOR:
			DialogBox(_hInst, MAKEINTRESOURCE(IDD_DIALOGO4), Dlg, ProcDialogo4);
			return true;

		

		case IDC_BOTONBUSCAR:
		{
			char buscado[80];
			SendDlgItemMessage(Dlg, IDC_EDIT1, WM_GETTEXT, WPARAM(80), (LPARAM)buscado);
			BuscarMascota(Dlg, buscado);
			llenalista(Dlg);
			SendDlgItemMessage(Dlg, IDC_LIST1, LB_RESETCONTENT, 0, 0);
			
			return true;

		}
			//
		case IDOK222: 
			EndDialog(Dlg, 0);
			return true;


		}
		return true;
	}
	case WM_CLOSE:
	{
		EndDialog(Dlg, 0);
	}
	case WM_DESTROY:
	{
		return true;
	}
	}
	return false;
}

//INFO DOCTOR
BOOL CALLBACK ProcDialogo4(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam)
{



	//CC
	HDC hdcStatic = (HDC)wParam;
	static HBRUSH  hbrBkgnd = CreateSolidBrush(RGB(186, 201, 200));
	//


	switch (Mensaje)
	{

		//Cambiar color
	case WM_CTLCOLORDLG:
		return(INT_PTR)hbrBkgnd;

	case WM_CTLCOLORSTATIC:
		SetTextColor(hdcStatic, RGB(0, 0, 0));
		SetBkColor(hdcStatic, RGB(186, 201, 200));
		return(INT_PTR)hbrBkgnd;
		//

	case WM_INITDIALOG:

	{
		PonerImagen(Dlg, IDC_STATIC1, _pic, 50, 70);

		return true;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
			//MENU
		case ID_INICIO_SALIR:
		{

			MessageBox(Dlg, "¿Seguro que quieres salir?", "Salir", MB_YESNO);
			PostQuitMessage(0);
			//Guardar 
			return true;
		}



		case ID_INICIO_PROXIMAS:
		{
			DialogBox(_hInst, MAKEINTRESOURCE(IDD_DIALOGO5), Dlg, ProcDialogo5);
			return true;
		}

		case ID_CITAS_AGREGARCITA:
		{
			DialogBox(_hInst, MAKEINTRESOURCE(IDD_DIALOGO2), Dlg, ProcDialogo2);
			return true;
		}

		case ID_CITAS_MODIFICARCITA:
		{
			DialogBox(_hInst, MAKEINTRESOURCE(IDD_DIALOGO2), Dlg, ProcDialogo2);
			return true;
		}

		case ID_BUSCAR_CITA:
		{
			DialogBox(_hInst, MAKEINTRESOURCE(IDD_DIALOGO3), Dlg, ProcDialogo3);
			return true;

		}
		case ID_AYUDA_INFORMACIONDELDOCTOR:
		{
			DialogBox(_hInst, MAKEINTRESOURCE(IDD_DIALOGO4), Dlg, ProcDialogo4);
			return true;

		}
	


		//

		case IDOK:
			EndDialog(Dlg, 0);
			return true;


		}
		return true;
	}
	case WM_CLOSE:
	{
		EndDialog(Dlg, 0);
	}
	case WM_DESTROY:
	{
		return true;
	}
	}
	return false;
}

//AGENDA
BOOL CALLBACK ProcDialogo5(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam)
{
	//CC
	HDC hdcStatic = (HDC)wParam;
	static HBRUSH  hbrBkgnd = CreateSolidBrush(RGB(186, 201, 200));
	//
	static HWND hList = 0;

	//hList = GetDlgItem(Dlg, IDC_LIST1);


	static HWND hComboEspecie = 0;
	switch (Mensaje)
	{
		//Cambiar color
	case WM_CTLCOLORDLG:
		return(INT_PTR)hbrBkgnd;

	case WM_CTLCOLORSTATIC:
		SetTextColor(hdcStatic, RGB(0, 0, 0));
		SetBkColor(hdcStatic, RGB(186, 201, 200));
		return(INT_PTR)hbrBkgnd;
		//

	case WM_INITDIALOG:
	{
		llenalista(Dlg);
		
		return true;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
			//MENU
		case ID_INICIO_SALIR:
		{

			MessageBox(Dlg, "¿Seguro que quieres salir?", "Salir", MB_YESNO);
			PostQuitMessage(0);
			//Guardar 
			return true;
		}



		case ID_INICIO_PROXIMAS:
		{
			DialogBox(_hInst, MAKEINTRESOURCE(IDD_DIALOGO5), Dlg, ProcDialogo5);
			return true;
		}

		case ID_CITAS_AGREGARCITA:
		{
			DialogBox(_hInst, MAKEINTRESOURCE(IDD_DIALOGO2), Dlg, ProcDialogo2);
			return true;
		}

		case ID_CITAS_MODIFICARCITA:
		{
			DialogBox(_hInst, MAKEINTRESOURCE(IDD_DIALOGO2), Dlg, ProcDialogo2);
			return true;
		}

		case ID_BUSCAR_CITA:
		{
			DialogBox(_hInst, MAKEINTRESOURCE(IDD_DIALOGO3), Dlg, ProcDialogo3);
			return true;

		}
		case ID_AYUDA_INFORMACIONDELDOCTOR:
		{
			DialogBox(_hInst, MAKEINTRESOURCE(IDD_DIALOGO4), Dlg, ProcDialogo4);
			return true;

		}
	

		
		


		//


		case IDC_BUTTON11:
			
		{
			//Solo me pone uno, quien sabe porque... // Ya lo corregi

			char fectem[12];
			int fecha;
			SendDlgItemMessage(Dlg, IDC_DATETIMEPICKER10, WM_GETTEXT, WPARAM(12), (LPARAM)fectem);
			strcpy(fectem, ConvierteFecha(fectem));
			fecha = atoi(fectem);


			llenalistaFecha(Dlg, fecha);
			return true;

		}


		case IDOK: {


		}
			

			DialogBox(_hInst, MAKEINTRESOURCE(IDD_DIALOGO2), Dlg, ProcDialogo2);
			return true;

		case IDOK2:
			EndDialog(Dlg, 0);
			return true;
			
//
		case IDOK4:
		{
			char texto[20];
			//Obtiene el indice de la lista
			int index = SendDlgItemMessage(Dlg, IDC_LIST1, LB_GETCURSEL, 0, 0);

			
			
			if (index == 0 || index == 5 || index == 10 || index == 15 || index == 20 || index == 25 || index == 30 || index == 35  )
			{

				SendMessage(GetDlgItem(Dlg, IDC_LIST1), LB_GETTEXT, (WPARAM)index, (LPARAM)texto);

				BuscarNodo(Dlg, texto);


				MessageBox(Dlg, "Elemento Eliminado", "Borrado", MB_OK | MB_ICONINFORMATION);
				SendDlgItemMessage(Dlg, IDC_LIST1, LB_RESETCONTENT, 0, 0);
				llenalista(Dlg);
			}
			else
			{
				MessageBox(Dlg, "Seleccione un elmento", "Error", MB_OK | MB_ICONINFORMATION);
			}


			return true;
//

		}




		}
		return true;
	}
	case WM_CLOSE:
	{
		EndDialog(Dlg, 0);
	}
	case WM_DESTROY:
	{
		return true;
	}
	}
	return false;
}

//Modificar
BOOL CALLBACK ProcDialogo6(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam)
{
	//CC
	HDC hdcStatic = (HDC)wParam;
	static HBRUSH  hbrBkgnd = CreateSolidBrush(RGB(186, 201, 200));
	//

	static HWND hComboEspecie = 0;
	switch (Mensaje)
	{
		//Cambiar color
	case WM_CTLCOLORDLG:
		return(INT_PTR)hbrBkgnd;

	case WM_CTLCOLORSTATIC:
		SetTextColor(hdcStatic, RGB(0, 0, 0));
		SetBkColor(hdcStatic, RGB(186, 201, 200));
		return(INT_PTR)hbrBkgnd;
		//

	case WM_INITDIALOG:
	{
		return true;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
			//MENU
		case ID_INICIO_SALIR:
		{

			MessageBox(Dlg, "¿Seguro que quieres salir?", "Salir", MB_YESNO);
			PostQuitMessage(0);
			//Guardar 
			return true;
		}



		case ID_INICIO_PROXIMAS:
		{
			DialogBox(_hInst, MAKEINTRESOURCE(IDD_DIALOGO5), Dlg, ProcDialogo5);
			return true;
		}

		case ID_CITAS_AGREGARCITA:
		{
			DialogBox(_hInst, MAKEINTRESOURCE(IDD_DIALOGO2), Dlg, ProcDialogo2);
			return true;
		}

		case ID_CITAS_MODIFICARCITA:
		{
			DialogBox(_hInst, MAKEINTRESOURCE(IDD_DIALOGO2), Dlg, ProcDialogo2);
			return true;
		}

		case ID_BUSCAR_CITA:
		{
			DialogBox(_hInst, MAKEINTRESOURCE(IDD_DIALOGO3), Dlg, ProcDialogo3);
			return true;

		}
		case ID_AYUDA_INFORMACIONDELDOCTOR:
		{
			DialogBox(_hInst, MAKEINTRESOURCE(IDD_DIALOGO4), Dlg, ProcDialogo4);
			return true;

		}
	


		//

		case IDC_BUTTON2:
			MessageBox(Dlg, "Elemento eliminado...", "Eliminar", MB_YESNO);

			return true;

		case IDCANCEL:
			EndDialog(Dlg, 0);
			return true;

		}
		return true;
	}
	case WM_CLOSE:
	{
		EndDialog(Dlg, 0);
	}
	case WM_DESTROY:
	{
		return true;
	}
	}
	return false;
}


//Hay un problema con el costo al momento de guardar...

void LlenaEspecies(HWND objeto, UINT mensaje, char *NombreDeDocumento)
{
	ifstream archivo;
	char row[30] = "";
	archivo.open(NombreDeDocumento);
	if (archivo.is_open())
	{
		archivo.getline(row, 30);
		while (!archivo.eof())
		{
			SendMessage(objeto, mensaje, 0, (LPARAM)row);
			archivo.getline(row, 30);
		}

		archivo.close();
	}

}

//Funciona
void AgregaNodo() {
	cita *aux = new cita;

	*aux = AUX;

	if (inicio == 0) {

		inicio = aux;
		aux->sig = 0;
		ultimo = aux;
	}
	else {
		aux->prev = ultimo;
		ultimo->sig = aux;
		ultimo = aux;
		aux->sig = 0;
	}
}

//Funciona
void GuardarArchivo()
{

	cita *aux = 0;

	ofstream archivaldo;
	archivaldo.open(file, ios::binary | ios::trunc);
	if (archivaldo.is_open())
	{
		// LEER la lista ligada
		aux = inicio;
		while (aux != 0)
		{
			archivaldo.write((char*)aux, sizeof(cita));

			aux = aux->sig;

		}
		archivaldo.close();
	}
	else
	{

	}




}

//Funciona
void LeeArchivo()
{
	cita *pinfo = 0;

	ifstream archivaldo;
	archivaldo.open(file, ios::binary);  // | ios::trunc
	if (archivaldo.is_open())
	{
		pinfo = new cita;
		archivaldo.read((char*)pinfo, sizeof(cita));
		pinfo->prev = 0;
		pinfo->sig = 0;

		while (!archivaldo.eof()) {
			if (inicio == 0)
			{
				inicio = pinfo;
				ultimo = pinfo;
			}
			else
			{
				ultimo->sig = pinfo;
				pinfo->prev = ultimo;

				ultimo = pinfo;
			}

			pinfo = new cita;
			archivaldo.read((char*)pinfo, sizeof(cita));
			pinfo->prev = 0;
			pinfo->sig = 0;

		}
		//AgregaNodo(info);                                //aqui se acomoda el nodo en la lista ligada
		//archivaldo.read((char*)&info, sizeof(node));

		archivaldo.close();
	}
	else
	{

	}
};

//Funciona
char*ConvierteFecha(char*Fecha)
{
	char FechaOk[11] = "";

	string cutMonth, FechaOri;
	cutMonth = Fecha; //Guarda en un string el valor de un char
	string Day = cutMonth.substr(0, 2);
	string Month = cutMonth.substr(3, 2);
	string Year = cutMonth.substr(6, 4);

	FechaOri = Day + Month + Year;
	strcat(FechaOk, FechaOri.c_str());

	return FechaOk;
}

//Funciona
void llenalista(HWND Dlg) {

	cita *aux = inicio;
	//cita datos;



	SendDlgItemMessage(Dlg, IDC_LIST1, LB_RESETCONTENT, 0, 0);


	while (aux != 0) {
		//char fecha_temp[11] = "";
		//char hora_temp[11] = "";
		//char mascota[80] = "";
		//strcpy(mascota, aux->Nombre);
		//strcpy(fecha_temp, aux->Fecha);
		//strcpy(hora_temp, aux->Hora);
		//	  
		//			
		//char MostrarDatos[MAX_PATH] = "";
		//string nombre = mascota;
		//string fecha = fecha_temp;
		//string hora = hora_temp;
		//string datos = mascota + fecha_temp + hora_temp;
		//
		//strcat(MostrarDatos, datos.c_str());


		SendDlgItemMessage(Dlg, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)aux->Mascota);
		SendDlgItemMessage(Dlg, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)aux->Fecha);
		SendDlgItemMessage(Dlg, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)aux->Hora);
		SendDlgItemMessage(Dlg, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)aux->Especie);
		SendDlgItemMessage(Dlg, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)"_________________________________________________________________________________________________");
		//SendDlgItemMessage(Dlg, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)aux->id);





		aux = aux->sig;


	}


}

//Funciona
void validarCita(HWND Dlg) {

	aceptado = 0;


	int contador = FALSE;

	for (int i = 0; AUX.Telefono[i] != NULL; i++) {
		contador++;
	}


	if (!aceptado) {
		if (AUX.Nombre[0] != 0) {
			if (contador == 8 || contador == 10 || contador == 12) {

				if (AUX.Especie[0] != 0) {
					MessageBox(Dlg, "Cita guardada", "Agregado Correctamente", MB_OK | MB_ICONINFORMATION);
					aceptado = TRUE;

				}
				else {

					MessageBox(Dlg, "Ingrese la especie de la mascota", "Error", MB_OK | MB_ICONERROR);
				}


			}
			else {

				MessageBox(Dlg, "Ingresa numero de telefono valido", "Error", MB_OK | MB_ICONERROR);
			}
		}
		else {

			MessageBox(Dlg, "Ingrese el nombre del Cliente", "Error", MB_OK | MB_ICONERROR);
		}



		if (aceptado) {
			AgregaNodo();
			GuardarArchivo();

		}


	}



}

// No Funciona
void eliminarNodo() {
	cita *aux = inicio;
	while (aux != 0)
	{

	}



}


void llenalistaFecha(HWND Dlg, int fecha) {
	char FechaHoy[12];
	cita *aux = inicio;
	//cita datos;
	bool siloencontre = false;
	int fechaaux;
	char fechaus[20];

	SendDlgItemMessage(Dlg, IDC_LIST1, LB_RESETCONTENT, 0, 0);
	
	while (aux != 0)

	{
		strcpy(fechaus, ConvierteFecha(aux->Fecha));
		fechaaux = atoi(fechaus);
		if (fecha == fechaaux)
		{

			siloencontre = true;

			SendDlgItemMessage(Dlg, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)aux->Mascota);
			SendDlgItemMessage(Dlg, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)aux->Fecha);
			SendDlgItemMessage(Dlg, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)aux->Hora);
			SendDlgItemMessage(Dlg, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)aux->Especie);
			SendDlgItemMessage(Dlg, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)"_________________________________________________________________________________________________");
			
			
		}
		aux = aux->sig;
	}
		
		
	
	



}
//Ya busca...

void BuscarNodo(HWND Dlg, char *text) {

	char texto[12];
	cita *aux = inicio;
	//cita datos;
	bool siloencontre = false;

	
	
	char mascotausuario[20];

	

	while (aux != 0)

	{
		strcpy(texto, aux->Mascota);
		if (strcmp(text,texto) ==0)
		{
			cita *auxiliar = aux;
			cita *previo = aux;
			if (auxiliar == inicio)
			{
				inicio = inicio->sig;
				delete auxiliar;

			}
			else
			{
				previo->prev->sig = auxiliar->sig;
				auxiliar->sig->prev = previo->prev;
				delete auxiliar;

			}

			break;
		}

		aux = aux->sig;
	}
	


}


void BuscarMascota(HWND Dlg, char *text) {

	char texto[12];
	cita *aux = inicio;
	//cita datos;
	bool siloencontre = false;



	char mascotausuario[20];



	while (aux != 0)

	{
		strcpy(texto, aux->Mascota);
		if (strcmp(text, texto) == 0)
		{
			SendDlgItemMessage(Dlg, IDC_LIST15, LB_ADDSTRING, 0, (LPARAM)aux->Mascota);
			SendDlgItemMessage(Dlg, IDC_LIST15, LB_ADDSTRING, 0, (LPARAM)aux->Fecha);
			SendDlgItemMessage(Dlg, IDC_LIST15, LB_ADDSTRING, 0, (LPARAM)aux->Hora);
			SendDlgItemMessage(Dlg, IDC_LIST15, LB_ADDSTRING, 0, (LPARAM)aux->Especie);
			SendDlgItemMessage(Dlg, IDC_LIST15, LB_ADDSTRING, 0, (LPARAM)aux->Telefono);
			SendDlgItemMessage(Dlg, IDC_LIST15, LB_ADDSTRING, 0, (LPARAM)aux->Motivo);
			//SendDlgItemMessage(Dlg, IDC_LIST15, LB_ADDSTRING, 0, (LPARAM)aux->Costo);
		}

		aux = aux->sig;
	}



}

//Funciona

void PonerImagen(HWND dlg, WPARAM IDC, char *imagen, int m, int n) {

	static HBITMAP bmp1, bmp2;
	//Al objeto bmp1, se le asigna sin imagen:
	bmp1 = (HBITMAP)SendDlgItemMessage(dlg, IDC,
		STM_GETIMAGE, IMAGE_BITMAP, 0);
	//Al objeto bmp2, se le asigna una imagen local:
	bmp2 = (HBITMAP)LoadImage(NULL, imagen,
		IMAGE_BITMAP, m, n, LR_LOADFROMFILE);

	SendDlgItemMessage(
		dlg,
		IDC,
		STM_SETIMAGE,
		IMAGE_BITMAP,
		(LPARAM)bmp2);

}



	