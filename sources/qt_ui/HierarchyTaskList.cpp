#include "HierarchyTaskList.h"

#include <QtWidgets>
#include "Task.h"
#include "Project.h"
#include "HierarchyTaskListModel.h"

HierarchyTaskList::HierarchyTaskList( QWidget* pParent )
    : QTreeView( pParent )
      , m_pModel( new HierarchyTaskListModel( this ))
{
    initialize();
}

void HierarchyTaskList::initialize()
{
    QTreeView::setModel( m_pModel );
    QTreeView::header()->moveSection( 1, 0 );
    QTreeView::header()->moveSection( 2, 1 );

    QObject::connect( selectionModel(), &QItemSelectionModel::selectionChanged,
        this, &HierarchyTaskList::slotHandleSelectedTask );
}

void HierarchyTaskList::setProject( Project* pProject )
{
    if ( pProject != nullptr )
    {
        m_pProject = pProject;
        m_pModel->setProject( m_pProject );

        auto index = m_pModel->index( 0, 0, QModelIndex());
        QTreeView::setCurrentIndex( index );

        emit selectedTaskChanged( m_pModel->getTaskFromIndex( index ));

        update();
    }
    else
    {
        m_pProject = nullptr;
        m_pModel->setProject( nullptr );
        emit selectedTaskChanged( nullptr );

        emit signalProjectUpdated( tr( "No project has selected" ));

        QTreeView::reset();
    }
}

void HierarchyTaskList::slotAddNewTask()
{
    const auto& index = QTreeView::selectionModel()->currentIndex();

    if ( index.isValid())
    {
        if ( m_pModel->insertRow( index.row() + 1, index.parent()))
            m_pModel->updateComplete( index );
    }
    else
    {
        if ( m_pModel->insertRow( 0, QModelIndex()))
            QTreeView::setCurrentIndex( m_pModel->index( 0, 0, QModelIndex()));
    }

    update();
}

void HierarchyTaskList::slotRemoveTask()
{
    const auto index = QTreeView::selectionModel()->currentIndex();

    if ( !index.isValid())
        return;

    if ( m_pModel->removeRow( index.row(), index.parent()))
    {
        const auto& parentIndex = index.parent();
        m_pModel->updateComplete( parentIndex );
        update();
    }
}

void HierarchyTaskList::slotAddChildTask()
{
    const auto index = QTreeView::selectionModel()->currentIndex();

    if ( m_pModel->insertRow( 0, index ))
    {
        m_pModel->updateComplete( index );
        update();
    }
}

void HierarchyTaskList::slotUpTaskPosition()
{
    const auto currentIndex = QTreeView::selectionModel()->currentIndex();

    if ( !currentIndex.isValid())
        return;

    const auto parentIndex = currentIndex.parent();

    const auto nCurrentPosition = currentIndex.row();

    if ( nCurrentPosition == 0 )
    {
        auto nParentPosition = parentIndex.row();

        auto aboveTaskIndex = m_pModel->index( nParentPosition - 1, 0, parentIndex.parent());
        if ( !aboveTaskIndex.isValid())
            return;

        if ( const auto nRowCount = m_pModel->rowCount( aboveTaskIndex );
            m_pModel->moveRow( parentIndex, nCurrentPosition, aboveTaskIndex, nRowCount ))
        {
            QTreeView::setCurrentIndex( m_pModel->index( nRowCount, 0, aboveTaskIndex ));
            update();
        }
    }
    else
    {
        if ( m_pModel->moveRow( parentIndex, nCurrentPosition, parentIndex, nCurrentPosition - 1 ))
            update();
    }
}

