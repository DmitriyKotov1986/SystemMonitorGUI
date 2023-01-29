#include "tconfig.h"

//Qt
#include <QSettings>
#include <QFileInfo>
#include <QDebug>
#include <QDir>
#include <QTime>

//static for singleton
static TConfig* config_ptr = nullptr;

TConfig* TConfig::config(const QString& configFileName)
{
    Q_ASSERT_X(!((config_ptr == nullptr) && configFileName.isEmpty()), "static TConfig* TConfig::config(const QString& configFileName)", "Object was not created");

    if (config_ptr == nullptr)
    {
        config_ptr = new TConfig(configFileName);
    }

    return config_ptr;
};

void TConfig::deleteConfig()
{
    Q_ASSERT_X(config_ptr != nullptr , "static void TConfig::deleteConfig()", "Object was already deleted");

    if (config_ptr != nullptr)
    {
        delete config_ptr;

        config_ptr = nullptr;
    }
};

//class
TConfig::TConfig(const QString& configFileName) :
    _configFileName(configFileName)
{
    if (_configFileName.isEmpty())
    {
        _errorString = "Configuration file name cannot be empty";
        return;
    }
    if (!QFileInfo::exists(_configFileName))
    {
        _errorString = "Configuration file not exist. File name: " + _configFileName;
        return;
    }

    qDebug() << QString("%1 %2").arg(QTime::currentTime().toString("hh:mm:ss")).arg("Reading configuration from " +  _configFileName);

    QSettings ini(_configFileName, QSettings::IniFormat);

    QStringList groups = ini.childGroups();
    if (!groups.contains("DATABASE"))
    {
        _errorString = "Configuration file not contains [DATABASE] group";
        return;
    }

    //Database
    ini.beginGroup("DATABASE");
    _db_Driver = ini.value("Driver", "QODBC").toString();
    _db_DBName = ini.value("DataBase", "SystemMonitorDB").toString();
    _db_UserName = ini.value("UID", "").toString();
    _db_Password = ini.value("PWD", "").toString();
    _db_ConnectOptions = ini.value("ConnectionOprions", "").toString();
    _db_Port = ini.value("Port", "").toUInt();
    _db_Host = ini.value("Host", "localhost").toString();
    ini.endGroup();

    ini.beginGroup("SYSTEM");
    _sys_DebugMode = ini.value("DebugMode", "0").toBool();
    _sys_PathForSaveFile = ini.value("PathForSaveFile").toString();
    ini.endGroup();

    ini.beginGroup("GUI");
    auto GUIKeys = ini.allKeys();
    for (const auto& key: GUIKeys)
    {
        _windowsParams[key] = ini.value(key);
    }
    ini.endGroup();
}

TConfig::~TConfig()
{
    save();
}

bool TConfig::save()
{
    QSettings ini(_configFileName, QSettings::IniFormat);

    if (!ini.isWritable())
    {
        _errorString = "Can not write configuration file " +  _configFileName;

        return false;
    }

    ini.beginGroup("System");
    ini.setValue("PathForSaveFile", _sys_PathForSaveFile);
    ini.endGroup();

    ini.remove("GUI");
    ini.beginGroup("GUI");
    for (auto windowsParamsIter = _windowsParams.begin(); windowsParamsIter != _windowsParams.end(); ++windowsParamsIter)
    {
        ini.setValue(windowsParamsIter.key(), windowsParamsIter.value());
    }
    ini.endGroup();

    ini.sync();

    qDebug() << QString("%1 %2").arg(QTime::currentTime().toString("hh:mm:ss")).arg("Save configuration to " +  _configFileName);

    return true;
}

void TConfig::clearWindowParam()
{
    _windowsParams.clear();
}

static QVariant defaultWindowParam;

const QVariant& TConfig::windowParam(const QString &windowName, const QString &paramName) const
{
    auto key = QString("%1_%2").arg(windowName).arg(paramName);

    auto it = _windowsParams.find(key);

    if (it != _windowsParams.end())
    {
        return *it;
    }

    return defaultWindowParam;
}

void TConfig::setWindowParam(const QString &windowName, const QString &paramName, const QVariant &value)
{
    if ((windowName.isEmpty()) || paramName.isEmpty())
    {
        return;
    }

    auto key = QString("%1_%2").arg(windowName).arg(paramName);

    _windowsParams[key] = value;
}

