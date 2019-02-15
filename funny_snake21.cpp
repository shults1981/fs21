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
#include <gtkmm.h>
#include <gtkmm/drawingarea.h>
#include <cairomm/context.h>


#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <unistd.h>


#include <signal.h>
#include <sys/time.h>


//-----------------------------
#include "game.h"
#include "unit.h"


using namespace std;



void init_scr(int* RowMax, int* ColMax);
void CreateGameFild(Fild GameFild, int RowMax, int ColMax);
void destr_scr();
void gameMenuOpen();
void gameMenuClose();
void render(Game *GameController,int FrameFlag);
void GTI(int signom);



static int row,col;
static int row_max,col_max;
static int ch;
//static MoveDirection mvf;
static char str_BUF1[5],str_BUF2[5],str_BUF3[5];
char buf1[2]={'0',0x00};
int level;
int GameImpuls=0;
int ImpulsFront=0;
int Watchdog=0;

//++++++++++++++++++++++++++++
//==================================================
class MyArea:public Gtk::DrawingArea
{
public:
	Fild pole;
	PointArr unit_snake,unit_rabbit;
	GameStatus PST;


	MyArea();
	~MyArea();

protected:

	guint X_max,Y_max;


	bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);
};

MyArea::MyArea():unit_snake(0),unit_rabbit(0)
{
//	unit_snake(0);
//	unit_rabit(0);

}
MyArea::~MyArea()
{
}

bool MyArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
	Point pen;
	guint width, height;
	GdkRGBA color;
	GtkStyleContext *context;

	guint  x,y;
	guint i,k,m;
	guint scr_border_x_min,scr_border_x_max,scr_border_y_min,scr_border_y_max;
	gfloat hStep,vStep;


	Gtk::Allocation allocation = get_allocation();	

	width=allocation.get_width();
	height=allocation.get_height();

	X_max=width;
	Y_max=height;

	scr_border_x_min=(X_max-9*X_max/10);
	scr_border_x_max=(X_max-1*X_max/10);
	scr_border_y_min=(Y_max-9*Y_max/10);
	scr_border_y_max=(Y_max-1*Y_max/10);

	hStep=(gfloat)(scr_border_x_max-scr_border_x_min)/(gfloat)(pole.border_x_max-pole.border_x_min);
	vStep=(gfloat)(scr_border_y_max-scr_border_y_min)/(gfloat)(pole.border_y_max-pole.border_y_min);

//	gtk_render_background(context,cr,0,0,width,height);

	//---------- Make game fild ----------------------
	color.red=0.0;
	color.green=0.0;
	color.blue=0.0;
	color.alpha=1.0;
	cr->set_source_rgb(0.0,0.0,0.0);


	cr->move_to (scr_border_x_min,scr_border_y_min);
	cr->line_to(scr_border_x_max+hStep,scr_border_y_min);
	cr->line_to(scr_border_x_max+hStep,scr_border_y_max+vStep);
	cr->line_to(scr_border_x_min,scr_border_y_max+vStep);
	cr->line_to(scr_border_x_min,scr_border_y_min);
	cr->set_line_width(2.0);
	cr->stroke();	

	cr->move_to(scr_border_x_min,scr_border_y_min-10);
	cr->show_text(" Game SNAKE  ");


	