void HierarchyTaskList::slotDownTaskPosition()
{
    const auto currentIndex = QTreeView::selectionModel()->currentIndex();

    if ( !currentIndex.isValid())
        return;

    const auto nCurrentPosition = currentIndex.row();
    auto parentIndex = currentIndex.parent();

    if ( nCurrentPosition == m_pModel->rowCount( parentIndex ) - 1 )
    {
        auto nParentPosition = parentIndex.row();

        if ( nParentPosition == -1 )
            return;

        auto undexTaskIndex = m_pModel->index( nParentPosition + 1, 0, parentIndex.parent());
        if ( !undexTaskIndex.isValid())
            return;

        if ( m_pModel->moveRow( parentIndex, nCurrentPosition, undexTaskIndex, 1 ))
        {
            QTreeView::setCurrentIndex( m_pModel->index( 0, 0, undexTaskIndex ));
            update();
        }
    }
    else
    {
        const auto nNewPosition = nCurrentPosition + 1;

        if ( m_pModel->moveRow( parentIndex, nCurrentPosition, parentIndex, nNewPosition ))
        {
            QTreeView::setCurrentIndex( m_pModel->index( nNewPosition, 0, parentIndex ));
            update();
        }
    }
}

void HierarchyTaskList::slotDiveTask()
{
    const auto currentIndex = QTreeView::selectionModel()->currentIndex();

    if ( !currentIndex.isValid())
        return;

    const auto parentIndex = currentIndex.parent();

    const auto nCurrentPosition = currentIndex.row();

    auto aboveTaskIndex = m_pModel->index( nCurrentPosition - 1, 0, parentIndex );
    if ( !aboveTaskIndex.isValid())
        return;

    if ( m_pModel->moveRow( parentIndex, nCurrentPosition,
        aboveTaskIndex, m_pModel->rowCount( aboveTaskIndex )))
        update();
}

void HierarchyTaskList::slotAscentTask()
{
    const auto currentIndex = QTreeView::selectionModel()->currentIndex();

    if ( !currentIndex.isValid())
        return;

    const auto parentIndex = currentIndex.parent();

    const auto nCurrentPosition = currentIndex.row();

    auto nParentPosition = parentIndex.row();

    if ( nParentPosition == -1 )
        return;

    const auto grandParentIndex = parentIndex.parent();

    if ( m_pModel->moveRow( parentIndex, nCurrentPosition, grandParentIndex, 0 ))
    {
        QTreeView::setCurrentIndex(
            m_pModel->index( m_pModel->rowCount( grandParentIndex ) - 1,
                0, grandParentIndex ));
        update();
    }
}

void HierarchyTaskList::slotCompleteTask()
{
    const auto currentIndex = QTreeView::selectionModel()->currentIndex();
    if ( !currentIndex.isValid())
        return;

    m_pModel->setComplete( currentIndex );
    update();
}

void HierarchyTaskList::slotUpTaskPriority()
{
    const auto currentIndex = QTreeView::selectionModel()->currentIndex();
    if ( !currentIndex.isValid())
        return;

    m_pModel->changePriority( currentIndex, true );
    update();
}

void HierarchyTaskList::slotDownTaskPriority()
{
    const auto currentIndex = QTreeView::selectionModel()->currentIndex();
    if ( !currentIndex.isValid())
        return;

    m_pModel->changePriority( currentIndex, false );
    update();
}

void HierarchyTaskList::slotHandleSelectedTask( const QItemSelection& selected, const QItemSelection& )
{
    if ( selected.isEmpty())
        return;

    auto index = selected.indexes().first();

    if ( !index.isValid())
        return;

    auto pTask = m_pModel->getTaskFromIndex( index );
    if ( pTask == nullptr )
        return;

    emit selectedTaskChanged( pTask );
}

void HierarchyTaskList::update()
{
    QTreeView::resizeColumnToContents( 0 );
    QTreeView::resizeColumnToContents( 1 );
    QTreeView::resizeColumnToContents( 2 );

    QTreeView::expandAll();

    QString strMessage;

    if ( m_pProject )
        emit signalProjectUpdated( tr( "The project %1 is %2% complete" )
            .arg( m_pProject->getStrName().c_str()).arg( m_pProject->getComplete()));

}

