#pragma once

#include <memory>
#include <QTreeWidget>

class Task;

class HierarhyTaskList : public QTreeWidget
{
public:
    HierarhyTaskList( QWidget *pParent = nullptr );

//    void initialize();

private:
    std::vector<std::shared_ptr<Task>> m_tasks;
};
