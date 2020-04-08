#pragma once

#include <memory>
#include <QMainWindow>

class QTreeView;

class DataStore;
class Project;

class HierarchyTaskListModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

    void initialize();

    DataStore * getTasksRepository() const;
    void setTasksRepository( DataStore * pTasksRepository );

public slots:
    void slotExit();
    void slotAddNewTask();

private:
    DataStore * m_pDataStore;
    Project *m_pCurrentProject;
    HierarchyTaskListModel *m_pModel;
    QTreeView *m_pView;
};
