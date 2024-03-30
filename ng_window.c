
#include "ng_window.h"

#include <stdlib.h>
#include <Windows.h>


typedef struct NG_Window_T
{
  HWND window_handle;
  HDC device_context;

  int width;
  int height;

  int should_close;
} NG_Window_T;


NG_Result ngCreateWindow(NG_Window* p_window, const char* title, int width, int height)
{
  // Allocate the window
  NG_Window_T* window = (NG_Window_T*)malloc(sizeof(NG_Window_T));

  if (!window)
    return NG_ERROR_ALLOC;

  // Set up initial values
  window->width = width;
  window->height = height;

  window->should_close = 0;

  // Register window class
  WNDCLASS window_class = (WNDCLASS){ 0 };

  window_class.lpfnWndProc = DefWindowProc;
  window_class.hInstance = GetModuleHandle(NULL);
  window_class.lpszClassName = title;

  if (!RegisterClass(&window_class))
  {
    ngDestroyWindow(window);
    return NG_ERROR_OS;
  }

  // Set up the window rect
  RECT window_rect = (RECT){ 0, 0, width, height };
  AdjustWindowRect(&window_rect, WS_OVERLAPPEDWINDOW, FALSE);

  // Create the window
  window->window_handle = CreateWindow
  (
    title,
    title,
    WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT,
    CW_USEDEFAULT,
    window_rect.right - window_rect.left,
    window_rect.bottom - window_rect.top,
    NULL,
    NULL,
    GetModuleHandle(NULL),
    NULL
  );

  if (!window->window_handle)
  {
    ngDestroyWindow(window);
    return NG_ERROR_OS;
  }

  // Get the device context
  window->device_context = GetDC(window->window_handle);

  if (!window->device_context)
  {
    ngDestroyWindow(window);
    return NG_ERROR_OS;
  }

  // Show and refresh
  ShowWindow(window->window_handle, SW_SHOWNORMAL);
  UpdateWindow(window->window_handle);

  // Done
  *p_window = window;

  return NG_SUCCESS;
}

void ngDestroyWindow(NG_Window window)
{
  if (!window)
    return;

  if (window->device_context)
    ReleaseDC(window->window_handle, window->device_context);

  if (window->window_handle)
    DestroyWindow(window->window_handle);

  free(window);
}


int ngWindowShouldClose(NG_Window window)
{
  return window->should_close;
}

void ngWindowUpdate(NG_Window window)
{
  MSG msg;

  while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  if (!IsWindow(window->window_handle))
    window->should_close = 1;
}
