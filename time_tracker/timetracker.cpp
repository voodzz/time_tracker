#include "timetracker.h"
#include "./ui_timetracker.h"
#include <QTabBar>

TimeTracker::TimeTracker(QWidget *parent) : QWidget(parent), ui(new Ui::TimeTracker) {
    ui->setupUi(this);
    ui->tabWidget->tabBar()->setDocumentMode(true);
    ui->tabWidget->tabBar()->setExpanding(true);
}

TimeTracker::~TimeTracker()
{
    delete ui;
}
