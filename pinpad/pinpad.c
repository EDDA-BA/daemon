/*
This file is part of edda.

edda is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

edda is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with edda.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "pinpad.h"


char* handleClick (GtkWidget *widget, gpointer data)
{
  static char i[5] = {0};
  static int idx = 0;

  switch (GPOINTER_TO_INT(data))
  {
    case 11:
    {
      idx = 0;
      memset(i,0,4);
      return NULL;
    }
    case 10:
    {
      return i;
    }
    default:
    {
      if (idx == 0)
      {
        memset(i,0,4);
      }
      if (idx < 4)
      {
        i[idx] = GPOINTER_TO_INT(data);
        idx++;
      }
      return NULL;
    }
  }
}


void click_ok(GtkWidget *button, GtkWidget *window)
{
  gtk_widget_destroy(window);
  gtk_main_quit();
}


void destroy( GtkWidget *widget, gpointer   data )
{
  gtk_main_quit();
}


int _makePinpad (GtkWidget *window)
{
  GtkWidget *grid;
  GtkWidget *button;

  gtk_init_check (0, NULL);

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (window), "Grid");
  g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);
  gtk_container_set_border_width (GTK_CONTAINER (window), 10);

  grid = gtk_grid_new ();

  gtk_container_add (GTK_CONTAINER (window), grid);

  GtkSettings *default_settings = gtk_settings_get_default();
  g_object_set(default_settings, "gtk-button-images", TRUE, NULL); 

  GtkWidget *buttonImage0;
  GtkWidget *buttonImage1;
  GtkWidget *buttonImage2;
  GtkWidget *buttonImage3;
  GtkWidget *buttonImage4;
  GtkWidget *buttonImage5;
  GtkWidget *buttonImage6;
  GtkWidget *buttonImage7;
  GtkWidget *buttonImage8;
  GtkWidget *buttonImage9;
  buttonImage0 = gtk_image_new();
  buttonImage1 = gtk_image_new();
  buttonImage2 = gtk_image_new();
  buttonImage3 = gtk_image_new();
  buttonImage4 = gtk_image_new();
  buttonImage5 = gtk_image_new();
  buttonImage6 = gtk_image_new();
  buttonImage7 = gtk_image_new();
  buttonImage8 = gtk_image_new();
  buttonImage9 = gtk_image_new();
  gtk_image_set_from_file(GTK_IMAGE(buttonImage0),"pinpad/0.jpg");
  gtk_image_set_from_file(GTK_IMAGE(buttonImage1),"pinpad/1.jpg");
  gtk_image_set_from_file(GTK_IMAGE(buttonImage2),"pinpad/2.jpg");
  gtk_image_set_from_file(GTK_IMAGE(buttonImage3),"pinpad/3.jpg");
  gtk_image_set_from_file(GTK_IMAGE(buttonImage4),"pinpad/4.jpg");
  gtk_image_set_from_file(GTK_IMAGE(buttonImage5),"pinpad/5.jpg");
  gtk_image_set_from_file(GTK_IMAGE(buttonImage6),"pinpad/6.jpg");
  gtk_image_set_from_file(GTK_IMAGE(buttonImage7),"pinpad/7.jpg");
  gtk_image_set_from_file(GTK_IMAGE(buttonImage8),"pinpad/8.jpg");
  gtk_image_set_from_file(GTK_IMAGE(buttonImage9),"pinpad/9.jpg");

  button = gtk_button_new_with_label ("1");
  gtk_button_set_image((GtkButton *)button, buttonImage0);
  g_signal_connect (button, "clicked", G_CALLBACK (handleClick), GINT_TO_POINTER('1'));
  gtk_grid_attach (GTK_GRID (grid), button, 0, 0, 1, 1);

  button = gtk_button_new_with_label ("2");
  gtk_button_set_image((GtkButton *)button, buttonImage1);
  g_signal_connect (button, "clicked", G_CALLBACK (handleClick), GINT_TO_POINTER('2'));
  gtk_grid_attach (GTK_GRID (grid), button, 1, 0, 1, 1);

  button = gtk_button_new_with_label ("3");
  gtk_button_set_image((GtkButton *)button, buttonImage2);
  g_signal_connect (button, "clicked", G_CALLBACK (handleClick), GINT_TO_POINTER('3'));
  gtk_grid_attach (GTK_GRID (grid), button, 2, 0, 1, 1);

  button = gtk_button_new_with_label ("4");
  gtk_button_set_image((GtkButton *)button, buttonImage3);
  g_signal_connect (button, "clicked", G_CALLBACK (handleClick), GINT_TO_POINTER('4'));
  gtk_grid_attach (GTK_GRID (grid), button, 0, 1, 1, 1);

  button = gtk_button_new_with_label ("5");
  gtk_button_set_image((GtkButton *)button, buttonImage4);
  g_signal_connect (button, "clicked", G_CALLBACK (handleClick), GINT_TO_POINTER('5'));
  gtk_grid_attach (GTK_GRID (grid), button, 1, 1, 1, 1);

  button = gtk_button_new_with_label ("6");
  gtk_button_set_image((GtkButton *)button, buttonImage5);
  g_signal_connect (button, "clicked", G_CALLBACK (handleClick), GINT_TO_POINTER('6'));
  gtk_grid_attach (GTK_GRID (grid), button, 2, 1, 1, 1);

  button = gtk_button_new_with_label ("7");
  gtk_button_set_image((GtkButton *)button, buttonImage6);
  g_signal_connect (button, "clicked", G_CALLBACK (handleClick), GINT_TO_POINTER('7'));
  gtk_grid_attach (GTK_GRID (grid), button, 0, 2, 1, 1);

  button = gtk_button_new_with_label ("8");
  gtk_button_set_image((GtkButton *)button, buttonImage7);
  g_signal_connect (button, "clicked", G_CALLBACK (handleClick), GINT_TO_POINTER('8'));
  gtk_grid_attach (GTK_GRID (grid), button, 1, 2, 1, 1);

  button = gtk_button_new_with_label ("9");
  gtk_button_set_image((GtkButton *)button, buttonImage8);
  g_signal_connect (button, "clicked", G_CALLBACK (handleClick), GINT_TO_POINTER('9'));
  gtk_grid_attach (GTK_GRID (grid), button, 2, 2, 1, 1);

  button = gtk_button_new_with_label ("0");
  gtk_button_set_image((GtkButton *)button, buttonImage9);
  g_signal_connect (button, "clicked", G_CALLBACK (handleClick), GINT_TO_POINTER('0'));
  gtk_grid_attach (GTK_GRID (grid), button, 1, 3, 1, 1);

  button = gtk_button_new_with_label ("OK");
  g_signal_connect (button, "clicked", G_CALLBACK (click_ok), (gpointer)window);
  gtk_grid_attach (GTK_GRID (grid), button, 0, 4, 3, 2);
  
  gtk_widget_show_all (window);
  
  gtk_main();

  return 0;
}


char* makePinpad()
{
  handleClick(NULL,GINT_TO_POINTER(11));
  GtkWidget window;
  _makePinpad(&window);
  return handleClick (NULL,GINT_TO_POINTER(10));
}