//*************************************************

	if (PST==game_stop)
	{
		// -- menu border
		cr->move_to (scr_border_x_max/2-25,scr_border_y_max/2-10);
		cr->line_to(scr_border_x_max/2+65,scr_border_y_max/2-10);
		cr->line_to(scr_border_x_max/2+65,scr_border_y_max/2+45);
		cr->line_to(scr_border_x_max/2-25,scr_border_y_max/2+45);
		cr->line_to(scr_border_x_max/2-25,scr_border_y_max/2-10);
		cr->set_line_width(1.0);
		cr->stroke();
		// -- menu text
		cr->move_to(scr_border_x_max/2-20,scr_border_y_max/2);
		cr->show_text("       MENU:");
		cr->move_to(scr_border_x_max/2-20,scr_border_y_max/2+10);
		cr->show_text("NEW GAME....'n'");
		cr->move_to(scr_border_x_max/2-20,scr_border_y_max/2+20);
		cr->show_text("MENU/PAUSE.'m'");
		cr->move_to(scr_border_x_max/2-20,scr_border_y_max/2+30);
		cr->show_text("CONTINUE.......'c'");
		cr->move_to(scr_border_x_max/2-20,scr_border_y_max/2+40);
		cr->show_text("EXIT.....'ALT+F4'");
	}


	if (PST==game_on||PST==game_new_level)
	{
		unit_rabbit.getElement(0,pen);
		cr->rectangle(	scr_border_x_min+pen._x*hStep,
				scr_border_y_min+pen._y*vStep,
				hStep,
				vStep);
		cr->set_line_width(1.5);
		cr->fill();

		for(i=0;i<unit_snake.getLen();i++ )
		{
			unit_snake.getElement(i,pen);
			cr->rectangle(	scr_border_x_min+pen._x*hStep,
					scr_border_y_min+pen._y*vStep,
					hStep,
					vStep);			
		}

		//====  information ====
//		sprintf (str_BUF1,"%d",Score);
		cr->move_to(scr_border_x_min,scr_border_y_max+15);
		cr->show_text("Score-");
		cr->move_to(scr_border_x_min+60,scr_border_y_max+15);
//		cr->show_text(str_BUF1);

//		sprintf (str_BUF2,"%d",Level);
		cr->move_to(scr_border_x_min,scr_border_y_max+25);
		cr->show_text("Level-");
		cr->move_to(scr_border_x_min+60,scr_border_y_max+25);
//		cr->show_text(str_BUF2);
	}
/*
	if (PST==game_next_level)
	{
		cairo_move_to(cr,scr_border_x_max/2-30,scr_border_y_max/2-20);
		cairo_show_text(cr,"N E X T    L E V E L !!!!!");
	}

	if (PST==game_over)
	{
		cairo_move_to(cr,scr_border_x_max/2-30,scr_border_y_max/2-20);
		cairo_show_text(cr,"G A M E   O V E R !!!!!");
	}

	cairo_fill(cr);
*/	



/*		
	if ((GameCntrl->getGameStatus()==game_on)||(GameCntrl->getGameStatus()==game_new_level)){
		GameCntrl->getRabbitPlace(pen);
//		mvaddch(pen._y,pen._x,'*');


		for (int i=0;i<GameCntrl->getSnakeLen();i++){

			if (GameCntrl->getSnakeBodyPartsCords(i,pen)){
			        if (FrameFlag){
//					mvaddch(pen._y,pen._x,'@');
				;
				}	
				else
//					mvaddch(pen._y,pen._x,' ');
					;
			}
		}

//			sprintf(str_BUF1,"%d",GameCntrl->getGameScore());
//			mvaddstr(pole.border_y_max+2,pole.border_x_min,"Score-");			
//			mvaddstr(pole.border_y_max+2,pole.border_x_min+7,str_BUF1);		
//			sprintf(str_BUF2,"%d",GameCntrl->getGameLevel());
//			mvaddstr(pole.border_y_max+3,pole.border_x_min,"Level-");			
//			mvaddstr(pole.border_y_max+3,pole.border_x_min+7,str_BUF2);			
	}
*/
/*	
	if (GameCntrl->getGameStatus()==game_new_level){
//		mvaddstr(pole.border_y_max/2-5,pole.border_x_max/2-5,"N E X T     L E V E L !!!!");
//		wrefresh(stdscr);
//		napms(2000);
	}
	if (GameCntrl->getGameStatus()==game_over){
//		mvaddstr(pole.border_y_max/2-5,pole.border_x_max/2-5,"G A M E   O V E R !!!!");
//		wrefresh(stdscr);
//		napms(2000);
	}
*/







	
//**********************************

	return false;

}

//====================================================
class MainWindow: public Gtk::Window
{
public:
	MainWindow();		
	virtual ~MainWindow();

	Fild gameFild;
	Game *GameController; //!!!!!!
	MoveDirection mvf;    ///!!

protected:

	GameStatus PST;

	MyArea area;


	bool Tic();
	bool Main_Loop();
	bool on_key_press_event(GdkEventKey* key_event);	
	void OnQuit();
	void _render();	
};

MainWindow::MainWindow()
{
	PST=game_stop;
	gameFild.border_x_min=0;
	gameFild.border_x_max=50;
	gameFild.border_y_min=0;
	gameFild.border_y_max=50;

	GameController=new Game(gameFild,6,0);
	area.pole=GameController->getGameFild();

	this->set_default_size(300,300);
	this->set_title("FunnySnake21");
	this->add(area);

	add_events(Gdk::KEY_PRESS_MASK);
	
	Glib::signal_timeout().connect( sigc::mem_fun(*this, &MainWindow::Tic), 200 );



	area.show();		

}

