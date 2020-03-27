#include "Task.h"

#include<algorithm>
#include<sstream>

#include <iostream>

enum class Task::Priority { Low, Normal, Hight };

Task::Task( const std::string &strName, ptr_t pParent )
    : m_pParent( pParent )
    , m_strName( strName )
    , m_strUuid( strName )
    , m_priority( Priority::Normal )
{
    if ( m_pParent )
        m_pParent->addChild( this );
}

Task::~Task()
{
    std::for_each( std::begin( m_childs ), std::end( m_childs ),
    [=] ( ptr_t pChild ) {
        delete pChild;
        pChild = nullptr;
    } );

    m_childs.clear();
}

void Task::addChild( ptr_t pChild )
{
    if ( !pChild )
        throw std::logic_error( "Child pointer is null" );

    pChild->setParent( this );

    m_childs.push_back( pChild );
}

const Task::ptr_t Task::getChild( std::size_t nChildIndex ) const
{
    if ( nChildIndex >= m_childs.size() )
        throw std::logic_error( "The index exceeds the size of the collection of children." );

    auto it = std::next( std::begin( m_childs ), nChildIndex );

    return *it;
}

void Task::insertChild( Task::ptr_t pChild, std::size_t nChildIndex )
{
    if ( nChildIndex > m_childs.size() )
        throw std::logic_error( "The index exceeds the size of the collection of children." );

    pChild->setParent( this );

    auto pNeighbourChild = getChild( nChildIndex );

    auto itNeighbourChild = std::find( m_childs.begin(), m_childs.end(), pNeighbourChild );

    m_childs.insert( itNeighbourChild, pChild );
}

void Task::removeChild( std::size_t n )
{
    m_childs.remove( getChild( n ) );
}

void Task::removeChild( const ptr_t pChild )
{
    if ( !pChild )
        throw std::logic_error( "Child pointer is null" );

    auto it = std::find( m_childs.begin(), m_childs.end(), pChild );

    if ( it != m_childs.end() )
        m_childs.erase( it );
    else
        throw std::logic_error( "Child not found" );
}

void Task::swapChilds( ptr_t pChild1, ptr_t pChild2 )
{
    if ( !pChild1 || !pChild2 )
        throw std::logic_error( "Child pointers are null" );

    auto itChild1 = std::find( std::begin( m_childs ), std::end( m_childs ), pChild1 );
    if ( itChild1 == std::end( m_childs ) )
        throw std::logic_error( "pChild1 is not child" );

    auto itChild2 = std::find( std::begin( m_childs ), std::end( m_childs ), pChild2 );
    if ( itChild2  == std::end( m_childs ) )
        throw std::logic_error( "pChild2 is not child" );

    m_childs.insert( itChild1, pChild2 );
    m_childs.insert( itChild2, pChild1 );

    m_childs.erase( itChild2 );
    m_childs.erase( itChild1 );
}

void Task::swapChilds( std::size_t nChildIndex1, std::size_t nChildIndex2 )
{
    auto pChild1 = getChild( nChildIndex1 );
    auto pChild2 = getChild( nChildIndex2 );

    swapChilds( pChild1, pChild2 );
}

std::size_t Task::getChildsCount() const
{
    return m_childs.size();
}

std::size_t Task::getChildIndex( const ptr_t pChild ) const
{
    if ( !pChild )
        throw std::logic_error( "Child pointer is null" );

    auto it = std::find( std::begin( m_childs ), std::end( m_childs ), pChild );

    if ( it != m_childs.end() )
        return std::distance( std::begin( m_childs ), it );
    else
        throw std::logic_error( "Child does not find" );
}

Task::ptr_t Task::getParent() const
{
    return m_pParent;
}

void Task::setParent( const ptr_t pParent )
{
    if ( !pParent )
        throw std::logic_error( "Parent pointer is null" );

    m_pParent = pParent;
}

void Task::changePatent( const Task::ptr_t pNewParent )
{
    if ( !pNewParent )
        throw std::logic_error( "Parent pointer is null" );

    if ( m_pParent )
        m_pParent->removeChild( this );

    m_pParent = pNewParent;
    m_pParent->addChild( this );
}

std::string Task::getName() const
{
    return m_strName;
}

void Task::setName(const std::string &strName)
{
    m_strName = strName;
}

std::string Task::getUuid() const
{
    return m_strUuid;
}

Task::Priority Task::getPriority() const
{
    return m_priority;
}

void Task::setPriority(const Task::Priority & priority)
{
    m_priority = priority;
}

Task::ptr_t Task::create( const std::string &strName, ptr_t pParent )
{
    return new Task( strName, pParent );
}



