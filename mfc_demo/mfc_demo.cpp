// mfc_demo.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "mfc_demo.h"

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 新增：MP3文件路径
WCHAR g_szFile1[MAX_PATH] = {0};
WCHAR g_szFile2[MAX_PATH] = {0};

// 控件句柄
HWND g_hBtnFile1 = NULL;
HWND g_hBtnFile2 = NULL;
HWND g_hBtnMerge = NULL;
HWND g_hStaticFile1 = NULL;
HWND g_hStaticFile2 = NULL;

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
BOOL                SelectMp3File(HWND hWnd, LPWSTR szFilePath, int nMaxPath);
BOOL                MergeMp3Files(LPCWSTR szFile1, LPCWSTR szFile2, LPCWSTR szOutputFile);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此处放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MFCDEMO, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MFCDEMO));

    MSG msg;

    // 主消息循环:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MFCDEMO));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MFCDEMO);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        {
            // 创建按钮和静态文本控件
            g_hBtnFile1 = CreateWindowW(L"BUTTON", L"文件1", 
                WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                30, 30, 100, 30,
                hWnd, (HMENU)IDC_BTN_FILE1, hInst, NULL);
            
            g_hBtnFile2 = CreateWindowW(L"BUTTON", L"文件2", 
                WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                30, 80, 100, 30,
                hWnd, (HMENU)IDC_BTN_FILE2, hInst, NULL);
            
            g_hBtnMerge = CreateWindowW(L"BUTTON", L"合成", 
                WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                30, 130, 100, 30,
                hWnd, (HMENU)IDC_BTN_MERGE, hInst, NULL);
            
            g_hStaticFile1 = CreateWindowW(L"STATIC", L"未选择", 
                WS_VISIBLE | WS_CHILD | SS_LEFT,
                150, 35, 400, 20,
                hWnd, (HMENU)IDC_STATIC_FILE1, hInst, NULL);
            
            g_hStaticFile2 = CreateWindowW(L"STATIC", L"未选择", 
                WS_VISIBLE | WS_CHILD | SS_LEFT,
                150, 85, 400, 20,
                hWnd, (HMENU)IDC_STATIC_FILE2, hInst, NULL);
        }
        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            case IDC_BTN_FILE1:
                if (SelectMp3File(hWnd, g_szFile1, MAX_PATH))
                {
                    SetWindowTextW(g_hStaticFile1, g_szFile1);
                }
                break;
            case IDC_BTN_FILE2:
                if (SelectMp3File(hWnd, g_szFile2, MAX_PATH))
                {
                    SetWindowTextW(g_hStaticFile2, g_szFile2);
                }
                break;
            case IDC_BTN_MERGE:
                {
                    if (wcslen(g_szFile1) == 0 || wcslen(g_szFile2) == 0)
                    {
                        MessageBoxW(hWnd, L"请先选择两个MP3文件！", L"提示", MB_OK | MB_ICONWARNING);
                        break;
                    }
                    
                    // 打开保存文件对话框
                    OPENFILENAMEW ofn = {0};
                    WCHAR szOutputFile[MAX_PATH] = {0};
                    
                    ofn.lStructSize = sizeof(ofn);
                    ofn.hwndOwner = hWnd;
                    ofn.lpstrFile = szOutputFile;
                    ofn.nMaxFile = MAX_PATH;
                    ofn.lpstrFilter = L"MP3文件 (*.mp3)\0*.mp3\0所有文件 (*.*)\0*.*\0";
                    ofn.nFilterIndex = 1;
                    ofn.lpstrFileTitle = NULL;
                    ofn.nMaxFileTitle = 0;
                    ofn.lpstrInitialDir = NULL;
                    ofn.Flags = OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY;
                    ofn.lpstrDefExt = L"mp3";
                    
                    if (GetSaveFileNameW(&ofn))
                    {
                        if (MergeMp3Files(g_szFile1, g_szFile2, szOutputFile))
                        {
                            MessageBoxW(hWnd, L"MP3文件合成成功！", L"提示", MB_OK | MB_ICONINFORMATION);
                        }
                        else
                        {
                            MessageBoxW(hWnd, L"MP3文件合成失败！", L"错误", MB_OK | MB_ICONERROR);
                        }
                    }
                    break;
                }
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 在此处添加使用 hdc 的任何绘图代码...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
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

