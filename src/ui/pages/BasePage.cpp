#include "BasePage.h"

BasePage::BasePage(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_StyledBackground, true);
}

BasePage::~BasePage()
{
}
