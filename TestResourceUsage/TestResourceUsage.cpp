//===============================================================================================================================
//
// TestResourceUsage.cpp : Defines the entry point for the application.
//
//
//
//
//===============================================================================================================================

#include "stdafx.h"
#include "TestResourceUsage.h"
#include "Car.h"
#include <string>
#include <iostream>
#include "time.h"

#import "C:\program files\common files\system\ado\msado15.dll" rename("EOF", "ADOEOF")

using namespace std;
using namespace ADODB;

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

HWND hWnd;
HWND hWndSelected;
int cTxtLen;
LPWSTR pszMem;

bool boolWindowHidden = false;

// Forward declarations of functions included in this code module:
BOOL				InitInstance(HINSTANCE, int);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
//LRESULT CALLBACK	DlgProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK	DlgProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

clock_t start, end;

//===============================================================================================================================
void timerHandler (int signum)
{
	static int counter= 0;
	//printf ("timerHandler: counter= %d\n", counter++);
	fflush (stdout);
}

//===============================================================================================================================
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	
 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_TESTRESOURCEUSAGE, szWindowClass, MAX_LOADSTRING);
	
	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TESTRESOURCEUSAGE));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}

//===============================================================================================================================
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	//DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, reinterpret_cast<DLGPROC>(DlgProc));
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, DlgProc);

	return TRUE;
}

//===============================================================================================================================
BOOL OpenADORecordset(HWND hWndParent)
{
	HRESULT hr;
	CoInitialize(NULL);
	
	try
	{
		_ConnectionPtr conn;
		_RecordsetPtr rs;

		hr = conn.CreateInstance(__uuidof(Connection));
		hr = rs.CreateInstance(__uuidof(Recordset));

		conn->CursorLocation = adUseClient;
		
		_bstr_t strConn("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=C:\\InStore\\OMInStore.mdb;Persist Security Info=False");
		hr = conn->Open(strConn, "", "", adConnectUnspecified);

		rs->Open("SELECT * FROM Ingredient", conn.GetInterfacePtr(), adOpenForwardOnly, adLockReadOnly, adCmdText);

		while(!rs->ADOEOF)
		{
			_bstr_t strData(rs->Fields->GetItem(L"Description")->GetValue());

			cTxtLen = sizeof(strData);
			pszMem = (LPWSTR) VirtualAlloc((LPVOID)NULL, (DWORD)(cTxtLen + 1), MEM_COMMIT, PAGE_READWRITE);
			pszMem = strData;
			
			int index = SendDlgItemMessage(hWndParent, IDC_LIST1, LB_ADDSTRING, 0, (DWORD)((LPSTR)pszMem));
			int nTimes = rs->Fields->GetItem(L"ID")->GetValue();
			SendDlgItemMessage(hWndParent, IDC_LIST1, LB_SETITEMDATA, (WPARAM)index, (LPARAM)nTimes);
		   
			rs->MoveNext();
		}

		rs->Close();
	}
	catch(_com_error &e)
	{
		//dump_error(e);
		MessageBox(NULL, L"" + e.Error(), NULL, NULL);
	}
	catch(...)
	{
		cout << "Unhandled Exception";
	}; 
	
	return true;
}

//===============================================================================================================================
// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

//===============================================================================================================================
// Message handler for about box.
INT_PTR CALLBACK NewConnection(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

//===============================================================================================================================
INT_PTR CALLBACK Utilities(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

//===============================================================================================================================
//---------------------------------------------------------------------------
//LRESULT CALLBACK DlgProc(HWND hWndDlg, UINT Msg, WPARAM wParam, LPARAM lParam)
INT_PTR CALLBACK DlgProc(HWND hWndDlg, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	HWND hCtl;
	RECT rect;
	
	Car c;
	wstring strDisplay;
	
	switch(Msg)
	{
	case WM_INITDIALOG:
		//SetWindowPos(HWND hWnd, HWND hWndInsertAfter, int X, int Y, int cx, int cy, UINT uFlags);
		//SetWindowPos(hWndDlg, NULL, 400, 400, NULL, NULL, SWP_SHOWWINDOW);
		return TRUE;

	case WM_COMMAND:
		switch(wParam)
		{
		case IDC_BUTTON2:
			hWndSelected = GetDlgItem(hWndDlg, IDC_EDIT1);
			cTxtLen = GetWindowTextLength(hWndSelected);
			pszMem = (LPWSTR) VirtualAlloc((LPVOID)NULL, (DWORD)(cTxtLen + 1), MEM_COMMIT, PAGE_READWRITE);
			GetWindowText(hWndSelected, pszMem, cTxtLen + 1);
			if (pszMem != NULL) 
            { 
                SendDlgItemMessage(hWndDlg, IDC_COMBO1, CB_ADDSTRING, 0, (DWORD)((LPSTR)pszMem));
				
				hCtl = GetDlgItem(hWndDlg, IDC_COMBO1);
				GetClientRect(hCtl, &rect);
				MapDialogRect(hCtl, &rect);
				//SetWindowPos(hCtl, 0, 0, 0, rect.right, (n_choices + 1) * rect.bottom, SWP_NOMOVE);
				SetWindowPos(hCtl, 0, 0, 0, rect.right, 50, SWP_NOMOVE);

                SetWindowText(hWndSelected, (LPCWSTR)NULL); 
            } 
			MessageBox(hWndDlg, (LPCWSTR)pszMem, L"The Text is", NULL);
			VirtualFree(pszMem, 0, MEM_RELEASE);
			break;

		case IDOK:
			//EndDialog(hWndDlg, 0);
			//c = new Car();
			c.TopSpeed = 100;
			c.BodyColor = L"Green";
			//strDisplay = L"Hello";
			strDisplay = L"" + c.BodyColor;
			MessageBox(hWndDlg, strDisplay.c_str(), L"The color is", NULL);
			hWndSelected = GetDlgItem(hWndDlg, IDC_EDIT1);
			SetWindowText(hWndSelected, L"These are the times");
			return TRUE;

		case IDCANCEL:
			PostQuitMessage(0);
			//MessageBox(NULL, L"The Cancel Command", NULL, NULL);
			return TRUE;

		case IDM_EXIT:
			PostQuitMessage(0);
			break;

		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWndDlg, About);
			break;

		case ID_FILE_NEWCONNECTION:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG2), hWndDlg, NewConnection);
			break;

		case ID_FILE_UTILITIES:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG3), hWndDlg, Utilities);
			break;

		case ID_FILE_HIDEPANEL:
			hWndSelected = GetDlgItem(hWndDlg, IDC_STATIC2);
			if(boolWindowHidden == false)
			{
				ShowWindow(hWndSelected, SW_HIDE);
				boolWindowHidden = true;
			}
			else
			{
				ShowWindow(hWndSelected, SW_SHOW);
				boolWindowHidden = false;
			}
			break;

		case ID_FILE_CALLREADLOOP:
			OpenADORecordset(hWndDlg);
			break;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return FALSE;
}
//---------------------------------------------------------------------------

//===============================================================================================================================
// end of file
//===============================================================================================================================