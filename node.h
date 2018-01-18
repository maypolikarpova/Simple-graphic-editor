#ifndef NODE_H
#define NODE_H

#include <QApplication>
#include <QColor>
#include <QGraphicsItem>
#include <QSet>
#include "triangle.h"
class Link;

class Node : public QGraphicsItem
{
    Q_DECLARE_TR_FUNCTIONS(Node)

public:
    Node(int i=0);
    ~Node();
    void setOutlineColor(const QColor &color);
    QColor outlineColor() const;
    void setBackgroundColor(const QColor &color);
    QColor backgroundColor() const;

    void addLink(Link *link);
    void removeLink(Link *link);
    void setSize(int widths,int heights);
    QRectF boundingRect() const;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option, QWidget *widget);
    double height;
    double width;
protected:
    QVariant itemChange(GraphicsItemChange change,
                        const QVariant &value);

private:
    int roundness(double size) const;
int type;
    double x1;
    double y1;
    double x2;
    double y2;
    QSet<Link *> myLinks;
    QColor myBackgroundColor;
    QColor myOutlineColor;
};

#endif
