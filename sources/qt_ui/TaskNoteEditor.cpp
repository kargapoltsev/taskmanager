#include "TaskNoteEditor.h"

#include <QtWidgets>
#include "Task.h"

TaskNoteEditor::TaskNoteEditor(QWidget * pParent)
    : QTextEdit( pParent )
{

}

void TaskNoteEditor::setTask( Task * pTask )
{
    m_pTask = pTask;

    QTextEdit::setText( QString::fromStdString( pTask->getName() ) );
}
