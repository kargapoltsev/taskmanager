#pragma once

#include <memory>
#include <QAbstractItemModel>


class Project;
class Task;

class HierarchyTaskListModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    HierarchyTaskListModel( QObject * pParent = nullptr );

    // QAbstractItemModel interface
    QModelIndex index(int row, int column, const QModelIndex & parent) const override;
    QModelIndex parent(const QModelIndex & child) const override;
    int rowCount(const QModelIndex & parent) const override;
    int columnCount(const QModelIndex & parent) const override;
    QVariant data(const QModelIndex & index, int role) const override;
    bool setData(const QModelIndex & index, const QVariant & value, int role) override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    bool insertRows(int row, int count, const QModelIndex & parent) override;
    bool removeRows(int row, int count, const QModelIndex & parent) override;
    bool moveRows(const QModelIndex & sourceParent, int sourceRow, int count, const QModelIndex & destinationParent, int destinationChild) override;
    Qt::ItemFlags flags(const QModelIndex & index) const override;

    void setProject( Project *pProject );
    void updateComplete( const QModelIndex &task );
    void setComplete( const QModelIndex &task );

private:
    Task * getTaskFromIndex( const QModelIndex &index ) const;

private:
    Project * m_pProject;

    // QAbstractItemModel interface
public:
};
