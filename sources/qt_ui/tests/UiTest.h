#pragma once

#include <QObject>


class Project;

class QtUiTest : public QObject
{
Q_OBJECT

public:
    QtUiTest();
    ~QtUiTest();

private:
    Project *m_pProject;
};
