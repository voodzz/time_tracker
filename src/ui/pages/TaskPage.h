#ifndef TASKPAGE_H
#define TASKPAGE_H

#include "BasePage.h"
#include <QLabel>
#include <QVBoxLayout>

class TaskPage : public BasePage
{
    Q_OBJECT
public:
    explicit TaskPage(QWidget *parent = nullptr);
    ~TaskPage();

protected:
    void setupUi() override;

private:
    QVBoxLayout *m_layout;
    QLabel *m_titleLabel;
};

#endif // TASKPAGE_H
