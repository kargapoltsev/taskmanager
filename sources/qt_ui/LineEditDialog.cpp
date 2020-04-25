#include "LineEditDialog.h"

#include <QtWidgets>

LineEditDialog::LineEditDialog( QWidget *parent )
    : QDialog(parent)
    , m_pLabel( new QLabel( tr( "Text" ), this ) )
    , m_pLineEdit( new QLineEdit( this ) )
    , m_pSaveButton( new QPushButton( tr( "Save"), this ) )
    , m_pCancelButton( new QPushButton ( tr( "Cancel"), this ) )
{
    auto pHBoxLayoutBottom = new QHBoxLayout();
    pHBoxLayoutBottom->addStretch();
    pHBoxLayoutBottom->addWidget( m_pSaveButton );
    pHBoxLayoutBottom->addWidget( m_pCancelButton );

    auto pVBoxLayout = new QVBoxLayout( this );
    pVBoxLayout->addWidget( m_pLabel );
    pVBoxLayout->addWidget( m_pLineEdit );
    pVBoxLayout->addLayout( pHBoxLayoutBottom );

    QObject::connect( m_pSaveButton, &QPushButton::clicked,
                      this, &LineEditDialog::handleSaveButtonClicked );

    QObject::connect( m_pCancelButton, &QPushButton::clicked,
                      m_pLineEdit, &QLineEdit::clear );

    QObject::connect( m_pCancelButton, &QPushButton::clicked,
                      this, &QDialog::close );

    QDialog::resize( 300, 100 );
    QDialog::setModal( true );
}

void LineEditDialog::setText( const QString & strText )
{
    m_pLineEdit->setText( strText );
}

void LineEditDialog::setLabelText(const QString & strText)
{
    m_pLabel->setText( strText );
}

void LineEditDialog::handleSaveButtonClicked()
{
    emit textSaved( m_pLineEdit->text() );
    m_pLineEdit->clear();
    QDialog::close();
}
