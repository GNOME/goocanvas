/*
 * GooCanvas. Copyright (C) 2005 Damon Chaplin.
 * Released under the GNU LGPL license. See COPYING for details.
 *
 * goocanvasrect.c - rectangle item.
 */

/**
 * SECTION:goocanvasrect
 * @Title: GooCanvasRect
 * @Short_Description: a rectangle item.
 *
 * GooCanvasRect represents a rectangle item.
 *
 * It is a subclass of #GooCanvasItemSimple and so inherits all of the style
 * properties such as "stroke-color", "fill-color" and "line-width".
 *
 * It also implements the #GooCanvasItem interface, so you can use the
 * #GooCanvasItem functions such as goo_canvas_item_raise() and
 * goo_canvas_item_rotate().
 *
 * To create a #GooCanvasRect use goo_canvas_rect_new().
 *
 * To get or set the properties of an existing #GooCanvasRect, use
 * g_object_get() and g_object_set().
 */
#include <config.h>
#include <math.h>
#include <glib/gi18n-lib.h>
#include <gtk/gtk.h>
#include "goocanvasrect.h"
#include "goocanvas.h"


enum {
  PROP_0,

  PROP_X,
  PROP_Y,
  PROP_WIDTH,
  PROP_HEIGHT,
  PROP_RADIUS_X,
  PROP_RADIUS_Y
};


G_DEFINE_TYPE (GooCanvasRect, goo_canvas_rect, GOO_TYPE_CANVAS_ITEM_SIMPLE)


static void
goo_canvas_rect_init (GooCanvasRect *rect)
{

}


/**
 * goo_canvas_rect_new:
 * @parent: (skip): the parent item, or %NULL. If a parent is specified, it will assume
 *  ownership of the item, and the item will automatically be freed when it is
 *  removed from the parent. Otherwise call g_object_unref() to free it.
 * @x: the x coordinate of the left of the rectangle.
 * @y: the y coordinate of the top of the rectangle.
 * @width: the width of the rectangle.
 * @height: the height of the rectangle.
 * @...: optional pairs of property names and values, and a terminating %NULL.
 * 
 * Creates a new rectangle item.
 * 
 * <!--PARAMETERS-->
 *
 * Here's an example showing how to create a rectangle at (100,100) with a
 * width of 200 and a height of 100.
 *
 * <informalexample><programlisting>
 *  GooCanvasItem *rect = goo_canvas_rect_new (mygroup, 100.0, 100.0, 200.0, 100.0,
 *                                             "stroke-color", "red",
 *                                             "line-width", 5.0,
 *                                             "fill-color", "blue",
 *                                             NULL);
 * </programlisting></informalexample>
 * 
 * Returns: (transfer full): a new rectangle item.
 **/
GooCanvasItem*
goo_canvas_rect_new (GooCanvasItem *parent,
		     gdouble        x,
		     gdouble        y,
		     gdouble        width,
		     gdouble        height,
		     ...)
{
  GooCanvasItem *item;
  GooCanvasRect *rect;
  const char *first_property;
  va_list var_args;

  item = g_object_new (GOO_TYPE_CANVAS_RECT, NULL);
  rect = (GooCanvasRect*) item;

  rect->x = x;
  rect->y = y;
  rect->width = width;
  rect->height = height;
  rect->radius_x = 0;
  rect->radius_y = 0;

  va_start (var_args, height);
  first_property = va_arg (var_args, char*);
  if (first_property)
    g_object_set_valist ((GObject*) item, first_property, var_args);
  va_end (var_args);

  if (parent)
    {
      goo_canvas_item_add_child (parent, item, -1);
      g_object_unref (item);
    }

  return item;
}


