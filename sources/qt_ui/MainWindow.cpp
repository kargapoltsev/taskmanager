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

    pMenuTask->addAction( "Add &new task", this,
                          &MainWindow::slotAddNewTask, QKeySequence( Qt::Key_Enter ) );

    pMenuTask->addAction( "&Remove task", this,
                          &MainWindow::slotRemoveTask, QKeySequence( Qt::Key_Delete ) );

    pMenuTask->addAction( "Add &child task", this,
                          &MainWindow::slotAddChildTask, QKeySequence( Qt::CTRL + Qt::Key_Enter ) );

    pMenuTask->addAction( "&Up task", this,
                          &MainWindow::slotUpTask, QKeySequence( Qt::CTRL + Qt::Key_Up ) );

    pMenuTask->addAction( "&Down task", this,
                          &MainWindow::slotDownTask, QKeySequence( Qt::CTRL + Qt::Key_Down ) );

    pMenuTask->addAction( "&Dive task", this,
                          &MainWindow::slotDiveTask, QKeySequence( Qt::CTRL + Qt::Key_Right ) );

    pMenuTask->addAction( "&Ascent task", this,
                          &MainWindow::slotAscentTask, QKeySequence( Qt::CTRL + Qt::Key_Left ) );

    pMenuTask->addAction( "&Complete", this,
                          &MainWindow::slotCompleteTask, QKeySequence( Qt::Key_Space ) );

    QMainWindow::menuBar()->addMenu(( pMenuTask ));

    QMainWindow::statusBar()->showMessage( tr( "Developming..." ) );

    m_pCurrentProject = m_pDataStore->getProject( "Free tasks" );

    m_pModel->setProject( m_pCurrentProject );

    m_pView = new QTreeView( this );
    m_pView->setModel( m_pModel );
    m_pView->header()->moveSection( 1, 0 );
    m_pView->header()->moveSection( 2, 1 );

    QMainWindow::setCentralWidget( m_pView );

    update();
}

void MainWindow::slotExit()
{
    exit( 0 );
}

void MainWindow::slotAddNewTask()
{
    const auto &index = m_pView->selectionModel()->currentIndex();

    if ( index.isValid() )
    {
        if ( m_pModel->insertRow( index.row() + 1, index.parent() ) )
            m_pModel->updateComplete( index );
    }
    else
    {
        if ( m_pModel->insertRow( 0, QModelIndex() ) )
            m_pView->setCurrentIndex( m_pModel->index( 0, 0, QModelIndex() ) );
    }

    update();
}

void MainWindow::slotRemoveTask()
{
    const auto index = m_pView->selectionModel()->currentIndex();

    if ( !index.isValid() )
        return;

    if ( m_pModel->removeRow( index.row(), index.parent() ) )
    {
        const auto &parentIndex = index.parent();
        m_pModel->updateComplete( parentIndex );
        update();
    }

}

void MainWindow::slotAddChildTask()
{
    const auto index = m_pView->selectionModel()->currentIndex();

    if ( m_pModel->insertRow( 0, index ) )
    {
        m_pModel->updateComplete( index );
        update();
    }
}

void MainWindow::slotUpTask()
{
    const auto currentIndex = m_pView->selectionModel()->currentIndex();

    if ( !currentIndex.isValid() )
        return;

    const auto parentIndex = currentIndex.parent();

    const auto nCurrentPosition = currentIndex.row();

    if ( nCurrentPosition == 0 )
    {
        auto nParentPosition = parentIndex.row();

        auto aboveTaskIndex = m_pModel->index( nParentPosition - 1, 0, parentIndex.parent() );
        if ( !aboveTaskIndex.isValid() )
            return;

        if ( const auto nRowCount = m_pModel->rowCount( aboveTaskIndex );
             m_pModel->moveRow( parentIndex, nCurrentPosition, aboveTaskIndex, nRowCount ) )
        {
            m_pView->setCurrentIndex( m_pModel->index( nRowCount, 0, aboveTaskIndex ) );
            update();
        }
    }
    else
    {

        if ( m_pModel->moveRow( parentIndex, nCurrentPosition, parentIndex, nCurrentPosition - 1 ) )
            update();
    }
}

void MainWindow::slotDownTask()
{
    const auto currentIndex = m_pView->selectionModel()->currentIndex();

    if ( !currentIndex.isValid() )
        return;

    const auto nCurrentPosition = currentIndex.row();
    auto parentIndex = currentIndex.parent();

    if ( nCurrentPosition == m_pModel->rowCount( parentIndex ) - 1 )
    {
        auto nParentPosition = parentIndex.row();

        if ( nParentPosition == -1 )
            return;

        auto undexTaskIndex = m_pModel->index( nParentPosition + 1, 0, parentIndex.parent() );
        if ( !undexTaskIndex.isValid() )
            return;

        if ( m_pModel->moveRow( parentIndex, nCurrentPosition, undexTaskIndex, 1 ) )
        {
            m_pView->setCurrentIndex( m_pModel->index( 0, 0, undexTaskIndex ) );
            update();
        }
    }
    else
    {
        const auto nNewPosition = nCurrentPosition + 1;

        if ( m_pModel->moveRow( parentIndex, nCurrentPosition, parentIndex, nNewPosition ) )
        {
            m_pView->setCurrentIndex( m_pModel->index( nNewPosition, 0, parentIndex ) );
            update();
        }
    }
}

void MainWindow::slotDiveTask()
{
    const auto currentIndex = m_pView->selectionModel()->currentIndex();

    if ( !currentIndex.isValid() )
        return;

    const auto parentIndex = currentIndex.parent();

    const auto nCurrentPosition = currentIndex.row();

    auto aboveTaskIndex = m_pModel->index( nCurrentPosition - 1, 0, parentIndex );
    if ( !aboveTaskIndex.isValid() )
        return;

    if ( m_pModel->moveRow( parentIndex, nCurrentPosition,
                            aboveTaskIndex, m_pModel->rowCount( aboveTaskIndex ) ) )
        update();
}

void MainWindow::slotAscentTask()
{
    const auto currentIndex = m_pView->selectionModel()->currentIndex();

    if ( !currentIndex.isValid() )
        return;

    const auto parentIndex = currentIndex.parent();

    const auto nCurrentPosition = currentIndex.row();

    auto nParentPosition = parentIndex.row();

    if ( nParentPosition == -1 )
        return;

    const auto grandParentIndex = parentIndex.parent();

    if ( m_pModel->moveRow( parentIndex, nCurrentPosition, grandParentIndex, 0 ) )
    {
        m_pView->setCurrentIndex(
                    m_pModel->index( m_pModel->rowCount( grandParentIndex ) - 1,
                    0, grandParentIndex ) );
        update();
    }
}

void MainWindow::slotCompleteTask()
{
    const auto currentIndex = m_pView->selectionModel()->currentIndex();
    if ( !currentIndex.isValid() )
        return;

    m_pModel->setComplete( currentIndex );
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

    QMainWindow::statusBar()->showMessage( QString( tr("Project: %1 %2% complete") )
    .arg( m_pCurrentProject->getStrName().c_str() ).arg( m_pCurrentProject->getRootTask()->getComplete() ) );
}
