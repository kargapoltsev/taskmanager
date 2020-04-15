#pragma once

#include <memory>
#include <QMainWindow>


class DataStore;
class Project;
class HierarchyTaskList;
class TaskNoteEditor;

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
    void createCentralWidget();

private slots:
    void setStatusbarMessage( const QString &strMessage );

private:
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


    DataStore *m_pDataStore;
    Project *m_pCurrentProject;

    HierarchyTaskList *m_pHierarchyTaskList;
    TaskNoteEditor *m_pTaskNoteEditor;
    QWidget *m_pWidget;
};
