#include "interface.h"
#include "../encode/encode.h"
#include "../decode/decode.h"

GtkWidget* encode_window;
GtkWidget *encode_entry;
GtkWidget* encode_image;
GtkWidget *encode_box_droite;
GtkWidget* encode_label;
GtkWidget* generale_window;
GtkWidget* decode_window;
GtkWidget* decode_image;
GtkWidget* decode_box_droite;
GtkWidget* decode_box_gauche;
GtkWidget* decode_text;
char * path_image;


void f_encode()
{
	
	
	const gchar * encode_data = gtk_entry_get_text(GTK_ENTRY (encode_entry));
	
	int res = encode((char *)encode_data);
	res = res+0;
	
	if(*encode_data)
	{
	
		if (encode_image != NULL)
		{
			gtk_widget_destroy(encode_image);
		}
		
	
		encode_image = gtk_image_new_from_file("out.bmp");
	
		gtk_container_add(GTK_CONTAINER (encode_box_droite), encode_image);
		gtk_widget_set_margin_top(encode_box_droite, 70);
	
		gtk_widget_show_all(encode_window);
	
		gtk_label_set_text(GTK_LABEL(encode_label), "QR-Code genéré !");
}

else
{
	gtk_label_set_text(GTK_LABEL(encode_label), "Aucune donnee n'a ete encodee");
	
	gtk_widget_show_all(encode_window);
}

	
}

void encode_retour()
{
	
	gtk_widget_show_all(generale_window);
	gtk_widget_hide(encode_window);
	
}

void decode_retour()
{
	
	gtk_widget_show_all(generale_window);
	gtk_widget_hide(decode_window);
	
}

char* choose_file()
{
	GtkWidget * decode_dialog = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    
    GtkWidget* decode_file = gtk_file_chooser_dialog_new("Choisir un QR-Code a decoder", GTK_WINDOW(decode_dialog),GTK_FILE_CHOOSER_ACTION_OPEN,"_Cancel",GTK_RESPONSE_CANCEL,"_Open",GTK_RESPONSE_ACCEPT,NULL);
    
    gint result_dialog = gtk_dialog_run(GTK_DIALOG(decode_file));
    
    if(result_dialog == GTK_RESPONSE_ACCEPT)
    {
		if (decode_image != NULL)
		{
			gtk_widget_destroy(decode_image);
		}
			
		decode_image = gtk_image_new_from_file(gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(decode_file)));
		
		path_image = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(decode_file));
	
		gtk_container_add(GTK_CONTAINER (decode_box_droite), decode_image);
		
	
		gtk_widget_show_all(decode_window);
	}
	gtk_widget_hide(decode_file);
	gtk_widget_destroy(decode_dialog);

	return path_image;
	
	
	}


void create_encode()
{
	
	gtk_widget_hide(generale_window);
	
	//widgets

    encode_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_maximize(GTK_WINDOW(encode_window));
    gtk_window_set_title(GTK_WINDOW(encode_window),"RATIO");
    gtk_window_set_icon_from_file(GTK_WINDOW(encode_window),"app.jpg",NULL);
    
    encode_label = gtk_label_new("Texte à convertir");
    
    GtkWidget * encode_box_gauche = gtk_box_new(GTK_ORIENTATION_VERTICAL, 50);
  
    encode_box_droite = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);
    
    GtkWidget * encode_box_generale = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 100);
    
    
    GtkWidget * encode_bouton_generate = gtk_button_new_with_label("Generer le QR-Code");
    
    encode_entry =	gtk_entry_new();
    
    GtkWidget * encode_bouton_retour = gtk_button_new_with_label("Page d'accueil");
    
    //ajout des widgets dans les boxs
    
    gtk_container_add(GTK_CONTAINER (encode_box_gauche), encode_label);
    
    gtk_container_add( GTK_CONTAINER (encode_box_gauche), encode_entry);
    
    gtk_container_add( GTK_CONTAINER (encode_box_gauche), encode_bouton_generate);    
    
    gtk_container_add( GTK_CONTAINER (encode_box_gauche), encode_bouton_retour);
    
    //gtk_container_add(GTK_CONTAINER (encode_box_droite), encode_image);
    
    gtk_container_add(GTK_CONTAINER (encode_box_generale), encode_box_gauche);
    
	gtk_container_add(GTK_CONTAINER (encode_box_generale), encode_box_droite);
	
	gtk_container_add(GTK_CONTAINER (encode_window), encode_box_generale);
    
	//g_signal_connect(encode_window,"delete_event",gtk_main_quit,NULL);

	gtk_widget_show_all(encode_window);

	
	g_signal_connect(encode_bouton_generate,"clicked", G_CALLBACK(f_encode),NULL);
	
	//visuel
	
	//gtk_widget_set_margin_top(encode_bouton_retour, 30);
	gtk_widget_set_margin_start(encode_box_gauche, 50);
	gtk_widget_set_margin_top(encode_box_gauche, 50);
	
	
	
	//detection signaux
	g_signal_connect(encode_bouton_retour,"clicked", G_CALLBACK(encode_retour), NULL);
	

}

