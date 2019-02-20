//++++++++++++++++++++++
//++++++++++++++++++++++
//   mainwindow.h
//++++++++++++++++++++++
//+++++++++++++++++++++


#ifndef MAINWINDOW_H
#define MAINWINDOW_H
//
//
#include <gtkmm.h>
#include <gtkmm/drawingarea.h>
#include <cairomm/context.h>


//-----------------------------
#include "game.h"
#include "unit.h"



//++++++++++++++++++++++++++++
//==================================================
class MyArea:public Gtk::DrawingArea
{
public:
	Fild pole;
	PointArr unit_snake,unit_rabbit;
	GameStatus PST;
	int Score;
	int Level;

	MyArea();
	~MyArea();

protected:

	guint X_max,Y_max;


	bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);
};



//====================================================
class MainWindow: public Gtk::Window
{
public:
	MainWindow();		
	virtual ~MainWindow();



protected:
	Fild gameFild;
	Game *GameController; //!!!!!!
	MoveDirection mvf;    ///!!

	sigc::connection timerSource;
	//int timerSource;
	guint TimeBase;
	guint LevelTimeStep;
	guint GamePause;
	guint delay_cnt;

	GameStatus PST;

	MyArea area;

	bool Tic();
	bool Main_Loop();
	bool on_key_press_event(GdkEventKey* key_event);	
	void OnQuit();
	void _render();	
};


#endif

