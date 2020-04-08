#include "UiTest.h"
#include <QAbstractItemModelTester>
#include <QtWidgets>

#include "HierarchyTaskListModel.h"
#include "Project.h"
#include "Task.h"


QtUiTest::QtUiTest() {

    auto pTask1 = new Task( "Task1" );
    auto pSubTask1 = new Task( "Subtask 1", pTask1 );
    auto pSubTask2 = new Task( "Subtask 2", pTask1 );
    auto pTask2 = new Task( "Task2" );

    m_pProject = new Project( "Test Project" );

    m_pProject->addTask( pTask1 );
    m_pProject->addTask( pSubTask1 );
    m_pProject->addTask( pSubTask2 );
    m_pProject->addTask( pTask2 );

    auto pHierarchyTaskListModel = new HierarchyTaskListModel( this );
    pHierarchyTaskListModel->setProject( m_pProject );

    new QAbstractItemModelTester( pHierarchyTaskListModel, QAbstractItemModelTester::FailureReportingMode::QtTest, this );
}

QtUiTest::~QtUiTest()
{
    delete m_pProject;
}
