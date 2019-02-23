/*
|*****************************************************************
|*****************************************************************
* Project                        :  <Funny Snake #2>

* Programm name                  :  funny_snake21.cpp

* Author                         :  Shults1981

* Data create                    :  03/02/2019

* Purpose                        :  classical game "SNAKE";
				    testing library gtk  #2 (gtkmm)

|****************************************************************
|****************************************************************
*/

#include "mainwindow.h"

using namespace std;
		
int main (int argc, char** argv)
{	
	Glib::RefPtr<Gtk::Application> app;
	//auto app=Gtk::Application::create(argc,argv,"org.gtkmm.examples.base"); //if using C++11 or hi
	app=Gtk::Application::create(argc,argv,"org.gtkmm.examples.base");

	MainWindow w;
	
	app->run(w);

	return 0;
}
