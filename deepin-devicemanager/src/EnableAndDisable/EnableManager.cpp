// 项目自身文件
#include "EnableManager.h"
#include "DBusInterface.h"
#include "DeviceManager.h"

// Qt库文件
#include <QDebug>
#include <QProcess>
#include <QTime>

// 其它头文件
#include <stdlib.h>

EnableManager *EnableManager::s_Instance = nullptr;

EnableManager::EnableManager()
{

}

EnableDeviceStatus EnableManager::enableDeviceByInput(bool enable, int id)
{
    if (enable == isDeviceEnable(id))
        return EDS_Faild;

    // 通过ID禁用启用设备
    QString cmd = QString("xinput %1 %2").arg(enable ? "enable" : "disable").arg(id);
    QProcess process;
    int msecs = -1;
    process.start(cmd);
    process.waitForFinished(msecs);
    int exitCode = process.exitCode();
    QString output = process.readAllStandardOutput();
    if (exitCode == 0)
        return EDS_Success;

    return EDS_Faild;
}

bool EnableManager::isDeviceEnable(const QString &name)
{
    // 根据输入设备名称判断设备禁用启用状态
    QString cmd = "xinput list-props \"" + name + "\" ";
    QProcess process;
    int msecs = -1;
    process.start(cmd);
    process.waitForFinished(msecs);
    QString output = process.readAllStandardOutput();
    QStringList listOutput = output.split("\n");

    // 获取禁用启用信息
    foreach (const QString &str, listOutput) {
        if (!str.contains("Device Enabled"))
            continue;

        QStringList items = str.trimmed().split(":");
        if (items.size() != 2)
            return true;

        if (items[1].trimmed() == "1")
            return true;
        else
            return false;
    }
    return true;
}

bool EnableManager::isDeviceEnable(int id)
{
    // 根据输入设备ID判断设备禁用启用状态
    QString cmd = QString("xinput list-props %1").arg(id);
    QProcess process;
    int msecs = -1;
    process.start(cmd);
    process.waitForFinished(msecs);
    QString output = process.readAllStandardOutput();
    QStringList listOutput = output.split("\n");

    // 获取禁用启用信息
    foreach (const QString &str, listOutput) {
        if (!str.contains("Device Enabled"))
            continue;

        QStringList items = str.trimmed().split(":");
        if (items.size() != 2)
            return true;

        // 1:启用状态
        if (items[1].trimmed() == "1")
            return true;
        else
            return false;
    }
    return true;
}

EnableDeviceStatus EnableManager::enableDeviceByDriver(bool enable, const QString &driver)
{
    // 生成命令
    QString cmd;
    if (enable)
        cmd = QString("insmod %1").arg(getDriverPath(driver));
    else
        cmd = QString("rmmod %1").arg(driver);

    // 通知后台执行禁用操作
    if (DBusInterface::getInstance()->execDriverOrder(cmd))
        return EDS_Success;
    else
        return EDS_Faild;
}

bool EnableManager::isDeviceEnableByDriver(const QString &driver)
{
    if (driver == "")
        return false;

    // 获取lsmod信息
    QString cmd = "lsmod";
    QProcess process;
    int msecs = -1;
    process.start(cmd);
    process.waitForFinished(msecs);
    QString output = process.readAllStandardOutput();
    QStringList drivers = output.split("\n");

    // 判断驱动是否在lsmod列表中
    foreach (const QString &d, drivers) {
        if (d.startsWith(driver))
            return true;
    }

    if (driver.contains("loongson-audio", Qt::CaseInsensitive))
        return true;

    /*
    // 获取cat /boot/config* | grep '=y'信息
    */
    QList<QMap<QString, QString>> cmdInfo = DeviceManager::instance()->cmdInfo("dr_config");
    foreach (auto info, cmdInfo) {
        if (info["drivers"].contains(driver, Qt::CaseInsensitive))
            return true;
    }
    return false;
}

EnableDeviceStatus EnableManager::enablePrinter(const QString &name, bool enable)
{
    // 打印机禁用、启用
    QString cmd;
    if (true == enable)
        cmd = "cupsenable " + name;
    else
        cmd = "cupsdisable " + name;

    QProcess process;
    int msecs = -1;
    process.start(cmd);
    process.waitForFinished(msecs);
    QString output = process.readAllStandardOutput();
    if (output == "")
        return EDS_Success;
    else
        return EDS_Faild;
}

EnableDeviceStatus EnableManager::enableNetworkByIfconfig(const QString &logicalName, bool enable)
{
    // 生成命令
    QString cmd;
    if (enable)
        cmd = QString("ifconfig %1 up").arg(logicalName);
    else
        cmd = QString("ifconfig %1 down").arg(logicalName);

    // 执行命令
    if (DBusInterface::getInstance()->execIfconfigOrder(cmd))
        return EDS_Success;
    else
        return EDS_Faild;
}

bool EnableManager::isNetworkEnableByIfconfig(const QString &logicalName)
{
    // 获取ifconfig信息
    QString cmd = "ifconfig";
    QProcess process;
    int msecs = -1;
    process.start(cmd);
    process.waitForFinished(msecs);
    QString output = process.readAllStandardOutput();

    // 判断网卡是否通过ifconfig配置
    QStringList items = output.split("\n\n");
    foreach (const QString &item, items) {
        if (item.startsWith(logicalName))
            return true;
    }
    return false;
}

int EnableManager::getDeviceID(const QString &name, const QString &key)
{
    // 先判断有没有同名
    QString cmd = "xinput list";
    QProcess process;
    int msecs = -1;
    process.start(cmd);
    process.waitForFinished(msecs);
    QString output = process.readAllStandardOutput();
    QStringList items = output.split("\n");

    foreach (const QString &item, items) {
        QRegExp re = QRegExp(".*↳(.*)id=([0-9]{1,2}).*");
        if (re.exactMatch(item)) {
            QString n = re.cap(1).trimmed();
            int curId = re.cap(2).toInt();
            if (n != name)
                continue;
            if (isDeviceId(curId, key))
                return curId;
        }
    }
    return -1;
}

bool EnableManager::isDeviceId(const int &id, const QString key)
{
    QString cmd = QString("xinput list-props %1").arg(id);
    QProcess process;
    int msecs = -1;
    process.start(cmd);
    process.waitForFinished(msecs);

    QString output = process.readAllStandardOutput();
    QStringList items = output.split("\n");
    foreach (const QString &item, items) {
        if (!item.contains(key))
            continue;
        else
            return true;
    }

    return false;
}

QString EnableManager::getDriverPath(const QString &driver)
{
    // 执行modinfo命令
    QString path;
    QString cmd = QString("modinfo %1").arg(driver);
    QProcess process;
    int msecs = -1;
    process.start(cmd);
    process.waitForFinished(msecs);

    // 获取驱动路径
    QString output = process.readAllStandardOutput();
    QStringList lst = output.split("\n");
    foreach (const QString &item, lst) {
        if (item.startsWith("filename")) {
            QStringList kv = item.split(":");
            if (kv.size() == 2)
                path = kv[1].trimmed();
            break;
        }
    }
    return path;
}

