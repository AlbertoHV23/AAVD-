#include <Windows.h>
#include "resource.h"
#include <string>
#include <string.h>
#include <fstream>

using namespace std;

HWND ghDlg = 0;
HINSTANCE _hInst;
int _show = 0;

bool validar = false;
char _TipoUsuario[] = "TipoUsuario.txt"; //Nombre del txt donde esta los tipos de usuarios
char _SuperUser[] = "SuperUser.txt";

class SuperUser
{
public:
	char _User[30] = "1814187"; //Temporal 
	char _Password[30] = "Password";
};
SuperUser* coordinador = new SuperUser;






BOOL CALLBACK ProcDialogo1(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam);
BOOL CALLBACK ProcDialogo2(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam);
BOOL CALLBACK ProcDialogo4(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam);

void LlenaEspecies(HWND objeto, UINT mensaje, char* file);
void leerSuperUser();
//void GuardarArchivo();



int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, PSTR cmd, int show)
{
	
	_hInst = hInst;
	_show = show;

	ghDlg = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG1),
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



BOOL CALLBACK ProcDialogo1(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam)
{

	//CC
	HDC hdcStatic = (HDC)wParam;
	static HBRUSH  hbrBkgnd = CreateSolidBrush(RGB(186, 201, 200));
	//
	static HWND hComboUser = 0;


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
		leerSuperUser();

		hComboUser = GetDlgItem(Dlg, IDC_COMBO1);
		LlenaEspecies(GetDlgItem(Dlg, IDC_COMBO1), CB_ADDSTRING, _TipoUsuario);

		
			return true;
	}

	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		
		case IDC_OLVIDE:
		{
			DialogBox(_hInst, MAKEINTRESOURCE(IDD_DIALOG4), Dlg, ProcDialogo4);
			return true;

		}
		
		case IDOK:
		{ 
			int index = SendMessage(hComboUser, CB_GETCURSEL, 0, 0);

			char UserT[30] = "";
			SendDlgItemMessage(Dlg, IDC_EDIT1, WM_GETTEXT, WPARAM(80), (LPARAM)UserT);
			char ContraseñaT[30] = "";
			SendDlgItemMessage(Dlg, IDC_EDIT2, WM_GETTEXT, WPARAM(80), (LPARAM)ContraseñaT);
			
			
			
			if (index ==0)
			{				   
				if (strcmp(coordinador->_User, UserT) == 0 && strcmp(coordinador->_Password, ContraseñaT) == 0)
				{
					validar = true;
				}


				if (validar ==true)
				{
					DialogBox(_hInst, MAKEINTRESOURCE(IDD_DIALOG2), Dlg, ProcDialogo2);
				}

				else
				{
					MessageBox(Dlg, "Verifique que la contaseña o cuenta sean correctos", "Ingrese nuevamente los datos"
						, MB_OK | MB_ICONERROR);
					SendDlgItemMessage(Dlg, IDC_EDIT1, WM_SETTEXT, 0, (LPARAM)"");
					SendDlgItemMessage(Dlg, IDC_EDIT2, WM_SETTEXT, 0, (LPARAM)"");
				}
			
			}
			else if(index == -1)
			{
				MessageBox(Dlg, "Ingrese un tipo de usuario", "Error"
					, MB_OK | MB_ICONERROR);
			}
			else
			{
				DialogBox(_hInst, MAKEINTRESOURCE(IDD_DIALOG3), Dlg, ProcDialogo2);
			}
			
			


			
			return true;
		}
		
		


		
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

BOOL CALLBACK ProcDialogo2(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam)
{

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
	{

		return true;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDCANCEL:
		{
			EndDialog(Dlg, 0);
		}







		}
		return true;
	}
	case WM_CLOSE:
	{
		//PostQuitMessage(0);
		EndDialog(Dlg,0);
	}
	case WM_DESTROY:
	{
		
		return true;
	}
	}


	return false;
}

BOOL CALLBACK ProcDialogo4(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam)
{

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
	{
		
		return true;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDOKA:
		{
			SendDlgItemMessage(Dlg, IDC_EDIT1, WM_GETTEXT, WPARAM(80), (LPARAM)coordinador->_User);
			SendDlgItemMessage(Dlg, IDC_EDIT2, WM_GETTEXT, WPARAM(80), (LPARAM)coordinador->_Password);
			
			EndDialog(Dlg, 0);
		}







		}
		return true;
	}
	case WM_CLOSE:
	{
		//PostQuitMessage(0);
		EndDialog(Dlg, 0);
	}
	case WM_DESTROY:
	{

		return true;
	}
	}


	return false;
}

void LlenaEspecies(HWND objeto, UINT mensaje, char* NombreArchivo)
{
	ifstream archivo;
	char row[30] = "";
	archivo.open(NombreArchivo);
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


void leerSuperUser() {
	
		ifstream archivo(_SuperUser);
		char linea[128];
		int contador = 0;
	
		while (!archivo.eof())
			{
				archivo.getline(linea, sizeof(linea));
				if (contador == 0) {

					strcpy(coordinador->_User, linea);
				}
				else if(contador ==1)
				{
					strcpy(coordinador->_Password, linea);
				}

				contador++;
				
			}
		archivo.close();
	}

/*void GuardarArchivo()
{

	SuperUser* aux = 0;

	ofstream archivaldo;
	archivaldo.open(_SuperUser);
	if (archivaldo.is_open())
	{
		// LEER la lista ligada
		aux = coordinador;
		while (aux != 0)
		{
			archivaldo.write((char*)aux, sizeof(SuperUser));



		}
		archivaldo.close();
	}




}


*/