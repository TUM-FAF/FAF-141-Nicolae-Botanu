#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <stdio.h>

#define BTN_SUBMIT	1			// Button identifier
#define BTN_F1	2			// Edit box identifier
#define BTN_F2	3			// Edit box identifier
#define QUIT_BTN	4			// Edit box identifier

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");
HWND button,buttonn, buttonm,buttonex, hEdit,hwndOutputText;

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
    wincl.style = CS_HREDRAW | CS_VREDRAW;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
;




    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("Lab work 1 Botnari Nicolae"),       /* Title Text */
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
    UpdateWindow (hwnd) ;

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
    int iScreenW, iScreenH, iClientWidth, iClientHeight;
	static int cxChar, cyChar, iColor = 1;
	const char * szMove = "MOVE";
	static char szSubmitBuffer[512];
    HINSTANCE hProgramInstance;
    HFONT hfFont;
    HDC hDC ;
    PAINTSTRUCT paintStruct;
    RECT rect ;
    RECT rcWindow, rcClient;
    switch (message)                  /* handle the messages */
    {

        case WM_PAINT:
            GetClientRect(hwnd, &rcClient);
			iClientWidth = rcClient.right - rcClient.left;
			iClientHeight = rcClient.bottom - rcClient.top;
			hDC = BeginPaint(hwnd,&paintStruct);
            GetClientRect (hwnd, &rect) ;
			/*	Set txt color to blue*/
			SetTextColor(hDC, COLORREF(0x00FF0000));
            /*Display text in middle of window*/

            DrawText(hDC,TEXT("Done with Oride and Prejudice by Botnari Nicolae"),-1,&rect,DT_SINGLELINE | DT_CENTER | DT_VCENTER);



			EndPaint(hwnd, &paintStruct);
		    break;

        case WM_CREATE:
            cxChar = LOWORD (GetDialogBaseUnits ());
			cyChar = HIWORD (GetDialogBaseUnits ());

			iScreenW = GetSystemMetrics(SM_CXSCREEN);
			iScreenH = GetSystemMetrics(SM_CYSCREEN);

			GetClientRect(hwnd, &rcClient);
			iClientWidth = rcClient.right - rcClient.left;
			iClientHeight = rcClient.bottom - rcClient.top;

			button = CreateWindow("BUTTON",
                                 "Times New Roman",
                                 WS_VISIBLE | WS_CHILD |WS_BORDER,
                                 20,
                                 300,
                                 200,
                                 20,
                                 hwnd,
                                 (HMENU)BTN_F1,
                                 hProgramInstance,
                                 NULL);

			buttonn = CreateWindow("BUTTON",
                                 "Courier New",
                                 WS_VISIBLE | WS_CHILD |WS_BORDER,
                                 280,
                                 300,
                                 200,
                                 20,
                                 hwnd,
                                 (HMENU) BTN_F2,
                                 hProgramInstance,
                                 NULL);

              hEdit=CreateWindowEx(WS_EX_CLIENTEDGE,
                                 "EDIT",
		                         "Write something ...",
                                 WS_CHILD|WS_VISIBLE|ES_MULTILINE|ES_AUTOVSCROLL|ES_AUTOHSCROLL,
		                         20,
		                         250,
                                 400,
		                         20,
		                         hwnd,
		                         (HMENU) 10,
		                         GetModuleHandle(NULL),
                                 NULL);

               hwndOutputText = CreateWindowEx(
				                 (DWORD)NULL,
				                  TEXT("edit"),
				                  TEXT("Text will be here ..."),
                                  WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY | ES_MULTILINE,
                                  20,
				                  100,
				                  400,
				                  20,
				                  hwnd,
				                  (HMENU) 2,
				                  GetModuleHandle(NULL),
                                  NULL);

                buttonm = CreateWindow("BUTTON",
                                  "Go",
                                  WS_VISIBLE | WS_CHILD |WS_BORDER,
                                  429,250,50,20,
                                  hwnd, (HMENU) BTN_SUBMIT, NULL, NULL);
                buttonex= CreateWindowEx(
                (DWORD)NULL,
                TEXT("button"),
                TEXT("Quit"),
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
                220, 260, 60, 20,
                hwnd,
                (HMENU) QUIT_BTN,
                hProgramInstance,
                NULL);
	    break;

        case WM_COMMAND:
	         switch(LOWORD(wParam))
	         {
            case BTN_F1: {
                        hfFont = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, TEXT("Times New Roman"));
                        SendMessage(hwndOutputText, WM_SETFONT, (WPARAM)hfFont, 1);

            break;
            }

            case BTN_F2: {
                        hfFont = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, TEXT("Courier New"));
                        SendMessage(hwndOutputText, WM_SETFONT, (WPARAM)hfFont, 1);

            break;
            }
            case QUIT_BTN: {
                    if (MessageBox(NULL, TEXT("Are you sure you want to exit?"), TEXT("Exit?"),MB_YESNO | MB_DEFBUTTON2 | MB_ICONWARNING) == IDYES) {
                    SendMessage(hwnd, WM_DESTROY, 0, 0);

                    };

                    break;
                    }
              case BTN_SUBMIT:
		     {
                    SendMessage(hEdit,
						WM_GETTEXT,
						sizeof(szSubmitBuffer)/sizeof(szSubmitBuffer[0]),
						reinterpret_cast<LPARAM>(szSubmitBuffer));

					if (MessageBox(NULL,
							TEXT("Are you sure you want to submit?"),
							TEXT("Submit?"),
							MB_YESNO | MB_DEFBUTTON1 | MB_ICONWARNING) == IDYES) {
						SendMessage(hwndOutputText, WM_SETTEXT, 0, (LPARAM)szSubmitBuffer);
					};

					if ( strcmp(szMove, szSubmitBuffer) == 0 )
					{
						ShowWindow(hwnd, SW_MAXIMIZE);
					}


		break;
	}

	break;

	         }
	case WM_SIZE:
			GetClientRect(hwnd, &rcClient);
			iClientWidth = rcClient.right - rcClient.left;
			iClientHeight = rcClient.bottom - rcClient.top;

			MoveWindow(hEdit,
				iClientWidth * 1/10,
				iClientHeight * 18/30,
				iClientWidth * 78/100,
				iClientHeight * 13/100,
				TRUE);

			MoveWindow(hwndOutputText,
				iClientWidth * 1/10,
				iClientHeight * 1/10,
				iClientWidth * 78/100,
				iClientHeight * 3/10,
				TRUE);

			MoveWindow(buttonex,
				iClientWidth * 68/100,
				iClientHeight * 74/100,
				iClientWidth * 20/100,
                iClientHeight * 10/100,
				TRUE);
			MoveWindow(buttonm,
				iClientWidth * 1/10,
				iClientHeight * 74/100,
				iClientWidth * 20/100,
                iClientHeight * 10/100,
				TRUE);

			MoveWindow(button,
				iClientWidth * 1/10,
				iClientHeight * 85/100,
                iClientWidth * 38/100,
                iClientHeight * 10/100,
				TRUE);

			MoveWindow(buttonn,
				iClientWidth * 5/10,
				iClientHeight * 85/100,
                iClientWidth * 38/100,
                iClientHeight * 10/100,
				TRUE);
			break;

            case WM_DRAWITEM: {
                if ((UINT)wParam == QUIT_BTN) {
                    LPDRAWITEMSTRUCT lpdis = (DRAWITEMSTRUCT*)lParam;
                    SIZE size;
                    char szQuitBtnText[5];

                    strcpy(szQuitBtnText, "Exit");
                    GetTextExtentPoint32(lpdis->hDC, szQuitBtnText, strlen(szQuitBtnText), &size);
                    SetTextColor(lpdis->hDC, RGB(250, 250, 250));
                    SetBkColor(lpdis->hDC, RGB(200, 50, 50));

                    ExtTextOut(
                        lpdis->hDC,
                        ((lpdis->rcItem.right - lpdis->rcItem.left) - size.cx) / 2,
                        ((lpdis->rcItem.bottom - lpdis->rcItem.top) - size.cy) / 2,
                        ETO_OPAQUE | ETO_CLIPPED,
                        &lpdis->rcItem,
                        szQuitBtnText,
                        strlen(szQuitBtnText),
                        NULL);

                    DrawEdge(
                        lpdis->hDC,
                        &lpdis->rcItem,
                        (lpdis->itemState & ODS_SELECTED ? EDGE_SUNKEN : EDGE_RAISED ),
                        BF_RECT);
                    return TRUE;
                }

            break;
            }

       case WM_SYSCOMMAND:
		{
			switch(wParam)
			{
				case SC_MINIMIZE:
					return MessageBox(NULL,
						TEXT("What do we say to minimize?\n     Not today!"),
						TEXT("Minimize Button Clicked!"),
						MB_OK | MB_ICONWARNING);
					break;

				case SC_CLOSE:
					return MessageBox(NULL,
						TEXT("This is not what do you think it is... Check Exit Button!"),
						TEXT("Maximize Button Clicked!"),
						MB_OK | MB_ICONASTERISK);
					break;

				case SC_MAXIMIZE:
					iScreenW = GetSystemMetrics(SM_CXSCREEN);
					iScreenH = GetSystemMetrics(SM_CYSCREEN);

					GetWindowRect(hwnd, &rcWindow);
					SetWindowPos(
						hwnd, 0,
						(iScreenW - rcWindow.right) / 10 * (rand() % 11),
						(iScreenH - rcWindow.bottom) / 10 * (rand() % 11),
						0, 0,
						SWP_NOZORDER|SWP_NOSIZE);

					return MessageBox(NULL,
								TEXT("Don't do this again!"),
								TEXT("Meh"),
								MB_OK | MB_ICONASTERISK);

				default:
					return DefWindowProc(hwnd, message, wParam, lParam);
			}
		}

            case WM_DESTROY:
                PostQuitMessage (0);

            break;

		default:                     /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
