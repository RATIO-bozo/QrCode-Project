#include <X11/Xlib.h>
#include <stdio.h>
#include <png.h>
#include <stdlib.h>
#include <X11/Xutil.h>
#include <X11/X.h>


int min(int x,int y)
{
	if(x>y)
	{
		return y;
	}
	else
	{
		return x;
	}
}

int save_screenshot(int x1, int y1, int x2, int y2) {
  Display *display = XOpenDisplay(NULL);
  Window root = RootWindow(display, DefaultScreen(display));

  // Calcule les dimensions de la région à capturer
  int width = abs(x2 - x1);
  int height = abs(y2 - y1);
  int x = min(x1, x2);
  int y = min(y1, y2);

  XImage *image = XGetImage(display, root, x, y, width, height, AllPlanes, ZPixmap);

  png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png_ptr) {
    fprintf(stderr, "Error: Failed to create PNG write struct\n");
    return 1;
  }

  png_infop info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr) {
    png_destroy_write_struct(&png_ptr, NULL);
    fprintf(stderr, "Error: Failed to create PNG info struct\n");
    return 1;
  }

  FILE *fp = fopen("screenshot.png", "wb");
  if (!fp) {
    png_destroy_write_struct(&png_ptr, &info_ptr);
    fprintf(stderr, "Error: Failed to open file\n");
    return 1;
  }

  png_init_io(png_ptr, fp);
  png_set_IHDR(png_ptr, info_ptr, width, height, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
  png_write_info(png_ptr, info_ptr);

  int row_stride = width * 3;
  png_bytep row = (png_bytep)malloc(row_stride * sizeof(png_byte));
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      unsigned long pixel = XGetPixel(image, j, i);
      row[j * 3 + 0] = (pixel & image->red_mask) >> 16;
      row[j * 3 + 1] = (pixel & image->green_mask) >> 8;
      row[j * 3 + 2] = (pixel & image->blue_mask);
    }
    png_write_row(png_ptr, row);
  }

  free(row);
  png_write_end(png_ptr, NULL);
  png_destroy_write_struct(&png_ptr, &info_ptr);
  fclose(fp);
  XDestroyImage(image);
  XCloseDisplay(display);
  return 0;
}





int main()
{
    Display *display;
    Window window;
    XEvent event;
    int x, y, x1, y1;

    display = XOpenDisplay(NULL);
    window = DefaultRootWindow(display);

    XGrabPointer(display, window, False,
        ButtonPressMask, GrabModeAsync, GrabModeAsync,
        None, None, CurrentTime);

    XNextEvent(display, &event);
    switch (event.type)
    {
        case ButtonPress:
            x = event.xbutton.x;
            y = event.xbutton.y;
            break;
    }

    XNextEvent(display, &event);
    switch (event.type)
    {
        case ButtonPress:
            x1 = event.xbutton.x;
            y1 = event.xbutton.y;
            break;
    }
    save_screenshot(x,y,x1,y1);
    XCloseDisplay(display);
    return 0;
}

