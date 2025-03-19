#ifndef BASEPAGE_H
#define BASEPAGE_H

#include <QWidget>

class BasePage : public QWidget
{
    Q_OBJECT
public:
    explicit BasePage(QWidget *parent = nullptr);
    virtual ~BasePage();

protected:
    virtual void setupUi() = 0;
};

#endif // BASEPAGE_H
