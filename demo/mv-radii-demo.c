#include <stdlib.h>
#include <goocanvas.h>


static gboolean on_rect_button_press (GooCanvasItem  *view,
				      GooCanvasItem  *target,
				      GdkEventButton *event,
				      gpointer        data);

static gboolean on_delete_event      (GtkWidget      *window,
				      GdkEvent       *event,
				      gpointer        unused_data);


int
main (int argc, char *argv[])
{
  GtkWidget *window, *scrolled_win, *canvas;
  GooCanvasItemModel *root, *rect_model, *text_model;
  GooCanvasItem *rect_item;

  /* Initialize GTK+. */
  gtk_init (&argc, &argv);

  /* Create the window and widgets. */
  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size (GTK_WINDOW (window), 640, 600);
  gtk_widget_show (window);
  g_signal_connect (window, "delete_event", G_CALLBACK (on_delete_event),
		    NULL);

  scrolled_win = gtk_scrolled_window_new (NULL, NULL);
  gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolled_win),
				       GTK_SHADOW_IN);
  gtk_widget_show (scrolled_win);
  gtk_container_add (GTK_CONTAINER (window), scrolled_win);

  canvas = goo_canvas_new ();
  gtk_widget_set_size_request (canvas, 600, 450);
  goo_canvas_set_bounds (GOO_CANVAS (canvas), 0, 0, 1000, 1000);
  gtk_widget_show (canvas);
  gtk_container_add (GTK_CONTAINER (scrolled_win), canvas);

  root = goo_canvas_group_model_new (NULL, NULL);

  /* Add an item with all border modified. */
  rect_model = goo_canvas_rect_model_new (root, 100, 100, 400, 400,
					  "line-width", 10.0,
					  "radius-top-left-x", 30.0,
					  "radius-top-left-y", 20.0,
					  "radius-top-right-x", 5.0,
					  "radius-top-right-y", 10.0,
					  "radius-bottom-left-x", 20.0,
					  "radius-bottom-left-y", 30.0,
					  "radius-bottom-right-x", 10.0,
					  "radius-bottom-right-y", 5.0,
					  "stroke-color", "green",
					  "fill-color", "blue",
					  NULL);


  text_model = goo_canvas_text_model_new (root, "Hello World", 300, 300, -1,
					  GOO_CANVAS_ANCHOR_CENTER,
					  "font", "Sans 24",
					  NULL);
  goo_canvas_item_model_rotate (text_model, 45, 300, 300);

  goo_canvas_set_root_item_model (GOO_CANVAS (canvas), root);

  rect_item = goo_canvas_get_item (GOO_CANVAS (canvas), rect_model);

  /* Connect a signal handler for the rectangle item. */
  g_signal_connect (rect_item, "button_press_event",
		    G_CALLBACK (on_rect_button_press), NULL);

  /* Pass control to the GTK+ main event loop. */
  gtk_main ();

  return 0;
}


/* The signal handler for the rectangle item. We show a dialog here. */
static gboolean
on_rect_button_press (GooCanvasItem  *item,
		      GooCanvasItem  *target,
		      GdkEventButton *event,
		      gpointer        data)
{
  GooCanvas *canvas;
  GtkWidget *window, *dialog;

  canvas = goo_canvas_item_get_canvas (item);
  window = gtk_widget_get_toplevel (GTK_WIDGET (canvas));
  dialog = gtk_message_dialog_new (GTK_WINDOW (window),
				   GTK_DIALOG_DESTROY_WITH_PARENT,
				   GTK_MESSAGE_INFO,
				   GTK_BUTTONS_CLOSE,
				   "rect item received button press event");
  gtk_dialog_run (GTK_DIALOG (dialog));
  gtk_widget_destroy (dialog);

  return TRUE;
}


/* This is our handler for the "delete-event" signal of the window, which
   is emitted when the 'x' close button is clicked. We just exit here. */
static gboolean
on_delete_event (GtkWidget *window,
		 GdkEvent  *event,
		 gpointer   unused_data)
{
  exit (0);
}
