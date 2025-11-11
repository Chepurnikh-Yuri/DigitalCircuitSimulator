#include "scene.hpp"

Scene::Scene(QObject* parent) : QGraphicsScene(parent) { }

void Scene::drawBackground(QPainter *painter, const QRectF &rect)
{
    qreal gridStep = 35;
    qreal scale = painter->worldTransform().m11();

    gridStep *= scale < 1 ? 1 / scale : scale;

    painter->fillRect(rect, QColor("#121212"));
    painter->setPen(QPen(QColor("#1C1C1C")));

    for (qreal vertical = std::floor(rect.left() / gridStep) * gridStep; vertical <= rect.right(); vertical += gridStep)
        painter->drawLine(QLineF(vertical, rect.top(), vertical, rect.bottom()));

    for (qreal horizontal = std::floor(rect.top() / gridStep) * gridStep; horizontal <= rect.bottom(); horizontal += gridStep)
        painter->drawLine(QLineF(rect.left(), horizontal, rect.right(), horizontal));
}
