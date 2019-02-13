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
static MoveDirection mvf;
static char str_BUF1[5],str_BUF2[5],str_BUF3[5];
char buf1[2]={'0',0x00};
int level;
int GameImpuls=0;
int ImpulsFront=0;
int Watchdog=0;

//++++++++++++++++++++++++++++

class MainWindow: public Gtk::Window
{
public:
	MainWindow();		
	virtual ~MainWindow();

	Fild gameFild;
	Game *GameController; //!!!!!!
	MoveDirection mvf;    ///!!

protected:

//	enum _ProgrammStatus {g_exit=0,g_stop,g_new,g_continue} PST; 
	GameStatus PST;

	Gtk::DrawingArea area;


	bool Tic();
	bool Main_Loop();
	bool on_key_press_event(GdkEventKey* key_event);	
	//	bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);

};

MainWindow::MainWindow()
{
	PST=game_stop;
	gameFild.border_x_min=0;
	gameFild.border_x_max=50;
	gameFild.border_y_min=0;
	gameFild.border_y_max=50;

	GameController=new Game(gameFild,6,0);
	
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
	
	return true;
}

bool MainWindow::Main_Loop()
{
	switch (PST)
	{
		case game_exit:
			GameController->setGameStatus(game_over);
			GameController->setGameStatus(game_exit);
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




	g_print("Game Step\n");					
	return true;
}

bool MainWindow::on_key_press_event(GdkEventKey* key_event)
{

	if (key_event->keyval==GDK_KEY_m){
		PST=game_stop;
	}
	if (PST==game_stop||PST==game_over){//!!!!!!!!!!!11
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

	return true;
}
/*
bool MyArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
	Gtk::Allocation allocation = get_allocation();	
	return true;
}
*/

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
	
//	wrefresh(stdscr);

}

void GTI (int signom)
{
	GameImpuls+=1;
}

