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
#include "DeviceNetwork.h"
#include "DeviceBios.h"
#include "EnableManager.h"

#include "ut_Head.h"
#include "stub.h"

#include <QCoreApplication>
#include <QPaintEvent>
#include <QPainter>

#include <gtest/gtest.h>

class UT_DeviceNetwork : public UT_HEAD
{
public:
    void SetUp()
    {
        m_deviceNetwork = new DeviceNetwork;
    }
    void TearDown()
    {
        delete m_deviceNetwork;
    }
    DeviceNetwork *m_deviceNetwork;
};

void ut_network_setlshwinfo(QMap<QString, QString> &mapinfo)
{
    mapinfo.insert("description", "description");
    mapinfo.insert("product", "product");
    mapinfo.insert("description", "description");
    mapinfo.insert("vendor", "vendor");
    mapinfo.insert("version", "version");
    mapinfo.insert("bus info", "bus info");
    mapinfo.insert("logical name", "enp2s0");
    mapinfo.insert("serial", "serial");
    mapinfo.insert("irq", "irq");
    mapinfo.insert("memory", "memory");
    mapinfo.insert("width", "width");
    mapinfo.insert("clock", "clock");
    mapinfo.insert("capabilities", "capabilities");
    mapinfo.insert("autonegotiation", "autonegotiation");
    mapinfo.insert("broadcast", "broadcast");
    mapinfo.insert("driver", "driver");
    mapinfo.insert("driverversion", "driverversion");
    mapinfo.insert("duplex", "duplex");
    mapinfo.insert("firmware", "firmware");
    mapinfo.insert("port", "port");
    mapinfo.insert("link", "link");
    mapinfo.insert("ip", "ip");
    mapinfo.insert("size", "size");
    mapinfo.insert("capacity", "capacity");
    mapinfo.insert("latency", "latency");
    mapinfo.insert("multicast", "multicast");
}

void ut_network_sethwinfomap(QMap<QString, QString> &mapinfo)
{
    mapinfo.insert("Device File", "enp2s0");
    mapinfo.insert("Model", "Model");
}

TEST_F(UT_DeviceNetwork, DeviceNetwork_UT_setInfoFromHwinfo_001)
{
    QMap<QString, QString> mapinfo;
    ut_network_sethwinfomap(mapinfo);
    m_deviceNetwork->m_LogicalName = "enp2s0";

    EXPECT_TRUE(m_deviceNetwork->setInfoFromHwinfo(mapinfo));
    EXPECT_STREQ("Model", m_deviceNetwork->m_Name.toStdString().c_str());
}

TEST_F(UT_DeviceNetwork, DeviceNetwork_UT_setInfoFromHwinfo_002)
{
    QMap<QString, QString> mapinfo;
    ut_network_sethwinfomap(mapinfo);
    m_deviceNetwork->m_LogicalName = "enp2s1";

    EXPECT_FALSE(m_deviceNetwork->setInfoFromHwinfo(mapinfo));
}

TEST_F(UT_DeviceNetwork, DeviceNetwork_UT_setInfoFromLshw)
{
    QMap<QString, QString> mapinfo;
    ut_network_setlshwinfo(mapinfo);

    m_deviceNetwork->setInfoFromLshw(mapinfo);
    EXPECT_STREQ("description", m_deviceNetwork->m_Model.toStdString().c_str());
    EXPECT_STREQ("product", m_deviceNetwork->m_Name.toStdString().c_str());
    EXPECT_STREQ("vendor", m_deviceNetwork->m_Vendor.toStdString().c_str());
    EXPECT_STREQ("version", m_deviceNetwork->m_Version.toStdString().c_str());
    EXPECT_STREQ("bus info", m_deviceNetwork->m_BusInfo.toStdString().c_str());
    EXPECT_STREQ("enp2s0", m_deviceNetwork->m_LogicalName.toStdString().c_str());
    EXPECT_STREQ("serial", m_deviceNetwork->m_MACAddress.toStdString().c_str());
    EXPECT_STREQ("irq", m_deviceNetwork->m_Irq.toStdString().c_str());
    EXPECT_STREQ("memory", m_deviceNetwork->m_Memory.toStdString().c_str());
    EXPECT_STREQ("width", m_deviceNetwork->m_Width.toStdString().c_str());
    EXPECT_STREQ("clock", m_deviceNetwork->m_Clock.toStdString().c_str());
    EXPECT_STREQ("capabilities", m_deviceNetwork->m_Capabilities.toStdString().c_str());
    EXPECT_STREQ("autonegotiation", m_deviceNetwork->m_Autonegotiation.toStdString().c_str());
    EXPECT_STREQ("broadcast", m_deviceNetwork->m_Broadcast.toStdString().c_str());
    EXPECT_STREQ("driver", m_deviceNetwork->m_Driver.toStdString().c_str());
    EXPECT_STREQ("driverversion", m_deviceNetwork->m_DriverVersion.toStdString().c_str());
    EXPECT_STREQ("duplex", m_deviceNetwork->m_Duplex.toStdString().c_str());
    EXPECT_STREQ("firmware", m_deviceNetwork->m_Firmware.toStdString().c_str());
    EXPECT_STREQ("link", m_deviceNetwork->m_Link.toStdString().c_str());
    EXPECT_STREQ("ip", m_deviceNetwork->m_Ip.toStdString().c_str());
    EXPECT_STREQ("size", m_deviceNetwork->m_Speed.toStdString().c_str());
    EXPECT_STREQ("capacity", m_deviceNetwork->m_Capacity.toStdString().c_str());
    EXPECT_STREQ("latency", m_deviceNetwork->m_Latency.toStdString().c_str());
    EXPECT_STREQ("multicast", m_deviceNetwork->m_Multicast.toStdString().c_str());
}

