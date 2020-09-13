// VideoManager.cpp : 定义应用程序的入口点。
//

#ifdef WIN32
#include <shlobj.h>
#endif

#include "framework.h"
#include "VideoManager.h"
#include "VMDef.h"
#include "shlwapi.h"

#define MAX_LOADSTRING 100
#pragma comment(lib, "shlwapi.lib") 
// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 自定义全局变量
CONFIG g_config;


// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

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
    LoadStringW(hInstance, IDC_VIDEOMANAGER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }
	// 初始化配置
	IniConfig();

	// 若无根目录配置，主动弹出根目录选择对话框



    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_VIDEOMANAGER));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_VIDEOMANAGER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_VIDEOMANAGER);
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
			case ID_CHOSEFILEROOT:	//选择系统文件的根目录文件夹
				ChoseFileRoot();
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

//选择系统的根目录文件夹
void ChoseFileRoot()
{
	//弹出文件选择对话框
	BROWSEINFO  bi;
	bi.hwndOwner = NULL;
	bi.pidlRoot = CSIDL_DESKTOP;//文件夹的根目录，此处为桌面
	bi.pszDisplayName = NULL;
	bi.lpszTitle = NULL;//显示位于对话框左上部的提示信息
	bi.ulFlags = BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;//有新建文件夹按钮
	bi.lpfn = NULL;
	bi.iImage = 0;
	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);//调用选择对话框
	if (pidl == NULL)
	{
		MessageBox(NULL, "没有配置玩你妈呢", NULL, 0);
		return ;
	}
	TCHAR strFolder[MAX_PATH];
	SHGetPathFromIDList(pidl, strFolder);

	//将根目录文件夹的路径存储到全局标识符中
	g_config.rootPath = strFolder;
	MessageBox(NULL, g_config.rootPath, NULL, 0);
}


///<summary>
/// Function:获得当前程序所在路径
/// Author  :Webster
/// Date    :2020年9月13日18:27:27
/// ParamIn :NULL
/// ParamOut:path 程序路径
///</summary>      
bool VM_GetModuleFilePath(char* path)
{
	TCHAR szPath[MAX_PATH] = { 0 };
	if (!GetModuleFileName(NULL, szPath, MAX_PATH))
	{
		return false;
	}
	char drive[5];
	char dir[100];
	char filename[100];
	char fileext[10];
	
	// 拆分获取当前程序所处的驱动器号、文件夹路径、文件名和文件类型
	_splitpath_s(szPath, drive, dir, filename, fileext);

	// 组合path的到当前程序的所在路径
	_makepath_s(path, _MAX_PATH, drive, dir, NULL, NULL);

	return true;
}


///<summary>
/// Function:读取文件初始化配置
/// Author  :Webster
/// Date    :2020年9月13日18:34:25
/// ParamIn :path config.ini文件路径
/// ParamOut:
///</summary> 
bool IniConfigByPath(char* iniPath)
{
	// 确定工作根目录
	char buf[MAX_PATH] = {0};
	GetPrivateProfileString("ROOT", //节名
		"path", //项名
		"No Path", //没找到此项时的返回值
		buf, //目标缓冲区地址
		MAX_PATH, //目标缓冲区长度
		iniPath); //配置文件的准确路径
	g_config.rootPath = buf;
	// 检查根目录是否存在
	if (PathIsDirectory(g_config.rootPath) != FILE_ATTRIBUTE_DIRECTORY)
	{
		return CONFIG_NOROOTFILE;
	}
	return CONFIG_SUCESS;
}


///<summary>
/// Function:初始化配置
/// Author  :Webster
/// Date    :2020年9月13日18:37:34
/// ParamIn :NULL
/// ParamOut:NULL
///</summary>   
bool IniConfig()
{
	// 获得程序所在文件夹
	char path[MAX_PATH] = { 0 };
	VM_GetModuleFilePath(path); 
	// 获得配置文件地址iniPath
	char iniPath[MAX_PATH];
	char fileName[] = "config.ini";
	sprintf_s(iniPath, "%.*s%.*s", sizeof(path), path, sizeof(fileName), fileName);
	

	if (IniConfigByPath(iniPath) == CONFIG_NOROOTFILE)	// 无工作目录配置信息
	{
		ChoseFileRoot(); // 选择工作目录
		// TODO:将当前工作目录写入配置文件
	}

	return CONFIG_SUCESS;
}