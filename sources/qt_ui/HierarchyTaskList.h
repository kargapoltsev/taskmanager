#pragma once

#include <memory>
#include <QTreeView>


//class QTreeView;

class Project;
class Task;

class HierarchyTaskListModel;

class HierarchyTaskList : public QTreeView
{
    Q_OBJECT

public:
    HierarchyTaskList( QWidget *pParent = nullptr );

    void initialize();



signals:
    void signalProjectUpdated( const QString &strMessage );
    void selectedTaskChanged( Task *pTask );

public slots:
    void slotAddNewTask();
    void slotRemoveTask();
    void slotAddChildTask();
    void slotUpTaskPosition();
    void slotDownTaskPosition();
    void slotDiveTask();
    void slotAscentTask();
    void slotCompleteTask();
    void slotUpTaskPriority();
    void slotDownTaskPriority();

    void setProject( Project *pProject );

private slots:
    void slotHandleSelectedTask( const QItemSelection&, const QItemSelection& );

private:
    void update();

private:
    HierarchyTaskListModel *m_pModel;
    Project *m_pProject;
//    QTreeView *m_pView;
};
