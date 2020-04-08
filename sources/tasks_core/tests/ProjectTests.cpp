#include <memory>
#include <string>

#include "gtest/gtest.h"
#include "Task.h"
#include "Project.h"

class ProjectTest : public ::testing::Test
{
public:
    ProjectTest()
        : m_project( new Project( "Test project" ) )
    {
    }

protected:
    // Test interface
    void SetUp() override
    {
        m_pTask = new Task( "Task 1" );
        auto pSubTask = new Task( "Subtask 1", m_pTask );
        new Task( "Subtask 2", pSubTask );

        m_project->addTask( m_pTask );
    }
    void TearDown() override
    {

    }

private:
    void TestBody() override
    {
    }

protected:
    Project * m_project;
    Task *m_pTask;
};


TEST_F( ProjectTest, TestGetTask )
{
    auto pRequestTask = m_pTask->getChild( 0 );

    auto pTask = m_project->getTask( pRequestTask->getUuid() );

    ASSERT_EQ( pTask, pRequestTask );

    auto pBadTask = m_project->getTask( "not_a_uuid" );

    ASSERT_EQ( pBadTask, nullptr );
}

TEST_F( ProjectTest, TestAddTask )
{
    auto nRootTaskChildCount = m_project->getRootTask()->getChildsCount();

    auto pTask = new Task( "Test Task" );

    m_project->addTask( pTask );
    m_project->addTask( new Task( "Test SubTask" ), m_project->getTask( pTask->getUuid() ) );

    ASSERT_EQ( m_project->getRootTask()->getChildsCount(), nRootTaskChildCount + 1 );
}

TEST_F( ProjectTest, TestRemoveTask )
{
    m_project->removeTask( m_pTask->getUuid() );

    ASSERT_EQ( m_project->getTasksCount(), 0 );
}