static void
goo_canvas_rect_get_property (GObject              *object,
			      guint                 prop_id,
			      GValue               *value,
			      GParamSpec           *pspec)
{
  GooCanvasRect *rect = (GooCanvasRect*) object;

  switch (prop_id)
    {
    case PROP_X:
      g_value_set_double (value, rect->x);
      break;
    case PROP_Y:
      g_value_set_double (value, rect->y);
      break;
    case PROP_WIDTH:
      g_value_set_double (value, rect->width);
      break;
    case PROP_HEIGHT:
      g_value_set_double (value, rect->height);
      break;
    case PROP_RADIUS_X:
      g_value_set_double (value, rect->radius_x);
      break;
    case PROP_RADIUS_Y:
      g_value_set_double (value, rect->radius_y);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}


static void
goo_canvas_rect_set_property (GObject              *object,
			      guint                 prop_id,
			      const GValue         *value,
			      GParamSpec           *pspec)
{
  GooCanvasItemSimple *simple = (GooCanvasItemSimple*) object;
  GooCanvasRect *rect = (GooCanvasRect*) object;

  switch (prop_id)
    {
    case PROP_X:
      rect->x = g_value_get_double (value);
      break;
    case PROP_Y:
      rect->y = g_value_get_double (value);
      break;
    case PROP_WIDTH:
      rect->width = g_value_get_double (value);
      break;
    case PROP_HEIGHT:
      rect->height = g_value_get_double (value);
      break;
    case PROP_RADIUS_X:
      rect->radius_x = g_value_get_double (value);
      break;
    case PROP_RADIUS_Y:
      rect->radius_y = g_value_get_double (value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }

  goo_canvas_item_simple_changed (simple, TRUE);
}


static void
goo_canvas_rect_create_path (GooCanvasItemSimple *simple,
			     cairo_t             *cr)
{
  GooCanvasRect *rect = (GooCanvasRect*) simple;

  cairo_new_path (cr);

  /* Check if we need to draw rounded corners. */
  if (rect->radius_x > 0 && rect->radius_y > 0)
    {
      /* The radii can't be more than half the size of the rect. */
      double rx = MIN (rect->radius_x, rect->width / 2);
      double ry = MIN (rect->radius_y, rect->height / 2);

      /* Draw the top-right arc. */
      cairo_save (cr);
      cairo_translate (cr, rect->x + rect->width - rx, rect->y + ry);
      cairo_scale (cr, rx, ry);
      cairo_arc (cr, 0.0, 0.0, 1.0, 1.5 * M_PI, 2.0 * M_PI);
      cairo_restore (cr);

      /* Draw the line down the right side. */
      cairo_line_to (cr, rect->x + rect->width, rect->y + rect->height - ry);

      /* Draw the bottom-right arc. */
      cairo_save (cr);
      cairo_translate (cr, rect->x + rect->width - rx,
		       rect->y + rect->height - ry);
      cairo_scale (cr, rx, ry);
      cairo_arc (cr, 0.0, 0.0, 1.0, 0.0, 0.5 * M_PI);
      cairo_restore (cr);

      /* Draw the line left across the bottom. */
      cairo_line_to (cr, rect->x + rx, rect->y + rect->height);

      /* Draw the bottom-left arc. */
      cairo_save (cr);
      cairo_translate (cr, rect->x + rx, rect->y + rect->height - ry);
      cairo_scale (cr, rx, ry);
      cairo_arc (cr, 0.0, 0.0, 1.0, 0.5 * M_PI, M_PI);
      cairo_restore (cr);

      /* Draw the line up the left side. */
      cairo_line_to (cr, rect->x, rect->y + ry);

      /* Draw the top-left arc. */
      cairo_save (cr);
      cairo_translate (cr, rect->x + rx, rect->y + ry);
      cairo_scale (cr, rx, ry);
      cairo_arc (cr, 0.0, 0.0, 1.0, M_PI, 1.5 * M_PI);
      cairo_restore (cr);

      /* Close the path across the top. */
      cairo_close_path (cr);
    }
  else
    {
      /* Draw the plain rectangle. */
      cairo_rectangle (cr, rect->x, rect->y, rect->width, rect->height);
    }
}


static void
goo_canvas_rect_update  (GooCanvasItemSimple *simple,
			 cairo_t             *cr)
{
  GooCanvasRect *rect = (GooCanvasRect*) simple;
  gdouble half_line_width;

  /* We can quickly compute the bounds as being just the rectangle's size
     plus half the line width around each edge. */
  half_line_width = goo_canvas_item_simple_get_line_width (simple) / 2;

  simple->bounds.x1 = rect->x - half_line_width;
  simple->bounds.y1 = rect->y - half_line_width;
  simple->bounds.x2 = rect->x + rect->width + half_line_width;
  simple->bounds.y2 = rect->y + rect->height + half_line_width;
}


static void
goo_canvas_rect_class_init (GooCanvasRectClass *klass)
{
  GObjectClass *gobject_class = (GObjectClass*) klass;
  GooCanvasItemSimpleClass *simple_class = (GooCanvasItemSimpleClass*) klass;

  gobject_class->get_property = goo_canvas_rect_get_property;
  gobject_class->set_property = goo_canvas_rect_set_property;

  simple_class->simple_create_path = goo_canvas_rect_create_path;
  simple_class->simple_update      = goo_canvas_rect_update;

  g_object_class_install_property (gobject_class, PROP_X,
				   g_param_spec_double ("x",
							"X",
							_("The x coordinate of the rectangle"),
							-G_MAXDOUBLE,
							G_MAXDOUBLE, 0.0,
							G_PARAM_READWRITE));

  g_object_class_install_property (gobject_class, PROP_Y,
				   g_param_spec_double ("y",
							"Y",
							_("The y coordinate of the rectangle"),
							-G_MAXDOUBLE,
							G_MAXDOUBLE, 0.0,
							G_PARAM_READWRITE));

  g_object_class_install_property (gobject_class, PROP_WIDTH,
				   g_param_spec_double ("width",
							_("Width"),
							_("The width of the rectangle"),
							0.0, G_MAXDOUBLE, 0.0,
							G_PARAM_READWRITE));

  g_object_class_install_property (gobject_class, PROP_HEIGHT,
				   g_param_spec_double ("height",
							_("Height"),
							_("The height of the rectangle"),
							0.0, G_MAXDOUBLE, 0.0,
							G_PARAM_READWRITE));

  g_object_class_install_property (gobject_class, PROP_RADIUS_X,
				   g_param_spec_double ("radius_x",
							_("Radius X"),
							_("The horizontal radius to use for rounded corners"),
							0.0, G_MAXDOUBLE, 0.0,
							G_PARAM_READWRITE));

  g_object_class_install_property (gobject_class, PROP_RADIUS_Y,
				   g_param_spec_double ("radius_y",
							_("Radius Y"),
							_("The vertical radius to use for rounded corners"),
							0.0, G_MAXDOUBLE, 0.0,
							G_PARAM_READWRITE));
}