// 选择MP3文件
BOOL SelectMp3File(HWND hWnd, LPWSTR szFilePath, int nMaxPath)
{
    OPENFILENAMEW ofn = {0};
    szFilePath[0] = L'\0';
    
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = szFilePath;
    ofn.nMaxFile = nMaxPath;
    ofn.lpstrFilter = L"MP3文件 (*.mp3)\0*.mp3\0所有文件 (*.*)\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    ofn.lpstrDefExt = L"mp3";
    
    return GetOpenFileNameW(&ofn);
}

// 合成MP3文件
BOOL MergeMp3Files(LPCWSTR szFile1, LPCWSTR szFile2, LPCWSTR szOutputFile)
{
    HANDLE hFile1 = INVALID_HANDLE_VALUE;
    HANDLE hFile2 = INVALID_HANDLE_VALUE;
    HANDLE hOutput = INVALID_HANDLE_VALUE;
    BOOL bResult = FALSE;
    DWORD dwBytesRead = 0;
    DWORD dwBytesWritten = 0;
    const DWORD BUFFER_SIZE = 65536; // 64KB buffer
    BYTE* pBuffer = NULL;
    
    __try
    {
        // 分配缓冲区
        pBuffer = (BYTE*)malloc(BUFFER_SIZE);
        if (pBuffer == NULL)
        {
            OutputDebugStringW(L"内存分配失败\n");
            __leave;
        }
        
        // 打开第一个文件
        hFile1 = CreateFileW(szFile1, GENERIC_READ, FILE_SHARE_READ, NULL, 
            OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile1 == INVALID_HANDLE_VALUE)
        {
            OutputDebugStringW(L"无法打开文件1\n");
            __leave;
        }
        
        // 打开第二个文件
        hFile2 = CreateFileW(szFile2, GENERIC_READ, FILE_SHARE_READ, NULL, 
            OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile2 == INVALID_HANDLE_VALUE)
        {
            OutputDebugStringW(L"无法打开文件2\n");
            __leave;
        }
        
        // 创建输出文件
        hOutput = CreateFileW(szOutputFile, GENERIC_WRITE, 0, NULL, 
            CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hOutput == INVALID_HANDLE_VALUE)
        {
            OutputDebugStringW(L"无法创建输出文件\n");
            __leave;
        }
        
        // 复制第一个文件
        while (ReadFile(hFile1, pBuffer, BUFFER_SIZE, &dwBytesRead, NULL) && dwBytesRead > 0)
        {
            if (!WriteFile(hOutput, pBuffer, dwBytesRead, &dwBytesWritten, NULL) || 
                dwBytesWritten != dwBytesRead)
            {
                OutputDebugStringW(L"写入文件1失败\n");
                __leave;
            }
        }
        
        // 复制第二个文件
        while (ReadFile(hFile2, pBuffer, BUFFER_SIZE, &dwBytesRead, NULL) && dwBytesRead > 0)
        {
            if (!WriteFile(hOutput, pBuffer, dwBytesRead, &dwBytesWritten, NULL) || 
                dwBytesWritten != dwBytesRead)
            {
                OutputDebugStringW(L"写入文件2失败\n");
                __leave;
            }
        }
        
        bResult = TRUE;
    }
    __finally
    {
        // 清理资源
        if (pBuffer != NULL)
        {
            free(pBuffer);
        }
        if (hFile1 != INVALID_HANDLE_VALUE)
        {
            CloseHandle(hFile1);
        }
        if (hFile2 != INVALID_HANDLE_VALUE)
        {
            CloseHandle(hFile2);
        }
        if (hOutput != INVALID_HANDLE_VALUE)
        {
            CloseHandle(hOutput);
            if (!bResult)
            {
                // 如果失败，删除不完整的输出文件
                DeleteFileW(szOutputFile);
            }
        }
    }
    
    return bResult;
}
