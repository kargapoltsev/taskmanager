#pragma once

#include <memory>
#include <QMainWindow>


class DataStore;
class Project;
class HierarchyTaskList;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

    void initialize();

    DataStore * getDataStore() const;
    void setDataStore(DataStore * pDataStore);

private:
    void update();
    void createActions();
    void createMenus();
    void createStatusBar();

private slots:
    void setStatusbarMessage( const QString &strMessage );

private:
    DataStore *m_pDataStore;
    HierarchyTaskList *m_pHierarchyTaskList;
    Project *m_pCurrentProject;

    QAction *m_pExitAction;
    QAction *m_pAddNewTaskAction;
    QAction *m_pRemoveTaskAction;
    QAction *m_pAddChildTaskAction;
    QAction *m_pUpTaskPositionAction;
    QAction *m_pDownTaskPositionAction;
    QAction *m_pDiveTaskAction;
    QAction *m_pAscentTaskAction;
    QAction *m_pCompleteTaskAction;
    QAction *m_pUpTaskPriorityAction;
    QAction *m_pDownTaskPriorityAction;

    QMenu *m_pFileMenu;
    QMenu *m_pTaskMenu;
};
