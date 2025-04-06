#include "TaskSettingsDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QDateEdit>
#include <QSpinBox>
#include <QPushButton>

// Constructor initializes UI and sets default values for the dialog fields
TaskSettingsDialog::TaskSettingsDialog(const QString &name,
                                       const QString &description,
                                       const QDate &deadline,
                                       int cycles,
                                       QWidget *parent)
    : QDialog(parent)
{
    setupUi();
    m_nameEdit->setText(name);
    m_descriptionEdit->setPlainText(description);
    m_deadlineEdit->setDate(deadline);
    m_cyclesSpinBox->setValue(cycles);
}

TaskSettingsDialog::~TaskSettingsDialog() { }

// Set up dialog UI components with consistent styling and object names
void TaskSettingsDialog::setupUi()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QLabel *nameLabel = new QLabel(tr("Task Name:"), this);
    m_nameEdit = new QLineEdit(this);
    m_nameEdit->setObjectName("tt_ts_nameEdit");

    QLabel *descLabel = new QLabel(tr("Description:"), this);
    m_descriptionEdit = new QTextEdit(this);
    m_descriptionEdit->setObjectName("tt_ts_descriptionEdit");

    QLabel *deadlineLabel = new QLabel(tr("Deadline:"), this);
    m_deadlineEdit = new QDateEdit(this);
    m_deadlineEdit->setObjectName("tt_ts_deadlineEdit");
    m_deadlineEdit->setCalendarPopup(true);
    m_deadlineEdit->setDisplayFormat("dd.MM.yyyy");

    QLabel *cyclesLabel = new QLabel(tr("Number of Cycles:"), this);
    m_cyclesSpinBox = new QSpinBox(this);
    m_cyclesSpinBox->setObjectName("tt_ts_cyclesSpinBox");
    m_cyclesSpinBox->setRange(1, 10);

    m_applyButton = new QPushButton(tr("Apply"), this);
    m_applyButton->setObjectName("tt_ts_applyButton");
    m_deleteButton = new QPushButton(tr("Delete Task"), this);
    m_deleteButton->setObjectName("tt_ts_deleteButton");

    mainLayout->addWidget(nameLabel);
    mainLayout->addWidget(m_nameEdit);
    mainLayout->addWidget(descLabel);
    mainLayout->addWidget(m_descriptionEdit);
    mainLayout->addWidget(deadlineLabel);
    mainLayout->addWidget(m_deadlineEdit);
    mainLayout->addWidget(cyclesLabel);
    mainLayout->addWidget(m_cyclesSpinBox);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(m_applyButton);
    buttonLayout->addWidget(m_deleteButton);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);

    // Connect buttons to corresponding slots
    connect(m_applyButton, &QPushButton::clicked, this, &TaskSettingsDialog::onApply);
    connect(m_deleteButton, &QPushButton::clicked, this, &TaskSettingsDialog::onDelete);
}

void TaskSettingsDialog::onApply()
{
    // Accept the dialog (indicating that the user applied the changes)
    accept();
}

void TaskSettingsDialog::onDelete()
{
    // Emit taskDeleted signal and reject the dialog
    emit taskDeleted();
    reject();
}

QString TaskSettingsDialog::taskName() const { return m_nameEdit->text(); }
QString TaskSettingsDialog::taskDescription() const { return m_descriptionEdit->toPlainText(); }
QDate TaskSettingsDialog::taskDeadline() const { return m_deadlineEdit->date(); }
int TaskSettingsDialog::taskCycles() const { return m_cyclesSpinBox->value(); }
