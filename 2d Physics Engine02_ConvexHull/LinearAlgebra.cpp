// LinearAlgebra.cpp : Defines the entry point for the application.
//

#include "LinearAlgebra.h"
//_20180519_jintaeks
#include "KVectorUtil.h"
#include <MMSystem.h>
#include "KMatrix2.h"
#include <objidl.h>
#include <gdiplus.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <windowsx.h>
#include "KVector2.h"
#include <vector>
#include <limits>
#include <string>
#undef min
#undef max
#include <cstring> // strlen, memcpy, etc.
#include <cstdlib> // exit
#include <cfloat>  // FLT_MAX
#include <vector>
#include "KRigidbody.h"
#include "KShape.h"
#include "KCircleShape.h"
#include "KPolygonShape.h"
#include "KWorld.h"

#pragma warning(disable:4244)

using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")
#pragma comment(lib,"winmm.lib")

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
// _20180519_jintaeks
HWND    g_hwnd = NULL;
HDC     g_hdc = 0;
HBITMAP g_hBitmap = 0;
RECT    g_clientRect;
int		g_mouseLButtonDown = 0;
int		g_mouseRButtonDown = 0;
std::vector<KVector2>   g_vertices;
std::vector<KVector2> g_points;

using KPolygon = std::vector<KVector2>;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void                Initialize();
void                Finalize();
void				DeleteAllObjects();
void                OnSize();
void                OnUpdate(float fElapsedTime_);
void                OnRender(HDC hdc);
void                OnIdle(float fElapsedTime_);
void                LButtonDown(int x, int y);
void                OnLButtonDown(int x, int y);
void				OnLButtonUp();
void                OnRButtonDown(int x, int y);
void				OnRButtonUp();
void				OnKeyDown(UINT wParam);
void                DrawPolygon(HDC hdc, const KPolygon& a, COLORREF color);
void				CreateDefaultShapes();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR           gdiplusToken;

    // Initialize GDI+.
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LINEARALGEBRA, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LINEARALGEBRA));

    Initialize();

    DWORD dwOldTime = ::timeGetTime();

    MSG msg;

    // Main message loop:
    while (true)
    {
        ::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
        const DWORD dwNewTime = ::timeGetTime();
        const BOOL bIsTranslateMessage = TranslateAccelerator(msg.hwnd, hAccelTable, &msg);
        if (!bIsTranslateMessage)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }//if

        OnIdle(float(dwNewTime - dwOldTime) / 1000.f);
        Sleep(10);

        dwOldTime = dwNewTime;

        if (msg.message == WM_QUIT)
        {
            break;
        }//if
    }//while

    Finalize();
    GdiplusShutdown(gdiplusToken);

    return (int) msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LINEARALGEBRA));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LINEARALGEBRA);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   // _20180519_jintaeks
   g_hwnd = hWnd;
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

