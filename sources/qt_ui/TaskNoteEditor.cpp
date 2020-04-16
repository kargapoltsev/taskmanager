#include "TaskNoteEditor.h"

#include <QtWidgets>
#include "Task.h"

TaskNoteEditor::TaskNoteEditor(QWidget * pParent)
    : QTextEdit( pParent )
{
    QObject::connect( this, &QTextEdit::textChanged, this, &TaskNoteEditor::saveNote );
}

void TaskNoteEditor::setTask( Task * pTask )
{
    m_pTask = pTask;

    QTextEdit::setText( QString::fromStdString( pTask->getNote() ) );
}

void TaskNoteEditor::saveNote()
{
    m_pTask->setNote( QTextEdit::toPlainText().toStdString() );
}
