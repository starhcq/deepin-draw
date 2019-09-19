#include "cdrawscene.h"
#include "cgraphicsrectitem.h"
#include "idrawtool.h"
#include "cdrawtoolmanagersigleton.h"
#include "cdrawparamsigleton.h"
#include "globaldefine.h"
#include "cgraphicspolygonitem.h"
#include "cgraphicspolygonalstaritem.h"
#include "cgraphicspenitem.h"
#include "frame/cpicturewidget.h"
#include "cgraphicstextitem.h"

#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QRect>
#include <QGraphicsView>
#include <drawshape/cpictureitem.h>


CDrawScene::CDrawScene(QObject *parent)
    : QGraphicsScene(parent)
{
    connect(this, SIGNAL(selectionChanged()), this, SLOT(slotSelectionChange()));
}

void CDrawScene::mouseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    switch ( mouseEvent->type() ) {
    case QEvent::GraphicsSceneMousePress:
        QGraphicsScene::mousePressEvent(mouseEvent);
        break;
    case QEvent::GraphicsSceneMouseMove:
        QGraphicsScene::mouseMoveEvent(mouseEvent);
        break;
    case QEvent::GraphicsSceneMouseRelease:
        QGraphicsScene::mouseReleaseEvent(mouseEvent);
        break;
    default:
        break;
    }
}

void CDrawScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    QGraphicsScene::drawBackground(painter, rect);
    painter->fillRect(sceneRect(), Qt::white);
}

void CDrawScene::setCursor(const QCursor &cursor)
{
    QList<QGraphicsView *> views  = this->views();
    if ( views.count() > 0 ) {
        QGraphicsView *view = views.first();
        view->setCursor(cursor);
    }
}

void CDrawScene::attributeChanged()
{
    QList<QGraphicsItem *> items = this->selectedItems();

    QGraphicsItem *item = nullptr;
    foreach (item, items) {
        CGraphicsItem *tmpitem = static_cast<CGraphicsItem *>(item);

        if (tmpitem->pen() != CDrawParamSigleton::GetInstance()->getPen() ||
                tmpitem->brush() != CDrawParamSigleton::GetInstance()->getBrush() ) {
            emit itemPropertyChange(tmpitem, tmpitem->pen(), tmpitem->brush(),
                                    tmpitem->pen() != CDrawParamSigleton::GetInstance()->getPen(),
                                    tmpitem->brush() != CDrawParamSigleton::GetInstance()->getBrush());
            tmpitem->setPen(CDrawParamSigleton::GetInstance()->getPen());
            tmpitem->setBrush(CDrawParamSigleton::GetInstance()->getBrush());
        }


        if (item->type() == TextType) {
            static_cast<CGraphicsTextItem *>(item)->setTextColor(CDrawParamSigleton::GetInstance()->getTextColor());
            static_cast<CGraphicsTextItem *>(item)->setFont(CDrawParamSigleton::GetInstance()->getTextFont());
            static_cast<CGraphicsTextItem *>(item)->setFontSize(CDrawParamSigleton::GetInstance()->getTextSize());
        } else if (item->type() == PolygonType) {
            if (CDrawParamSigleton::GetInstance()->getSideNum() != static_cast<CGraphicsPolygonItem *>(item)->nPointsCount()) {
                emit itemPolygonPointChange(static_cast<CGraphicsPolygonItem *>(item), static_cast<CGraphicsPolygonItem *>(item)->nPointsCount());
                static_cast<CGraphicsPolygonItem *>(item)->setPointCount(CDrawParamSigleton::GetInstance()->getSideNum());
            }
        } else if (item->type() == PolygonalStarType) {
            CGraphicsPolygonalStarItem *tmpItem = static_cast<CGraphicsPolygonalStarItem *>(item);
            if (tmpItem->anchorNum() != CDrawParamSigleton::GetInstance()->getAnchorNum() || tmpItem->innerRadius() != CDrawParamSigleton::GetInstance()->getRadiusNum()) {
                emit itemPolygonalStarPointChange(tmpItem, tmpItem->anchorNum(), tmpItem->innerRadius());
                tmpItem->updatePolygonalStar(CDrawParamSigleton::GetInstance()->getAnchorNum(),
                                             CDrawParamSigleton::GetInstance()->getRadiusNum());
            }
        }
    }
}

