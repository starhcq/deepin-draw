/*
 * Copyright (C) 2019 ~ %YEAR% Deepin Technology Co., Ltd.
 *
 * Author:     Renran
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef POLYGONALSTARATTRIBUTEWIDGET_H
#define POLYGONALSTARATTRIBUTEWIDGET_H

#include <DWidget>
#include <DSlider>
#include <DLineEdit>
#include "drawshape/globaldefine.h"

DWIDGET_USE_NAMESPACE

class BigColorButton;
class BorderColorButton;
class CSideWidthWidget;
class SeperatorLine;

class PolygonalStarAttributeWidget : public DWidget
{
    Q_OBJECT
public:
    PolygonalStarAttributeWidget(DWidget *parent = nullptr);
    ~PolygonalStarAttributeWidget();
    void changeButtonTheme();

public slots:
    void updatePolygonalStarWidget();

signals:
    void resetColorBtns();
    void showColorPanel(DrawStatus drawstatus, QPoint pos, bool visible = true);
    void signalPolygonalStarAttributeChanged();

private:
    BigColorButton *m_fillBtn;
    BorderColorButton *m_strokeBtn;
    CSideWidthWidget *m_sideWidthWidget;
    DSlider *m_anchorNumSlider; //锚点数滑块
    DLineEdit *m_anchorNumEdit;//锚点数编辑框
    DSlider *m_radiusNumSlider;//半径滑块
    DLineEdit *m_radiusNumEdit;//半径编辑框
    SeperatorLine *m_sepLine;


private:
    void initUI();
    void initConnection();
    QPoint getBtnPosition(const DPushButton *btn);
};

#endif // POLYGONALSTARATTRIBUTEWIDGET_H
