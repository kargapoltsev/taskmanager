#include "DataStore.h"

#include "Task.h"
#include "Project.h"

DataStore::DataStore()
{

}

void DataStore::initialize()
{

}

void DataStore::addProject( std::unique_ptr<Project> upProject )
{
    m_projects.emplace( upProject->getUuid(), std::move( upProject ) );
}

Project * DataStore::getProject( const std::string &strUuid )
{
    if ( auto it = m_projects.find( strUuid ); it != m_projects.end() )
        return it->second.get();
    else
        return nullptr;
}
