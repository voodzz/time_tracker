#ifndef HISTORYPAGE_H
#define HISTORYPAGE_H

#include "BasePage.h"
#include <QLabel>
#include <QVBoxLayout>

class HistoryPage : public BasePage
{
    Q_OBJECT
public:
    explicit HistoryPage(QWidget *parent = nullptr);
    ~HistoryPage();

protected:
    void setupUi() override;

private:
    QVBoxLayout *m_layout;
    QLabel *m_titleLabel;
};

#endif // HISTORYPAGE_H
