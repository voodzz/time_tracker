#include "timetracker.h"
#include "./ui_timetracker.h"
#include <QPainter>
#include <QResizeEvent>
#include <QStyleOption>

// ========================= Constructors & Destructors ===================
TimeTracker::TimeTracker(QWidget *parent) : QWidget(parent), ui(new Ui::TimeTracker) {
    ui->setupUi(this);

    // Window Icon
    setWindowIcon(QIcon(":/icons/icons/program_icon.png"));

    initWidgets();
    initButtons();
    initLabels();
    initLayouts();
    setAnimations();
    initConnections();

    // Dark theme by default
    setStyleBlack();
}

TimeTracker::~TimeTracker() {
    delete ui;
}


// ========================== Init Functions ==========================
void TimeTracker::initButtons() {
    // Buttons in side menu
    listButton = new QPushButton("List", sideMenu);
    statButton = new QPushButton("Statistic", sideMenu);
    settButton = new QPushButton("Setting", sideMenu);
    styleButton = new QPushButton("Theme", sideMenu);

    // Menu Button
    menuButton = new QPushButton(this);
    menuButton->setFixedSize(50,50);
    menuButton->move(10, 10);
    menuButton->setObjectName("menuButton");

    // Setting cursor for every button
    menuButton->setCursor(Qt::PointingHandCursor);
    listButton->setCursor(Qt::PointingHandCursor);
    statButton->setCursor(Qt::PointingHandCursor);
    settButton->setCursor(Qt::PointingHandCursor);
    styleButton->setCursor(Qt::PointingHandCursor);
}

void TimeTracker::setAnimations()
{
    // Animation for side menu
    animation = new QPropertyAnimation(sideMenu, "geometry");
    animation->setDuration(400); // Change number for change duration of animation (in ms)
}

void TimeTracker::initLabels()
{
    listLabel = new QLabel("This is List Page!", listPage);
    statLabel = new QLabel("This is Statistic Page!", statPage);
    settLabel = new QLabel("This is Settings Page!", settPage);
}

void TimeTracker::initLayouts()
{
    //Pages Layouts
    listLayout = new QVBoxLayout(listPage);
    listLayout->addWidget(listLabel);
    statLayout = new QVBoxLayout(statPage);
    statLayout->addWidget(statLabel);
    settLayout = new QVBoxLayout(settPage);
    settLayout->addWidget(settLabel);


    // Layout for side menu, idk what is it, but it neccessary
    menuLayout = new QVBoxLayout(sideMenu);
    menuLayout->addSpacing(75);  // Make a space so that the menuButton doesn't overlap the sideMenu buttons
    menuLayout->addWidget(listButton);
    menuLayout->addWidget(statButton);
    menuLayout->addWidget(settButton);
    menuLayout->addWidget(styleButton);
    menuLayout->addStretch(); // It does it beautiful :)
}

void TimeTracker::initConnections()
{
    // Connect a menuButton
    connect(menuButton, &QPushButton::clicked, [this]() {
        if (sideMenu->x() < 0) {
            animation->setStartValue(QRect(-sideMenu->width(), 0, 250, height()));
            animation->setEndValue(QRect(0, 0, 250, height()));
        } else {
            animation->setStartValue(QRect(0, 0, 250, height()));
            animation->setEndValue(QRect(-sideMenu->width(), 0, 250, height()));
        }
        animation->start();
    });

    // Connect a styleButton
    connect(styleButton, &QPushButton::clicked, this, &TimeTracker::toggleStyle);

    //Connect a sideMenu buttons
    connect(listButton, &QPushButton::clicked, this, [this]() {
        stackedWidget->setCurrentIndex(0);
    });

    connect(statButton, &QPushButton::clicked, this, [this]() {
        stackedWidget->setCurrentIndex(1);
    });

    connect(settButton, &QPushButton::clicked, this, [this]() {
        stackedWidget->setCurrentIndex(2);
    });
}

void TimeTracker::initWidgets()
{
    //Pages
    listPage = new QWidget(this);
    listPage->setGeometry(0, 0, 1366, 768);
    statPage = new QWidget(this);
    settPage = new QWidget(this);

    //Side Menu
    sideMenu = new QWidget(this);
    sideMenu->setGeometry(QRect(0,0, 250, 768));
    sideMenu->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    sideMenu->move(-sideMenu->width(), 0);
    sideMenu->setObjectName("sideMenu");
    sideMenu->setMinimumSize(250, 768);

    //Stacked Widget
    stackedWidget = new QStackedWidget(this);
    stackedWidget->addWidget(listPage);
    stackedWidget->addWidget(statPage);
    stackedWidget->addWidget(settPage);
    stackedWidget->setCurrentIndex(0);

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

void TimeTracker::setStyleBlack() {
    applyStyle(":/styles/qss/black.qss");
    currentStyle = "black";
}

void TimeTracker::setStyleWhite() {
    applyStyle(":/styles/qss/white.qss");
    currentStyle = "white";
}


void TimeTracker::toggleStyle()
{
    if (currentStyle == "white") {
        setStyleBlack();
    } else {
        setStyleWhite();
    }
}

