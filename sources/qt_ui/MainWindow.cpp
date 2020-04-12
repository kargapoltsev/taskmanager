#include "MainWindow.h"

#include <string>
#include <iostream>

#include <QMenuBar>
#include <QStatusBar>
#include <QLabel>
#include <QHeaderView>

#include "HierarchyTaskList.h"

#include "DataStore.h"

MainWindow::MainWindow()
    : m_pHierarchyTaskList( new HierarchyTaskList( this ) )
{

}

void MainWindow::initialize()
{
    createActions();
    createMenus();
    createStatusBar();

    m_pCurrentProject = m_pDataStore->getProject( "Free tasks" );
    m_pHierarchyTaskList->setProject( m_pCurrentProject );

    QMainWindow::setCentralWidget( m_pHierarchyTaskList );
}


DataStore * MainWindow::getDataStore() const
{
    return m_pDataStore;
}

void MainWindow::setDataStore(DataStore * pDataStore)
{
    m_pDataStore = pDataStore;
}

void MainWindow::update()
{


}

void MainWindow::createActions()
{
    m_pExitAction = new QAction( tr("Exit"), this );
    m_pExitAction->setShortcut( QKeySequence( Qt::Key_Escape ) );
    connect( m_pExitAction, &QAction::triggered, this, &QMainWindow::close );

    m_pAddNewTaskAction = new QAction( tr( "Add new task" ), this );
    m_pAddNewTaskAction->setShortcut( QKeySequence( Qt::Key_Enter ) );
    connect( m_pAddNewTaskAction, &QAction::triggered,
             m_pHierarchyTaskList, &HierarchyTaskList::slotAddNewTask );

    m_pRemoveTaskAction = new QAction( tr( "Remove task" ), this );
    m_pRemoveTaskAction->setShortcut( QKeySequence( Qt::Key_Delete ) );
    connect( m_pRemoveTaskAction, &QAction::triggered,
             m_pHierarchyTaskList, &HierarchyTaskList::slotRemoveTask );

    m_pAddChildTaskAction = new QAction( tr( "Add child task" ), this );
    m_pAddChildTaskAction->setShortcut( QKeySequence( Qt::CTRL + Qt::Key_Enter ) );
    connect( m_pAddChildTaskAction, &QAction::triggered,
             m_pHierarchyTaskList, &HierarchyTaskList::slotAddChildTask );

    m_pUpTaskPositionAction = new QAction( tr( "Up position" ), this );
    m_pUpTaskPositionAction->setShortcut( QKeySequence( Qt::CTRL + Qt::Key_Up ) );
    connect( m_pUpTaskPositionAction, &QAction::triggered,
             m_pHierarchyTaskList, &HierarchyTaskList::slotUpTaskPosition );

    m_pDownTaskPositionAction = new QAction( tr( "Down position" ), this );
    m_pDownTaskPositionAction->setShortcut( QKeySequence( Qt::CTRL + Qt::Key_Down ) );
    connect( m_pDownTaskPositionAction, &QAction::triggered,
             m_pHierarchyTaskList, &HierarchyTaskList::slotDownTaskPosition );

    m_pDiveTaskAction = new QAction( tr( "Dive task" ), this );
    m_pDiveTaskAction->setShortcut( QKeySequence( Qt::CTRL + Qt::Key_Right ) );
    connect( m_pDiveTaskAction, &QAction::triggered,
             m_pHierarchyTaskList, &HierarchyTaskList::slotDiveTask );

    m_pAscentTaskAction = new QAction( tr("Ascent task"), this );
    m_pAscentTaskAction->setShortcut( QKeySequence( Qt::CTRL + Qt::Key_Left ) );
    connect( m_pAscentTaskAction, &QAction::triggered,
             m_pHierarchyTaskList, &HierarchyTaskList::slotAscentTask );

    m_pCompleteTaskAction = new QAction( tr( "Complete" ), this );
    m_pCompleteTaskAction->setShortcut( QKeySequence( Qt::Key_Space ) );
    connect( m_pCompleteTaskAction, &QAction::triggered,
             m_pHierarchyTaskList, &HierarchyTaskList::slotCompleteTask );

    m_pUpTaskPriorityAction = new QAction( tr("Up priority"), this );
    m_pUpTaskPriorityAction->setShortcut( QKeySequence( Qt::CTRL + Qt::SHIFT + Qt::Key_Up ) );
    connect( m_pUpTaskPriorityAction, &QAction::triggered,
             m_pHierarchyTaskList, &HierarchyTaskList::slotUpTaskPriority );

    m_pDownTaskPriorityAction = new QAction( tr("Down priority"), this );
    m_pDownTaskPriorityAction->setShortcut( QKeySequence( Qt::CTRL + Qt::SHIFT + Qt::Key_Down ) );
    connect( m_pDownTaskPriorityAction, &QAction::triggered,
             m_pHierarchyTaskList, &HierarchyTaskList::slotDownTaskPriority );
}

void MainWindow::createMenus()
{
    auto m_pFileMenu = new QMenu( "&File", this );
    m_pFileMenu->addAction( m_pExitAction );

    QMainWindow::menuBar()->addMenu( m_pFileMenu );


    auto m_pTaskMenu = new QMenu( "&Task", this );
    m_pTaskMenu->addAction( m_pAddNewTaskAction );
    m_pTaskMenu->addAction( m_pRemoveTaskAction );
    m_pTaskMenu->addAction( m_pAddChildTaskAction );
    m_pTaskMenu->addAction( m_pUpTaskPositionAction );
    m_pTaskMenu->addAction( m_pDownTaskPositionAction );
    m_pTaskMenu->addAction( m_pDiveTaskAction );
    m_pTaskMenu->addAction( m_pAscentTaskAction );
    m_pTaskMenu->addAction( m_pCompleteTaskAction );
    m_pTaskMenu->addAction( m_pUpTaskPriorityAction );
    m_pTaskMenu->addAction( m_pDownTaskPriorityAction );

    QMainWindow::menuBar()->addMenu( m_pTaskMenu );
}

void MainWindow::createStatusBar()
{
    connect( m_pHierarchyTaskList, &HierarchyTaskList::signalProjectUpdated,
             this, &MainWindow::setStatusbarMessage );
}

void MainWindow::setStatusbarMessage(const QString & strMessage)
{
    QMainWindow::statusBar()->showMessage( strMessage );
}
