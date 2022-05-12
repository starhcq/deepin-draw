#include<QHBoxLayout>
#include "attributionwidget.h"
//#include "positionattriwidget.h"

AttributionWidget::AttributionWidget(QWidget *parent) : QWidget(parent)
{
    setWgtAccesibleName(this, "AttributionWidget");
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::Widget | Qt::NoDropShadowWindowHint);
    QVBoxLayout *l = new QVBoxLayout(this);
    setLayout(l);
    l->setAlignment(Qt::AlignTop);
    setShowFlags(ToolActived | ItemSelected | ItemAttriChanged);
    l->setContentsMargins(0, 0, 0, 0);
    l->setMargin(0);

    setMinimumWidth(250);

    initWidgetOrder();
}

void AttributionWidget::addAttriWidget(QWidget *w)
{
    //已经添加了的就不再添加
    bool bAdd = false;
    for (auto child : m_childWidgets) {
        if (child == w) {
            bAdd = true;
            return;
        }
    }

    if (!bAdd) {
        m_childWidgets.append(w);
    }

    for (auto c : m_childWidgets) {
        layout()->removeWidget(c);
    }

    for (int attri : m_attriShowOrder) {
        for (auto child : m_childWidgets) {
            AttributeWgt *p = dynamic_cast<AttributeWgt *>(child);
            if (p && p->attribution() == attri) {
                layout()->addWidget(p);
                break;
            }
        }
    }
}

void AttributionWidget::removeAttriWidget(QWidget *w)
{
    layout()->removeWidget(w);
    for (int i = 0; i < m_childWidgets.count(); i++) {
        if (m_childWidgets.at(i) == w) {
            m_childWidgets.removeAt(i);
            break;
        }
    }
}

void AttributionWidget::removeAll()
{
    for (auto w : m_childWidgets) {
        layout()->removeWidget(w);
        if (!w->property(ChildAttriWidget).toBool()) {
            w->hide();
        }
    }

    m_childWidgets.clear();
}

void AttributionWidget::showAt(int active, const QPoint &pos)
{
    Q_UNUSED(active)
    Q_UNUSED(pos)
}

void AttributionWidget::showWidgets(int active, const QList<QWidget *> &oldWidgets, const QList<QWidget *> &exposeWidgets)
{
    removeAll();
    for (auto w : oldWidgets) {
        removeAttriWidget(w);
        w->hide();
    }

    for (auto w : exposeWidgets) {
        w->setEnabled(AttributionManager::ForceShow != active);
        w->show();
        //如果是子窗口属性，不添加到显示. 添加其父窗口
        if (!w->property(ChildAttriWidget).toBool() && !w->property(ParentAttriWidget).toBool()) {
            addAttriWidget(w);
        } else if (w->property(ChildAttriWidget).toBool()) {
            QWidget *p = qobject_cast<QWidget *>(w->parent());
            if (w->parent() && p && p->property(ParentAttriWidget).toBool()) {
                addAttriWidget(p);
                p->show();
            }
        }
    }
}

void AttributionWidget::initWidgetOrder()
{
    m_attriShowOrder << ERotProperty << EGroupWgt << EOrderProperty << EImageAdaptScene << EStyleProper;
}
