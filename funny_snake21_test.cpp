/*
|*****************************************************************
|*****************************************************************
* Project                        :  <Funny Snake #2>

* Programm name                  :  funny_snake_2.cpp

* Author                         :  Shults1981

* Data create                    :  01/03/2018

* Purpose                        :  classical game "SNAKE";
				    testing library ncurses #2

|****************************************************************
|****************************************************************
*/
#include <iostream>



//-----------------------------
#include "unit.h"
#include "game.h"
#include "ncurses.h"







using namespace std;
	
//============================= MAIN ======================================
		
int main (int argc, char** argv)
{	
	
cout<<"Funny snake v2"<<endl;

//#define UNIT_TEST_PointArr
//#define UNIT_TEST_UNIT
//#define UNIT_TEST_SNAKE
#define UNIT_TEST_GAME

//------ test for class PoinArr---------------------------------------
#ifdef UNIT_TEST_PointArr
 
cout<<"============================================="<<endl;
Point bufVar={0,0,0};
Point p1={1,2,3};
Point p2={4,5,6};
Point p3={7,8,9};
PointArr pa1(2);
PointArr pa3(1);


cout<<pa1.getLen()<<endl;
cout<<"+++++++++++++++test constructor +++++++++++++"<<endl;
for (int i=0;i<pa1.getLen();i++){
	pa1.getElement(i,bufVar);
	cout<<bufVar._x<<"--"<<bufVar._y<<"--"<<bufVar._d<<"--"<<endl;
}

cout<<"+++++++++++ test insert in back and in begin+++++++++++++++++++++++"<<endl;
pa1.addElementInBegin(p1);
pa1.addElementInBack(p2);
for (int i=0;i<pa1.getLen();i++){
	pa1.getElement(i,bufVar);
	cout<<bufVar._x<<"--"<<bufVar._y<<"--"<<bufVar._d<<"--"<<endl;
}

cout<<"++++++++++++ test copy constructor  +++++++++++++++++++++++"<<endl;
PointArr pa2(pa1);
for (int i=0;i<pa2.getLen();i++){
	pa2.getElement(i,bufVar);
	cout<<bufVar._x<<"--"<<bufVar._y<<"--"<<bufVar._d<<"--"<<endl;
}

cout<<"+++++++++++++test operator= ++++++++++++++++++"<<endl;
pa3=pa1;
for (int i=0;i<pa3.getLen();i++){
	pa3.getElement(i,bufVar);
	cout<<bufVar._x<<"--"<<bufVar._y<<"--"<<bufVar._d<<"--"<<endl;
}

cout<<"+++++++++++ test  del from back and  begin+++++++++++++++++++++++"<<endl;
pa1.delElementFromBegin();
pa1.delElementFromBack();
for (int i=0;i<pa1.getLen();i++){
	pa1.getElement(i,bufVar);
	cout<<bufVar._x<<"--"<<bufVar._y<<"--"<<bufVar._d<<"--"<<endl;
}

cout<<"+++++++++++ test set element[i]    ++++++++++++++++++++++"<<endl;
pa1.setElement(p3,1);
for (int i=0;i<pa1.getLen();i++){
	pa1.getElement(i,bufVar);
	cout<<bufVar._x<<"--"<<bufVar._y<<"--"<<bufVar._d<<"--"<<endl;
}

cout<<"+++++++++++ test del element i]    ++++++++++++++++++++++"<<endl;
pa1.delElement(0);
for (int i=0;i<pa1.getLen();i++){
	pa1.getElement(i,bufVar);
	cout<<bufVar._x<<"--"<<bufVar._y<<"--"<<bufVar._d<<"--"<<endl;
}


cout<<"+++++++++++++ insert element after index ++++++++++++++++++"<<endl;
pa3.insertElementAfterIndex(3,p3);
for (int i=0;i<pa3.getLen();i++){
	pa3.getElement(i,bufVar);
	cout<<bufVar._x<<"--"<<bufVar._y<<"--"<<bufVar._d<<"--"<<endl;
}

PointArr pa4(0);
cout<<"+++++++++++++ creaete instance this leight=0  ++++++++++++++++++"<<endl;
//for (int i=0;i<pa4.getLen();i++){
	pa4.getElement(0,bufVar);
	cout<<bufVar._x<<"--"<<bufVar._y<<"--"<<bufVar._d<<"--"<<pa4.getLen()<<endl;
//}


#endif
//---------------------end of test--------------------------------------


// --------------------test for class Unit ---------------------------
#ifdef UNIT_TEST_UNIT

cout<<"=====================test for class Unit====================="<<endl;
Point bufVar2={0,0,0};
Point p1={1,1,1};
Point p2={2,2,2};
Point p3={3,3,3};
Point p4={4,4,4};

Unit u1(1);
Unit *pu1;

pu1=new Unit(2);


cout<<u1.getBodyLen()<<endl;
cout<<u1.getBodyTPANum()<<endl;
cout<<"_______"<<endl;
cout<<pu1->getBodyLen()<<endl;
cout<<pu1->getBodyTPANum()<<endl;

cout<<"+++++++++++++ insert element after index ++++++++++++++++++"<<endl;
u1.addNewElementInUnitBody(p1);
u1.addNewElementInBodyTPA(p2);
for (int i=0;i<u1.getBodyLen();i++){
	u1.getBodyCords(i,bufVar2);
	cout<<bufVar2._x<<"--"<<bufVar2._y<<"--"<<bufVar2._d<<"--"<<endl;
}

for (int i=0;i<u1.getBodyTPANum();i++){
	u1.getBodyTPA(i,bufVar2);
	cout<<bufVar2._x<<"--"<<bufVar2._y<<"--"<<bufVar2._d<<"--"<<endl;
}

delete pu1;
cout<<"=====================end of class Unit test======================"<<endl;
#endif
//------------------------- of test -------------------------------




//---------------------test for class Snake ----------------------------------
#ifdef UNIT_TEST_SNAKE
cout<<"========================test for class Snake================="<<endl;
{
BaseUnitFace *bu1;
Point p11={5,5,5};
Point p12={2,2,2};
Point p13={3,3,3};
Point bufVar12={9,9,9};
Unit un1(1);
Snake sn1(p12);
Rabbit rb1(p13);

cout<<"..........................................."<<endl;

bu1=&un1;
cout<<"--"<<bu1->addNewElementInUnitBody(p11)<<endl;
cout<<"--"<<bu1->getBodyLen()<<endl;
cout<<"--"<<bu1->getBodyCords(1,bufVar12)<<endl;
cout<<"--"<<bufVar12._x<<"--"<<bufVar12._y<<"--"<<bufVar12._d<<endl;
bu1=&sn1;
cout<<"--"<<bu1->addNewElementInUnitBody(p11)<<endl;
cout<<"--"<<bu1->getBodyLen()<<endl;
cout<<"--"<<bu1->getBodyCords(1,bufVar12)<<endl;
cout<<"--"<<bufVar12._x<<"--"<<bufVar12._y<<"--"<<bufVar12._d<<endl;
bu1=&rb1;
cout<<"--"<<bu1->addNewElementInUnitBody(p11)<<endl;
cout<<"--"<<bu1->getBodyLen()<<endl;
cout<<"--"<<bu1->getBodyCords(1,bufVar12)<<endl;
cout<<"--"<<bufVar12._x<<"--"<<bufVar12._y<<"--"<<bufVar12._d<<endl;

}
cout<<"===========================end of class Snake test============="<<endl;
#endif
//-------------------end of test -------------------------------------------------


//------------------------- test for class game --------------------------------
#ifdef UNIT_TEST_GAME
cout<<"================ test for class game=================================="<<endl;
int row_max=50,col_max=50;


Point p31={1,1,1};
Point p32={2,2,2};
Point p33={3,3,3};
Point p34={4,4,4};
Point tempPoint={9,9,9};
Fild gFild;
gFild.border_x_min=col_max-9*col_max/10;
gFild.border_x_max=col_max-2*col_max/10;
gFild.border_y_min=row_max-9*row_max/10;
gFild.border_y_max=row_max-2*row_max/10;



Game gameCntr(gFild);
gameCntr.setGameStatus(game_new);
gameCntr.newGameUnitsSet();

//cout<<"keys codes(l,r,u,d): "<<KEY_LEFT<<","<<KEY_RIGHT<<","<<KEY_UP<<","<<KEY_DOWN<<endl;


cout<<"GST-"<<gameCntr.getGameStatus()<<endl;
cout<<"Rabbit Len-"<<gameCntr.getRabbitLen()<<endl;
gameCntr.getRabbitPlace(tempPoint);
cout<<"rabbit place: x-"<<tempPoint._x<<" y-"<<tempPoint._y<<" d-"<<tempPoint._d<<endl;
cout<<"Snake Len-"<<gameCntr.getSnakeLen()<<endl;
for (int i=0;i<gameCntr.getSnakeLen();i++){
	if(gameCntr.getSnakeBodyPartsCords(i,tempPoint))
		cout<<"snake place:"<<i<<" x-"<<tempPoint._x<<" y-"<<tempPoint._y<<" d-"<<tempPoint._d<<endl;
}

gameCntr.setGameStatus(game_on);
cout<<"GST-"<<gameCntr.getGameStatus()<<endl;
gameCntr.SnakeControl(Left);
gameCntr.SnakeMoveToOneStep();
gameCntr.SnakeMoveToOneStep();
gameCntr.SnakeMoveToOneStep();
gameCntr.SnakeMoveToOneStep();
gameCntr.SnakeControl(Up);
gameCntr.SnakeMoveToOneStep();
gameCntr.SnakeMoveToOneStep();


for (int i=0;i<gameCntr.getSnakeLen();i++){
	if(gameCntr.getSnakeBodyPartsCords(i,tempPoint))
		cout<<"snake place:"<<i<<" x-"<<tempPoint._x<<" y-"<<tempPoint._y<<" d-"<<tempPoint._d<<endl;
}
cout<<"GST-"<<gameCntr.getGameStatus()<<endl;

cout <<"================= end of class game test============================="<<endl;
#endif
// -------------------------------------end of test ----------------------------

return 0;
}	



