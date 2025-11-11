#ifndef SCENE_HPP
#define SCENE_HPP

#include <QObject>
#include <QGraphicsScene>
#include <QPainter>
#include <cmath>

class Scene : public QGraphicsScene
{
    Q_OBJECT

public:
    Scene(QObject* parent = nullptr);

protected:
    void drawBackground(QPainter *painter, const QRectF &rect) override;
};

#endif // SCENE_HPP