MainWindow::~MainWindow()
{

	delete GameController;

}

bool MainWindow::Tic()
{
	Main_Loop();
	_render();	
	
	return true;
}

bool MainWindow::Main_Loop()
{
	switch (PST)
	{
		case game_exit:
			GameController->setGameStatus(game_over);
			GameController->setGameStatus(game_exit);
			OnQuit();
			break;
		case game_new:
			GameController->setGameStatus(game_stop);
			GameController->setGameStatus(game_over);
			GameController->setGameStatus(game_new);
			GameController->setGameStatus(game_on);
			mvf=static_cast<MoveDirection>(0);
			PST=game_on;
			break;
		case game_stop:
			GameController->setGameStatus(game_stop);
			break;
		case game_on:
			if(GameController->getGameStatus()!=game_over){
				GameController->setGameStatus(game_on);
			}
			else{
				 PST=game_stop;
			}
			break;
		default:
			break;
	}

	
		if (GameController->getGameStatus()==game_on)
		{
				GameController->SnakeControl(mvf);
				GameController->SnakeMoveToOneStep();
		}

		if (GameController->getGameStatus()==game_new_level){
			GameController->setGameStatus(game_stop);
			GameController->setGameStatus(game_over);
			GameController->setGameStatus(game_new);
			GameController->setGameStatus(game_on);
			mvf=static_cast<MoveDirection>(0);
		}

		if (GameController->getGameStatus()==game_over)
			PST=game_over;


	g_print("Game Step\n");					
	return true;
}

bool MainWindow::on_key_press_event(GdkEventKey* key_event)
{

	if (key_event->keyval==GDK_KEY_m){
		PST=game_stop;
	}

	if (PST==game_on){
		switch(key_event->keyval)
		{
			case GDK_KEY_Left:
					mvf=Left;
					g_print("Pressed key -Left key-\n");					
					break;			
			case GDK_KEY_Right:
					mvf=Right;
					g_print("Pressed key -Right key-\n");					
					break;
			case GDK_KEY_Up:
					mvf=Up;
					g_print("Pressed key -Up key-\n");					
					break;
			case GDK_KEY_Down:
					mvf=Down;
					g_print("Pressed key -Down key-\n");					
					break; 
			default : break;
			}
	}

	if (PST==game_stop||PST==game_over){
		switch (key_event->keyval)  {
			case GDK_KEY_e:
				PST=game_exit;
				break;
			case GDK_KEY_n:
				PST=game_new;
				break;
			case GDK_KEY_c:
				PST=game_on;
				break;
			default:
				break;
		}
	}

	return true;
}
void MainWindow::_render()
{
	int i,LS, ls;
	Point tp1;

	area.PST=GameController->getGameStatus();
	GameController->getRabbitPlace(tp1);
	area.unit_rabbit.setElement(tp1,0);
	
	if(PST==game_on)
	{
	/*
		if ((GameController->getSnakeLen()-area.unit_snake.getLen())==1){ // !! may be not use operator- "=="
			GameController->getSnakeBodyPartsCords(GameController->getSnakeLen()-1,tp1);
			area.unit_snake.addElementInBack(tp1);
		} 
	*/
	
		if ((GameController->getSnakeLen()-area.unit_snake.getLen())==0){// !! may be use only- "!"
			for (i=0;i<GameController->getSnakeLen();i++){
				GameController->getSnakeBodyPartsCords(i,tp1);
				area.unit_snake.setElement(tp1,i);
			}		
		}
	}
	if (PST==game_over/*||PST==game_new*/){
		for (i=0;i<GameController->getSnakeLen();i++){
				area.unit_snake.delElementFromBack();
			}
		area.unit_rabbit.delElementFromBack();
	}

	area.queue_draw();
}

void MainWindow::OnQuit()
{
	hide();
}


//=======================================================================


//++++++++++++++++++++++++++++


//ofstream fout;//**********************-------------------

//============================= MAIN ======================================
		
