#include "HierarchyTaskListModel.h"

#include <QtWidgets>

#include "Project.h"
#include "Task.h"


HierarchyTaskListModel::HierarchyTaskListModel( QObject * pParent  )
    : QAbstractItemModel( pParent )
{

}

QModelIndex HierarchyTaskListModel::index( int row, int column, const QModelIndex & parent ) const
{
    if ( !m_pProject->getRootTask() || row < 0 || column < 0 )
        return QModelIndex();

    auto pParentTask = getTaskFromIndex( parent );
    auto pChildTask = pParentTask->getChild( static_cast<std::size_t>( row ) );

    if ( !pChildTask )
        return QModelIndex();

    return createIndex( row, column, pChildTask );
}

QModelIndex HierarchyTaskListModel::parent( const QModelIndex & child ) const
{
    auto pChildTask = getTaskFromIndex( child );

    if ( !pChildTask )
        return QModelIndex();

    auto pParentTask = pChildTask->getParent();

    if ( !pParentTask )
        return QModelIndex();

    auto pGrandParentTask = pParentTask->getParent();

    if ( !pGrandParentTask )
        return QModelIndex();

    auto row = pGrandParentTask->getChildIndex( pParentTask );

    return createIndex( static_cast<int>(row), 0, pParentTask );
}

int HierarchyTaskListModel::rowCount( const QModelIndex & parent ) const
{
    if ( parent.column() > 0 )
        return 0;

    auto pParentTask = getTaskFromIndex( parent );

    if ( !pParentTask )
        return 0;

    return pParentTask->getChildsCount();
}

int HierarchyTaskListModel::columnCount( const QModelIndex & parent ) const
{
    return 3;
}

QVariant HierarchyTaskListModel::data( const QModelIndex & index, int role ) const
{
    if( role != Qt::DisplayRole )
        return QVariant();

    auto pTask = getTaskFromIndex( index );

    if ( !pTask )
        return QVariant();

    switch ( index.column() )
    {
    case 0:
        return tr( pTask->getName().c_str() );
        break;
    case 1:
        return tr( "Normal" );
        break;
    case 2:
        return tr( "0%" );
        break;
    }

    return QVariant();
}

bool HierarchyTaskListModel::setData( const QModelIndex & index, const QVariant &value, int role )
{
    return true;
}

QVariant HierarchyTaskListModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
    if ( orientation == Qt::Horizontal && role == Qt::DisplayRole )
    {
        switch ( section )
        {
        case 0:
            return tr( "Name" );
            break;
        case 1:
            return tr( "Priority" );
            break;
        case 2:
            return tr( "Complete" );
            break;
        }
    }

    return QVariant();
}

bool HierarchyTaskListModel::insertRows( int row, int count, const QModelIndex & parent )
{
    auto parentItem = getTaskFromIndex( parent );
    if ( !parentItem )
        return false;

    QAbstractItemModel::beginInsertRows( parent, row, row + count - 1 );
    parentItem->addChild( new Task( "New task" ) );
    QAbstractItemModel::endInsertRows();

    return true;
}

bool HierarchyTaskListModel::removeRows( int row, int count, const QModelIndex & parent )
{
    auto parentItem = getTaskFromIndex( parent );
    if ( !parentItem )
        return false;

    QAbstractItemModel::beginRemoveRows( parent, row, row + count - 1);
    parentItem->removeChild( row );
    QAbstractItemModel::endRemoveRows();

    return true;
}

bool HierarchyTaskListModel::moveRows(const QModelIndex & sourceParent, int sourceRow, int count, const QModelIndex & destinationParent, int destinationChild)
{
    return true;
}

Qt::ItemFlags HierarchyTaskListModel::flags(const QModelIndex & index) const
{
    if ( !index.isValid() )
        return Qt::NoItemFlags;

    return Qt::ItemIsEnabled | QAbstractItemModel::flags(index);
}

void HierarchyTaskListModel::setProject(Project * pProject)
{
    m_pProject = pProject;
    resetInternalData();
}

void HierarchyTaskListModel::addTask( Task * pTask )
{
//    m_pProject->getRootTask()->addChild( pTask );
//    emit dataChanged();
}

Task *HierarchyTaskListModel::getTaskFromIndex(const QModelIndex & index) const
{
    if ( m_pProject == nullptr )
        throw std::logic_error( "Project pointer is empty" );

    if ( index.isValid() )
        return static_cast<Task *>( index.internalPointer() );
    else
        return m_pProject->getRootTask();
}