TEST_F(UT_DeviceNetwork, DeviceNetwork_UT_name)
{
    QMap<QString, QString> mapinfo;
    ut_network_setlshwinfo(mapinfo);
    m_deviceNetwork->setInfoFromLshw(mapinfo);

    QString name = m_deviceNetwork->name();
    EXPECT_STREQ("product", name.toStdString().c_str());
}

TEST_F(UT_DeviceNetwork, DeviceNetwork_UT_driver)
{
    QMap<QString, QString> mapinfo;
    ut_network_setlshwinfo(mapinfo);
    m_deviceNetwork->setInfoFromLshw(mapinfo);

    QString driver = m_deviceNetwork->driver();
    EXPECT_STREQ("driver", driver.toStdString().c_str());
}

TEST_F(UT_DeviceNetwork, DeviceNetwork_UT_subTitle)
{
    QMap<QString, QString> mapinfo;
    ut_network_setlshwinfo(mapinfo);
    m_deviceNetwork->setInfoFromLshw(mapinfo);

    QString title = m_deviceNetwork->subTitle();
    EXPECT_STREQ("product", title.toStdString().c_str());
}

TEST_F(UT_DeviceNetwork, DeviceNetwork_UT_getOverviewInfo)
{
    QMap<QString, QString> mapinfo;
    ut_network_setlshwinfo(mapinfo);
    m_deviceNetwork->setInfoFromLshw(mapinfo);

    QString overview = m_deviceNetwork->getOverviewInfo();
    EXPECT_STREQ("product", overview.toStdString().c_str());
}

bool ut_network_enableNetwork()
{
    return true;
}

bool ut_network_enable_true()
{
    return true;
}

bool ut_network_enable_false()
{
    return false;
}

TEST_F(UT_DeviceNetwork, DeviceNetwork_UT_setEnable)
{
    Stub stub;
    stub.set(ADDR(EnableManager, enableNetworkByIfconfig), ut_network_enableNetwork);
    EXPECT_TRUE(m_deviceNetwork->setEnable(true));
}

TEST_F(UT_DeviceNetwork, DeviceNetwork_UT_enable)
{
    Stub stub;
    stub.set(ADDR(EnableManager, isNetworkEnableByIfconfig), ut_network_enable_true);
    EXPECT_TRUE(m_deviceNetwork->enable());
}

TEST_F(UT_DeviceNetwork, DeviceNetwork_UT_correctCurrentLinkStatus)
{
    QMap<QString, QString> mapinfo;
    ut_network_setlshwinfo(mapinfo);
    mapinfo.insert("link", "link1");
    m_deviceNetwork->setInfoFromLshw(mapinfo);

    m_deviceNetwork->correctCurrentLinkStatus("link");
    EXPECT_STREQ("link", m_deviceNetwork->m_Link.toStdString().c_str());
}

TEST_F(UT_DeviceNetwork, DeviceNetwork_UT_logicalName)
{
    QMap<QString, QString> mapinfo;
    ut_network_setlshwinfo(mapinfo);
    m_deviceNetwork->setInfoFromLshw(mapinfo);

    QString logicalName = m_deviceNetwork->logicalName();
    EXPECT_STREQ("enp2s0", logicalName.toStdString().c_str());
}

TEST_F(UT_DeviceNetwork, DeviceNetwork_UT_initFilterKey)
{
    m_deviceNetwork->initFilterKey();
    EXPECT_EQ(3, m_deviceNetwork->m_FilterKey.size());
}

TEST_F(UT_DeviceNetwork, DeviceNetwork_UT_loadBaseDeviceInfo)
{
    QMap<QString, QString> mapinfo;
    ut_network_setlshwinfo(mapinfo);
    m_deviceNetwork->setInfoFromLshw(mapinfo);

    m_deviceNetwork->loadBaseDeviceInfo();
    EXPECT_EQ(8, m_deviceNetwork->m_LstBaseInfo.size());
}

TEST_F(UT_DeviceNetwork, DeviceNetwork_UT_loadOtherDeviceInfo)
{
    QMap<QString, QString> mapinfo;
    ut_network_setlshwinfo(mapinfo);
    m_deviceNetwork->setInfoFromLshw(mapinfo);

    m_deviceNetwork->loadOtherDeviceInfo();
    EXPECT_EQ(17, m_deviceNetwork->m_LstOtherInfo.size());
}

TEST_F(UT_DeviceNetwork, DeviceNetwork_UT_loadTableData_001)
{
    Stub stub;
    stub.set(ADDR(EnableManager, isNetworkEnableByIfconfig), ut_network_enable_true);

    m_deviceNetwork->loadTableData();
    EXPECT_EQ(3, m_deviceNetwork->m_TableData.size());
}

TEST_F(UT_DeviceNetwork, DeviceNetwork_UT_loadTableData_002)
{
    Stub stub;
    stub.set(ADDR(EnableManager, isNetworkEnableByIfconfig), ut_network_enable_false);

    m_deviceNetwork->loadTableData();
    EXPECT_EQ(3, m_deviceNetwork->m_TableData.size());
}
