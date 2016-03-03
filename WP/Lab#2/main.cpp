
#include <tchar.h>
#include <windows.h>
#include "resource.h"
#include <commdlg.h>
#include <stdio.h>
#include <iostream>

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
BOOL    CALLBACK   AboutDlgProc  (HWND, UINT, WPARAM, LPARAM);


/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");
HINSTANCE hInst;

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    hInst = hThisInstance;

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;
    wincl.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
    wincl.cbSize = sizeof (wincl);
    wincl.hIcon   = LoadIcon (GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON));     // custom icon
    wincl.hIconSm = LoadIcon (GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON));     // custom icon
    wincl.hCursor = LoadCursor (GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_CURSOR)); // custom cursor
    wincl.lpszMenuName = MAKEINTRESOURCE(IDI_MENU);                                  // custom menu
    wincl.cbClsExtra = 0;                                 // no extra bytes after the window class
    wincl.cbWndExtra = 0;                                 // structure or the window instance
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;      // use Windows's default colour as the background of the window
    //choosing the background color

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("Lab#2 PW"),       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           544,                 /* The programs width */
           375,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

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
    int TextLength,index,index2;
    char* szText;
    RECT rect ;
    RECT rcWindow, rcClient;
    static int cxChar, cyChar;
    int  iClientWidth, iClientHeight;
    int SysWidth,SysHeight;
    int WinWidth,WinHeight;
    int x,y;
    SCROLLINFO si;

    static HWND hwndListBox,hwndNewItem,hwndAddButton,hwndRemoveButton,hwndClearButton;
    static HWND hwndLabel1,hwndLabel2,hwndWidthScroll,hwndHeightScroll;

    switch (message)                  /* handle the messages */
    {
        case WM_CREATE:
            cxChar = LOWORD (GetDialogBaseUnits ());
			cyChar = HIWORD (GetDialogBaseUnits ());

			GetClientRect(hwnd, &rcClient);
			iClientWidth = rcClient.right - rcClient.left;
			iClientHeight = rcClient.bottom - rcClient.top;

            hwndListBox = CreateWindowEx(
                (DWORD)NULL,
                TEXT("ListBox"),
                NULL,
                WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_NOTIFY | LBS_WANTKEYBOARDINPUT,
                0, 0, 0, 0, hwnd,
                (HMENU)IDM_LIST_BOX,
                hInst,
                NULL);

            hwndNewItem = CreateWindowEx(
                (DWORD)NULL,
                TEXT("Edit"),
                TEXT(""),
                WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL,
                0, 0, 0, 0, hwnd,
                (HMENU)IDM_NEW_ITEM,
                hInst,
                NULL);

            hwndAddButton = CreateWindowEx(
                (DWORD)NULL,
                TEXT("Button"),
                TEXT("Add"),
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                0, 0, 0, 0, hwnd,
                (HMENU)IDM_ADD_BUTTON,
                hInst,
                NULL);

            hwndRemoveButton = CreateWindowEx(
                (DWORD)NULL,
                TEXT("Button"),
                TEXT("Remove"),
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                0, 0, 0, 0, hwnd,
                (HMENU)IDM_REMOVE_BUTTON,
                hInst,
                NULL);

            hwndClearButton = CreateWindowEx(
                (DWORD)NULL,
                TEXT("Button"),
                TEXT("Clear"),
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                0, 0, 0, 0, hwnd,
                (HMENU)IDM_CLEAR_BUTTON,
                hInst,
                NULL);

            hwndLabel1 = CreateWindowEx(
                (DWORD)NULL,
                TEXT("static"),
                TEXT("Window Width and Height"),
                WS_CHILD | WS_VISIBLE | SS_LEFT,
                0, 0, 0, 0, hwnd,
                (HMENU)IDM_LABEL1,
                hInst,
                NULL);

            hwndLabel2 = CreateWindowEx(
                (DWORD)NULL,
                TEXT("static"),
                TEXT("Window Height"),
                WS_CHILD | WS_VISIBLE | SS_LEFT,
                0, 0, 0, 0,
                hwnd,
                (HMENU)IDM_LABEL2,
                hInst,
                NULL);

            hwndWidthScroll = CreateWindow(
                "Scrollbar",
                NULL,
                WS_CHILD | WS_VISIBLE | SBS_HORZ | SBS_BOTTOMALIGN,
                0, 0, 0, 0, hwnd,
                (HMENU)ID_WIDTH_SCROLL,
                hInst,
                NULL);
            SetScrollRange(hwndWidthScroll, SB_CTL, 0, 100, TRUE);

            hwndHeightScroll = CreateWindow(
                "Scrollbar",
                NULL,
                WS_CHILD | WS_VISIBLE | SBS_HORZ | SBS_BOTTOMALIGN,
                0, 0, 0, 0, hwnd,
                (HMENU)ID_HEIGHT_SCROLL,
                hInst,
                NULL);
            SetScrollRange(hwndHeightScroll, SB_CTL, 0, 100, TRUE);

        break;

        	case WM_SIZE:

			GetClientRect(hwnd, &rcClient);
			iClientWidth = rcClient.right - rcClient.left;
			iClientHeight = rcClient.bottom - rcClient.top;

			MoveWindow(hwndNewItem,
				iClientWidth * 1/10,
				iClientHeight * 18/30,
				iClientWidth * 78/100,
				iClientHeight * 13/100,
				TRUE);

			MoveWindow(hwndListBox,
				iClientWidth * 1/10,
				iClientHeight * 1/10,
				iClientWidth * 78/100,
				iClientHeight * 5/10,
				TRUE);

			MoveWindow(hwndRemoveButton,
				iClientWidth * 30/100,
				iClientHeight * 74/100,
				iClientWidth * 15/100,
                iClientHeight * 10/100,
				TRUE);
			MoveWindow(hwndAddButton,
				iClientWidth * 1/10,
				iClientHeight * 74/100,
				iClientWidth * 15/100,
                iClientHeight * 10/100,
				TRUE);

			MoveWindow(hwndClearButton,
				iClientWidth * 50/100,
				iClientHeight * 74/100,
                iClientWidth * 38/100,
                iClientHeight * 10/100,
				TRUE);
          MoveWindow(hwndLabel1,
				iClientWidth * 1/10,
				iClientHeight * 85/100,
                iClientWidth * 40/100,
                iClientHeight * 5/100,
				TRUE);
			MoveWindow(hwndWidthScroll,
				iClientWidth * 1/10,
				iClientHeight * 91/100,
                iClientWidth * 78/100,
                iClientHeight * 3/100,
				TRUE);
			MoveWindow(hwndHeightScroll,
				iClientWidth * 1/10,
				iClientHeight * 95/100,
                iClientWidth * 78/100,
                iClientHeight * 3/100,
				TRUE);
			break;

        case WM_VSCROLL:
            // Get all the vertical scroll bar information
            si.cbSize = sizeof(si);
            si.fMask = SIF_ALL;
            GetScrollInfo(hwnd, SB_VERT, &si);

            // Save the position for later comparison
            y = si.nPos;

            switch(LOWORD(wParam)) {
                case SB_TOP:
                    si.nPos = si.nMin;
                    break;

                case SB_BOTTOM:
                    si.nPos = si.nMax;
                    break;

                case SB_LINEUP:
                    si.nPos -= 1;
                    break;

                case SB_LINEDOWN:
                    si.nPos += 1;
                    break;

                case SB_PAGEUP:
                    si.nPos -= si.nPage;
                    break;

                case SB_PAGEDOWN:
                    si.nPos += si.nPage;
                    break;

                case SB_THUMBTRACK:
                    si.nPos = si.nTrackPos;
                    break;

                default:
                    break;
            }

            // Set the position and then retrieve it
            si.fMask = SIF_POS;
            SetScrollInfo(hwnd, SB_VERT, &si, TRUE);
            GetScrollInfo(hwnd, SB_VERT, &si);

            // If the position has changed, scroll the window and update it
            if(si.nPos != y) {
                ScrollWindow(hwnd, 0, cyChar * (y - si.nPos), NULL, NULL);
                UpdateWindow(hwnd);
            }
            break;

        case WM_HSCROLL:
            GetWindowRect(hwnd, &rect);
            WinWidth = rect.right - rect.left;
            WinHeight = rect.bottom - rect.top;
            SysWidth = GetSystemMetrics(SM_CXSCREEN);
            SysHeight = GetSystemMetrics(SM_CYSCREEN);
            if(GetWindowLong((HWND)lParam, GWL_ID) == ID_WIDTH_SCROLL) {
                si.cbSize = sizeof(si);
                si.fMask = SIF_ALL;
                GetScrollInfo(hwndWidthScroll, SB_CTL, &si);
                x = si.nPos;
                switch(LOWORD(wParam)) {
                    case SB_LINELEFT:
                        si.nPos -= 1;
                        break;
                    case SB_LINERIGHT:
                        si.nPos += 1;
                        break;
                    case SB_THUMBPOSITION:
                        si.nPos = si.nTrackPos;
                        break;
                    default:
                        break;
                }
                si.fMask = SIF_POS;
                SetScrollInfo(hwndWidthScroll, SB_CTL, &si, TRUE);
                GetScrollInfo(hwndWidthScroll, SB_CTL, &si);
                if(si.nPos != x) {
                    SetScrollPos(hwndWidthScroll, SB_CTL, si.nPos, TRUE);
                }
                // Set window width
                MoveWindow(hwnd, rect.left, rect.top, (si.nPos * SysWidth / 100), WinHeight, TRUE);
                break;
            }

            if(GetWindowLong((HWND)lParam, GWL_ID) == ID_HEIGHT_SCROLL) {
                si.cbSize = sizeof(si);
                si.fMask = SIF_ALL;
                GetScrollInfo(hwndHeightScroll, SB_CTL, &si);
                x = si.nPos;
                switch(LOWORD(wParam)) {
                    case SB_LINELEFT:
                        si.nPos -= 1;
                        break;
                    case SB_LINERIGHT:
                        si.nPos += 1;
                        break;
                    case SB_THUMBPOSITION:
                        si.nPos = si.nTrackPos;
                        break;
                    default:
                        break;
                }
                si.fMask = SIF_POS;
                SetScrollInfo(hwndHeightScroll, SB_CTL, &si, TRUE);
                GetScrollInfo(hwndHeightScroll, SB_CTL, &si);
                if(si.nPos != x) {
                    SetScrollPos(hwndHeightScroll, SB_CTL, si.nPos, TRUE);
                }
                // Set window height
                MoveWindow(hwnd, rect.left, rect.top, WinWidth, (si.nPos * SysHeight / 100), TRUE);
                break;
            }

            // Get all the vertical scroll bar information
            si.cbSize = sizeof(si);
            si.fMask = SIF_ALL;
            GetScrollInfo(hwnd, SB_HORZ, &si);

            // Save the position for later comparison
            x = si.nPos;
            switch(LOWORD(wParam)) {
                case SB_LINELEFT:
                    si.nPos -= 1;
                    break;

                case SB_LINERIGHT:
                    si.nPos += 1;
                    break;

                case SB_PAGELEFT:
                    si.nPos -= si.nPage;
                    break;

                case SB_PAGERIGHT:
                    si.nPos += si.nPage;
                    break;

                case SB_THUMBPOSITION:
                    si.nPos = si.nTrackPos;
                    break;

                default:
                    break;
            }
            // Set the position and then retrieve it
            si.fMask = SIF_POS;
            SetScrollInfo(hwnd, SB_HORZ, &si, TRUE);
            GetScrollInfo(hwnd, SB_HORZ, &si);

            // If the position has changed, scroll the window and update it
            if(si.nPos != x) {
                ScrollWindow(hwnd, cxChar * (x - si.nPos), 0, NULL, 0);
                UpdateWindow(hwnd);
            }
            break;



        case WM_COMMAND:

            switch (LOWORD(wParam)) {

                case IDM_LIST_BOX:
                    switch (HIWORD(wParam)) {
                        case LBN_DBLCLK:
                            index = SendMessage(hwndListBox, LB_GETCURSEL, 0, 0);
                            SendMessage(hwndListBox, LB_DELETESTRING, (WPARAM)index, 0);
                            RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
                            break;
                    }
                    break;

                case IDM_ADD_BUTTON:
                    TextLength = SendMessage(hwndNewItem, WM_GETTEXTLENGTH, 0, 0);
                    szText = (char*)malloc(TextLength+1);
                    SendMessage(hwndNewItem, WM_GETTEXT, TextLength+1, (LPARAM)szText);
                    SendMessage(hwndListBox, LB_ADDSTRING, 0, (LPARAM)szText);
                    SendMessage(hwndNewItem, WM_SETTEXT, 0, (LPARAM)"");
                    free(szText);
                    break;

                case IDM_REMOVE_BUTTON:
                    index2 = SendMessage(hwndListBox, LB_GETCURSEL, 0, 0);
                    if(index2 != LB_ERR) {
                        SendMessage(hwndListBox, LB_DELETESTRING, index2, 0);
                    }
                    break;

                case IDM_CLEAR_BUTTON:
                    SendMessage(hwndListBox, LB_RESETCONTENT, 0, 0);
                    break;

                case IDI_ABOUT:

                    DialogBox(hInst, MAKEINTRESOURCE(IDI_DIALOG), hwnd, AboutDlgProc);
                    break;

                case IDI_EXIT:

                    DestroyWindow(hwnd);
                    break;

                case IDI_DEFAULT:

  			   SendMessage(hwnd, WM_COMMAND, MAKEWPARAM(IDM_CLEAR_BUTTON, 0), 0);

                    break;

                default:
                    return DefWindowProc (hwnd, message, wParam, lParam);


            }
            break;
        case WM_KEYDOWN:   // keyboard input
        {
            switch(wParam)
            {
                case KEY_E:  // LeftSHIFT + E
                {
                    if (GetAsyncKeyState(VK_LSHIFT))
                    {
                        DestroyWindow(hwnd);
                    }
                break;
                }

                case F1:   // F1
                {
                    if (GetAsyncKeyState(VK_F1))
                    {
                        DialogBox(hInst, MAKEINTRESOURCE(IDI_DIALOG), hwnd, AboutDlgProc);
                    }
                 break;
                }

                case KEY_C:   // LShift + C
                {
                    if (GetAsyncKeyState(VK_LSHIFT))
                    {

                SendMessage(hwnd, WM_COMMAND, MAKEWPARAM(IDM_CLEAR_BUTTON, 0), 0);
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

 BOOL CALLBACK AboutDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) // works with DialogBox
 {
    switch(message)
    {
        case WM_INITDIALOG:
            return TRUE;

        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
            case IDOK:
            case IDCANCEL:
                EndDialog(hDlg,0);
                return TRUE;
            }
            break;
        }
        return FALSE;
 }
