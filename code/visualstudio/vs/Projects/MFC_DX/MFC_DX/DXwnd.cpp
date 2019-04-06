// DXwnd.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC_DX.h"
#include "DXwnd.h"

using namespace myd3d;

// CDXwnd

IMPLEMENT_DYNAMIC(CDXwnd, CWnd)

CDXwnd::CDXwnd()
    : m_pDevice(NULL)
    , m_IsWindowed(true)
{
	m_pDevice = 0;
	m_pVB     = 0;
	m_pIB     = 0;
	m_DevType = D3DDEVTYPE_HAL;
}

CDXwnd::~CDXwnd()
{
	if(m_pDevice)
	{
		m_pDevice->Release();
		m_pDevice = 0;
    	m_pVB     = 0;
    	m_pIB     = 0;
	}
}


BEGIN_MESSAGE_MAP(CDXwnd, CWnd)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()



// CDXwnd 消息处理程序



BOOL CDXwnd::Create(LPCTSTR lpszAppName,int nWidth,int nHeight)
{
	// TODO: 在此添加专用代码和/或调用基类
	BOOL rs;
	m_Rect.top        = 0;
	m_Rect.left       = 0;	
	m_Rect.right      = nWidth;
	m_Rect.bottom     = nHeight;
	CString className = AfxRegisterWndClass(CS_DBLCLKS,
                  ::LoadCursor(NULL,IDC_ARROW),NULL,AfxGetApp()->LoadIcon(IDR_MAINFRAME));

	rs                = (CWnd::CreateEx(WS_EX_APPWINDOW,className,lpszAppName,WS_VISIBLE|WS_SYSMENU/*|WS_POPUP*/, 
		0, 0,nWidth,nHeight,NULL,NULL));
	if(rs!=FALSE)
	{
		InitDX();
		Setup();

//		DispIt(0.0f);
	
		MSG msg;
		::ZeroMemory(&msg, sizeof(MSG));

		for(::PeekMessage(&msg, 0, 0, 0, PM_REMOVE); msg.message != WM_QUIT; ::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))   
		{  
			//GetMessage无消息则阻塞进程
			::TranslateMessage(&msg);  
			::DispatchMessage(&msg);   
			DispIt(0.0f);
			Sleep(1000 / MAX_FRAMERATE);
		}
	}

	return rs;
}

bool CDXwnd::InitDX(void)
{
	//    
    // Init D3D:     
    //    
   
    HRESULT hr = 0; 
	// Step 1: Create the IDirect3D9 object.    
   
    IDirect3D9* d3d9 = 0;   
    d3d9 = Direct3DCreate9(D3D_SDK_VERSION);   
   
    if(!d3d9)   
    {   
        AfxMessageBox(_T("Direct3DCreate9() - FAILED"));   
        return false;   
    } 

	// Step 2: Check for hardware vp.    
   
    D3DCAPS9 caps;   
    d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT,m_DevType, &caps);   
   
    int vp = 0;   
    if( caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT )   
        vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;   
    else   
        vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;   
   
    // Step 3: Fill out the D3DPRESENT_PARAMETERS structure.
    
    D3DPRESENT_PARAMETERS d3dpp;   
	d3dpp.BackBufferWidth            = m_Rect.right;   
	d3dpp.BackBufferHeight           = m_Rect.bottom;   
    d3dpp.BackBufferFormat           = D3DFMT_A8R8G8B8;   
    d3dpp.BackBufferCount            = 1;   
    d3dpp.MultiSampleType            = D3DMULTISAMPLE_NONE;   
    d3dpp.MultiSampleQuality         = 0;   
    d3dpp.SwapEffect                 = D3DSWAPEFFECT_DISCARD;    
    d3dpp.hDeviceWindow              = this->m_hWnd;   
    d3dpp.Windowed                   = true;   
    d3dpp.EnableAutoDepthStencil     = true;    
    d3dpp.AutoDepthStencilFormat     = D3DFMT_D24S8;   
    d3dpp.Flags                      = 0;   
    d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;   
    d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;   
   
    // Step 4: Create the device.    
   
    hr = d3d9->CreateDevice(   
        D3DADAPTER_DEFAULT, // primary adapter    
        m_DevType,         // device type    
        this->m_hWnd,               // window associated with device    
        vp,                 // vertex processing    
        &d3dpp,             // present parameters    
        &m_pDevice);            // return created device    
   
    if( FAILED(hr) )   
    {   
        // try again using a 16-bit depth buffer    
        d3dpp.AutoDepthStencilFormat = D3DFMT_D16;   
           
        hr = d3d9->CreateDevice(   
            D3DADAPTER_DEFAULT,   
            m_DevType,   
            this->m_hWnd,   
            vp,   
            &d3dpp,   
            &m_pDevice);   
   
        if( FAILED(hr) )   
        {   
            d3d9->Release(); // done with d3d9 object    
            AfxMessageBox(_T("CreateDevice() - FAILED"));   
            return false;   
        }   
    }   
   
    d3d9->Release(); // done with d3d9 object  

	return true;
}