void f_decode(){
	//ici ca marche

	

	
	
	if(path_image != NULL)
	{
		char * msg = malloc(sizeof(char)*64);
		
		decode_main(path_image, msg);

		printf("RESULTAT = %s \n",msg);

		gtk_label_set_text(GTK_LABEL(decode_text),msg );
		gtk_label_set_selectable(GTK_LABEL(decode_text), 1);
		gtk_widget_show_all(decode_window);
		free(msg);
		
	}
	
	else
	{
		char * text_decode = "Veuillez choisir un QR-Code a decoder";
	
	gtk_label_set_text(GTK_LABEL(decode_text), text_decode );
	gtk_widget_show_all(decode_window);
	//gtk_label_set_selectable(decode_text, 1);
	
		
	}
	
	
	
	
	
}

void create_decode()
{
	
	//def des widgets
	
	gtk_widget_hide(generale_window);
	
	decode_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_maximize(GTK_WINDOW(decode_window));
    gtk_window_set_title(GTK_WINDOW(decode_window),"RATIO");
    gtk_window_set_icon_from_file(GTK_WINDOW(decode_window),"app.jpg",NULL);
	
	GtkWidget * decode_box_generale = gtk_box_new(GTK_ORIENTATION_VERTICAL, 50);
	
	GtkWidget * decode_box_gauche = gtk_box_new(GTK_ORIENTATION_VERTICAL, 50);
	
	GtkWidget* decode_box_haut = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 50);
	
	
  
    decode_box_droite = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);
    
    GtkWidget * decode_box_bas= gtk_box_new(GTK_ORIENTATION_VERTICAL, 100);
    
    GtkWidget * decode_bouton_generate = gtk_button_new_with_label("Decoder le QR-Code");  
    
    GtkWidget * decode_bouton_retour = gtk_button_new_with_label("Page d'accueil");
    
    decode_text = gtk_label_new("");
    
    GtkWidget * decode_choose = gtk_button_new_with_label("Choisir un QR-Code a decoder");
    
    //ajout des widgets box
    
    gtk_container_add( GTK_CONTAINER (decode_window), decode_box_generale);
    
    gtk_container_add( GTK_CONTAINER (decode_box_generale), decode_box_haut);
    
    gtk_container_add( GTK_CONTAINER (decode_box_generale), decode_box_bas);
    
    gtk_container_add( GTK_CONTAINER (decode_box_haut), decode_box_gauche);
    
    gtk_container_add( GTK_CONTAINER (decode_box_haut), decode_box_droite);
   
   gtk_container_add( GTK_CONTAINER (decode_box_gauche), decode_choose);
   
       gtk_container_add( GTK_CONTAINER (decode_box_gauche), decode_bouton_generate);
   
   gtk_container_add( GTK_CONTAINER (decode_box_gauche), decode_bouton_retour);
 
    gtk_container_add( GTK_CONTAINER (decode_box_bas), decode_text);
        
    g_signal_connect(decode_choose, "clicked", G_CALLBACK(choose_file),NULL);
    
    //gtk_misc_set_alignment(GTK_MISC(decode_text), 0.5,05);
    
	//detection signaux
	g_signal_connect(decode_bouton_retour,"clicked", G_CALLBACK(decode_retour), NULL);
	
	//g_signal_connect(decode_window,"delete_event",gtk_main_quit,NULL);
	
	g_signal_connect(decode_bouton_generate,"clicked",G_CALLBACK(f_decode),NULL);
	
	gtk_widget_show_all(decode_window);
	//visuel
	
	gtk_widget_set_margin_start(decode_box_gauche, 50);
	gtk_widget_set_margin_top(decode_box_gauche, 50);
	gtk_widget_set_margin_top(decode_box_droite, 50);
	gtk_widget_set_margin_start(decode_box_droite, 50);
	
}

