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

    // result, position
    std::pair<bool, std::size_t> addProject( std::unique_ptr<Project> upProject );

    std::pair<Project*, std::size_t> createProject( const std::string& strName );

    [[nodiscard]] Project* getProject( std::size_t nProjectIndex ) const;
    [[nodiscard]] std::size_t getProjectsCount() const;

    void removeProject( std::size_t nProjectIndex );

    std::pair<bool, std::size_t> renameProject( std::size_t nProjectIndex, const std::string &strName );

    bool isHas( const std::string &strName );

 private:
    std::map<std::string, std::unique_ptr<Project>> m_projects;
};
