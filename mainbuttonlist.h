#ifndef MAINBUTTONLIST_H
#define MAINBUTTONLIST_H

#include <QtGui>
#include "ui_mainbuttonlist.h"

class MainButtonList : public QFrame
{
	Q_OBJECT

public:
	MainButtonList(QWidget *parent = 0);
	~MainButtonList();

private:
	Ui::MainButtonListClass ui;
};

#endif // MAINBUTTONLIST_H
