
#include "ng_renderer.h"

#include "ng_color.h"
#include "ng_result.h"

#include <stdint.h>
#include <stdlib.h>
#include <Windows.h>


// Definition copied from "ng_window.h"
typedef struct NG_Window_T
{
  HWND window_handle;
  HDC device_context;

  int width;
  int height;

  int should_close;
} NG_Window_T;

typedef struct NG_Renderer_T
{
  NG_Window_T* window;

  NG_Color* pixels;
  int64_t width;
  int64_t height;

  HBITMAP bitmap;
  HDC mem;
} NG_Renderer_T;


NG_Result ngCreateRenderer(NG_Renderer* p_renderer, NG_Window window, size_t width, size_t height)
{
  // Allocate the renderer
  NG_Renderer_T* renderer = (NG_Renderer_T*)malloc(sizeof(NG_Renderer_T));

  if (!renderer)
    return NG_ERROR_ALLOC;

  // Set up initial values
  renderer->window = window;

  renderer->width = (int64_t)width;
  renderer->height = (int64_t)height;

  // Allocate the pixels
  renderer->pixels = (NG_Color*)calloc(width * height, sizeof(NG_Color));

  if (!renderer->pixels)
  {
    ngDestroyRenderer(renderer);
    return NG_ERROR_ALLOC;
  }

  // Create compatible device context and bitmap
  renderer->mem = CreateCompatibleDC(window->device_context);
  renderer->bitmap = CreateCompatibleBitmap(window->device_context, width, height);
  SelectObject(renderer->mem, renderer->bitmap);

  // Done
  *p_renderer = renderer;

  return NG_SUCCESS;
}

void ngDestroyRenderer(NG_Renderer renderer)
{
  if (!renderer)
    return;

  if (renderer->pixels)
    free(renderer->pixels);

  if (renderer->bitmap)
    DeleteObject(renderer->bitmap);

  if (renderer->mem)
    DeleteDC(renderer->mem);

  free(renderer);
}


NG_Color* ngRendererData(const NG_Renderer renderer)
{
  return renderer->pixels;
}

size_t ngRendererNumPixels(const NG_Renderer renderer)
{
  return renderer->width * renderer->height;
}

void ngRendererFill(NG_Renderer renderer, NG_Color color)
{
  const size_t num_pixels = renderer->width * renderer->height;

  for (size_t i = 0; i < num_pixels; i++)
    renderer->pixels[i] = color;
}

void ngRendererIndex(NG_Renderer renderer, size_t index, NG_Color color)
{
  renderer->pixels[index] = color;
}

void ngRendererPixel(NG_Renderer renderer, size_t x, size_t y, NG_Color color)
{
  const size_t index = (y * renderer->width) + x;

  renderer->pixels[index] = color;
}

void ngRendererPresent(const NG_Renderer renderer)
{
  BITMAPINFO info = (BITMAPINFO){ 0 };
  info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  info.bmiHeader.biWidth = renderer->width;
  info.bmiHeader.biHeight = -renderer->height;
  info.bmiHeader.biPlanes = 1;
  info.bmiHeader.biBitCount = 32;
  info.bmiHeader.biCompression = BI_RGB;

  SetDIBits(renderer->mem, renderer->bitmap, 0, renderer->height, renderer->pixels, &info, DIB_RGB_COLORS);
  StretchBlt(renderer->window->device_context, 0, 0, renderer->window->width, renderer->window->height, renderer->mem, 0, 0, renderer->width, renderer->height, SRCCOPY);
}
