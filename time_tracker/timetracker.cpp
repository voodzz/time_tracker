#include "timetracker.h"
#include "./ui_timetracker.h"
#include <QFontDatabase>
#include <QPainter>
#include <QResizeEvent>
#include <QStyleOption>

// ========================= Constructors & Destructors ===================
TimeTracker::TimeTracker(QWidget *parent) : QWidget(parent), ui(new Ui::TimeTracker) {
    ui->setupUi(this);

    // Window Icon
    setWindowIcon(QIcon(":/icons/icons/program_icon.png"));

    // Inits
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
    listButton->setObjectName("listButton");

    statButton = new QPushButton("Statistics", sideMenu);
    statButton->setObjectName("statButton");

    settButton = new QPushButton("Settings", sideMenu);
    settButton->setObjectName("settButton");

    styleButton = new QPushButton("", settPage);
    styleButton->setObjectName("styleButton");


    // Menu Button
    menuButton = new QPushButton(this);
    menuButton->setFixedSize(50,50);
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
    animation = new QPropertyAnimation(sideMenu, "size");
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
    // List page layouts
    listLayout = new QGridLayout(listPage);
    listLayout->setColumnMinimumWidth(0, 250);
    listLayout->setRowMinimumHeight(0, 150);

    listLayout->addWidget(listLabel, 1, 1);

    listLayout->setVerticalSpacing(50);
    listLayout->setHorizontalSpacing(25);

    // Statistic page layout
    statLayout = new QGridLayout(statPage);
    statLayout->setColumnMinimumWidth(0, 250);
    statLayout->setRowMinimumHeight(0, 150);

    statLayout->addWidget(statLabel, 1, 1);

    statLayout->setVerticalSpacing(50);
    statLayout->setHorizontalSpacing(25);

    // Settings page layout
    settLayout = new QGridLayout(settPage);
    settLayout->setColumnMinimumWidth(0, 250);
    settLayout->setRowMinimumHeight(0, 150);

    settLayout->addWidget(styleButton, 1, 1);
    settLayout->addWidget(settLabel, 1, 2);
    settLayout->addWidget(new QLabel("Second Label", settPage), 2, 2);
    settLayout->addWidget(new QLabel("Third Label", settPage), 3, 2);

    settLayout->setVerticalSpacing(50);
    settLayout->setHorizontalSpacing(25);


    // Layout for side menu
    menuLayout = new QVBoxLayout(sideMenu);
    menuLayout->addSpacing(75);  // Make a space so that the menuButton doesn't overlap the sideMenu buttons

    menuLayout->addWidget(listButton);
    menuLayout->addWidget(statButton);
    menuLayout->addWidget(settButton);

    menuLayout->addStretch(); // It does it beautiful :)
    menuLayout->setContentsMargins(0, 0, 0, 0); // Remove margins on all sides (left, top, right, bottom)
    menuLayout->setSpacing(0); // Set spacing between buttons to zero
}

void TimeTracker::initConnections()
{
    // Connect a menuButton
    connect(menuButton, &QPushButton::clicked, [this]() {
        if (sideMenu->width() == 200) {
            animation->setStartValue(QSize(200, this->height()));
            animation->setEndValue(QSize(50, this->height()));

        } else {
            animation->setStartValue(QSize(50, this->height()));
            animation->setEndValue(QSize(200, this->height()));

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
    statPage = new QWidget(this);
    settPage = new QWidget(this);

    //Stacked Widget
    stackedWidget = new QStackedWidget(this);
    stackedWidget->addWidget(listPage);
    stackedWidget->addWidget(statPage);
    stackedWidget->addWidget(settPage);
    stackedWidget->setCurrentIndex(0);

    //Side Menu
    sideMenu = new QWidget(this);
    sideMenu->setGeometry(QRect(0,0, 50, 768));
    sideMenu->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    sideMenu->setObjectName("sideMenu");
    sideMenu->setMinimumSize(50, 768);
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


// ============================ Additional Functions ===================
void TimeTracker::enableButtons() {
    listButton->setEnabled(true);
    statButton->setEnabled(true);
    settButton->setEnabled(true);
    styleButton->setEnabled(true);
}

void TimeTracker::disableButtons() {
    listButton->setEnabled(false);
    statButton->setEnabled(false);
    settButton->setEnabled(false);
    styleButton->setEnabled(false);
}

