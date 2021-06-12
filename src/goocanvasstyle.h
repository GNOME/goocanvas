/*
 * GooCanvas. Copyright (C) 2005-6 Damon Chaplin.
 * Released under the GNU LGPL license. See COPYING for details.
 *
 * goocanvasstyle.h - cascading styles.
 */
#ifndef __GOO_CANVAS_STYLE_H__
#define __GOO_CANVAS_STYLE_H__

#include <gtk/gtk.h>
#include "goocanvasexports.h"

G_BEGIN_DECLS


/* GQuarks for the basic properties. */
GOOCANVAS_EXTERNAL_V GQuark goo_canvas_style_stroke_pattern_id;
GOOCANVAS_EXTERNAL_V GQuark goo_canvas_style_fill_pattern_id;
GOOCANVAS_EXTERNAL_V GQuark goo_canvas_style_fill_rule_id;
GOOCANVAS_EXTERNAL_V GQuark goo_canvas_style_operator_id;
GOOCANVAS_EXTERNAL_V GQuark goo_canvas_style_antialias_id;
GOOCANVAS_EXTERNAL_V GQuark goo_canvas_style_line_width_id;
GOOCANVAS_EXTERNAL_V GQuark goo_canvas_style_line_cap_id;
GOOCANVAS_EXTERNAL_V GQuark goo_canvas_style_line_join_id;
GOOCANVAS_EXTERNAL_V GQuark goo_canvas_style_line_join_miter_limit_id;
GOOCANVAS_EXTERNAL_V GQuark goo_canvas_style_line_dash_id;
GOOCANVAS_EXTERNAL_V GQuark goo_canvas_style_font_desc_id;
GOOCANVAS_EXTERNAL_V GQuark goo_canvas_style_hint_metrics_id;


/**
 * GooCanvasStyleProperty:
 * @id: the unique property identifier.
 * @value: the value of the property.
 *
 * #GooCanvasStyleProperty represents a property setting.
 */
typedef struct _GooCanvasStyleProperty GooCanvasStyleProperty;
struct _GooCanvasStyleProperty
{
  GQuark id;
  GValue value;
};


#define GOO_TYPE_CANVAS_STYLE            (goo_canvas_style_get_type ())
#define GOO_CANVAS_STYLE(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GOO_TYPE_CANVAS_STYLE, GooCanvasStyle))
#define GOO_CANVAS_STYLE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GOO_TYPE_CANVAS_STYLE, GooCanvasStyleClass))
#define GOO_IS_CANVAS_STYLE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GOO_TYPE_CANVAS_STYLE))
#define GOO_IS_CANVAS_STYLE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GOO_TYPE_CANVAS_STYLE))
#define GOO_CANVAS_STYLE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GOO_TYPE_CANVAS_STYLE, GooCanvasStyleClass))


typedef struct _GooCanvasStyle       GooCanvasStyle;
typedef struct _GooCanvasStyleClass  GooCanvasStyleClass;

/**
 * GooCanvasStyle:
 * @parent: the parent style.
 * @properties: an array of #GooCanvasStyleProperty property settings.
 *
 * #GooCanvasStyle holds the style properties of a canvas item, as well as a
 * pointer to the parent style.
 */
struct _GooCanvasStyle
{
  /* <private> */
  GObject parent_object;

  /* <public> */
  GooCanvasStyle *parent;
  GArray *properties;
};

struct _GooCanvasStyleClass
{
  GObjectClass parent_class;

  /*< private >*/

  /* Padding for future expansion */
  void (*_goo_canvas_reserved1) (void);
  void (*_goo_canvas_reserved2) (void);
  void (*_goo_canvas_reserved3) (void);
  void (*_goo_canvas_reserved4) (void);
};


GOOCANVAS_API
GType           goo_canvas_style_get_type           (void) G_GNUC_CONST;
GOOCANVAS_API
GooCanvasStyle* goo_canvas_style_new                (void);
GOOCANVAS_API
GooCanvasStyle* goo_canvas_style_copy               (GooCanvasStyle *style);

GOOCANVAS_API
GooCanvasStyle* goo_canvas_style_get_parent         (GooCanvasStyle *style);
GOOCANVAS_API
void            goo_canvas_style_set_parent         (GooCanvasStyle *style,
						     GooCanvasStyle *parent);

GOOCANVAS_API
GValue*         goo_canvas_style_get_property       (GooCanvasStyle *style,
						     GQuark          property_id);
GOOCANVAS_API
void            goo_canvas_style_set_property	    (GooCanvasStyle *style,
						     GQuark          property_id,
						     const GValue   *value);

/* Convenience functions to set the standard cairo stroke and fill options. */
GOOCANVAS_API
gboolean        goo_canvas_style_set_stroke_options (GooCanvasStyle *style,
						     cairo_t        *cr);
GOOCANVAS_API
gboolean        goo_canvas_style_set_fill_options   (GooCanvasStyle *style,
						     cairo_t        *cr);

G_END_DECLS

#endif /* __GOO_CANVAS_STYLE_H__ */
