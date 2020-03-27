#include <memory>
#include <string>

#include "gtest/gtest.h"
#include "Task.h"


class TaskTest : public ::testing::Test
{
public:
    TaskTest()
        : m_pRootTask( new Task( "Root" ) )
    {
    }

protected:
    // Test interface
    void SetUp() override
    {
        m_pTask1 = Task::create( "Task1", m_pRootTask );
        m_pTask2 = Task::create( "Task2", m_pRootTask );

        Task::create( "Subtask 1", m_pTask1 );
        Task::create( "Subtask 2", m_pTask1 );
        Task::create( "Subtask 3", m_pTask1 );

    }
    void TearDown() override
    {
        delete m_pRootTask;
    }

private:
    void TestBody() override
    {
    }

protected:
    Task *m_pRootTask;
    Task *m_pTask1;
    Task *m_pTask2;
};

TEST_F( TaskTest, TestChildsCount )
{
    ASSERT_EQ( 2, m_pRootTask->getChildsCount() );
    ASSERT_EQ( 3, m_pTask1->getChildsCount() );
    ASSERT_EQ( 0, m_pTask2->getChildsCount() );
}

TEST_F( TaskTest, TestRemoveChild )
{
    m_pTask1->removeChild( static_cast<std::size_t>( 0 ) );
    ASSERT_EQ( 2, m_pTask1->getChildsCount() );

    m_pTask1->removeChild( static_cast<std::size_t>( 0 ) );
    ASSERT_EQ( 1, m_pTask1->getChildsCount() );

    m_pTask1->removeChild( static_cast<std::size_t>( 0 ) );
    ASSERT_EQ( 0, m_pTask1->getChildsCount() );

    m_pRootTask->removeChild( m_pTask2 );
    ASSERT_EQ( 1, m_pRootTask->getChildsCount() );

    m_pRootTask->removeChild( m_pTask1 );
    ASSERT_EQ( 0, m_pRootTask->getChildsCount() );
}

TEST_F( TaskTest, TestAddChild )
{
    std::string strTaskName( "Subtask 4" );

    const std::size_t nAddPosition = m_pTask1->getChildsCount();

    m_pTask1->addChild( Task::create( strTaskName ) );

    ASSERT_EQ( m_pTask1->getChild( nAddPosition )->getName(), strTaskName );
}

TEST_F( TaskTest, TestSwapChildsByIndex )
{
    const std::size_t nReplacementPosition1 = 0;
    const std::size_t nReplacementPosition2 = 2;

    const auto strTask1Name = m_pTask1->getChild( nReplacementPosition1 )->getName();
    const auto strTask2Name = m_pTask1->getChild( nReplacementPosition2 )->getName();

    m_pTask1->swapChilds( nReplacementPosition1, nReplacementPosition2 );

    ASSERT_EQ( m_pTask1->getChild( nReplacementPosition1 )->getName(), strTask2Name );
    ASSERT_EQ( m_pTask1->getChild( nReplacementPosition2 )->getName(), strTask1Name );
}

TEST_F( TaskTest, TestSwapChildsByPointer )
{
    const std::size_t nReplacementPosition1 = 0;
    const std::size_t nReplacementPosition2 = 1;

    const auto strTask1Name = m_pTask1->getChild( nReplacementPosition1 )->getName();
    const auto strTask2Name = m_pTask1->getChild( nReplacementPosition2 )->getName();

    auto pChild1 = m_pTask1->getChild( nReplacementPosition1 );
    auto pChild2 = m_pTask1->getChild( nReplacementPosition2 );

    m_pTask1->swapChilds( pChild1, pChild2 );

    ASSERT_EQ( m_pTask1->getChild( nReplacementPosition1 )->getName(), strTask2Name );
    ASSERT_EQ( m_pTask1->getChild( nReplacementPosition2 )->getName(), strTask1Name );
}

TEST_F( TaskTest, TestInsertChild )
{
    const std::string strTaskName( "Subtask 4" );
    const std::size_t nInsertionPosition = 2;

    m_pTask1->insertChild( Task::create( strTaskName ), nInsertionPosition );

    ASSERT_EQ( m_pTask1->getChild( nInsertionPosition )->getName(), strTaskName );
}

TEST_F( TaskTest, TestChangeParent )
{
    const auto task1ChildsCount = m_pTask1->getChildsCount();
    const auto task2ChildsCount = m_pTask2->getChildsCount();

    auto pChild = m_pTask1->getChild( 0 );
    pChild->changePatent( m_pTask2 );

    ASSERT_EQ( m_pTask2->getChildsCount(), task2ChildsCount + 1 );
    ASSERT_EQ( m_pTask1->getChildsCount(), task1ChildsCount - 1 );
}

TEST_F( TaskTest, TestGetChildIndex )
{
    const std::size_t nSourceIndex = 1;

    const auto pChild = m_pTask1->getChild( nSourceIndex );

    const auto nResultingIndex = m_pTask1->getChildIndex( pChild );

    ASSERT_EQ( nSourceIndex, nResultingIndex );
}

