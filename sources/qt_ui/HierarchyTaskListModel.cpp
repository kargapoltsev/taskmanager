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
    if ( !pParentTask )
        return QModelIndex();

    if ( static_cast<std::size_t>( row ) >= pParentTask->getChildsCount() )
        return QModelIndex();

    auto pChildTask = pParentTask->getChild( static_cast<std::size_t>( row ) );
    if ( !pChildTask )
        return QModelIndex();

    return createIndex( row, column, pChildTask );
}

QModelIndex HierarchyTaskListModel::parent( const QModelIndex & child ) const
{
    if (!child.isValid())
        return QModelIndex();

    auto pChildTask = getTaskFromIndex( child );
    if ( !pChildTask )
        return QModelIndex();

    auto pParentTask = pChildTask->getParent();
    if ( !pParentTask )
        return QModelIndex();

    auto pGrandParentTask = pParentTask->getParent();
    if ( !pGrandParentTask )
        return QModelIndex();

    auto nParentPosition = pGrandParentTask->getChildIndex( pParentTask );

    return createIndex( static_cast<int>(nParentPosition), 0, pParentTask );
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

int HierarchyTaskListModel::columnCount( const QModelIndex & /* parent */) const
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
        return QString( "%1%" ).arg( pTask->getComplete() );
        break;
    }

    return QVariant();
}

bool HierarchyTaskListModel::setData( const QModelIndex &index, const QVariant &value, int role )
{
    if ( index.isValid() && role == Qt::EditRole )
    {
        auto pTask = getTaskFromIndex( index );

        pTask->setName( value.toString().toStdString() );

        emit dataChanged( index, index, { Qt::DisplayRole, Qt::EditRole } );

        return true;
    }

    return false;
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
    auto pParentTask = getTaskFromIndex( parent );
    if ( !pParentTask )
        return false;

    QAbstractItemModel::beginInsertRows( parent, row, row + count - 1 );

    auto pTask = new Task( QString("New task %1").arg( pParentTask->getChildsCount() + 1 ).toStdString() );

    auto nRow = static_cast<std::size_t>(row);

    if ( nRow != pParentTask->getChildsCount() )
        m_pProject->insertTask( pTask, pParentTask, nRow );
    else
        m_pProject->addTask( pTask , pParentTask );
    QAbstractItemModel::endInsertRows();

    return true;
}

bool HierarchyTaskListModel::removeRows( int row, int count, const QModelIndex &parent )
{
    auto pParentTask = getTaskFromIndex( parent );
    if ( !pParentTask )
        return false;

    QAbstractItemModel::beginRemoveRows( parent, row, row + count - 1);
    pParentTask->removeChild( row );
    QAbstractItemModel::endRemoveRows();

    return true;
}

bool HierarchyTaskListModel::moveRows( const QModelIndex &sourceParent,
                                       int sourceRow, int count,
                                       const QModelIndex &destinationParent,
                                       int destinationChild )
{
    if ( destinationChild < 0 )
        return false;

    auto pSourceParentTask = getTaskFromIndex( sourceParent );
    auto pDestinationParentTask = getTaskFromIndex( destinationParent );

    if ( !pSourceParentTask || !pDestinationParentTask )
        return false;

	const auto nSourceRow = static_cast<std::size_t>( sourceRow );

	auto pTask = pSourceParentTask->getChild( nSourceRow );
	if ( !pTask )
		return false;

	const auto nDestinationRow = static_cast<std::size_t>( destinationChild );

//    if ( nDestinationRow > pDestinationParentTask->getChildsCount() )
//        return false;

	auto retVal = QAbstractItemModel::beginMoveRows( sourceParent, sourceRow, sourceRow + count - 1, destinationParent, destinationChild );

	if ( pSourceParentTask == pDestinationParentTask )
		pDestinationParentTask->swapChilds( nSourceRow, nDestinationRow );
	else
	{
        if ( nDestinationRow == 0 || pDestinationParentTask->getChildsCount() == 0 || pDestinationParentTask->getChildsCount() == nDestinationRow )
            pDestinationParentTask->addChild(pTask);
		else
            pDestinationParentTask->insertChild(pTask, nDestinationRow - 1);
	}

	if ( retVal )
		QAbstractItemModel::endMoveRows();

    return true;
}

Qt::ItemFlags HierarchyTaskListModel::flags(const QModelIndex & index) const
{
    if ( !index.isValid() )
        return Qt::NoItemFlags;

    auto flags = QAbstractItemModel::flags(index);

    if ( index.column() == 0 )
        flags |= Qt::ItemIsEditable;

    return Qt::ItemIsEnabled | flags;
}

void HierarchyTaskListModel::setProject(Project * pProject)
{
    m_pProject = pProject;
    resetInternalData();
}

void HierarchyTaskListModel::updateComplete( const QModelIndex &index )
{
    if ( !index.isValid() )
        return;

    if ( auto pTask = getTaskFromIndex( index ) )
        pTask->recalculateComplete();
}

void HierarchyTaskListModel::setComplete( const QModelIndex &index )
{
    if ( !index.isValid() )
        return;

    if ( auto pChild = getTaskFromIndex( index ) )
    {
        if ( pChild->getChildsCount() != 0 )
            return;

        if ( pChild->getComplete() == 100 )
            pChild->setComplete( 0 );
        else
            pChild->setComplete( 100 );

        pChild->recalculateComplete();
    }
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