int main (int argc, char** argv)
{	
	
//	fout.open("fs2.log");//**************-------------------
		
	int PRG=1;
	struct itimerval tmr1; //,tmr2;

//	signal(SIGALRM,GTI);// registring game timer
	
//	init_scr(&row_max,&col_max); // initialize ncurses;

	Glib::RefPtr<Gtk::Application> app;
	//auto app=Gtk::Application::create(argc,argv,"org.gtkmm.examples.base"); //if using C++11 or hi
	app=Gtk::Application::create(argc,argv,"org.gtkmm.examples.base");

	MainWindow w;



//	CreateGameFild(gameFild,row_max,col_max);  //---------- Draw game fild ----------------------
	
	
	
	//--------------------- main cicle---------------		
/*
	while (PRG)
	{

		ch=wgetch(stdscr);
	
	
		if (ch=='m')
		{
			GameController->setGameStatus(game_stop);
			ImpulsFront=0;
		}

		if ((GameController->getGameStatus()==game_stop) || (GameController->getGameStatus()==game_over))
		{
			if (!MainMenu)
				gameMenuOpen();
			switch (ch)
			{
			case 'e':
				GameController->setGameStatus(game_over);
				GameController->setGameStatus(game_exit);
				gameMenuClose();
				PRG=0;
				break;
			case 'n':
				tmr1.it_value.tv_sec=0;
				tmr1.it_value.tv_usec=200000;
				tmr1.it_interval.tv_sec=0;
				tmr1.it_interval.tv_usec=200000;
				setitimer(ITIMER_REAL,&tmr1,NULL); // start game ciclic timer

				GameController->setGameStatus(game_stop);
				GameController->setGameStatus(game_over);
				gameMenuClose();
				CreateGameFild(gameFild,row_max,col_max);				
				GameController->setGameStatus(game_new);
				GameController->setGameStatus(game_on);
				mvf=static_cast<MoveDirection>(0);
				break;
			case 'c':
				if(GameController->getGameStatus()!=game_over)
				{
					GameController->setGameStatus(game_on);
					gameMenuClose();
					CreateGameFild(gameFild,row_max,col_max);				
				}
				break;
			
			}		
		}	
		
		if (GameController->getGameStatus()==game_new_level){
			GameController->setGameStatus(game_stop);
			GameController->setGameStatus(game_over);
			CreateGameFild(gameFild,row_max,col_max);				
			
			tmr1.it_value.tv_sec=0;
			tmr1.it_value.tv_usec=200000-GameController->getGameLevel()*10000;
			tmr1.it_interval.tv_sec=0;
			tmr1.it_interval.tv_usec=200000-GameController->getGameLevel()*10000;

			setitimer(ITIMER_REAL,&tmr1,NULL); // reload and restart game ciclic timer
							
			GameController->setGameStatus(game_new);
			GameController->setGameStatus(game_on);
			mvf=static_cast<MoveDirection>(0);
		}
				
		if (GameController->getGameStatus()==game_on)
		{
			if (GameImpuls!=Watchdog){
				ImpulsFront=1;
				Watchdog=GameImpuls;
			}
			else {
				ImpulsFront=0;
			}

			switch(ch)
			{
				case KEY_LEFT:
						mvf=Left;					
						break;			
				case KEY_RIGHT:
						mvf=Right;
						break;
				case KEY_UP:
						mvf=Up;
						break;
				case KEY_DOWN:
						mvf=Down;
						break; 
			
				default : break;
			}

			if (ImpulsFront){
				
				render(GameController,0);
	
				GameController->SnakeControl(mvf);
				GameController->SnakeMoveToOneStep();

				render(GameController,1);
			}

		}
	
	}
*/
//	destr_scr();//-----------delete screen -------------

	
//	fout.close();

	app->run(w);
	return 0;

}	




void init_scr(int *RowMax, int *ColMax)
{

	//-------------init ncurses -----------------------------------
/*
	initscr();
	start_color();
	nodelay(stdscr,TRUE);
	curs_set(0);
	keypad (stdscr, TRUE);
	noecho();
	init_pair (1,COLOR_WHITE,COLOR_BLUE);
	init_pair (2,COLOR_WHITE,COLOR_BLUE);
	attron(COLOR_PAIR(1));
	getmaxyx(stdscr,*RowMax,*ColMax);
*/
}

void destr_scr()
{
//	endwin();
}