void create_generale(GtkApplication * appli)
{
	//def widget
	
	generale_window = gtk_application_window_new(appli);
    gtk_window_maximize(GTK_WINDOW(generale_window));
    gtk_window_set_title(GTK_WINDOW(generale_window),"RATIO");
    gtk_window_set_icon_from_file(GTK_WINDOW(generale_window),"app.jpg",NULL);
    
    GtkWidget * generale_box_generale = gtk_box_new(GTK_ORIENTATION_VERTICAL, 50);
	
	GtkWidget * generale_box_haut = gtk_box_new(GTK_ORIENTATION_VERTICAL, 50);
	
	GtkWidget * generale_box_bas = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 50);
	
	GtkWidget * generale_bouton_decode = gtk_button_new_with_label("Decoder un QR-Code");
	
	GtkWidget * generale_bouton_encode = gtk_button_new_with_label("Generer un QR-Code");
	
	GtkWidget * generale_logo  = gtk_image_new_from_file("logo.jpg");
	
	//add dans les box
	
	gtk_container_add(GTK_CONTAINER (generale_window), generale_box_generale);
	
	gtk_container_add( GTK_CONTAINER (generale_box_generale), generale_box_haut);
	
	gtk_container_add( GTK_CONTAINER (generale_box_generale), generale_box_bas);
	
	gtk_container_add( GTK_CONTAINER (generale_box_bas), generale_bouton_decode);
	
	gtk_container_add( GTK_CONTAINER (generale_box_bas), generale_bouton_encode);
	
	gtk_container_add( GTK_CONTAINER (generale_box_haut), generale_logo);
	
	//GtkWidget * generale_box_generale  = g_object_ref(generale_box_generale);
	
	// Visuel
	
	gtk_widget_set_size_request(generale_bouton_encode, 150,100);
	gtk_widget_set_margin_start(generale_bouton_decode, 730);
	gtk_widget_set_margin_end(generale_bouton_encode, 200);
	gtk_widget_set_margin_top(generale_logo, 80);
	
	GdkRGBA color;
	
	//color.red = 0xffff;
	//color.green = 0xffff;
	//color.red = 0xffff;
	
	gdk_rgba_parse(&color,"white");

	//gtk_widget_modify_bg(generale_window, GTK_STATE_NORMAL, &color);
	
	gtk_widget_override_background_color(generale_window, GTK_STATE_NORMAL, &color);
	
	//detection signaux
	
	g_signal_connect(generale_bouton_encode,"clicked", G_CALLBACK(create_encode),NULL);

	g_signal_connect(generale_bouton_decode,"clicked", G_CALLBACK(create_decode),NULL);
	
	gtk_widget_show_all(generale_window);	
}



int app(int argumentcount, char * argument[])
{
    GtkApplication * app;
    app = gtk_application_new("test.com", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(create_generale), NULL);

    int status = g_application_run(G_APPLICATION(app),argumentcount, argument);

    g_object_unref(app);
    return status;
}

/*

int main(int argc, char *argv[])
{

GtkApplication * app;
app = gtk_application_new("test.com", G_APPLICATION_FLAGS_NONE);
g_signal_connect(app, "activate", G_CALLBACK(createwindow), NULL);

int status = g_application_run(G_APPLICATION(app),argc, argv);

g_object_unref(app);


return status;

}
*/
