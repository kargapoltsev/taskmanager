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

    if ( m_pTask != nullptr )
        QTextEdit::setText( QString::fromStdString( pTask->getNote() ) );
    else
        QTextEdit::clear();
}

void TaskNoteEditor::saveNote()
{
    if ( m_pTask != nullptr )
        m_pTask->setNote( QTextEdit::toPlainText().toStdString() );
}
