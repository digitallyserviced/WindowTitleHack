#include <dlfcn.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>

#include "wth.h"

typedef int (*XChangeProperty_func_type)(
    Display *display,
    Window w,
    Atom property,
    Atom type,
    int format,
    int mode,
    const unsigned char *data,
    int nelements
);

static struct wth_once once = WTH_ONCE_INITIALIZER;
static char *new_title;
static char *new_class;
const char placeholders[60] = "Use WTH_TITLE or WTH_CLASS environment variable to use this";
static XChangeProperty_func_type XChangeProperty_orig;
static Atom _NET_WM_NAME;
static Atom WM_CLASS;

static void init_once(void *user_arg)
{
    /* Cache original function pointer */
    XChangeProperty_orig = dlsym(RTLD_NEXT, "XChangeProperty");

     if (wth_get_title(&new_title)==1){
        Display *display = user_arg;
        _NET_WM_NAME = XInternAtom(display, "_NET_WM_NAME", 0);
     }

     if (wth_get_class(&new_class)==1){
        Display *display = user_arg;
        WM_CLASS = XInternAtom(display, "WM_CLASS", 0);
     }

}

int XChangeProperty(
    Display *display,
    Window w,
    Atom property,
    Atom type,
    int format,
    int mode,
    const unsigned char *data,
    int nelements
) {
    wth_init_once(&once, init_once, display);

    if (property == XA_WM_NAME || property == _NET_WM_NAME) {
      if((int)strlen(new_title)>0){
        data = (const unsigned char *)new_title;
        nelements = (int)strlen(new_title);
      }
    }
    if(property == XA_WM_CLASS || property == WM_CLASS){
      if((int)strlen(new_class)>0){
        data = (const unsigned char *)new_class;
        nelements = (int)strlen(new_class);
      }
    }

    return XChangeProperty_orig(display, w, property, type, format, mode, data, nelements);
}