void OnPaint(HDC hdc)
{
    //Graphics graphics(hdc);
    //Pen      pen(Color(255, 0, 0, 255));
    //graphics.DrawLine(&pen, 0, 0, 200, 100);
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            // _20180519_jintaeks
            OnPaint( hdc );
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_SIZE:
        OnSize();
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
	case WM_LBUTTONDOWN:
		OnLButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
	case WM_LBUTTONUP:
		OnLButtonUp();
		break;
	case WM_RBUTTONDOWN:
		OnRButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
	case WM_RBUTTONUP:
		OnRButtonUp();
		break;
	case WM_KEYDOWN:
		OnKeyDown(wParam);
		break;
	default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

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

void Initialize()
{
	srand(timeGetTime());
}//Initialize()

void Finalize()
{
}

void DeleteAllObjects()
{
    if (g_hdc != 0) {
        DeleteDC(g_hdc);
        g_hdc = 0;
    }//if
    if (g_hBitmap != 0) {
        DeleteObject(g_hBitmap);
        g_hBitmap = 0;
    }//if
}//DeleteAllObjects()

void OnSize()
{
	DeleteAllObjects();

    ::GetClientRect(g_hwnd, &g_clientRect);
    const int iWidth = g_clientRect.right - g_clientRect.left + 1;
    const int iHeight = g_clientRect.bottom - g_clientRect.top + 1;

    KVector2 origin;
    origin.x = iWidth / 2.0f;
    origin.y = iHeight / 2.0f;
    KVectorUtil::g_screenCoordinate.SetInfo(KVector2(20, 0), KVector2(0, 20), origin);

    HDC hdc = ::GetDC(g_hwnd);
    g_hdc = CreateCompatibleDC(hdc);
    g_hBitmap = CreateCompatibleBitmap(hdc, iWidth, iHeight);
    SelectObject(g_hdc, g_hBitmap);
}//OnSize()

void RenderPhysicsWorld()
{
    for (uint32 i = 0; i < _KWorld.m_bodies.size(); ++i)
    {
        std::shared_ptr<KRigidbody> b = _KWorld.m_bodies[i];
        b->shape->Draw();
    }
}

void OnUpdate(float fElapsedTime_)
{
}

void OnRender(HDC hdc)
{
    POINT mousePoint;
    GetCursorPos(&mousePoint);
    ScreenToClient(g_hwnd, &mousePoint);
    KVector2 vmouse = KVectorUtil::ScreenToWorld(KVector2(mousePoint.x, mousePoint.y));
    //KVector2 vdir = vmouse;
    //vdir.Normalize();
    //KVectorUtil::DrawLine(hdc, KVector2(0, 0), vdir* 1.5f, 2, PS_DASH);

	for (unsigned int i = 0; i < g_vertices.size(); i++)
	{
		KVectorUtil::DrawCircle(hdc, g_vertices[i], 0.2f, 6, 1, 0, RGB(255, 0, 0));
	}

    KPolygon b{ KVector2(1,1), KVector2(-1,1), KVector2(-1,-1), KVector2(1,-1) };
    for (auto& v : b)
    {
        v = v + vmouse;
    }

    DrawPolygon(hdc, b, RGB(0, 0, 0));

    // render Physics World.
    {
        std::string s = "Click: Add vertex, Space: Create shape from points cloud, Esc: Reset Scene";
        ::TextOutA(hdc, 1, 16, s.c_str(), s.length());
        RenderPhysicsWorld();
    }

	for (KVector2 p : g_points)
	{
		KVectorUtil::DrawCircle(hdc, p, 0.2f, 8, 2, 0, RGB(0, 0, 0));
	}
}

void OnIdle(float fElapsedTime_)
{
    const int iWidth = g_clientRect.right - g_clientRect.left + 1;
    const int iHeight = g_clientRect.bottom - g_clientRect.top + 1;

    OnUpdate( fElapsedTime_ );

    HDC hdc = ::GetDC(g_hwnd);

    HBRUSH brush;
    brush = CreateSolidBrush(RGB(255, 255, 255));
    HGDIOBJ oldBrush = SelectObject(g_hdc, brush);
    Rectangle(g_hdc, 0, 0, iWidth, iHeight);

    {
        KBasis2     basis2;
        basis2.SetInfo(KVector2(1, 0), KVector2(0, 1));
        KVectorUtil::SetBasis2(basis2);

        //KVectorUtil::DrawGrid(g_hdc, 100, 100);
        //KVectorUtil::DrawAxis(g_hdc, 100+1, 100+1, RGB(255,0,0), RGB(255,0,0));
    }

    OnRender(g_hdc);

    BitBlt(hdc, 0, 0, iWidth, iHeight, g_hdc, 0, 0, SRCCOPY);
    SelectObject(g_hdc, oldBrush);
    DeleteObject(brush);

    ::ReleaseDC(g_hwnd, hdc);
}//OnIdle()

void LButtonDown(int x, int y)
{
	// (x,y) is already point in client coordinate
	POINT mousePoint;
	mousePoint.x = x;
	mousePoint.y = y;
	//ScreenToClient(g_hwnd, &mousePoint); // test
	KVector2 point = KVector2(mousePoint.x, mousePoint.y);
	KVector2 vmouse = KVectorUtil::ScreenToWorld(point);

	{
		g_vertices.push_back(vmouse); // collect points for later processing. 20210425_jintaeks
	}
}

void OnLButtonDown(int x, int y)
{
	if (g_mouseLButtonDown != 1) {
		LButtonDown(x, y);
	}
	g_mouseLButtonDown = 1;
}

void OnLButtonUp()
{
	g_mouseLButtonDown = 0;
}

void RButtonDown(int x, int y)
{
	POINT mousePoint;
	mousePoint.x = x;
	mousePoint.y = y;
	KVector2 vmouse = KVectorUtil::ScreenToWorld(KVector2(mousePoint.x, mousePoint.y));
}

void OnRButtonDown(int x, int y)
{
	if (g_mouseRButtonDown != 1) {
		RButtonDown(x, y);
	}
	g_mouseRButtonDown = 1;
}

void OnRButtonUp()
{
	g_mouseRButtonDown = 0;
}

void OnKeyDown(UINT wParam)
{
	if (wParam == VK_RETURN)
	{
	}
	else if (wParam == VK_F1)
	{
	}
	else if (wParam == VK_SPACE)
	{
		if (g_vertices.size() == 1)
		{
            _KWorld.CreateCircle(Random(1.0f, 3.0f), g_vertices[0].x, g_vertices[0].y);
		}
		else if (g_vertices.size() >= 3)
		{
			KVector2 geoCenter = KVector2::zero;
			for (auto v : g_vertices)
			{
				geoCenter += v;
			}
			geoCenter.x /= g_vertices.size();
			geoCenter.y /= g_vertices.size();
			for (auto& v : g_vertices)
			{
				v -= geoCenter;
			}

            _KWorld.CreatePolygon(&g_vertices[0], g_vertices.size(), geoCenter.x, geoCenter.y);
		}
		g_vertices.clear();
	}
	else if (wParam == VK_ESCAPE)
	{
        _KWorld.Clear();
		g_vertices.clear();
		g_points.clear();
	}
}

void DrawPolygon(HDC hdc, const KPolygon& a, COLORREF color)
{
    const int numVertices = a.size() + 0;
    for (int i = 0; i < numVertices - 1; ++i) {
        KVectorUtil::DrawLine(hdc, a[i], a[i + 1], 2, 0, color);
    }
    KVectorUtil::DrawLine(hdc, a[numVertices-1], a[0], 2, 0, color);
}
