#include "tableview.h"
#include "tablemodel.h"
#include "ViewIdentify.h"
#include "databasehandler.h"

#include "depmgrview.h"
#include "bankmgrview.h"
#include "budgetmgrview.h"
#include "detailtitlemgrview.h"
#include "titlemgrview.h"
#include "incomemgrview.h"
#include "crcheckmgrview.h"

#include "mainview.h"

#define TopGapSize	7			// Top gap size between Frame and Table

MainView::MainView(QWidget *parent)
	: QFrame(parent)
{
	ui.setupUi(this);
	ui.tabWidget->removeTab(0);

	connect(ui.tabWidget, SIGNAL(tabCloseRequested(int)), SLOT(tableCloseReq(int)));
}

MainView::~MainView()
{
	m_createdWidgetList.clear();
}

void MainView::addItem()
{
	TableView* activeView = qobject_cast<TableView*>(ui.tabWidget->currentWidget());
	activeView->addItem();
}

void MainView::editItem()
{
	TableView* activeView = qobject_cast<TableView*>(ui.tabWidget->currentWidget());
	activeView->editItem();
}

void MainView::delItem()
{
	TableView* activeView = qobject_cast<TableView*>(ui.tabWidget->currentWidget());
	activeView->delItem();
}

void MainView::resizeEvent(QResizeEvent* event)
{
	QRect rect = frameRect();
	rect.setSize(QSize(event->size().width(), event->size().height()));
	rect.setTop(rect.top() + TopGapSize);

	ui.tabWidget->setGeometry(rect);
}

void MainView::openWindow(int nWindowId)
{
	TableView* tabView = 0;
	
	for (int i = 0; i < m_createdWidgetList.count(); i++)
	{
		if (m_createdWidgetList.at(i)->getViewId() == nWindowId)
		{
			tabView = m_createdWidgetList.at(i);
			break;
		}
	}
	
	// specified view has not created, create it now
	if (tabView == 0)
	{
		switch (nWindowId)
		{
		case DepartmentManagerView:
			tabView = new DepMgrView(this);
			break;
		case BankAccountView:
			tabView = new BankMgrView(this);
			break;
		case BudgetView:
			tabView = new BudgetMgrView(this);
			break;
		case TitleView:
			tabView = new TitleMgrView(this);
			break;
		case DetailTitleView:
			tabView = new DetailTitleMgrView(this);
			break;
		case IncomeView:
			tabView = new IncomeMgrView(this);
			break;
		case CRCheckView:
			tabView = new CRCheckMgrView(this);
			break;
		case CashView:
			//tabView = new cahs
			break;
		default:
			break;
		}
		ui.tabWidget->addTab(tabView, tabView->getTabName());
		ui.tabWidget->setCurrentWidget(tabView);
		m_createdWidgetList.append(tabView);
		tabView->loadData();
	}
	else
	{
		// specified view has created, if it's in the tab widget, active it, else add in tab
		bool find = false;
		for(int i = 0; i < ui.tabWidget->count(); i++)
		{
			if (ui.tabWidget->widget(i) == tabView)
			{
				find = true;
				break;
			}
		}
		
		if (!find)
		{
			ui.tabWidget->addTab(tabView, tabView->getTabName());
		}
		ui.tabWidget->setCurrentWidget(tabView);
	}
}

void MainView::tableCloseReq(int nIndex)
{
	ui.tabWidget->removeTab(nIndex);
}