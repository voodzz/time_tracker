#include "Application.h"

int main(int argc, char *argv[])
{
    system("chcp 65001");
    Application app(argc, argv);
    return app.exec();
}
