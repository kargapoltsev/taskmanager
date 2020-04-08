#include "HierarchyTaskList.h"

#include <QtWidgets>
#include "Task.h"
//#include "Registry.h"

HierarhyTaskList::HierarhyTaskList(QWidget *pParent)
    : QTreeWidget( pParent )
{
    QStringList headersList;
    headersList << "Priority" << "Name" << "Complete";
    QTreeWidget::setHeaderLabels( headersList );


    m_tasks.push_back( std::make_shared<Task>( "Task1" ) );
    m_tasks.push_back( std::make_shared<Task>( "Task11", m_tasks.front().get() ) );
    m_tasks.push_back( std::make_shared<Task>( "Task12", m_tasks.front().get() ) );
    m_tasks.push_back( std::make_shared<Task>( "Task2" ) );

    Registry<Task *, QTreeWidgetItem *> registry;

    for ( auto task : m_tasks )
    {
        QTreeWidgetItem *pTWgItem;

        if ( auto pParent = task->getParent() )
        {
            auto pParentTWgItem = registry.get( pParent );
            pTWgItem = new QTreeWidgetItem( pParentTWgItem );
        }
        else
        {
            pTWgItem = new QTreeWidgetItem( this );
        }

        pTWgItem->setText( 0, "!" );
        pTWgItem->setText( 1, QString( task->getName().c_str() ) );
        pTWgItem->setText( 2, "10%" );

        registry.add( task.get(), pTWgItem);
    }

    auto i = 1;
}

