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
    : m_pModel( new HierarchyTaskListModel( this ) )
{

}

void MainWindow::initialize()
{
    auto pMenuFile = new QMenu( "&File", this );
//    pMenuFile->addAction( "&Save" );
    pMenuFile->addAction( "E&xit", this, &MainWindow::slotExit, QKeySequence("Esc") );
    QMainWindow::menuBar()->addMenu( pMenuFile );

    auto pMenuTask = new QMenu( "&Task", this );
    pMenuTask->addAction( "Add &new task", this, &MainWindow::slotAddNewTask, QKeySequence("Enter") );
    pMenuTask->addAction( "&Remove task", this, &MainWindow::slotRemoveTask, QKeySequence("Delete") );
    pMenuTask->addAction( "Add &child task", this, &MainWindow::slotAddChildTask, QKeySequence("Ctrl + Enter") );
    QMainWindow::menuBar()->addMenu(( pMenuTask ));

    auto pLabel = new QLabel( "Developming...", this );
    QMainWindow::statusBar()->addWidget( pLabel );

    m_pCurrentProject = m_pDataStore->getProject( "Free tasks" );

    m_pModel->setProject( m_pCurrentProject );

    m_pView = new QTreeView( this );
    m_pView->setModel( m_pModel );
    m_pView->header()->moveSection( 1, 0 );

    QMainWindow::setCentralWidget( m_pView );

    update();
}

void MainWindow::slotExit()
{
    exit( 0 );
}

void MainWindow::slotAddNewTask()
{
    const auto index = m_pView->selectionModel()->currentIndex();

    if ( m_pModel->insertRow( index.row() + 1, index.parent() ) )
        update();
}

void MainWindow::slotRemoveTask()
{
    const auto index = m_pView->selectionModel()->currentIndex();

    if ( m_pModel->removeRow( index.row(), index.parent() ) )
        update();
}

void MainWindow::slotAddChildTask()
{
    const auto index = m_pView->selectionModel()->currentIndex();

    if ( m_pModel->insertRow( 0, index ) )
        update();
}

DataStore * MainWindow::getTasksRepository() const
{
    return m_pDataStore;
}

void MainWindow::setTasksRepository(DataStore * pTasksRepository)
{
    m_pDataStore = pTasksRepository;
}

void MainWindow::update()
{
    m_pView->expandAll();
    m_pView->resizeColumnToContents( 0 );
    m_pView->resizeColumnToContents( 1 );
    m_pView->resizeColumnToContents( 2 );
}