bool CDXwnd::Setup(void)
{
	m_pDevice->CreateVertexBuffer(8*sizeof(ColorVertex), D3DUSAGE_WRITEONLY, ColorVertex::FVF, D3DPOOL_MANAGED, &m_pVB, 0);
	m_pDevice->CreateIndexBuffer(36*sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB, 0);

	ColorVertex* vertices;
	m_pVB->Lock(0,0,(void**)&vertices,0);

	vertices[0] = ColorVertex(-1.0f, -1.0f, -1.0f, D3DCOLOR_XRGB(255,0,0));
	vertices[1] = ColorVertex(-1.0f, 1.0f, -1.0f, D3DCOLOR_XRGB(0,255,0));
	vertices[2] = ColorVertex(1.0f, 1.0f, -1.0f, D3DCOLOR_XRGB(0,0,255));
	vertices[3] = ColorVertex(1.0f, -1.0f, -1.0f, D3DCOLOR_XRGB(255,0,0));
	vertices[4] = ColorVertex(-1.0f, -1.0f, 1.0f, D3DCOLOR_XRGB(0,255,0));
	vertices[5] = ColorVertex(-1.0f, 1.0f, 1.0f, D3DCOLOR_XRGB(0,0,255));
	vertices[6] = ColorVertex(1.0f, 1.0f, 1.0f, D3DCOLOR_XRGB(255,0,0));
	vertices[7] = ColorVertex(1.0f, -1.0f, 1.0f, D3DCOLOR_XRGB(0,255,0));

	m_pVB->Unlock();


	WORD* indices = 0;
	m_pIB->Lock(0,0,(void**)&indices,0);

	indices[0]  = 0; indices[1]  = 1; indices[2]  = 2;
	indices[3]  = 0; indices[4]  = 2; indices[5]  = 3;

	indices[6]  = 4; indices[7]  = 6; indices[8]  = 5;
	indices[9]  = 4; indices[10] = 7; indices[11] = 6;

	indices[12] = 4; indices[13] = 5; indices[14] = 1;
	indices[15] = 4; indices[16] = 1; indices[17] = 0;

	indices[18] = 3; indices[19] = 2; indices[20] = 6;
	indices[21] = 3; indices[22] = 6; indices[23] = 7;

	indices[24] = 1; indices[25] = 5; indices[26] = 6;
	indices[27] = 1; indices[28] = 6; indices[29] = 2;

	indices[30] = 4; indices[31] = 0; indices[32] = 3;
	indices[33] = 4; indices[34] = 3; indices[35] = 7;

	m_pIB->Unlock();


/*	D3DXVECTOR3 position(0.0f,0.0f,-5.0f);
	D3DXVECTOR3 target(0.0f,0.0f,0.0f);
	D3DXVECTOR3 up(0.0f,1.0f,0.0f);
	D3DXMATRIX V;
	D3DXMatrixLookAtLH(&V,&position,&target,&up);

	m_Device->SetTransform(D3DTS_VIEW,&V);           //camera


	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(&proj,D3DX_PI*0.5f,(float)m_Rect.right/(float)m_Rect.bottom,1.0f,1000.0f);

	m_Device->SetTransform(D3DTS_PROJECTION,&proj);  //投影


	m_Device->SetRenderState(D3DRS_FILLMODE,D3DFILL_WIREFRAME);*/
	return true;
}

bool CDXwnd::DispIt(float timeDelta)
{
	if(m_pDevice) // Only use Device methods if we have a valid device.
	{
		//position and aim the camera
		D3DXVECTOR3 position(0.0f,0.0f,-5.0f);
	    D3DXVECTOR3 target(0.0f,0.0f,0.0f);
	    D3DXVECTOR3 up(0.0f,1.0f,0.0f);
    	D3DXMATRIX V;
    	D3DXMatrixLookAtLH(&V,&position,&target,&up);

    	m_pDevice->SetTransform(D3DTS_VIEW,&V);           


		//投影(Projection)
    	D3DXMATRIX proj;
		D3DXMatrixPerspectiveFovLH(&proj,D3DX_PI*0.5f,(float)m_Rect.right/(float)m_Rect.bottom,1.0f,1000.0f);

    	m_pDevice->SetTransform(D3DTS_PROJECTION,&proj);  

		m_pDevice->SetRenderState(/*D3DRS_FILLMODE,D3DFILL_WIREFRAME*/D3DRS_LIGHTING,false);


		//world space
		D3DXMATRIX Rx,Ry;
		D3DXMatrixRotationX(&Rx,3.14f/4.0f);

		static float y;
		D3DXMatrixRotationY(&Ry,y);
		y+=timeDelta;
		if(y>=6.28f)y=0.0f;

		D3DXMATRIX p=Rx*Ry;
		m_pDevice->SetTransform(D3DTS_WORLD,&p);

		m_pDevice->SetStreamSource(0,m_pVB,0,sizeof(ColorVertex));
		m_pDevice->SetIndices(m_pIB);
		m_pDevice->SetFVF(ColorVertex::FVF);

		m_pDevice->SetRenderState(D3DRS_SHADEMODE,D3DSHADE_GOURAUD/*D3DSHADE_FLAT/*D3DRS_FILLMODE,D3DFILL_WIREFRAME*/);
    // Instruct the device to set each pixel on the back buffer black -
    // D3DCLEAR_TARGET: 0x00000000 (black) - and to set each pixel on
    // the depth buffer to a value of 1.0 - D3DCLEAR_ZBUFFER: 1.0f.
        m_pDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,0x00000000, 1.0f, 0);

		m_pDevice->BeginScene();

		m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,8,0,12);

		m_pDevice->EndScene();

    // Swap the back and front buffers.
    	m_pDevice->Present(0, 0, 0, 0);
	}
	return true;
}

void CDXwnd::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if(nChar == VK_ESCAPE)
	{
		SendMessage(WM_CLOSE);
	}
	else
		this->DispIt(0.1f);

//	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}