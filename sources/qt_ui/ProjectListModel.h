#pragma once

#include <QAbstractListModel>

class DataStore;
class Project;

class ProjectListModel : public QAbstractListModel
{
 Q_OBJECT

 public:
    ProjectListModel( QObject* pParent = nullptr );

    // QAbstractItemModel interface
    int rowCount( const QModelIndex& parent ) const override;
    QVariant data( const QModelIndex& index, int role ) const override;
    bool setData( const QModelIndex& index, const QVariant& value, int role ) override;
    bool insertRows( int row, int count, const QModelIndex& parent ) override;
    bool removeRows( int row, int count, const QModelIndex& parent ) override;
    Qt::ItemFlags flags( const QModelIndex& index ) const override;

    void setDataStore( DataStore* pDataStore );

    void updateAll();
    void update( int nIndex );

 private:
    DataStore* m_pDataStore;
};

