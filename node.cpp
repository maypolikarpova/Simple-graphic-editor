#include <QtGui>

#include "link.h"
#include "node.h"

Node::Node(int i)
{
    type = i;
    myOutlineColor = Qt::darkBlue;
    myBackgroundColor = Qt::white;
    height=200;
    width=300;
    x1=-25;
    y1=-40;
    x2=10*width;
    y2=10*height;
    setFlags(ItemIsMovable | ItemIsSelectable);
}

Node::~Node()
{
    foreach (Link *link, myLinks)
        delete link;
}

void Node::setOutlineColor(const QColor &color)
{
    myOutlineColor = color;
    update();
}

QColor Node::outlineColor() const
{
    return myOutlineColor;
}

void Node::setBackgroundColor(const QColor &color)
{
    myBackgroundColor = color;
    update();
}

QColor Node::backgroundColor() const
{
    return myBackgroundColor;
}

void Node::addLink(Link *link)
{
    myLinks.insert(link);
}

void Node::removeLink(Link *link)
{
    myLinks.remove(link);
}

QRectF Node::boundingRect() const
{
    return QRectF(x1,y1,x2,y2);
}

void Node::setSize(int widths, int heights)
{
    width=widths;
    height=heights;
    update();
}

void Node::paint(QPainter *painter,
                 const QStyleOptionGraphicsItem *option,
                 QWidget * /* widget */)
{
 QPolygon polygon;
    polygon << QPoint((25+width)/2,-40) << QPoint(width,(height)) << QPoint(0,(height));
    painter->setBrush(myBackgroundColor);
 if(type==0)
    painter->drawPolygon(polygon);
if(type==1)
    painter->drawEllipse(((width/2)+20),((height/2)+20),(width/2),(height/2));
if(type==2)
    painter->drawLine(x1,y1,width,height);
if(type==3)
    painter->drawRect(x1,y1,width+10,height+10);
}



QVariant Node::itemChange(GraphicsItemChange change,
                          const QVariant &value)
{
    if (change == ItemPositionHasChanged) {
        foreach (Link *link, myLinks)
            link->trackNodes();
    }
    return QGraphicsItem::itemChange(change, value);
}


int Node::roundness(double size) const
{
    const int Diameter = 12;
    return 100 * Diameter / int(size);
}
