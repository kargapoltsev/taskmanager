#pragma once

#include <QWidget>

class QLabel;
class QComboBox;
class QPushButton;

class DataStore;
class Project;
class ProjectListModel;
class LineEditDialog;
class QMessageBox;

class ProjectManager : public QWidget
{
    Q_OBJECT

public:
    ProjectManager( QWidget *pParent );
    void setDataStore( DataStore *pDataStore );
    void initialize();

public slots:
    void showCreateNewProjectDialog();
    void showEditCurrentProjectDialog();
    void removeCurrentProject();

private slots:
    void createNewProject( const QString &strName );
    void renameCurrentProject( const QString &strName );
    void handleCurrentProjectIndexChanged( int nIndex  );

signals:
    void currentProjectChanged( Project *pProject );

private:
    void setComboBoxData();
    Project * getCurrentProject();

    DataStore *m_pDataStore;
    ProjectListModel *m_pProjectListModel;

    QLabel *m_pLabel;
    QComboBox *m_pProjectsComboBox;
    QPushButton *m_pAddProjectButton;
    QPushButton *m_pEditProjectButton;
    QPushButton *m_pRemoveProjectButton;

    LineEditDialog *m_pCreateProjectDialog;
    LineEditDialog *m_pEditCurrentProjectDialog;

    QMessageBox *m_pMessageBox;
};

