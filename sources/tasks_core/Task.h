#pragma once

#include <string>
#include <vector>
#include <memory>
#include <list>


//enum class TaskPriority { Low, Normal, Hight };

class Task
{
public:
    using ptr_t = Task *;

    enum class Priority;

    Task( const std::string &strName, ptr_t pParent = nullptr );
    ~Task();

    const ptr_t getChild( std::size_t nChildIndex ) const;

    void insertChild( ptr_t pChild, std::size_t nChildIndex );
    void addChild( ptr_t pChild );

    void removeChild( std::size_t nChildIndex );
    void removeChild( const ptr_t pChild );

    void swapChilds( ptr_t pChild1, ptr_t pChild2 );
    void swapChilds( std::size_t nChildIndex1, std::size_t nChildIndex2 );

    std::size_t getChildsCount() const;

    std::size_t getChildIndex( const ptr_t pChild ) const;

    ptr_t getParent() const;
    void setParent( const ptr_t pParent );

    void changePatent( const ptr_t pNewParent );

    std::string getName() const;
    void setName( const std::string &strName );

    std::string getUuid() const;

    Priority getPriority() const;
    void setPriority( const Priority &priority );

    static ptr_t create( const std::string &strName, ptr_t pParent = nullptr );


private:

private:
    ptr_t m_pParent;
    std::string m_strName;
    std::string m_strUuid;
    Priority m_priority;
    std::list<ptr_t> m_childs;
};
