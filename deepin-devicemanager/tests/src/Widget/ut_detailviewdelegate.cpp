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
#include "DetailViewDelegate.h"
#include "DetailTreeView.h"
#include "ut_Head.h"
#include "stub.h"

#include <DStyle>
#include <DApplication>

#include <QCoreApplication>
#include <QPaintEvent>
#include <QPainter>
#include <QCommonStyle>
#include <QStyle>
#include <QWidget>

#include <gtest/gtest.h>
DStyle *UT_DetailViewDelegate_style;
class UT_DetailViewDelegate : public UT_HEAD
{
public:
    void SetUp()
    {
        m_treeView = new DetailTreeView;
        m_dViewDelegate = new DetailViewDelegate(m_treeView);
        m_treeView->setItemDelegate(m_dViewDelegate);
        UT_DetailViewDelegate_style = new DStyle;
    }
    void TearDown()
    {
        delete m_treeView;
        delete  UT_DetailViewDelegate_style;
        UT_DetailViewDelegate_style = nullptr;
    }
    DetailViewDelegate *m_dViewDelegate;
    DetailTreeView *m_treeView;
};

bool ut_isValid()
{
    return false;
}

int ut_detailview_pixelMetric()
{
    return 10;
}

int ut_y()
{
    return 10;
}

int ut_column()
{
    return 0;
}

DStyle *DetailViewDelegate_UT_style_fun()
{
    return UT_DetailViewDelegate_style;
}

TEST_F(UT_DetailViewDelegate, UT_DetailViewDelegate_paint)
{
    QStyleOptionViewItem option;
    QPainter painter(m_treeView);
    m_treeView->setColumnCount(1);
    m_treeView->insertRow(0);
    QTableWidgetItem *item = new QTableWidgetItem("xxx");
    m_treeView->setItem(0, 0, item);
    QModelIndex index = m_treeView->indexAt(QPoint(0, 0));

    Stub stub;
    stub.set(ADDR(DApplication, style), DetailViewDelegate_UT_style_fun);
    stub.set(ADDR(QPoint, y), ut_y);
    m_dViewDelegate->paint(&painter, option, index);
    EXPECT_FALSE(m_treeView->grab().isNull());
    delete item;
}

TEST_F(UT_DetailViewDelegate, UT_DetailViewDelegate_createEditor)
{
    QStyleOptionViewItem m_item;

    QTableWidgetItem *item = new QTableWidgetItem;
    item->setText("/");
    m_treeView->setItem(0, 0, item);
    QModelIndex index = m_treeView->model()->index(0, 0);

    EXPECT_FALSE(m_dViewDelegate->createEditor(nullptr, m_item, index));
    delete item;
}

TEST_F(UT_DetailViewDelegate, UT_DetailViewDelegate_sizeHint)
{
    QStyleOptionViewItem m_item;

    QTableWidgetItem *item = new QTableWidgetItem;
    item->setText("/");
    m_treeView->setItem(0, 0, item);
    QModelIndex index = m_treeView->model()->index(0, 0);

    QSize size = m_dViewDelegate->sizeHint(m_item, index);
    EXPECT_EQ(150,size.width());
    EXPECT_EQ(50,size.height());
    delete item;
}
