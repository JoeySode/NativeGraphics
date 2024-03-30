
#ifndef NG_WINDOW_H_

#define NG_WINDOW_H_


#include "ng_result.h"


typedef struct NG_Window_T* NG_Window;


// Creates a window with the given title, width, and height
NG_Result ngCreateWindow(NG_Window* p_window, const char* title, int width, int height);

// Destroys the window
void ngDestroyWindow(NG_Window window);


// Returns 1 if the window should close, otherwise 0
int ngWindowShouldClose(NG_Window window);

// Updates the window
void ngWindowUpdate(NG_Window window);


#endif // NG_WINDOW_H_
