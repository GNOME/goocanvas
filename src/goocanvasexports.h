/*
 * GooCanvas. Copyright (C) 2005-6 Damon Chaplin.
 * Released under the GNU LGPL license. See COPYING for details.
 *
 * goocanvasexports.h - API export/import macros
 */
#ifndef __GOO_CANVAS_EXPORTS_H__
#define __GOO_CANVAS_EXPORTS_H__

#include <glib.h>

#if defined (G_OS_WIN32) && defined (_MSC_VER) && !defined (GOOCANVAS_BUILD_STATIC)
# ifdef BUILDING_GOOCANVAS
#  define GOOCANVAS_EXTERNAL_V __declspec(dllexport) extern
#  define GOOCANVAS_API        __declspec(dllexport)
# else
#  define GOOCANVAS_EXTERNAL_V __declspec(dllimport) extern
#  define GOOCANVAS_API        __declspec(dllimport)
# endif
#else
# define GOOCANVAS_EXTERNAL_V  extern
# define GOOCANVAS_API
#endif

#endif /* __GOO_CANVAS_EXPORTS_H__ */
