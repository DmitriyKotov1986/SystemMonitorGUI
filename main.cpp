#include <QApplication>
#include <QCommandLineParser>

//My
#include "mainwindow.h"
#include "Common/common.h"

using namespace Common;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setApplicationName("SystemMonitorGUI");
    a.setOrganizationName("OOO 'SA'");
    a.setApplicationVersion(QString("Version:0.1a Build: %1 %2").arg(__DATE__).arg(__TIME__));

    setlocale(LC_CTYPE, ""); //настраиваем локаль

    if (checkAlreadyRun())
    {
        qDebug() << QString("WARNING: %1 already running.").arg(QCoreApplication::applicationName());
    }

    //Создаем парсер параметров командной строки
    QCommandLineParser parser;
    parser.setApplicationDescription("Program for work database SystemMonitor on server.");
    parser.addHelpOption();
    parser.addVersionOption();

    //добавляем опцию Config
    QCommandLineOption Config(QStringList() << "Config", "Config file name", "ConfigFileNameValue", QString("%1.ini").arg(QCoreApplication::applicationName()));
    parser.addOption(Config);

    //парсим опции командной строки
    parser.process(a);

    //читаем конфигурацию из файла
    QString configFileName = parser.value(Config);
    if (!parser.isSet(Config))
    {
        configFileName = a.applicationDirPath() + "/" + parser.value(Config);
    }

    //Читаем конигурацию
    TConfig* cnf = TConfig::config(configFileName);
    if (cnf->isError())
    {
        QString msg = "Error load configuration: " + cnf->errorString();
        qCritical() << QString("%1 %2").arg(QTime::currentTime().toString("hh:mm:ss")).arg(msg);
        Common::writeLogFile("ERR>", msg);

        exit(EXIT_CODE::LOAD_CONFIG_ERR); // -1
    }


    //показываем основное окно
    MainWindow w;
    w.show();

    return a.exec();
}
