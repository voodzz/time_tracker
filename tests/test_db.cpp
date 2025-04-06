// test_database_manager.cpp
#include <QtTest>
#include "../src/core/DatabaseManager.h"
#include "../src/core/Task.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

// Если используется синглтон DatabaseManager::instance(), убедитесь, что он доступен

class TestDatabaseManager : public QObject
{
    Q_OBJECT

private slots:
    // Этот метод вызывается один раз перед запуском всех тестов
    void initTestCase()
    {
        // Настройка соединения с тестовой базой данных.
        // Используйте параметры, соответствующие вашей тестовой среде.
        bool ok = DatabaseManager::instance().openConnection("localhost", 5432, "time_tracker_db", "postgres", "642916");
        QVERIFY2(ok, "Database connection failed");
    }

    // Тестирование подключения
    void testConnection()
    {
        QVERIFY(DatabaseManager::instance().openConnection("localhost", 5432, "time_tracker_db", "postgres", "642916"));
    }

    // Тестирование вставки задачи
    void testInsertTask()
    {
        // Создаем тестовую задачу
        Task* task = new Task(0);  // ID может быть произвольным, так как база сама присвоит SERIAL
        task->updateTask("Test Task", "Test Description", QDate::currentDate().addDays(1), 3);
        
        // Вставляем задачу в БД и проверяем результат
        bool inserted = DatabaseManager::instance().insertTask(task);
        bool deleted = DatabaseManager::instance().deleteTask(task->id());
        QVERIFY2(inserted, "Insert task failed");
        QVERIFY2(deleted, "Delete task failed");

        // Освобождаем ресурсы
        delete task;
    }

    // Тестирование обновления задачи
    void testUpdateTask()
    {
        // Создаем и вставляем задачу
        Task* task = new Task(0);
        task->updateTask("Update Test", "Initial Description", QDate::currentDate().addDays(1), 3);
        QVERIFY(DatabaseManager::instance().insertTask(task));

        // Обновляем данные задачи
        task->updateTask("Updated Task", "Updated Description", QDate::currentDate().addDays(2), 4);
        bool updated = DatabaseManager::instance().updateTask(task);
        QVERIFY2(updated, "Update task failed");

        bool deleted = DatabaseManager::instance().deleteTask(task->id());
        QVERIFY2(deleted, "Delete task failed");

        delete task;
    }

    // Тестирование удаления задачи
    void testDeleteTask()
    {
        // Создаем и вставляем задачу
        Task* task = new Task(0);
        task->updateTask("Delete Test", "Test Description", QDate::currentDate().addDays(1), 2);
        QVERIFY(DatabaseManager::instance().insertTask(task));

        int taskId = task->id(); // В реальной ситуации ID следует получать из базы
        bool deleted = DatabaseManager::instance().deleteTask(taskId);
        QVERIFY2(deleted, "Delete task failed");

        delete task;
    }

    // Новый тест: вывод информации о таблицах и колонках базы данных
    void testShowDatabaseSchema()
    {
        // Получаем соединение с базой данных
        QSqlDatabase db = QSqlDatabase::database(); // по умолчанию используется соединение, установленное в DatabaseManager

        QSqlQuery query(db);
        // Получаем список таблиц в публичной схеме
        QString tableSql = "SELECT table_name FROM information_schema.tables "
                           "WHERE table_schema = 'public' AND table_type = 'BASE TABLE';";
        QVERIFY2(query.exec(tableSql), qPrintable("Failed to retrieve table list: " + query.lastError().text()));

        qDebug() << "=== Database Schema ===";
        while(query.next()) {
            QString tableName = query.value(0).toString();
            qDebug() << "Table:" << tableName;

            // Для каждой таблицы получаем список колонок и их типы
            QSqlQuery colQuery(db);
            QString colSql = QString("SELECT column_name, data_type FROM information_schema.columns WHERE table_name = '%1';").arg(tableName);
            QVERIFY2(colQuery.exec(colSql), qPrintable("Failed to retrieve columns for " + tableName + ": " + colQuery.lastError().text()));

            while(colQuery.next()) {
                QString colName = colQuery.value(0).toString();
                QString dataType = colQuery.value(1).toString();
                qDebug() << "  Column:" << colName << "Type:" << dataType;
            }
        }
        qDebug() << "=== End of Schema ===";
    }

    // Этот метод вызывается один раз после выполнения всех тестов
    void cleanupTestCase()
    {
        DatabaseManager::instance().closeConnection();
    }
};

QTEST_MAIN(TestDatabaseManager)
#include "test_db.moc"
