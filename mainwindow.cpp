//++++++++++++++++++++++
//++++++++++++++++++++++
//   mainwindow.cpp
//+++++++++++++++++++++
//+++++++++++++++++++++

#include <gtkmm.h>
#include <gtkmm/drawingarea.h>
#include <cairomm/context.h>

//-----------------------------
#include "mainwindow.h"
#include "game.h"
#include "unit.h"

//-----------------------------------------------------
//-----------------------------------------------------
//-----------------------------------------------------
//-----------------methods of class MyArea -----------

MyArea::MyArea():unit_snake(0),unit_rabbit(0)
{
	PST= game_stop;
	Level=0;
	Score=0;

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

	char str_BUF1[5],str_BUF2[5],str_BUF3[5];
	char buf1[2]={'0',0x00};

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


	if (PST==game_on)
	{

		cr->set_line_width(1.5);
		if (unit_rabbit.getElement(0,pen)){
				cr->rectangle(	scr_border_x_min+pen._x*hStep,
				scr_border_y_min+pen._y*vStep,
				hStep,
				vStep);
		}

		for(i=0;i<unit_snake.getLen();i++ )
		{
			if (unit_snake.getLen()){
				unit_snake.getElement(i,pen);
				cr->rectangle(	scr_border_x_min+pen._x*hStep,
						scr_border_y_min+pen._y*vStep,
						hStep,
						vStep);	
			}		
		}

		//====  information ====
		sprintf (str_BUF1,"%d",Score);
		cr->move_to(scr_border_x_min,scr_border_y_max+15);
		cr->show_text("Score-");
		cr->move_to(scr_border_x_min+60,scr_border_y_max+15);
		cr->show_text(str_BUF1);

		sprintf (str_BUF2,"%d",Level);
		cr->move_to(scr_border_x_min,scr_border_y_max+25);
		cr->show_text("Level-");
		cr->move_to(scr_border_x_min+60,scr_border_y_max+25);
		cr->show_text(str_BUF2);
	}

	if (PST==game_new_level)
	{
		cr->move_to(scr_border_x_max/2-30,scr_border_y_max/2-20);
		cr->show_text("N E X T    L E V E L !!!!!");
	}

	if (PST==game_over)
	{
		cr->move_to(scr_border_x_max/2-30,scr_border_y_max/2-20);
		cr->show_text("G A M E   O V E R !!!!!");
	}

	cr->fill();

	return false;

}
//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------
//----------------- methods of class MainWindow ---------------


MainWindow::MainWindow()
{
	PST=game_stop;

	TimeBase=200;
	LevelTimeStep=6;
	GamePause=5;
	delay_cnt=0;

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
	
	timerSource=Glib::signal_timeout().connect( sigc::mem_fun(*this, &MainWindow::Tic), TimeBase );

	area.show();		

}

MainWindow::~MainWindow()
{

	delete GameController;

}

bool MainWindow::on_key_press_event(GdkEventKey* key_event)
{

	if (key_event->keyval==GDK_KEY_m){
		PST=game_stop;
	}

	if (PST==game_stop||PST==game_over){
		switch (key_event->keyval)  {
			case GDK_KEY_e:
			case GDK_KEY_E:
				PST=game_exit;
				break;
			case GDK_KEY_n:
			case GDK_KEY_N:
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
	//				g_print("Pressed key -Left key-\n");					
					break;			
			case GDK_KEY_Right:
					mvf=Right;
	//				g_print("Pressed key -Right key-\n");					
					break;
			case GDK_KEY_Up:
					mvf=Up;
	//				g_print("Pressed key -Up key-\n");					
					break;
			case GDK_KEY_Down:
					mvf=Down;
	//				g_print("Pressed key -Down key-\n");					
					break; 
			default : break;
			}
	}

	return true;
}

void MainWindow::OnQuit()
{
	hide();
}


bool MainWindow::Tic()
{
	Main_Loop();
	_PreRender();	
	
	return true;
}

