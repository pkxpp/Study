/****************************************************************
* example4
* shows how to collect the information on video modes and formats
* that your video adapter supports
****************************************************************/
#include <windows.h>
#include <vector>
#include <string>
#include "resource.h"
#include <tchar.h>

// include directx9
#include <d3d9.h>

// global variables
LPDIRECT3D9             pD3D;				// the Direct3D Object
std::vector<std::string> adapterDetails;	// vector to hold the details
											// for the video adapter

////////////////////////////////////////////// forward declarations
bool initWindow(HINSTANCE hInstance);
INT_PTR CALLBACK DialogProc(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam);
void addItemToList(std::string item);
// DirectX functions
bool initDirect3D();	


/*********************************************************************
* WinMain
*********************************************************************/
int WINAPI WinMain(HINSTANCE hInstance, 
				   HINSTANCE hPrevInstance, 
				   LPSTR lpCmdLine, int nCmdShow)
{	
	// call our function to init direct3D
	if (!initDirect3D())
	{
		MessageBox(NULL, _T("Unable to init Direct3D"), _T("ERROR"), MB_OK);
		return false;
	}

	DialogBox(hInstance, MAKEINTRESOURCE(IDD_MODES), NULL, DialogProc);

	// release the device and the direct3D object
    if( pD3D != NULL)
        pD3D->Release();
		
	return 0;
}

INT_PTR CALLBACK DialogProc(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_INITDIALOG:
			for (unsigned int i=0; i < adapterDetails.size(); i++)
			{
				SendMessage(GetDlgItem(hwndDlg,IDC_LIST2), LB_INSERTSTRING, -1, (LPARAM)adapterDetails[i].c_str());
			}
			return TRUE;			
		break;

		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
				case IDOK:
				case IDCANCEL:
					EndDialog(hwndDlg, 0);
					return true;
				break;
			}
		break;

		case WM_CLOSE:
			PostQuitMessage(0);
			return 0;
		break;
	}
	
	return DefWindowProc(hwndDlg, uMsg, wParam, lParam);
}

/*********************************************************************
* initDirect3D
* initializes direct3D
*********************************************************************/
bool initDirect3D()
{
	pD3D = NULL;

	// create the directX object
	if( NULL == ( pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
	{
		return false;
	}

	// this section gets the adapter details
	D3DADAPTER_IDENTIFIER9 ident;
	pD3D->GetAdapterIdentifier(D3DADAPTER_DEFAULT, 0, &ident);	
	addItemToList("Adapter Details");
	addItemToList(ident.Description);
	addItemToList(ident.DeviceName);
	addItemToList(ident.Driver);
	
	// collects how many modes this adapter has
	UINT numModes = pD3D->GetAdapterModeCount(D3DADAPTER_DEFAULT, D3DFMT_X8R8G8B8);
	for (UINT i=0; i<numModes-1; i++)
	{
		D3DDISPLAYMODE mode;
		char modeString[255];
		// get the displaymode structure for this adapter mode
		pD3D->EnumAdapterModes(D3DADAPTER_DEFAULT, D3DFMT_X8R8G8B8, i, &mode);

		// draw a blank line in the listbox
		addItemToList("");
		// output the width
		sprintf(modeString, "Width=%d",mode.Width);
		addItemToList(modeString);
		// output the height
		sprintf(modeString, "Height=%d",mode.Height);		
		addItemToList(modeString);
		// output the refreshrate
		sprintf(modeString, "RefreshRate=%d",mode.RefreshRate);		
		addItemToList(modeString);		
	}	
	
	return true;
}

void addItemToList(std::string item)
{
	adapterDetails.push_back(item);
}