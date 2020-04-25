#pragma once

#include <QDialog>

class QLabel;
class QLineEdit;
class QPushButton;
class QString;

class LineEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LineEditDialog( QWidget *parent = nullptr );

signals:
    void textSaved( const QString &strText );

public slots:
    void setText( const QString &strText );
    void setLabelText( const QString &strText );

private slots:
    void handleSaveButtonClicked();

private:
    QLabel *m_pLabel;
    QLineEdit *m_pLineEdit;
    QPushButton *m_pSaveButton;
    QPushButton *m_pCancelButton;
};
