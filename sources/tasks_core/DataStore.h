#pragma once

#include <memory>
#include <map>
#include <list>
#include <string>

class Project;
class Task;

class DataStore
{
public:
    DataStore();
    void initialize();

    void addProject( std::unique_ptr<Project> upProject );
    Project * getProject( const std::string &strUuid );

private:
    std::map<std::string, std::unique_ptr<Project>> m_projects;
};
