#ifndef PROFILEPAGE_H
#define PROFILEPAGE_H

#include "BasePage.h"
#include <QLabel>
#include <QVBoxLayout>

class ProfilePage : public BasePage
{
    Q_OBJECT
public:
    explicit ProfilePage(QWidget *parent = nullptr);
    ~ProfilePage();

protected:
    void setupUi() override;

private:
    QVBoxLayout *m_layout;
    QLabel *m_titleLabel;
};

#endif // PROFILEPAGE_H
