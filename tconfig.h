#ifndef TCONFIG_H
#define TCONFIG_H

//QT
#include <QString>
#include <QFile>

class TConfig final
{
public:
    static TConfig* config(const QString& configFileName = "")
    {
        static TConfig* _config = nullptr;

        if (_config == nullptr){
            _config = new TConfig(configFileName);
        }

        return _config;
    };

private:
    explicit TConfig(const QString& configFileName);
    ~TConfig();

public:
    bool save();

    //[DATABASE]
    const QString& db_Driver() const { return _db_Driver; }
    const QString& db_DBName()  const { return _db_DBName; }
    const QString& db_UserName() const { return _db_UserName; }
    const QString& db_Password() const { return _db_Password; }
    const QString& db_ConnectOptions() const { return _db_ConnectOptions; }
    const QString& db_Host() const { return _db_Host; }
    quint16 db_Port() const { return _db_Port; }

    //[SYSTEM]
    bool sys_DebugMode() const { return _sys_DebugMode; }\
    const QString& errorString() const { return _errorString; }
    bool isError() const { return !_errorString.isEmpty(); }

private:
    const QString _configFileName;

    QString _errorString;

    //[DATABASE]
    QString _db_Driver;
    QString _db_DBName;
    QString _db_UserName;
    QString _db_Password;
    QString _db_ConnectOptions;
    QString _db_Host;
    quint16 _db_Port = 0;

    //[SYSTEM]
    bool _sys_DebugMode = false;
};

#endif // TCONFIG_H