void CreateGameFild(Fild GameFild, int RowMax,int ColMax)
{
int row, col;
/*
	for (row=0;row<=RowMax;row++)
	{
		for (col=0;col<=ColMax;col++)
		{
			move(row,col);
			if ((row>=GameFild.border_y_min)&&(row<=GameFild.border_y_max)&&(col>=GameFild.border_x_min)&&(col<=GameFild.border_x_max))
			{
				if ((row==GameFild.border_y_min) && (col==GameFild.border_x_min))
					addch(ACS_ULCORNER);
				if ((row==GameFild.border_y_min)&& (col>GameFild.border_x_min) && (col<GameFild.border_x_max))
					addch(ACS_HLINE);
				if ((row==GameFild.border_y_min) && (col==GameFild.border_x_max))
					addch(ACS_URCORNER);

				if ((row>GameFild.border_y_min)&&(row<GameFild.border_y_max)&&(col==GameFild.border_x_min))
					addch(ACS_VLINE);
				if ((row>GameFild.border_y_min)&&(row<GameFild.border_y_max)&&(col==GameFild.border_x_max))
					addch(ACS_VLINE);

				if ((row==GameFild.border_y_max) && (col==GameFild.border_x_min))
					addch(ACS_LLCORNER);
				if ((row==GameFild.border_y_max)&& (col>GameFild.border_x_min) && (col<GameFild.border_x_max))
					addch(ACS_HLINE);
				if ((row==GameFild.border_y_max) && (col==GameFild.border_x_max))
					addch(ACS_LRCORNER);		
			}
			addch(' ');
		}
	}
*/
}


void gameMenuOpen()
{
/*
	MainMenu=newwin(10,20,gameFild.border_y_max/2-2,gameFild.border_x_max/2-5);
	wbkgd(MainMenu,COLOR_PAIR(2));
	wattron(MainMenu,COLOR_PAIR(2));
	box(MainMenu,ACS_VLINE,ACS_HLINE);
	wmove(MainMenu,1,7);
	waddstr(MainMenu,"MENU:");
	wmove(MainMenu,3,1);
	waddstr(MainMenu,"NEW GAME...'n'");
	wmove(MainMenu,5,1);
	waddstr(MainMenu,"CONTINUE...'c'");
	wmove(MainMenu,7,1);
	waddstr(MainMenu,"EXIT.......'e'");
	wrefresh(MainMenu);
*/	
}


void gameMenuClose()
{
//	delwin(MainMenu);
//	MainMenu=NULL;
}


void render(Game *GameCntrl,int FrameFlag)
{
	Point pen;
	Fild pole;

	pole=GameCntrl->getGameFild();
/*		
	if ((GameCntrl->getGameStatus()==game_on)||(GameCntrl->getGameStatus()==game_new_level)){
		GameCntrl->getRabbitPlace(pen);
//		mvaddch(pen._y,pen._x,'*');


		for (int i=0;i<GameCntrl->getSnakeLen();i++){

			if (GameCntrl->getSnakeBodyPartsCords(i,pen)){
			        if (FrameFlag){
//					mvaddch(pen._y,pen._x,'@');
				;
				}	
				else
//					mvaddch(pen._y,pen._x,' ');
					;
			}
		}

//			sprintf(str_BUF1,"%d",GameCntrl->getGameScore());
//			mvaddstr(pole.border_y_max+2,pole.border_x_min,"Score-");			
//			mvaddstr(pole.border_y_max+2,pole.border_x_min+7,str_BUF1);		
//			sprintf(str_BUF2,"%d",GameCntrl->getGameLevel());
//			mvaddstr(pole.border_y_max+3,pole.border_x_min,"Level-");			
//			mvaddstr(pole.border_y_max+3,pole.border_x_min+7,str_BUF2);			
	}
*/
/*	
	if (GameCntrl->getGameStatus()==game_new_level){
//		mvaddstr(pole.border_y_max/2-5,pole.border_x_max/2-5,"N E X T     L E V E L !!!!");
//		wrefresh(stdscr);
//		napms(2000);
	}
	if (GameCntrl->getGameStatus()==game_over){
//		mvaddstr(pole.border_y_max/2-5,pole.border_x_max/2-5,"G A M E   O V E R !!!!");
//		wrefresh(stdscr);
//		napms(2000);
	}
*/

//	wrefresh(stdscr);

}

void GTI (int signom)
{
	GameImpuls+=1;
}

