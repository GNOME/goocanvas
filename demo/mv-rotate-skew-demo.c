#include <stdlib.h>
#include <math.h>
#include <goocanvas.h>

typedef enum {
  MODE_MOVE,
  MODE_RESIZE,
  MODE_ROTATE,
  MODE_SKETCH
} Mode;

Mode             drag_mode;
GooCanvasItem   *drag_item = NULL;
gdouble          drag_x = 0.0;
gdouble          drag_y = 0.0;

gdouble          item_x = 0.0;
gdouble          item_y = 0.0;
gdouble          item_width = 0.0;
gdouble          item_height = 0.0;

gdouble          canvas_event_x = 0.0;
gdouble          canvas_event_y = 0.0;

static gboolean
on_button_press_event_cb (GooCanvasItem *item,
                          GooCanvasItem *target_item,
                          GdkEventButton *event,
                          gpointer user_data)
{
  GooCanvasItemModel *model = goo_canvas_item_get_model (item);
  if (event->state & GDK_CONTROL_MASK || event->state & GDK_SHIFT_MASK)
  {
    if (event->button == 1 || event->button == 3)
    {
      if (event->state & GDK_CONTROL_MASK)
      {
        if (event->button == 1)
        {
          drag_mode = MODE_MOVE;
        }
        else
        {
          drag_mode = MODE_RESIZE;
        }
      }
      else
      {
        if (event->button == 1)
        {
          drag_mode = MODE_ROTATE;
        }
        else
        {
          drag_mode = MODE_SKETCH;
        }
      }

      drag_item = item;
      drag_x = event->x;
      drag_y = event->y;
      canvas_event_x = event->x;
      canvas_event_y = event->y;
      goo_canvas_convert_from_item_space(GOO_CANVAS(user_data), item, &canvas_event_x, &canvas_event_y);

      g_object_get (G_OBJECT (model),
                    "x", &item_x,
                    "y", &item_y,
                    "width", &item_width,
                    "height", &item_height,
                    NULL);

      goo_canvas_pointer_grab (GOO_CANVAS (user_data), item, GDK_POINTER_MOTION_MASK | GDK_POINTER_MOTION_HINT_MASK | GDK_BUTTON_RELEASE_MASK, NULL, event->time);
      return TRUE;
    }
  }

  return FALSE;
}

static gboolean
on_button_release_event_cb (GooCanvasItem  *item,
                            GooCanvasItem  *target_item,
                            GdkEventButton *event,
                            gpointer user_data)
{
  if (drag_item == item && drag_item != NULL)
  {
    goo_canvas_pointer_ungrab (GOO_CANVAS (user_data), drag_item, event->time);
    drag_item = NULL;
    g_object_get (G_OBJECT (item),
                  "x", &item_x,
                  "y", &item_y,
                  "width", &item_width,
                  "height", &item_height,
                  NULL);
    return TRUE;
  }

  return FALSE;
}

static gboolean
on_motion_notify_event_cb (GooCanvasItem  *item,
                           GooCanvasItem  *target_item,
                           GdkEventMotion *event,
                           gpointer user_data)
{
  GooCanvasItemModel *model = goo_canvas_item_get_model (item);

  GooCanvas *canvas = GOO_CANVAS(user_data);

  gboolean ret = FALSE;

  gdouble canvas_temp_event_x = event->x, canvas_temp_event_y = event->y;
  goo_canvas_convert_from_item_space(canvas, item, &canvas_temp_event_x, &canvas_temp_event_y);

  if (drag_item == item && drag_item != NULL)
  {
    gdouble rel_x = event->x - drag_x;
    gdouble rel_y = event->y - drag_y;

    if (drag_mode == MODE_MOVE)
    {
      goo_canvas_item_translate (item, rel_x, rel_y);
    }
    else if (drag_mode == MODE_RESIZE)
    {
      gdouble new_width = MAX (item_width + rel_x, 5.0);
      gdouble new_height = MAX (item_height + rel_y, 5.0);

      g_object_set (G_OBJECT (model), "width", new_width, "height", new_height, NULL);
    }
    else
    {
      double center_x = item_x + item_width / 2.0f;
      double center_y = item_y + item_height / 2.0f;

      gdouble canvas_center_x = center_x, canvas_center_y = center_y;
      goo_canvas_convert_from_item_space (canvas, item, &canvas_center_x, &canvas_center_y);

      double start_radians = atan2 (canvas_center_y - canvas_temp_event_y, canvas_temp_event_x - canvas_center_x );
      double radians = atan2 (canvas_center_y - canvas_event_y, canvas_event_x - canvas_center_x );
      radians = radians - start_radians;
      double rotation = radians * (180 / M_PI);

      if (drag_mode == MODE_ROTATE)
      {
        goo_canvas_item_model_rotate(model, rotation, center_x, center_y);
      }
      else
      {
        if (canvas_event_x - canvas_temp_event_x != 0)
        {
          goo_canvas_item_model_skew_x(model, rotation, center_x, center_y);
        }
        if (canvas_event_y - canvas_temp_event_y != 0)
        {
          goo_canvas_item_model_skew_y(model, rotation, center_x, center_y);
        }
      }
    }

    ret = TRUE;
  }

  canvas_event_x = canvas_temp_event_x;
  canvas_event_y = canvas_temp_event_y;

  return ret;
}


