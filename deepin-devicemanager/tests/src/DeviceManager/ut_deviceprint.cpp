/*
* Copyright (C) 2019 ~ 2020 UnionTech Software Technology Co.,Ltd
*
* Author:      zhangkai <zhangkai@uniontech.com>
* Maintainer:  zhangkai <zhangkai@uniontech.com>
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "DevicePrint.h"
#include "DeviceInfo.h"
#include "EnableManager.h"

#include "ut_Head.h"
#include "stub.h"

#include <QCoreApplication>
#include <QPaintEvent>
#include <QPainter>

#include <gtest/gtest.h>

class UT_DevicePrint : public UT_HEAD
{
public:
    void SetUp()
    {
        m_devicePrint = new DevicePrint;
    }
    void TearDown()
    {
        delete m_devicePrint;
    }
    DevicePrint *m_devicePrint;
};

void ut_print_setmap(QMap<QString, QString> &mapinfo)
{
    mapinfo.insert("printer-info", "Canon iR-ADV C3720 22.21");
    mapinfo.insert("Name", "Canon-iR-ADV-C3720-UFR-II");
    mapinfo.insert("device-uri", "socket://10.4.12.241");
    mapinfo.insert("printer-state", "3");
}

TEST_F(UT_DevicePrint, UT_DevicePrint_setInfo)
{
    QMap<QString, QString> mapinfo;
    ut_print_setmap(mapinfo);
    m_devicePrint->setInfo(mapinfo);

    EXPECT_STREQ("Canon", m_devicePrint->m_Vendor.toStdString().c_str());
    EXPECT_STREQ("iR-ADV C3720 22.21", m_devicePrint->m_Model.toStdString().c_str());
    EXPECT_STREQ("Canon-iR-ADV-C3720-UFR-II", m_devicePrint->m_Name.toStdString().c_str());
    EXPECT_STREQ("socket://10.4.12.241", m_devicePrint->m_URI.toStdString().c_str());
    EXPECT_STREQ("3", m_devicePrint->m_Status.toStdString().c_str());
    EXPECT_STREQ("socket", m_devicePrint->m_InterfaceType.toStdString().c_str());
}

TEST_F(UT_DevicePrint, UT_DevicePrint_name)
{
    QMap<QString, QString> mapinfo;
    ut_print_setmap(mapinfo);
    m_devicePrint->setInfo(mapinfo);

    QString name = m_devicePrint->name();
    EXPECT_STREQ("Canon-iR-ADV-C3720-UFR-II", name.toStdString().c_str());
}

TEST_F(UT_DevicePrint, UT_DevicePrint_driver)
{
    QString driver = m_devicePrint->driver();
    EXPECT_STREQ("", driver.toStdString().c_str());
}

TEST_F(UT_DevicePrint, UT_DevicePrint_subTitle)
{
    QMap<QString, QString> mapinfo;
    ut_print_setmap(mapinfo);
    m_devicePrint->setInfo(mapinfo);

    QString title = m_devicePrint->subTitle();
    EXPECT_STREQ("Canon-iR-ADV-C3720-UFR-II", title.toStdString().c_str());
}

TEST_F(UT_DevicePrint, UT_DevicePrint_getOverviewInfo)
{
    QMap<QString, QString> mapinfo;
    ut_print_setmap(mapinfo);
    m_devicePrint->setInfo(mapinfo);

    QString overview = m_devicePrint->getOverviewInfo();
    EXPECT_STREQ("Canon-iR-ADV-C3720-UFR-II", overview.toStdString().c_str());
}

EnableDeviceStatus ut_print_enableprinter()
{
    return EnableDeviceStatus::EDS_Success;
}

TEST_F(UT_DevicePrint, UT_DevicePrint_setEnable_001)
{
    Stub stub;
    stub.set(ADDR(EnableManager, enablePrinter), ut_print_enableprinter);

    EXPECT_EQ(EnableDeviceStatus::EDS_Success, m_devicePrint->setEnable(true));
    EXPECT_EQ("3", m_devicePrint->m_Status);
}

TEST_F(UT_DevicePrint, UT_DevicePrint_setEnable_002)
{
    Stub stub;
    stub.set(ADDR(EnableManager, enablePrinter), ut_print_enableprinter);

    EXPECT_EQ(EnableDeviceStatus::EDS_Success, m_devicePrint->setEnable(false));
    EXPECT_EQ("5", m_devicePrint->m_Status);
}


TEST_F(UT_DevicePrint, UT_DevicePrint_enable)
{
    m_devicePrint->m_Status = "5";
    EXPECT_FALSE(m_devicePrint->enable());
}

TEST_F(UT_DevicePrint, UT_DevicePrint_initFilterKey)
{
    m_devicePrint->initFilterKey();
    EXPECT_EQ(17, m_devicePrint->m_FilterKey.size());
}

TEST_F(UT_DevicePrint, UT_DevicePrint_loadBaseDeviceInfo)
{
    QMap<QString, QString> mapinfo;
    ut_print_setmap(mapinfo);
    m_devicePrint->setInfo(mapinfo);

    m_devicePrint->loadBaseDeviceInfo();
    EXPECT_EQ(3, m_devicePrint->m_LstBaseInfo.size());

}

TEST_F(UT_DevicePrint, UT_DevicePrint_loadOtherDeviceInfo)
{
    m_devicePrint->loadOtherDeviceInfo();
    EXPECT_EQ(0, m_devicePrint->m_LstOtherInfo.size());
}

TEST_F(UT_DevicePrint, UT_DevicePrint_loadTableData_001)
{
    QMap<QString, QString> mapinfo;
    ut_print_setmap(mapinfo);
    m_devicePrint->setInfo(mapinfo);

    m_devicePrint->m_Status = "5";
    m_devicePrint->loadTableData();
    EXPECT_EQ(3, m_devicePrint->m_TableData.size());
}

TEST_F(UT_DevicePrint, UT_DevicePrint_loadTableData_002)
{
    QMap<QString, QString> mapinfo;
    ut_print_setmap(mapinfo);
    m_devicePrint->setInfo(mapinfo);

    m_devicePrint->m_Status = "3";
    m_devicePrint->loadTableData();
    EXPECT_EQ(3, m_devicePrint->m_TableData.size());
}
