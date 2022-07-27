//LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
//{
//	switch (message)
//	{
//	case WM_CHAR: //this is just for a program exit besides window's borders/taskbar
//		if (wparam == VK_ESCAPE)
//		{
//			DestroyWindow(hwnd);
//		}
//	case WM_DESTROY:
//		PostQuitMessage(0);
//		break;
//	default:
//		return DefWindowProc(hwnd, message, wparam, lparam);
//	}
//}
//
//const char* const myclass = "myclass";
//WNDCLASSEX wndclass = { sizeof(WNDCLASSEX), CS_DBLCLKS, WndProc,
//0, 0, GetModuleHandle(0), LoadIcon(0,IDI_APPLICATION),
//LoadCursor(0,IDC_ARROW), HBRUSH(COLOR_WINDOW + 1),
//0, myclass, LoadIcon(0,IDI_APPLICATION) };
//if (RegisterClassEx(&wndclass))
//{
//	HWND window = CreateWindowEx(0, myclass, "title",
//		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
//		CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, GetModuleHandle(0), 0);
//	if (window)
//	{
//		ShowWindow(window, SW_SHOWDEFAULT);
//		HDC hdc = GetDC(window);
//		for (int x = 0; x < 256; ++x)
//			for (int y = 0; y < 256; ++y)
//				SetPixel(hdc, x, y, RGB(127, x, y));
//		MSG msg;
//		while (GetMessage(&msg, 0, 0, 0)) DispatchMessage(&msg);
//	}
//}