static void
on_item_created (GooCanvas          *canvas,
		 GooCanvasItem      *item,
		 GooCanvasItemModel *model,
		 gpointer            data)
{
  if (g_object_get_data (G_OBJECT (model), "setup-dnd-signals"))
    {
      g_signal_connect (G_OBJECT (item), "button-press-event", G_CALLBACK (on_button_press_event_cb), canvas);
      g_signal_connect (G_OBJECT (item), "button-release-event", G_CALLBACK (on_button_release_event_cb), canvas);
      g_signal_connect (G_OBJECT (item), "grab-broken-event", G_CALLBACK (on_button_release_event_cb), canvas);
      g_signal_connect (G_OBJECT (item), "motion-notify-event", G_CALLBACK (on_motion_notify_event_cb), canvas);
    }
}


static GooCanvasItemModel*
create_model (GdkPixbuf *pixbuf)
{
  GooCanvasItemModel *root;
  GooCanvasItemModel *item;
  GooCanvasItemModel* child;

  root = goo_canvas_group_model_new (NULL, NULL);

  child = goo_canvas_rect_model_new (root, 0.0, 0.0, 100, 100, "fill-color", "blue", NULL);
  g_object_set_data (G_OBJECT (child), "setup-dnd-signals", "TRUE");

  child = goo_canvas_ellipse_model_new (root, 150, 0, 50, 50, "fill-color", "red", NULL);
  g_object_set_data (G_OBJECT (child), "setup-dnd-signals", "TRUE");

  return root;
}


static GtkWidget*
create_window (GooCanvasItemModel *model)
{
  GtkWidget *window, *vbox, *label, *scrolled_win, *canvas;

  /* Create the window and widgets. */
  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size (GTK_WINDOW (window), 640, 600);
  gtk_widget_show (window);
  g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

  vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 4);
  gtk_container_set_border_width (GTK_CONTAINER (vbox), 4);
  gtk_widget_show (vbox);
  gtk_container_add (GTK_CONTAINER (window), vbox);

  label = gtk_label_new ("Use Ctrl+Left Click to move items or Ctrl+Right Click to resize items");
  g_object_set (label, "halign", GTK_ALIGN_START, NULL);
  gtk_box_pack_start (GTK_BOX (vbox), label, FALSE, FALSE, 0);
  gtk_widget_show (label);

  /* Create top canvas. */
  scrolled_win = gtk_scrolled_window_new (NULL, NULL);
  gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolled_win),
                                       GTK_SHADOW_IN);
  gtk_widget_show (scrolled_win);
  gtk_box_pack_start (GTK_BOX (vbox), scrolled_win, TRUE, TRUE, 0);

  canvas = goo_canvas_new ();
  g_object_set (G_OBJECT (canvas), "integer-layout", TRUE, NULL);
  goo_canvas_set_bounds (GOO_CANVAS (canvas), 0, 0, 1000, 1000);
  gtk_widget_show (canvas);
  gtk_container_add (GTK_CONTAINER (scrolled_win), canvas);

  g_signal_connect (canvas, "item_created", G_CALLBACK (on_item_created), NULL);

  goo_canvas_set_root_item_model (GOO_CANVAS (canvas), model);

  return window;
}


int
main (int argc, char *argv[])
{
  GooCanvasItemModel *model;
  GdkPixbuf *pixbuf;

  gtk_init (&argc, &argv);

  pixbuf = gtk_icon_theme_load_icon (gtk_icon_theme_get_default (), "dialog-warning", 48, 0, NULL);

  model = create_model (pixbuf);

  /* Create 2 windows to show off multiple views. */
  create_window (model);
#if 0 //set to 1 if you want to see a second view
  create_window (model);
#endif

  g_object_unref (model);

  gtk_main ();

  return 0;
}
