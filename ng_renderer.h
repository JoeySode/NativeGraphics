
#ifndef NG_RENDERER_H_

#define NG_RENDERER_H_


#include "ng_color.h"
#include "ng_result.h"
#include "ng_window.h"

#include <stddef.h>


typedef struct NG_Renderer_T* NG_Renderer;


// Creates a renderer for the given window
NG_Result ngCreateRenderer(NG_Renderer* p_renderer, NG_Window window, size_t width, size_t height);

// Destroys the renderer
void ngDestroyRenderer(NG_Renderer renderer);


// A pointer to the renderer's pixel data
NG_Color* ngRendererData(const NG_Renderer renderer);

// The number of pixels the renderer has
size_t ngRendererNumPixels(const NG_Renderer renderer);

// Fills the render context with the given color
void ngRendererFill(NG_Renderer renderer, NG_Color color);

// Draws the color to the given index
void ngRendererIndex(NG_Renderer renderer, size_t index, NG_Color color);

// Draws the color to the given renderer coordinates
void ngRendererPixel(NG_Renderer renderer, size_t x, size_t y, NG_Color color);

// Presents the renderer to its window
void ngRendererPresent(const NG_Renderer renderer);

#endif // NG_RENDERER_H_

