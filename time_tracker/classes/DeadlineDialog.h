#ifndef DEADLINEDIALOG_H
#define DEADLINEDIALOG_H

#include <QCalendarWidget>
#include <QDialog>

class DeadlineDialog : public QDialog
{
    Q_OBJECT
public:
    DeadlineDialog(QWidget* parent = nullptr);
    QDate selectedDate() const;
private:
    QCalendarWidget* calendar;
};

#endif // DEADLINEDIALOG_H