void CDrawScene::changeAttribute(bool flag, QGraphicsItem *selectedItem)
{
    if (flag) {
        //排除文字图元
        CDrawParamSigleton::GetInstance()->setPen(static_cast<CGraphicsItem *>(selectedItem)->pen());
        CDrawParamSigleton::GetInstance()->setBrush(static_cast<CGraphicsItem *>(selectedItem)->brush());
        ///特殊属性图元 读取额外的特殊属性并设置到全局属性中
        if (selectedItem->type() == PolygonType) {
            CDrawParamSigleton::GetInstance()->setSideNum(static_cast<CGraphicsPolygonItem *>(selectedItem)->nPointsCount());
        } else if (selectedItem->type() == PolygonalStarType) {
            CDrawParamSigleton::GetInstance()->setAnchorNum(static_cast<CGraphicsPolygonalStarItem *>(selectedItem)->anchorNum());
            CDrawParamSigleton::GetInstance()->setRadiusNum(static_cast<CGraphicsPolygonalStarItem *>(selectedItem)->innerRadius());
        } else if (selectedItem->type() ==  PenType) {
            CDrawParamSigleton::GetInstance()->setCurrentPenType(static_cast<CGraphicsPenItem *>(selectedItem)->currentType());
        } else if (selectedItem->type() == TextType) {

        }
    }
    emit signalAttributeChanged(flag, selectedItem->type());
}

void CDrawScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() & Qt::LeftButton) {
        EDrawToolMode currentMode = CDrawParamSigleton::GetInstance()->getCurrentDrawToolMode();

        IDrawTool *pTool = CDrawToolManagerSigleton::GetInstance()->getDrawTool(currentMode);
        if ( nullptr != pTool ) {
            pTool->mousePressEvent(mouseEvent, this);
        }
    }
}

void CDrawScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    EDrawToolMode currentMode = CDrawParamSigleton::GetInstance()->getCurrentDrawToolMode();
    IDrawTool *pTool = CDrawToolManagerSigleton::GetInstance()->getDrawTool(currentMode);
    if ( nullptr != pTool ) {
        pTool->mouseMoveEvent(mouseEvent, this);
    }
}

void CDrawScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    EDrawToolMode currentMode = CDrawParamSigleton::GetInstance()->getCurrentDrawToolMode();
    IDrawTool *pTool = CDrawToolManagerSigleton::GetInstance()->getDrawTool(currentMode);
    if ( nullptr != pTool ) {
        pTool->mouseReleaseEvent(mouseEvent, this);
    }
    CDrawParamSigleton::GetInstance()->setCurrentDrawToolMode(selection);
    emit signalChangeToSelect();
}



void CDrawScene::picOperation(int enumstyle)
{

    //qDebug() << "entered the  picOperation function" << endl;
    QList<QGraphicsItem *> items = this->selectedItems();
    if ( items.count() != 0 ) {
        QGraphicsItem *item = items.first();
        //需要区别图元或文字
        if (item->type() == PictureType) {
            CPictureItem *pictureItem = static_cast<CPictureItem *>(item);
            if (pictureItem != nullptr) {
                switch (enumstyle) {
                case CPictureWidget::LeftRotate:
                    pictureItem->setRotation90(true);
                    break;
                case CPictureWidget::RightRotate:
                    pictureItem->setRotation90(false);
                    break;
                case CPictureWidget::FlipHorizontal:
                    pictureItem->setMirror(true, false);
                    break;
                case CPictureWidget::FlipVertical:
                    pictureItem->setMirror(false, true);
                    break;
                default:
                    break;
                }

            }
        }

    }

}

void CDrawScene::slotSelectionChange()
{
    IDrawTool *pTool = CDrawToolManagerSigleton::GetInstance()->getDrawTool(selection);
    if ( nullptr != pTool ) {
        static_cast<CSelectTool *>(pTool)->selectionChange();
    }
}
