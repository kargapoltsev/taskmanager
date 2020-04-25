#include "Project.h"

#include <algorithm>

#include "Task.h"

Project::Project( const std::string &strName, const std::string &strUuid )
    : m_upRootTask( std::make_unique<Task>( strName, nullptr, strUuid ) )
{
}

Project::~Project()
{
}

std::string Project::getStrName() const
{
    return m_upRootTask->getName();
}

void Project::setStrName( const std::string &strName )
{
    m_upRootTask->setName( strName );
}

void Project::addTask( Task *pTask, Task *pParent )
{
    if ( !pParent )
        m_upRootTask->addChild( pTask );
    else
        pParent->addChild( pTask );

    registerTaskRecursive( pTask );
}

void Project::insertTask(Task *pTask, Task *pParent, std::size_t nPosition )
{
    if ( !pParent )
        m_upRootTask->insertChild( pTask, nPosition );
    else
        pParent->insertChild( pTask, nPosition );

    registerTaskRecursive( pTask );
}

Task * Project::getTask( const std::string strUuid ) const
{
    if ( auto it = m_tasksCache.find( strUuid ); it != m_tasksCache.end() )
        return m_tasksCache.at( strUuid );
    else
        return nullptr;
}

void Project::removeTask( const std::string strUuid )
{
    if ( auto it = m_tasksCache.find( strUuid ); it != m_tasksCache.end() )
    {
        auto pTask = it->second;

        pTask->remove();

        removeTaskRecursive( pTask );
    }
}

Task * Project::getRootTask() const
{
    return m_upRootTask.get();
}

std::string Project::getUuid() const
{
    return m_upRootTask->getUuid();
}

std::size_t Project::getTasksCount() const
{
    return m_tasksCache.size();
}

uint8_t Project::getComplete() const
{
    return m_upRootTask->getComplete();
}

Project * Project::create( const std::string strName )
{
    return new Project( strName );
}

void Project::registerTaskRecursive( Task * pTask )
{
    if ( pTask->getChildsCount() > 0 )
    {
        std::for_each( pTask->getChildsBegin(), pTask->getChildsEnd(),
        [=] ( Task *pTask )
        {
            registerTaskRecursive( pTask );
        } );
    }

    m_tasksCache.emplace( pTask->getUuid(), pTask );
}

void Project::removeTaskRecursive( Task *pTask )
{
    if ( pTask->getChildsCount() > 0 )
    {
        std::for_each( pTask->getChildsBegin(), pTask->getChildsEnd(),
        [=] ( Task *pTask )
        {
            removeTaskRecursive( pTask );
        } );
    }

    const auto &strUuid = pTask->getUuid();

    if ( auto it = m_tasksCache.find( strUuid ); it != m_tasksCache.end() )
    {
        m_tasksCache.erase( strUuid );
    }
}
