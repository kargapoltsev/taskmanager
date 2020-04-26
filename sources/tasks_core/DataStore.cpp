#include "DataStore.h"

#include <algorithm>
#include <utility>

#include "Task.h"
#include "Project.h"

#include "CoreExeptions.h"

DataStore::DataStore()
{

}

void DataStore::initialize()
{

}

std::pair<bool, std::size_t> DataStore::addProject( std::unique_ptr<Project> upProject )
{
    auto retVal = m_projects.emplace( upProject->getStrName(), std::move( upProject ));

    auto isEmplaced = retVal.second;

    std::size_t nPosition = 0;

    if ( isEmplaced )
        nPosition = std::distance( std::begin( m_projects ), retVal.first );

    return std::make_pair( isEmplaced, nPosition );
}

std::pair<Project*, std::size_t> DataStore::createProject( const std::string& strName )
{
    auto upProject = std::make_unique<Project>( strName );
    auto pProject = upProject.get();
    const auto &[isEmplaced, nPosition] = addProject( std::move( upProject ));

    if ( !isEmplaced )
        pProject = nullptr;

    return std::make_pair( pProject, nPosition );
}

std::size_t DataStore::getProjectsCount() const
{
    return m_projects.size();
}

void DataStore::removeProject( std::size_t nProjectIndex )
{
    if ( nProjectIndex > m_projects.size())
        throw std::logic_error( "The index exceeds the size of the collection of projects." );

    const auto it = std::next( std::begin( m_projects ), nProjectIndex );
    m_projects.erase( it );
}

Project* DataStore::getProject( std::size_t nProjectIndex ) const
{
    if ( nProjectIndex > m_projects.size())
        throw std::logic_error( "The index exceeds the size of the collection of projects." );

    const auto it = std::next( std::begin( m_projects ), nProjectIndex );

    return ( it->second ).get();
}

std::pair<bool, std::size_t> DataStore::renameProject( std::size_t nProjectIndex, const std::string &strName )
{
    if ( nProjectIndex > m_projects.size())
        throw std::logic_error( "The index exceeds the size of the collection of projects." );

    const auto itProject = std::next( std::begin( m_projects ), nProjectIndex );

    itProject->second->setStrName( strName );

    auto project = m_projects.extract( itProject );

    project.key() = strName;

    auto retVal = m_projects.insert( std::move( project ));

    const auto nPosition = std::distance( std::begin( m_projects ), retVal.position );

    return std::make_pair( true, nPosition );
}

bool DataStore::isHas( const std::string& strName )
{
    auto it = m_projects.find( strName );

    return it != std::end( m_projects );
}