bool MainWindow::Main_Loop()
{
	switch (PST){
		case game_exit:
			GameController->setGameStatus(game_over);
			GameController->setGameStatus(game_exit);
			timerSource.disconnect();
			OnQuit();
			break;
		case game_new:
			GameController->setGameStatus(game_stop);
			GameController->setGameStatus(game_over);
//			GameController->setGameStatus(game_new);
			mvf=static_cast<MoveDirection>(0);
//			delay_cnt=0;
			if ((delay_cnt)==1){
				GameController->setGameStatus(game_new);
				GameController->setGameStatus(game_on);
				PST=game_on;
				delay_cnt=0;
			}
			else
				delay_cnt++;
			timerSource.disconnect();
			timerSource=Glib::signal_timeout().connect( sigc::mem_fun(*this, &MainWindow::Tic), TimeBase );
			GTC.ResetCount();
			break;
		case game_stop:
			GameController->setGameStatus(game_stop);
			delay_cnt=0;
			break;
		case game_on:

			if(GameController->getGameStatus()!=game_over){
				GameController->setGameStatus(game_on);
			}
			if(GameController->getGameStatus()==game_over)	
				PST=game_over;

			break;
		case game_over:
			if ((delay_cnt++)>GamePause)
				PST=game_stop;
			break;

		default:
			break;
	}
	
		if (GameController->getGameStatus()==game_on)
		{
				GameController->SnakeControl(mvf);
				GameController->SnakeMoveToOneStep();
				GTC.CountUp();
		}

		if (GameController->getGameStatus()==game_new_level){
			PST=game_new_level;
			if ((delay_cnt++)>GamePause){

				GameController->setGameStatus(game_stop);
				GameController->setGameStatus(game_over);
				GameController->setGameStatus(game_new);
				GameController->setGameStatus(game_on);
				mvf=static_cast<MoveDirection>(0);
				delay_cnt=0;
				PST=game_on;
				timerSource.disconnect();
				timerSource=Glib::signal_timeout().connect( sigc::mem_fun(*this, &MainWindow::Tic), 
									   TimeBase-(GameController->getGameLevel()-1)*LevelTimeStep);
			}
		}

	g_print("Game step! PST-%d\n",PST);					

	return true;
}



void MainWindow::_PreRender()
{
	int i;
	Point tp1;

	area.PST=PST;
	area.btmr=GTC.getGameTime();
	if (PST==game_over||PST==game_new||PST==game_new_level){

		for (i=0;i<area.unit_snake.getLen();i++){
				if (area.unit_snake.getLen())
					area.unit_snake.delElementFromBack();
			}

		if (area.unit_rabbit.getLen())
			area.unit_rabbit.delElementFromBack();

		g_print("---clear frame----\n");
	}

	if(PST==game_on)
	{
		area.Score=GameController->getGameScore();
		area.Level=GameController->getGameLevel();

		GameController->getRabbitPlace(tp1);
		if (!area.unit_rabbit.getLen())
			area.unit_rabbit.addElementInBack(tp1);
		else
			area.unit_rabbit.setElement(tp1,0);
	
	
		if ((GameController->getSnakeLen()-area.unit_snake.getLen())){ // !! may be not use operator- "=="
			GameController->getSnakeBodyPartsCords(GameController->getSnakeLen()-1,tp1);
//			if (area.unit_snake.getLen()){
				area.unit_snake.addElementInBack(tp1);
				g_print("---add in frameback---\n");
//			}
		}
//		else {			
		if (!(GameController->getSnakeLen()-area.unit_snake.getLen())){// !! may be use only- "!"
			if (area.unit_snake.getLen()){
				for (i=0;i<GameController->getSnakeLen();i++){
					GameController->getSnakeBodyPartsCords(i,tp1);
					area.unit_snake.setElement(tp1,i);
				}
				g_print("----copy frame----\n");
			}		
		}
	}



	area.queue_draw();
}

//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------
//------------- methods of class  GameTimeCounter---------------

GameTimeCounter::GameTimeCounter()
{
	GameTime.sec=0;
	GameTime.min=0;
	GameTime.hour=0;
}

GameTimeCounter::~GameTimeCounter()
{

}

TimeData GameTimeCounter::getGameTime()
{

	return GameTime;
}	

bool GameTimeCounter::CountUp()
{
	GameTime.sec++;
	if(GameTime.sec==60){
		GameTime.min++;
		GameTime.sec=0;
	}
	if(GameTime.min==60){
		GameTime.hour++;
		GameTime.min=0;
	}
	if(GameTime.hour==24) 
		GameTime.hour=0;
	return true;
}

bool GameTimeCounter::CountDown()
{

	return true;
}

bool GameTimeCounter::ResetCount()
{
	GameTime.sec=0;
	GameTime.min=0;
	GameTime.hour=0;

	return true;
}















