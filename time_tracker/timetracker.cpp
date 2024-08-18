#include "timetracker.h"
#include "./ui_timetracker.h"


// ========================= Constructors & Destructors ===================
TimeTracker::TimeTracker(QWidget *parent) : QWidget(parent), ui(new Ui::TimeTracker) {
    ui->setupUi(this);

    initButtons();
    initMenues();
    initQVBoxLayout();
    setAnimations();
    initConnections();

    // TODO: Create separate function(s) for this!!!
    currentStyle = "black";
    applyStyle(":/qss/black.qss");

}

TimeTracker::~TimeTracker()
{
    delete ui;
}


// ========================== Init Functions ==========================
void TimeTracker::initButtons()
{
    // Buttons in side menu
    option1 = new QPushButton("Option 1", ui->sideMenu);
    option2 = new QPushButton("Option 2", ui->sideMenu);
    styleButton = new QPushButton("Theme", ui->sideMenu);
}

void TimeTracker::setAnimations()
{
    // Animation for side menu
    animation = new QPropertyAnimation(ui->sideMenu, "geometry");
    animation->setDuration(400); // Change number for change duration of animation (in ms)
}

void TimeTracker::initQVBoxLayout()
{

    // Layout for side menu, idk what is it, but it neccessary
    menuLayout = new QVBoxLayout(ui->sideMenu);
    menuLayout->addSpacing(75);  // Make a space so that the menuButton doesn't overlap the sideMenu buttons
    menuLayout->addWidget(option1);
    menuLayout->addWidget(option2);
    menuLayout->addWidget(styleButton);
    menuLayout->addStretch(); // It does it beautiful :)
}

void TimeTracker::initConnections()
{
    // Connect a menuButton
    connect(ui->menuButton, &QPushButton::clicked, [this]() {
        if (ui->sideMenu->x() < 0) {
            animation->setStartValue(QRect(-ui->sideMenu->width(), 0, 250, height()));
            animation->setEndValue(QRect(0, 0, 250, height()));
        } else {
            animation->setStartValue(QRect(0, 0, 250, height()));
            animation->setEndValue(QRect(-ui->sideMenu->width(), 0, 250, height()));
        }
        animation->start();
    });
    // Connect a styleButton
    connect(styleButton, &QPushButton::clicked, this, &TimeTracker::toggleStyle);
}

void TimeTracker::initMenues()
{
    ui->sideMenu->move(-ui->sideMenu->width(), 0);
}


// =============================== Style Functions & Style Slots =====================
void TimeTracker::applyStyle(const QString &styleFile)
{
    QFile file(styleFile);
    if (file.open(QFile::ReadOnly)) {
        QTextStream stream(&file);
        QString qss = stream.readAll();
        file.close();
        qApp->setStyleSheet(qss);
    }
}


void TimeTracker::toggleStyle()
{
    if (currentStyle == "white") {
        applyStyle(":/qss/black.qss");
        currentStyle = "black";
    } else {
        applyStyle(":/qss/white.qss");
        currentStyle = "white";
    }
}
