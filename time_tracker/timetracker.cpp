#include "timetracker.h"
#include "./ui_timetracker.h"
#include <QFontDatabase>
#include <QPainter>
#include <QResizeEvent>
#include <QStyleOption>

// =================== Constructors & Destructors ===================
TimeTracker::TimeTracker(QWidget *parent) : QWidget(parent), ui(new Ui::TimeTracker) {
    ui->setupUi(this);

    // Window Icon
    setWindowIcon(QIcon(":/icons/icons/program_icon.png"));

    // Inits (very british)
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

void TimeTracker::resizeEvent(QResizeEvent *event) {
    QSize newSize = event->size(); // new size
    int sideMenuWidth = isMenuOpen ? 200 : 50; // gatting the current menu width

    // New geometries
    sideMenu->setGeometry(0, 0, sideMenuWidth, newSize.height());
    stackedWidget->setGeometry(sideMenuWidth, 0, newSize.width() - sideMenuWidth, newSize.height());

    // Call the base class implementation (not necessary, but it'll be better with this line written)
    QWidget::resizeEvent(event);
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


    // Menu Button
    menuButton = new QPushButton(this);
    menuButton->setFixedSize(50,50);
    menuButton->setObjectName("menuButton");

    leftArrowThemeSettingButton = new QPushButton("<", settPage);
    leftArrowThemeSettingButton->setFixedSize(30,30);
    leftArrowThemeSettingButton->setObjectName("leftArrowThemeSettingButton");

    rightArrowThemeSettingButton = new QPushButton(">", settPage);
    rightArrowThemeSettingButton->setFixedSize(30,30);
    rightArrowThemeSettingButton->setObjectName("rightArrowThemeSettingButton");

    // Task button
    addTaskButton = new QPushButton("+", listPage);
    addTaskButton->setFixedSize(50,50);
    addTaskButton->setObjectName("addTaskButton");


    // Setting cursor for every button
    menuButton->setCursor(Qt::PointingHandCursor);
    listButton->setCursor(Qt::PointingHandCursor);
    statButton->setCursor(Qt::PointingHandCursor);
    settButton->setCursor(Qt::PointingHandCursor);
    leftArrowThemeSettingButton->setCursor(Qt::PointingHandCursor);
    rightArrowThemeSettingButton->setCursor(Qt::PointingHandCursor);
    addTaskButton->setCursor(Qt::PointingHandCursor);
}

void TimeTracker::setAnimations() {
    // Animation for side menu
    menuAnimation = new QPropertyAnimation(sideMenu, "size");
    menuAnimation->setDuration(300); // Change number for change duration of animation (in ms)

    // Animation for widget
    widgetGeometryAnimation = new QPropertyAnimation(stackedWidget, "geometry");
    widgetGeometryAnimation->setDuration(300);
}

void TimeTracker::initLabels() {
    listLabel = new QLabel("This is List Page!", listPage);
    statLabel = new QLabel("This is Statistic Page!", statPage);

    themeLabel = new QLabel("Theme", settPage);
    themeLabel->setObjectName("themeLabel");

    currentThemeLabel = new QLabel(" Dark ", settPage);
    currentThemeLabel->setObjectName("currentThemeLabel");
}

void TimeTracker::initLayouts()
{
    // Theme setting layout
    themeSettingLayout = new QHBoxLayout(settPage);
    themeSettingLayout->addWidget(themeLabel);
    themeSettingLayout->addStretch();
    themeSettingLayout->addWidget(leftArrowThemeSettingButton);
    themeSettingLayout->addWidget(currentThemeLabel);
    themeSettingLayout->addWidget(rightArrowThemeSettingButton);
    themeSettingLayout->addStretch();


    // List page layouts
    listLayout = new QGridLayout(listPage);
    listLayout->setColumnMinimumWidth(0, 250);
    listLayout->setRowMinimumHeight(0, 150);

    listLayout->addWidget(addTaskButton, 3, 2);

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

    settLayout->addLayout(themeSettingLayout, 4, 2);

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
    connect(menuButton, &QPushButton::clicked, this, &TimeTracker::performMenuAnimations);

    //Connect a sideMenu buttons
    connect(listButton, &QPushButton::clicked, this, [this]() {
        stackedWidget->setCurrentIndex(0);
        if (isMenuOpen) {
            performMenuAnimations();
        }
    });

    connect(statButton, &QPushButton::clicked, this, [this]() {
        stackedWidget->setCurrentIndex(1);
        if (isMenuOpen) {
            performMenuAnimations();
        }
    });

    connect(settButton, &QPushButton::clicked, this, [this]() {
        stackedWidget->setCurrentIndex(2);
        if (isMenuOpen) {
            performMenuAnimations();
        }
    });

    // Connect the theme setting arrows
    connect(leftArrowThemeSettingButton, &QPushButton::clicked, this, &TimeTracker::toggleStyle);
    connect(rightArrowThemeSettingButton, &QPushButton::clicked, this, &TimeTracker::toggleStyle);

}

void TimeTracker::initWidgets()
{
    //Pages
    listPage = new QWidget(this);
    statPage = new QWidget(this);
    settPage = new QWidget(this);

    //Stacked Widget
    stackedWidget = new QStackedWidget(this);
    stackedWidget->setGeometry(QRect(50, 0, width() - 50, height()));
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


// ==================== Style Functions & Style Slots =====================
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
    currentThemeLabel->setText(" Dark ");
}

void TimeTracker::setStyleWhite() {
    applyStyle(":/styles/qss/white.qss");
    currentStyle = "white";
    currentThemeLabel->setText(" Light ");
}

void TimeTracker::toggleStyle() {
    if (currentStyle == "white") {
        setStyleBlack();
    } else {
        setStyleWhite();
    }
}


// =================== Animation slots ===================
void TimeTracker::performMenuAnimations() {
    if (sideMenu->width() == 200) {
        menuAnimation->setStartValue(QSize(200, this->height()));
        menuAnimation->setEndValue(QSize(50, this->height()));
        widgetGeometryAnimation->setStartValue(QRect(200, 0, width() - 200, this->height()));
        widgetGeometryAnimation->setEndValue(QRect(50, 0, width() - 50, this->height()));
        isMenuOpen = false;
    } else {
        menuAnimation->setStartValue(QSize(50, this->height()));
        menuAnimation->setEndValue(QSize(200, this->height()));
        widgetGeometryAnimation->setStartValue(QRect(50, 0, width() - 50, this->height()));
        widgetGeometryAnimation->setEndValue(QRect(200, 0, width() - 200, this->height()));
        isMenuOpen = true;
    }
    menuAnimation->start();
    widgetGeometryAnimation->start();
}


// =================== Additional Functions ===================
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

