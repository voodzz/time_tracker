#include "timetracker.h"
#include "./ui_timetracker.h"

TimeTracker::TimeTracker(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TimeTracker)
{
    ui->setupUi(this);
}

TimeTracker::~TimeTracker()
{
    delete ui;
}
