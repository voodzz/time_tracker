#include "SideMenu.h"
#include <QIcon>
#include <QDebug>

SideMenu::SideMenu(QWidget *parent)
    : QWidget(parent), m_menuWidth(m_collapsedWidth), m_isOpen(false)
{
    initWidgets();
    initLayout();
    initConnections();
    updateMenuAppearance();

    setFixedWidth(m_menuWidth);

    m_animation = new QPropertyAnimation(this, "menuWidth", this);
    m_animation->setDuration(300);

    toggleMenu();
    toggleMenu();
}

void SideMenu::initWidgets()
{
    menuButton = new QPushButton(this);
    menuButton->setToolTip("Menu");
    menuButton->setObjectName("menuButton");

    listButton = new QPushButton(this);
    listButton->setToolTip("Tasks");
    listButton->setObjectName("listButton");

    histButton = new QPushButton(this);
    histButton->setToolTip("History");
    histButton->setObjectName("histButton");

    settButton = new QPushButton(this);
    settButton->setToolTip("Settings");
    settButton->setObjectName("settButton");

    profButton = new QPushButton(this);
    profButton->setToolTip("Profile");
    profButton->setObjectName("profButton");
}

void SideMenu::initLayout()
{
    menuLayout = new QVBoxLayout(this);
    menuLayout->setContentsMargins(0, 0, 0, 0);
    menuLayout->setSpacing(0);
    menuLayout->addWidget(menuButton);
    menuLayout->addWidget(listButton);
    menuLayout->addWidget(histButton);
    menuLayout->addWidget(settButton);
    menuLayout->addWidget(profButton);
    menuLayout->addStretch();
    setLayout(menuLayout);
}

void SideMenu::initConnections()
{
    connect(menuButton, &QPushButton::clicked, this, &SideMenu::toggleMenu);
    connect(listButton, &QPushButton::clicked, [this]() { emit menuItemClicked(0); });
    connect(histButton, &QPushButton::clicked, [this]() { emit menuItemClicked(1); });
    connect(settButton, &QPushButton::clicked, [this]() { emit menuItemClicked(2); });
    connect(profButton, &QPushButton::clicked, [this]() { emit menuItemClicked(3); });
}

void SideMenu::setMenuWidth(int width)
{
    m_menuWidth = width;
    setFixedWidth(m_menuWidth);
    updateMenuAppearance();
}

void SideMenu::toggleMenu()
{
    int startWidth = m_isOpen ? m_expandedWidth : m_collapsedWidth;
    int endWidth = m_isOpen ? m_collapsedWidth : m_expandedWidth;

    m_animation->stop();
    m_animation->setStartValue(startWidth);
    m_animation->setEndValue(endWidth);
    m_animation->start();

    m_isOpen = !m_isOpen;
    emit menuToggled(m_isOpen);
}

void SideMenu::updateMenuAppearance()
{
    if (m_isOpen) {
        listButton->setText("List");
        histButton->setText("History");
        settButton->setText("Settings");
        profButton->setText("Profile");
    } else {
        listButton->setText("");
        histButton->setText("");
        settButton->setText("");
        profButton->setText("");
    }
}
