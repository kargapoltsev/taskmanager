#include "MainWindow.h"

#include <string>
#include <iostream>

#include <QMenuBar>
#include <QStatusBar>
#include <QLabel>
#include <QHeaderView>

#include "HierarchyTaskList.h"
#include "HierarchyTaskListModel.h"

#include "DataStore.h"
#include "Project.h"
#include "Task.h"

MainWindow::MainWindow()
    : m_pHierarchyTaskListModel( new HierarchyTaskListModel( this ) )
{

}

void MainWindow::initialize()
{
    auto pMenuFile = new QMenu( "&File", this );
//    pMenuFile->addAction( "&Save" );
    pMenuFile->addAction( "&Exit", this, &MainWindow::slotExit, QKeySequence("Esc") );
    QMainWindow::menuBar()->addMenu( pMenuFile );

    auto pMenuTask = new QMenu( "&Task", this );
    pMenuTask->addAction( "&Add new task", this, &MainWindow::slotAddNewTask );
    QMainWindow::menuBar()->addMenu(( pMenuTask ));

    auto pLabel = new QLabel( "Developming...", this );
    QMainWindow::statusBar()->addWidget( pLabel );

    m_pCurrentProject = m_pDataStore->getProject( "Free tasks" );

    m_pHierarchyTaskListModel->setProject( m_pCurrentProject );

    m_pTreeView = new QTreeView( this );
    m_pTreeView->setModel( m_pHierarchyTaskListModel );
    m_pTreeView->header()->moveSection( 1, 0 );
    m_pTreeView->expandAll();
    m_pTreeView->resizeColumnToContents( 0 );
    m_pTreeView->resizeColumnToContents( 1 );
    m_pTreeView->resizeColumnToContents( 2 );

    QMainWindow::setCentralWidget( m_pTreeView );

}

void MainWindow::slotExit()
{
    exit( 0 );
}

void MainWindow::slotAddNewTask()
{
//    std::cout << "Add new task" << std::endl;

//    m_pHierarchyTaskListModel->setData( );
}

DataStore * MainWindow::getTasksRepository() const
{
    return m_pDataStore;
}

void MainWindow::setTasksRepository(DataStore * pTasksRepository)
{
    m_pDataStore = pTasksRepository;
}
