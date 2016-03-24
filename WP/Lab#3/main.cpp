#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif
#include "resource.h"
#include <tchar.h>
#include <windows.h>
#include <windowsx.h>
#include <string.h>
#include <iostream>
#include <stdio.h>

COLORREF fillColor = RGB(255, 255, 255);
COLORREF borderColor = RGB(0, 0, 0);
/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
int get_eraser(HWND);                 // width of eraser tool

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("Code::Blocks Template Windows App"),       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           985,                 /* The programs width */
           670,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);
    UpdateWindow(hwnd);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    RECT rect ;
    PAINTSTRUCT ps;
    HDC hdc = GetDC(hwnd);
    //HDC hdcMem;
    HPEN hpen;
    static HWND HwndLine, HwndPen, HwndPoly, HwndRect, HwndEllipse,
                HwndClear, HwndBezier, HwndEraser, weightArea, okButton,
                colorButton,hwndEraserWidth;
    // Color preview rectangles
    int xFillPreview = 115;
    int yFillPreview = 200;
    int xStrokePreview = 115;
    int yStrokePreview = 200;
    static RECT drawingArea = {0, 40, 985, 670};
    static RECT fillColorRect = {xFillPreview, yFillPreview, xFillPreview + 25, yFillPreview + 20};
    static RECT borderColorRect = {xStrokePreview, yStrokePreview, xStrokePreview + 25, yStrokePreview + 20};
    static RECT gradientRect = {25, 300, 140, 330};
    static RECT tempRect;
    static BOOL isDrawing = FALSE;
    static int xStart, yStart, xEnd, yEnd;
    static int xmouse, ymouse;
    int cxCoord, cyCoord;
    char buffer[12] = "Set weight";
    char buffer2[12] = "Set color";
    char buffer3[12] = "Fill";
    char textStore[50];
    POINT bez[4] = {{90, 130}, {88, 112}, {210, 67}, {160, 20}};
    static bool bezier = false;
    static int weight = 4;
    static HDC hdcMem, hdccMem;
    HINSTANCE hInstance = (HINSTANCE)GetWindowLong(hwnd, GWLP_HINSTANCE);
    static BITMAP bitmap,bitmapp;
    static HBITMAP filll = NULL ;
    static HBITMAP widt = NULL;

    filll = (HBITMAP)LoadImage(hInstance, "fil.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(filll, sizeof(bitmap), &bitmap);

    widt = (HBITMAP)LoadImage(hInstance, "clear.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(widt, sizeof(bitmapp), &bitmapp);

    xmouse = LOWORD(lParam);
    ymouse = HIWORD(lParam);
    COLORREF colour;
    HBRUSH hbrush;

    switch (message)                  /* handle the messages */
    {
        case WM_CREATE:
        {
            HwndLine = CreateWindowEx(NULL, "BUTTON", "Line",
                                      WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX | BS_PUSHLIKE,
                                      100, 0, 70, 40,
                                      hwnd, (HMENU)ID_LINE, GetModuleHandle(NULL), NULL);
            HwndPen = CreateWindowEx(NULL, "BUTTON", "Pen",
                                      WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX | BS_PUSHLIKE,
                                      170, 0, 70, 40,
                                      hwnd, (HMENU)ID_PEN, GetModuleHandle(NULL), NULL);
            HwndPoly = CreateWindowEx(NULL, "BUTTON", "Polyline",
                                      WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX | BS_PUSHLIKE,
                                      240, 0, 70, 40,
                                      hwnd, (HMENU)ID_POLYLINE, GetModuleHandle(NULL), NULL);
            HwndRect = CreateWindowEx(NULL, "BUTTON", "Rectangle",
                                      WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX | BS_PUSHLIKE,
                                      310, 0, 70, 40,
                                      hwnd, (HMENU)ID_RECT, GetModuleHandle(NULL), NULL);
            HwndEllipse = CreateWindowEx(NULL, "BUTTON", "Ellipse",
                                         WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX | BS_PUSHLIKE,
                                         380, 0, 70, 40,
                                         hwnd, (HMENU)ID_ELLIPSE, GetModuleHandle(NULL), NULL);
            HwndClear = CreateWindowEx(NULL, "BUTTON", "Reset",
                                         WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                                         0, 0, 100, 40,
                                         hwnd, (HMENU)IDI_CLEAR, NULL, NULL);
            HwndBezier = CreateWindowEx(NULL, "BUTTON", "Bezier",
                                         WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX | BS_PUSHLIKE,
                                         450, 0, 70, 40,
                                         hwnd, (HMENU)ID_BEZIER, GetModuleHandle(NULL), NULL);
            HwndEraser = CreateWindowEx(NULL, "BUTTON", "Eraser",
                                        WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX | BS_PUSHLIKE,
                                        520, 0, 70, 40,
                                        hwnd, (HMENU)ID_ERASER, GetModuleHandle(NULL), NULL);
            weightArea = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "",
                                        WS_VISIBLE | WS_CHILD | ES_NUMBER,
                                        715, 17, 25, 20,
                                        hwnd, (HMENU)ID_WEIGHT, GetModuleHandle(NULL), NULL);
            okButton = CreateWindowEx(NULL, "BUTTON", "OK",
                                      WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                                      744, 17, 30, 20,
                                      hwnd, (HMENU)ID_OK, GetModuleHandle(NULL), NULL);

            CreateWindowEx(NULL, "BUTTON", "Red",
                            WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON | WS_GROUP,
                            590, 0, 60, 18,
                            hwnd, (HMENU)ID_RED, GetModuleHandle(NULL), NULL);
            CreateWindowEx(NULL, "BUTTON", "Green",
                            WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,
                            590, 20, 60, 18,
                            hwnd, (HMENU)ID_GREEN, GetModuleHandle(NULL), NULL);
            CreateWindowEx(NULL, "BUTTON", "Blue",
                            WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,
                            650, 0, 60, 18,
                            hwnd, (HMENU)ID_BLUE, GetModuleHandle(NULL), NULL);
            CreateWindowEx(NULL, "BUTTON", "Black",
                            WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON | BST_CHECKED,
                            650, 20, 60, 18,
                            hwnd, (HMENU)ID_BLACK, GetModuleHandle(NULL), NULL);
            CreateWindowEx(NULL, "BUTTON", "",
                            WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
                            817, 10, 14, 18,
                            hwnd, (HMENU)ID_FILL, GetModuleHandle(NULL), NULL);

            hwndEraserWidth = CreateWindow(TEXT("Edit"), TEXT("10"),
                    WS_VISIBLE | WS_CHILD | WS_BORDER,
                    870, 10, 30, 20,
                    hwnd,(HMENU)ID_ERASERW,
                    NULL, NULL);

            Button_SetCheck(GetDlgItem(hwnd, ID_BLACK), BST_CHECKED);
            break;
        }
        case WM_COMMAND:
        {
            switch(LOWORD(wParam))
            {
                case ID_PEN:
                {
                    Button_SetCheck(HwndLine, BST_UNCHECKED);
                    Button_SetCheck(HwndPoly, BST_UNCHECKED);
                    Button_SetCheck(HwndRect, BST_UNCHECKED);
                    Button_SetCheck(HwndEllipse, BST_UNCHECKED);
                    Button_SetCheck(HwndClear, BST_UNCHECKED);
                    Button_SetCheck(HwndBezier, BST_UNCHECKED);
                    Button_SetCheck(HwndEraser, BST_UNCHECKED);
                    break;
                }

                case ID_LINE:
                {
                    Button_SetCheck(HwndPen, BST_UNCHECKED);
                    Button_SetCheck(HwndPoly, BST_UNCHECKED);
                    Button_SetCheck(HwndRect, BST_UNCHECKED);
                    Button_SetCheck(HwndEllipse, BST_UNCHECKED);
                    Button_SetCheck(HwndClear, BST_UNCHECKED);
                    Button_SetCheck(HwndBezier, BST_UNCHECKED);
                    Button_SetCheck(HwndEraser, BST_UNCHECKED);
                    break;
                }

                case ID_POLYLINE:
                {
                    Button_SetCheck(HwndLine, BST_UNCHECKED);
                    Button_SetCheck(HwndPen, BST_UNCHECKED);
                    Button_SetCheck(HwndRect, BST_UNCHECKED);
                    Button_SetCheck(HwndEllipse, BST_UNCHECKED);
                    Button_SetCheck(HwndClear, BST_UNCHECKED);
                    Button_SetCheck(HwndBezier, BST_UNCHECKED);
                    Button_SetCheck(HwndEraser, BST_UNCHECKED);
                    break;

                }

                case ID_ELLIPSE:
                {
                    Button_SetCheck(HwndLine, BST_UNCHECKED);
                    Button_SetCheck(HwndPen, BST_UNCHECKED);
                    Button_SetCheck(HwndRect, BST_UNCHECKED);
                    Button_SetCheck(HwndPoly, BST_UNCHECKED);
                    Button_SetCheck(HwndClear, BST_UNCHECKED);
                    Button_SetCheck(HwndBezier, BST_UNCHECKED);
                    Button_SetCheck(HwndEraser, BST_UNCHECKED);
                    break;

                }

                case ID_RECT:
                {
                    Button_SetCheck(HwndLine, BST_UNCHECKED);
                    Button_SetCheck(HwndPen, BST_UNCHECKED);
                    Button_SetCheck(HwndPoly, BST_UNCHECKED);
                    Button_SetCheck(HwndEllipse, BST_UNCHECKED);
                    Button_SetCheck(HwndClear, BST_UNCHECKED);
                    Button_SetCheck(HwndBezier, BST_UNCHECKED);
                    Button_SetCheck(HwndEraser, BST_UNCHECKED);
                    break;
                }

                case ID_BEZIER:
                {
                    Button_SetCheck(HwndLine, BST_UNCHECKED);
                    Button_SetCheck(HwndPen, BST_UNCHECKED);
                    Button_SetCheck(HwndPoly, BST_UNCHECKED);
                    Button_SetCheck(HwndEllipse, BST_UNCHECKED);
                    Button_SetCheck(HwndClear, BST_UNCHECKED);
                    Button_SetCheck(HwndRect, BST_UNCHECKED);
                    Button_SetCheck(HwndEraser, BST_UNCHECKED);
                    bezier = true;
                    InvalidateRect(hwnd, NULL, TRUE);
                    UpdateWindow(hwnd);
                    break;
                }

                case ID_CLEAR:
                {
                    Button_SetCheck(HwndLine, BST_UNCHECKED);
                    Button_SetCheck(HwndPen, BST_UNCHECKED);
                    Button_SetCheck(HwndPoly, BST_UNCHECKED);
                    Button_SetCheck(HwndEllipse, BST_UNCHECKED);
                    Button_SetCheck(HwndRect, BST_UNCHECKED);
                    Button_SetCheck(HwndBezier, BST_UNCHECKED);
                    Button_SetCheck(HwndEraser, BST_UNCHECKED);
                    InvalidateRect(hwnd, &rect, FALSE);
                    InvalidateRect(hwnd, &rect, TRUE);
                    Button_SetCheck(HwndClear, BST_UNCHECKED);
                    break;
                }

                case ID_ERASER:
                {
                    Button_SetCheck(HwndLine, BST_UNCHECKED);
                    Button_SetCheck(HwndPen, BST_UNCHECKED);
                    Button_SetCheck(HwndPoly, BST_UNCHECKED);
                    Button_SetCheck(HwndEllipse, BST_UNCHECKED);
                    Button_SetCheck(HwndClear, BST_UNCHECKED);
                    Button_SetCheck(HwndRect, BST_UNCHECKED);
                    Button_SetCheck(HwndBezier, BST_UNCHECKED);
                    break;
                }

                case IDI_CLEAR:
                {
                    InvalidateRect(hwnd, &rect, FALSE);
                    InvalidateRect(hwnd, &rect, TRUE);
                    break;

                }

                case ID_OK:
                {
                    GetWindowText(weightArea, textStore, 50);
                    weight = atoi(textStore);
                    printf("%d", weight);
                    break;
                }

                case ID_EXIT:
                {
                    PostQuitMessage(0);
                    break;
                }
            }
            break;
        }

        case WM_LBUTTONDOWN:
        {
            if(xmouse > 0  && ymouse > 40 )
            {
                if(Button_GetCheck(GetDlgItem(hwnd, ID_RED)) == BST_CHECKED)
                {
                    colour = RGB(255, 0, 0);
                }
                else if(Button_GetCheck(GetDlgItem(hwnd, ID_GREEN)) == BST_CHECKED)
                {
                    colour = RGB(0, 255, 0);
                }
                else if(Button_GetCheck(GetDlgItem(hwnd, ID_BLUE)) == BST_CHECKED)
                {
                    colour = RGB(0, 0, 255);
                }
                else if(Button_GetCheck(GetDlgItem(hwnd, ID_BLACK)) == BST_CHECKED)
                {
                    colour = RGB(0, 0, 0);
                }
                hpen = CreatePen(PS_SOLID, weight, colour);
                hdc = GetDC(hwnd);
                SelectObject(hdc, hpen);
                xStart = LOWORD(lParam);
                yStart = HIWORD(lParam);
                xEnd = LOWORD(lParam);
                yEnd = HIWORD(lParam);
                if(Button_GetCheck(HwndLine) == BST_CHECKED)
                {
                    SetROP2(hdc, R2_XORPEN);
                    MoveToEx(hdc, xStart, yStart, NULL);
                    LineTo(hdc, xEnd, yEnd);
                    isDrawing = TRUE;
                }
                if(Button_GetCheck(HwndPen) == BST_CHECKED)
                {
                    xStart = LOWORD(lParam);
                    yStart = HIWORD(lParam);
                    isDrawing = TRUE;
                }
                if(Button_GetCheck(HwndPoly) == BST_CHECKED)
                {
                    MoveToEx(hdc, xStart, yStart, NULL);
                    LineTo(hdc, xEnd, yEnd);
                    isDrawing = TRUE;
                }
                if(Button_GetCheck(HwndRect) == BST_CHECKED)
                {
                    if(Button_GetCheck(GetDlgItem(hwnd, ID_FILL)) == BST_CHECKED)
                    {
                        hbrush = CreateSolidBrush(colour);
                    }
                    else
                    {
                        hbrush = (HBRUSH)GetStockObject(NULL_BRUSH);
                    }
                    SelectObject(hdc, hbrush);
                    SetROP2(hdc, R2_XORPEN);
                    Rectangle(hdc, xStart, yStart, xEnd, yEnd);
                    isDrawing = TRUE;
                    DeleteObject(hbrush);
                }
                if(Button_GetCheck(HwndEllipse) == BST_CHECKED)
                {
                    if(Button_GetCheck(GetDlgItem(hwnd, ID_FILL)) == BST_CHECKED)
                    {
                        hbrush = CreateSolidBrush(colour);
                    }
                    else
                    {
                        hbrush = (HBRUSH)GetStockObject(NULL_BRUSH);
                    }
                    SetROP2(hdc, R2_XORPEN);
                    SelectObject(hdc, hbrush);
                    Ellipse(hdc, xStart, yStart, xEnd, yEnd);
                    isDrawing = TRUE;
                    DeleteObject(hbrush);
                }

                 if(Button_GetCheck(HwndEraser) == BST_CHECKED)
                {
                     int eraser_width = get_eraser(hwndEraserWidth);

                    rect.left = xmouse - (eraser_width / 2);
                    rect.right = xmouse + (eraser_width / 2);
                    rect.top = ymouse - (eraser_width / 2);
                    rect.bottom = ymouse + (eraser_width / 2);
                    InvalidateRect(hwnd, &rect, FALSE);
                    SendMessage(hwnd, WM_PAINT, 0, 0);
                }


                if(Button_GetCheck(HwndBezier) == BST_CHECKED)
                {
                }
                DeleteObject(hpen);
                ReleaseDC(hwnd, hdc);
            }
            break;
        }

        case WM_MOUSEMOVE:
        {
            if( xmouse > 0  && ymouse > 40 )
            {
                hdc = GetDC(hwnd);
                if(isDrawing == TRUE)
                {
                    if(Button_GetCheck(GetDlgItem(hwnd, ID_RED)) == BST_CHECKED)
                    {
                        colour = RGB(255, 0, 0);
                    }
                    else if(Button_GetCheck(GetDlgItem(hwnd, ID_GREEN)) == BST_CHECKED)
                    {
                        colour = RGB(0, 255, 0);
                    }
                    else if(Button_GetCheck(GetDlgItem(hwnd, ID_BLUE)) == BST_CHECKED)
                    {
                        colour = RGB(0, 0, 255);
                    }
                    else if(Button_GetCheck(GetDlgItem(hwnd, ID_BLACK)) == BST_CHECKED)
                    {
                        colour = RGB(0, 0, 0);
                    }
                    hpen = CreatePen(PS_SOLID, weight, colour);
                    SelectObject(hdc, hpen);
                    SetROP2(hdc, R2_NOTXORPEN);
                    if(Button_GetCheck(HwndLine) == BST_CHECKED)
                    {
                        MoveToEx(hdc, xStart, yStart, NULL);
                        LineTo(hdc, xEnd, yEnd);
                        xEnd = LOWORD(lParam);
                        yEnd = HIWORD(lParam);
                        MoveToEx(hdc, xStart, yStart, NULL);
                        LineTo(hdc, xEnd, yEnd);
                    }
                    if(Button_GetCheck(HwndPen) == BST_CHECKED)
                    {
                        MoveToEx(hdc, xEnd, yEnd, NULL);
                        xEnd = LOWORD(lParam);
                        yEnd = HIWORD(lParam);
                        LineTo(hdc, xEnd, yEnd);
                    }
                    if(Button_GetCheck(HwndPoly) == BST_CHECKED)
                    {
                        MoveToEx(hdc, xStart, yStart, NULL);
                        LineTo(hdc, xEnd, yEnd);
                        xEnd = LOWORD(lParam);
                        yEnd = HIWORD(lParam);
                        MoveToEx(hdc, xStart, yStart, NULL);
                        LineTo(hdc, xEnd, yEnd);
                    }
                    if(Button_GetCheck(HwndRect) == BST_CHECKED)
                    {
                        if(Button_GetCheck(GetDlgItem(hwnd, ID_FILL)) == BST_CHECKED)
                        {
                            hbrush = CreateSolidBrush(colour);
                        }
                        else
                        {
                            hbrush = (HBRUSH)GetStockObject(NULL_BRUSH);
                        }
                        SelectObject(hdc, hbrush);
                        SetROP2(hdc, R2_NOTXORPEN);
                        Rectangle(hdc, xStart, yStart, xEnd, yEnd);
                        xEnd = LOWORD(lParam);
                        yEnd = HIWORD(lParam);
                        Rectangle(hdc, xStart, yStart, xEnd, yEnd);
                        DeleteObject(hbrush);
                    }
                    if(Button_GetCheck(HwndEllipse) == BST_CHECKED)
                    {
                        if(Button_GetCheck(GetDlgItem(hwnd, ID_FILL)) == BST_CHECKED)
                        {
                            hbrush = CreateSolidBrush(colour);
                        }
                        else
                        {
                            hbrush = (HBRUSH)GetStockObject(NULL_BRUSH);
                        }
                        SelectObject(hdc, hbrush);
                        SetROP2(hdc, R2_NOTXORPEN);
                        Ellipse(hdc, xStart, yStart, xEnd, yEnd);
                        xEnd = LOWORD(lParam);
                        yEnd = HIWORD(lParam);
                        Ellipse(hdc, xStart, yStart, xEnd, yEnd);
                        DeleteObject(hbrush);
                    }
                 if(Button_GetCheck(HwndEraser) == BST_CHECKED)
                {
                     int eraser_width = get_eraser(hwndEraserWidth);  // function call, the width of eraser tool

                    rect.left = xmouse - (eraser_width / 2);
                    rect.right = xmouse + (eraser_width / 2);
                    rect.top = ymouse - (eraser_width / 2);
                    rect.bottom = ymouse + (eraser_width / 2);
                    InvalidateRect(hwnd, &rect, FALSE);
                    SendMessage(hwnd, WM_PAINT, 0, 0);
                }
                    if(Button_GetCheck(HwndBezier) == BST_CHECKED)
                    {

                    }
                }
                DeleteObject(hpen);
                ReleaseDC(hwnd, hdc);
            }
            break;
        }

        case WM_LBUTTONUP:
        {
            if( xmouse > 10  && ymouse > 40 )
            {
                hdc = GetDC(hwnd);
                xEnd = LOWORD(lParam);
                yEnd = HIWORD(lParam);
                SetROP2(hdc, R2_XORPEN);
                MoveToEx(hdc, xStart, yStart, NULL);
                LineTo(hdc, xEnd, yEnd);
                ReleaseDC(hwnd, hdc);
                if(Button_GetCheck(HwndLine) == BST_CHECKED)
                {
                    isDrawing = FALSE;
                }
                if(Button_GetCheck(HwndPen) == BST_CHECKED)
                {
                    isDrawing = FALSE;
                }
                if(Button_GetCheck(HwndRect) == BST_CHECKED)
                {
                    isDrawing = FALSE;
                }
                if(Button_GetCheck(HwndEllipse) == BST_CHECKED)
                {
                    isDrawing = FALSE;
                }
                if (Button_GetCheck(HwndEraser)== BST_CHECKED)
                {
                    isDrawing = FALSE;
                }
            break;
            }
        }

        case WM_RBUTTONDOWN:
        {
            if(Button_GetCheck(HwndPoly) == BST_CHECKED)
            {
                isDrawing = FALSE;
            }
            break;
        }

        case WM_HOTKEY:
        {
            switch(wParam)
            {
                case HK_Reset:
                {
                    SendMessage(hwnd, WM_COMMAND, MAKEWPARAM(IDI_CLEAR, 0), 0);
                    break;
                }
                default:
                    break;
            }
            break;
        }

case WM_GETMINMAXINFO:
        {
            LPMINMAXINFO winSize = (LPMINMAXINFO)lParam;
            winSize->ptMinTrackSize.x = 985;
            winSize->ptMinTrackSize.y = 670;
            winSize->ptMaxTrackSize.x = 985;
            winSize->ptMaxTrackSize.y = 670;
            break;
        }

        case WM_PAINT:
            hdc = BeginPaint(hwnd, &ps);
            FillRect(hdc, &rect, CreateSolidBrush(RGB(255, 255, 255)));

            // Blank draw area
            SelectObject(hdc, CreatePen(PS_SOLID, 1, RGB(0,0,0)));
            SelectObject(hdc, (HBRUSH)GetStockObject(WHITE_BRUSH));
            Rectangle(hdc, drawingArea.left, drawingArea.top, drawingArea.right, drawingArea.bottom);
            SetBkMode(hdc, TRANSPARENT);
            TextOut(hdc, 710, 0, buffer, strlen(buffer));

            hdcMem = CreateCompatibleDC(hdc);
            SelectObject(hdcMem, filll);
            BitBlt(hdc, 803, 14, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);
            DeleteDC(hdcMem);

            hdccMem = CreateCompatibleDC(hdc);
            SelectObject(hdccMem, widt);
            BitBlt(hdc, 850, 10, 16, 16, hdccMem, 0, 0, SRCCOPY);
            DeleteDC(hdccMem);

            TextOut(hdc, 780, 10, buffer3, strlen(buffer3));
            if(bezier == true)
            {
                HPEN localhpen = CreatePen(PS_SOLID, weight, colour);
                SelectObject(hdc, localhpen);
                SetFocus(hwnd);
                PolyBezier(hdc, bez, 4);
                bezier = false;
                DeleteObject(localhpen);
            }

            EndPaint(hwnd, &ps);
            break;

        case WM_CTLCOLORSTATIC:
        {
            SetBkColor((HDC)wParam, RGB(0, 0, 0));
            SetBkMode((HDC)wParam, TRANSPARENT);
            return (LRESULT)GetStockObject(HOLLOW_BRUSH);
        }
        case WM_KEYDOWN:   // keyboard input
        {
            switch(wParam)
            {
                case KEY_S:
				     isDrawing = FALSE;
				break;
                case KEY_P:  // LeftSHIFT + P
                {
                    if (GetAsyncKeyState(VK_LSHIFT))
                    {
                     SendMessage(hwnd, WM_COMMAND, MAKEWPARAM(ID_PEN, 0), 0);
                     Button_SetCheck(HwndPen, BST_UNCHECKED);
                    }
                break;
                }

                case KEY_B:  // LeftSHIFT + B
                {
                    if (GetAsyncKeyState(VK_LSHIFT))
                    {
                     SendMessage(hwnd, WM_COMMAND, MAKEWPARAM(ID_BLUE, 0), 0);
                    }
                break;
                }
                case KEY_G:  // LeftSHIFT + G
                {
                    if (GetAsyncKeyState(VK_LSHIFT))
                    {
                     SendMessage(hwnd, WM_COMMAND, MAKEWPARAM(ID_GREEN, 0), 0);
                    }
                break;
                }
                case KEY_R:  // LeftSHIFT + R
                {
                    if (GetAsyncKeyState(VK_LSHIFT))
                    {
                     SendMessage(hwnd, WM_COMMAND, MAKEWPARAM(ID_RED, 0), 0);
                    }
                break;
                }
                case KEY_C:   // LShift + C
                {
                    if (GetAsyncKeyState(VK_LSHIFT))
                    {

                SendMessage(hwnd, WM_COMMAND, MAKEWPARAM(ID_CLEAR, 0), 0);
                    }
                 break;
                }

                default: return DefWindowProc (hwnd, message, wParam, lParam);
            break;   // end of switch statement
            }
        break;       // end of case statement
        }
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
int get_eraser(HWND width)
{
    static int result;
    static int iLength = SendMessage(width, WM_GETTEXTLENGTH, 0, 0);
    char* szText = (char*)malloc(iLength+1);
    SendMessage(width, WM_GETTEXT, iLength+1, (LPARAM)szText);
    result = atoi(szText);
    free(szText);
    return result;
}
