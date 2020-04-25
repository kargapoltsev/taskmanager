#include "ProjectListModel.h"

#include <memory>

#include "DataStore.h"
#include "Project.h"

ProjectListModel::ProjectListModel( QObject *pParent )
    : QAbstractListModel( pParent )
{

}

int ProjectListModel::rowCount( const QModelIndex &parent ) const
{
    if ( parent.isValid() || m_pDataStore == nullptr )
        return 0;

    return static_cast<int>( m_pDataStore->getProjectsCount() );
}

QVariant ProjectListModel::data( const QModelIndex &index, int nRole ) const
{
    if ( !index.isValid() || m_pDataStore == nullptr )
        return QVariant();

    if ( index.row() < 0 || index.row() > static_cast<int>( m_pDataStore->getProjectsCount() ) )
        return  QVariant();

    if ( nRole == Qt::DisplayRole || nRole == Qt::EditRole )
    {
        const auto nIndex = static_cast<std::size_t>( index.row() );

        const auto pProject = m_pDataStore->getProject( nIndex );

        if ( !pProject )
            return QVariant();

        const auto strName = pProject->getStrName();

        return QString::fromStdString( strName );
    }
    else
        return QVariant();
}

bool ProjectListModel::setData( const QModelIndex &index, const QVariant &value, int nRole )
{
    if ( m_pDataStore == nullptr )
        return false;

    if ( index.isValid() && nRole == Qt::EditRole )
    {
        const auto nIndex = static_cast<std::size_t>( index.row() );

        auto pProject = m_pDataStore->getProject( nIndex );

        const auto strNewName = value.toString().toStdString();

        pProject->setStrName( strNewName );

        return true;
    }

    return false;
}

bool ProjectListModel::insertRows( int row, int count, const QModelIndex &parent )
{
    if ( m_pDataStore == nullptr )
        return false;

    if ( parent.isValid() )
        return false;

    QAbstractItemModel::beginInsertRows( parent, row, row + count - 1 );

    const auto nCount = static_cast<std::size_t>( count );

    for ( std::size_t i = 0; i < nCount; ++i )
    {
        auto upProject = std::make_unique<Project>( tr( "New project" ).toStdString() );
        m_pDataStore->addProject( std::move( upProject ) );
    }

    QAbstractItemModel::endInsertRows();

    return true;
}

bool ProjectListModel::removeRows( int row, int count, const QModelIndex & parent )
{
    if ( parent.isValid() )
        return false;

    QAbstractItemModel::beginRemoveRows( parent, row, row + count - 1 );

    const auto nCount = static_cast<std::size_t>( count );
    const auto nIndex = static_cast<std::size_t>( row );

    for ( std::size_t i = 0; i < nCount; ++i )
    {
        m_pDataStore->removeProject( nIndex );
    }

    QAbstractItemModel::endRemoveRows();

    return true;
}

Qt::ItemFlags ProjectListModel::flags( const QModelIndex &index ) const
{
    Qt::ItemFlags flags = QAbstractListModel::flags(index);

    if ( index.isValid() )
        return flags |= Qt::ItemIsEditable;

    return flags;
}

void ProjectListModel::setDataStore(DataStore * pDataStore)
{
    m_pDataStore = pDataStore;
}

void ProjectListModel::updateAll()
{
    emit dataChanged( index( 0, 0, QModelIndex() ), index( m_pDataStore->getProjectsCount(), 0, QModelIndex() ) );
}
void ProjectListModel::update( int nIndex )
{
    emit dataChanged( index( nIndex, 0, QModelIndex() ),
        index( nIndex, 0, QModelIndex() ) );
}
