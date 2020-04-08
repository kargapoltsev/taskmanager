#include <iostream>
#include <memory>
#include <vector>
#include <iterator>

#include <QtWidgets>

#include "MainWindow.h"

#include "DataStore.h"
#include "Project.h"
#include "Task.h"


//template<typename T>
//std::ostream & operator<< ( std::ostream &out, const std::vector<T> &vec )
//{
//    if ( !vec.empty() )
//    {
//        std::copy( std::begin( vec ), std::end( vec ), std::ostream_iterator<T>( out, "" ) );
//    }

//    return out;
//}


//void printTasks( Task::ptr_t pTask, std::size_t nDeep = 1 )
//{
//    if ( pTask->getParent() == nullptr )
//    {
//        std::cout << pTask->getName() << std::endl;
//    }

//    auto nChildsCount = pTask->getChildsCount();

//    for ( std::size_t i = 0; i < nChildsCount; ++i )
//    {
//        const auto pChild = pTask->getChild( i );

//        std::cout << std::vector<std::string>( nDeep, std::string("- ") ) << pChild->getName() << std::endl;

//        printTasks( pChild, nDeep + 1 );
//    }


//}


int main(int argc, char** argv)
{

//    auto pRootTask = Task::create( "Root" );

//    auto pTask1 = Task::create( "Task1", pRootTask );
//    Task::create( "Task2", pRootTask );

//    auto pSubTask1 = Task::create( "Subtask 1", pTask1 );
//    Task::create( "Subtask 2", pSubTask1 );
//    Task::create( "Subtask 3", pTask1 );

//    printTasks( pRootTask );


    auto pTask1 = new Task( "Task1" );
    auto pSubTask1 = new Task( "Subtask 1", pTask1 );
    auto pSubTask2 = new Task( "Subtask 2", pSubTask1 );
    auto pTask2 = new Task( "Task2" );

    auto upProject = std::make_unique<Project>( "Free tasks" );

    upProject->addTask( pTask1 );
    upProject->addTask( pTask2 );

    DataStore taskRepository;
    taskRepository.addProject( std::move( upProject ) );

    QApplication app(argc, argv);
    MainWindow mw;
    mw.setTasksRepository( &taskRepository );
    mw.initialize();
    mw.resize( 350, 310 );
    mw.show();

    return app.exec();
}
