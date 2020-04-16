#pragma once

#include <QTextEdit>

class Task;

class TaskNoteEditor : public QTextEdit
{
    Q_OBJECT
public:
    explicit TaskNoteEditor( QWidget *pParent = nullptr );

public slots:
    void setTask( Task *pTask );

private slots:
    void saveNote();

private:
    Task *m_pTask;
};
