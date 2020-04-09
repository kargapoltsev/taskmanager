#pragma once

#include <string>
#include <memory>
#include <string>
#include <map>

class Task;

class Project
{
public:
    Project( const std::string &strName, const std::string &strUuid = "" );
    ~Project();

    std::string getStrName() const;
    void setStrName( const std::string &strName );

    void addTask( Task *pTask, Task *pParent = nullptr );

    void insertTask( Task *pTask, Task *pParent = nullptr, std::size_t nPosition = 0 );

    Task * getTask( const std::string strUuid ) const;

    void removeTask( const std::string strUuid );

    Task * getRootTask() const;

    std::string getUuid() const;

    std::size_t getTasksCount() const;

private:
    void registerTaskRecursive( Task *pTask );
    void removeTaskRecursive( Task *pTask );

    std::unique_ptr<Task> m_upRootTask;
    std::map<std::string, Task *> m_tasksCache;
};
