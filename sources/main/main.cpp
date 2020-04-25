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
    auto pTask0 = new Task( "Task0" );
    auto pTask1 = new Task( "Task1" );
    new Task( "Subtask 11", pTask1 );
    new Task( "Subtask 12", pTask1 );
    new Task( "Subtask 13", pTask1 );
    auto pTask2 = new Task( "Task2" );
    new Task( "Subtask 21", pTask2 );
    new Task( "Subtask 22", pTask2 );
    new Task( "Subtask 23", pTask2 );
    auto pTask3 = new Task( "Task3" );
    auto pTask4 = new Task( "Task4" );
    auto pTask5 = new Task( "Task5" );

    auto upFreeTasksProject = std::make_unique<Project>( "Free tasks" );

    upFreeTasksProject->addTask( pTask0 );
    upFreeTasksProject->addTask( pTask1 );
    upFreeTasksProject->addTask( pTask2 );
    upFreeTasksProject->addTask( pTask3 );
    upFreeTasksProject->addTask( pTask4 );
    upFreeTasksProject->addTask( pTask5 );

    auto upSomeOneProject = std::make_unique<Project>( "Someone" );
    upSomeOneProject->addTask( Task::create( "Some task" ) );

    DataStore dataStore;
    dataStore.addProject( std::move( upFreeTasksProject ) );
    dataStore.addProject( std::move( upSomeOneProject ) );

    QApplication app(argc, argv);
    MainWindow mw;
    mw.setDataStore( &dataStore );
    mw.initialize();
    mw.resize( 700, 420 );
    mw.show();

    return app.exec();
}
