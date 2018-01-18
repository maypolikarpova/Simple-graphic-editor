#include <QtGui>
#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QtSvg/QSvgGenerator>
#include <QFileDialog>
#include <QPainter>
#include "diagramwindow.h"
#include "link.h"
#include "node.h"
#include "propertiesdialog.h"
#include "helpdialog.h"

DiagramWindow::DiagramWindow()
{   i=0;
    scene = new QGraphicsScene(0, 0, 600, 500);

    view = new QGraphicsView;
    view->setScene(scene);
    view->setDragMode(QGraphicsView::RubberBandDrag);
    view->setRenderHints(QPainter::Antialiasing
                         | QPainter::TextAntialiasing);
    view->setContextMenuPolicy(Qt::ActionsContextMenu);
    setCentralWidget(view);

    minZ = 0;
    maxZ = 0;
    seqNumber = 0;
    createActions();
    createMenus();
    createToolBars();

    connect(scene, SIGNAL(selectionChanged()),
            this, SLOT(updateActions()));

    setWindowTitle(tr("Graphic Editor"));
    updateActions();
}
void DiagramWindow::addTriangle()
{
    Node *node = new Node(0);
    setupNode(node);
}
void DiagramWindow::addOval()
{
    Node *node = new Node(1);
    setupNode(node);
}
void DiagramWindow::addLine()
{
    Node *node = new Node(2);
    setupNode(node);
}
void DiagramWindow::addRect()
{
    Node *node = new Node(3);
    setupNode(node);
}
void DiagramWindow::help()
{
 HelpDialog d;
 d.exec();
}
void DiagramWindow::addLink()
{
    NodePair nodes = selectedNodePair();
    if (nodes == NodePair())
        return;

    Link *link = new Link(nodes.first, nodes.second);
    scene->addItem(link);
}

void DiagramWindow::del()
{
    QList<QGraphicsItem *> items = scene->selectedItems();
    QMutableListIterator<QGraphicsItem *> i(items);
    while (i.hasNext()) {
        Link *link = dynamic_cast<Link *>(i.next());
        if (link) {
            delete link;
            i.remove();
        }
    }

    qDeleteAll(items);
}

void DiagramWindow::cut()
{
    Node *node = selectedNode();
    if (!node)
        return;

    copy();
    delete node;
}

void DiagramWindow::copy()
{
    Node *node = selectedNode();
    if (!node)
        return;

   QString str = QString("Node %1 %2 %3 %4")
                  .arg(node->outlineColor().name())
                  .arg(node->backgroundColor().name());
    QApplication::clipboard()->setText(str);
}

void DiagramWindow::paste()
{
    QString str = QApplication::clipboard()->text();
    QStringList parts = str.split(" ");

    if (parts.count() >= 5 && parts.first() == "Node") {
        Node *node = new Node(0);
        node->setOutlineColor(QColor(parts[1]));
        node->setBackgroundColor(QColor(parts[2]));
        setupNode(node);
    }
}

void DiagramWindow::bringToFront()
{
    ++maxZ;
    setZValue(maxZ);
}

void DiagramWindow::sendToBack()
{
    --minZ;
    setZValue(minZ);
}

void DiagramWindow::properties()
{
    Node *node = selectedNode();
    Link *link = selectedLink();

    if (node) {
        PropertiesDialog dialog(node, this);
        dialog.exec();
    }
    else if (link) {
        QColor color = QColorDialog::getColor(link->color(), this);
        if (color.isValid())
            link->setColor(color);
    }
}

void DiagramWindow::updateActions()
{
    bool hasSelection = !scene->selectedItems().isEmpty();
    bool isNode = (selectedNode() != 0);

    cutAction->setEnabled(isNode);
    copyAction->setEnabled(isNode);
    deleteAction->setEnabled(hasSelection);
    bringToFrontAction->setEnabled(isNode);
    sendToBackAction->setEnabled(isNode);
    propertiesAction->setEnabled(isNode);

    foreach (QAction *action, view->actions())
        view->removeAction(action);

    foreach (QAction *action, editMenu->actions()) {
        if (action->isEnabled())
            view->addAction(action);
    }
}

void DiagramWindow::clo(){
        QImage image(view->scene()->width()+100, view->scene()->height()+100, QImage::Format_ARGB32_Premultiplied);
        QPainter painter(&image);
        view->scene()->render(&painter);
        image.save("file.png");

}void DiagramWindow::createActions()
{
    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setShortcut(tr("Ctrl+Q"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    save = new QAction(tr("Save"), this);
    save->setIcon(QIcon("/images/save.png"));
    save->setShortcut(tr("Ctrl+S"));
    save->setIconText(tr("Save"));
    connect(save, SIGNAL(triggered()), this, SLOT(clo()));

    helpAction = new QAction(tr("Help"), this);
    helpAction->setShortcut(tr("Ctrl+H"));
    connect(helpAction, SIGNAL(triggered()), this, SLOT(help()));

    oval = new QAction(tr("Circle"), this);
    oval->setIcon(QIcon("/images/oval.png"));
    oval->setShortcut(tr("Ctrl+O"));
    oval->setIconText(tr("Oval"));
      connect(oval, SIGNAL(triggered()), this, SLOT(addOval()));

    rect = new QAction(tr("Rect"), this);
    rect->setShortcut(tr("Ctrl+R"));
    rect->setIconText(tr("Rect"));
    connect(rect, SIGNAL(triggered()), this, SLOT(addRect()));

    line = new QAction(tr("Line"), this);
    line->setIcon(QIcon("/images/line.png"));
    line->setShortcut(tr("Ctrl+L"));
    line->setIconText(tr("Line"));
    connect(line, SIGNAL(triggered()), this, SLOT(addLine()));

    triangle = new QAction(tr("Triangle"), this);
    triangle->setIcon(QIcon("/images/triangle.png"));
    triangle->setShortcut(tr("Ctrl+T"));
    triangle->setIconText(tr("Triangle"));
    connect(triangle, SIGNAL(triggered()), this, SLOT(addTriangle()));

    deleteAction = new QAction(tr("&Delete"), this);
    deleteAction->setIcon(QIcon(":/images/delete.png"));
    deleteAction->setShortcut(tr("Del"));
     deleteAction->setIconText(tr("Del"));
    connect(deleteAction, SIGNAL(triggered()), this, SLOT(del()));

    cutAction = new QAction(tr("Cut"), this);
    cutAction->setIcon(QIcon(":/images/cut.png"));
    cutAction->setShortcut(tr("Ctrl+X"));
     cutAction->setIconText(tr("Cut"));
    connect(cutAction, SIGNAL(triggered()), this, SLOT(cut()));

    copyAction = new QAction(tr("Copy"), this);
    copyAction->setIcon(QIcon(":/images/copy.png"));
    copyAction->setShortcut(tr("Ctrl+C"));
     copyAction->setIconText(tr("Copy"));
    connect(copyAction, SIGNAL(triggered()), this, SLOT(copy()));

    pasteAction = new QAction(tr("Paste"), this);
    pasteAction->setIcon(QIcon(":/images/paste.png"));
    pasteAction->setShortcut(tr("Ctrl+V"));
     pasteAction->setIconText(tr("Paste"));
    connect(pasteAction, SIGNAL(triggered()), this, SLOT(paste()));

    bringToFrontAction = new QAction(tr("Bring choosen element to front"), this);
    bringToFrontAction->setIcon(QIcon(":/images/bringtofront.png"));
    bringToFrontAction->setIconText(tr("Bring to Front"));
    connect(bringToFrontAction, SIGNAL(triggered()),
            this, SLOT(bringToFront()));

    sendToBackAction = new QAction(tr("&Send to Back selecting element"), this);
    sendToBackAction->setIcon(QIcon(":/images/sendtoback.png"));
     sendToBackAction->setIconText(tr("Send to Back"));
    connect(sendToBackAction, SIGNAL(triggered()),
            this, SLOT(sendToBack()));

    propertiesAction = new QAction(tr("Change properties"), this);
    propertiesAction->setIconText(tr("Properties"));
    connect(propertiesAction, SIGNAL(triggered()),
    this, SLOT(properties()));
}

void DiagramWindow::createMenus()
{

    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(exitAction);
    fileMenu->addAction(save);
    fileMenu->addAction(propertiesAction);
    fileMenu->addAction(helpAction);

    fileMenu->setToolTip("Exit,save and other options to work with file");

    adddMenu = menuBar()->addMenu(tr("&Add"));
    adddMenu->addAction(triangle);
    adddMenu->addAction(oval);
    adddMenu->addAction(line);
    adddMenu->addAction(rect);
    adddMenu->setToolTip("Add some figure");


    editMenu = menuBar()->addMenu(tr("&Work with figures"));
    editMenu->addAction(deleteAction);
    editMenu->addAction(cutAction);
    editMenu->addAction(copyAction);
    editMenu->addAction(pasteAction);
    editMenu->addAction(bringToFrontAction);
    editMenu->addAction(sendToBackAction);
    editMenu->setToolTip("Edit your file");
    editMenu->addSeparator();

}

void DiagramWindow::createToolBars()
{
    editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(deleteAction);
    editToolBar->addSeparator();
    editToolBar->addAction(cutAction);
    editToolBar->addAction(copyAction);
    editToolBar->addAction(pasteAction);
    editToolBar->addSeparator();
    editToolBar->addAction(bringToFrontAction);
    editToolBar->addAction(sendToBackAction);

}

void DiagramWindow::setZValue(int z)
{
    Node *node = selectedNode();
    if (node)
        node->setZValue(z);
}

void DiagramWindow::setupNode(Node *node)
{
    node->setPos(QPoint(80 + (100 * (seqNumber % 5)),
                        80 + (50 * ((seqNumber / 5) % 7))));
    scene->addItem(node);
    ++seqNumber;

    scene->clearSelection();
    node->setSelected(true);
    bringToFront();
}

Node *DiagramWindow::selectedNode() const
{
    QList<QGraphicsItem *> items = scene->selectedItems();
    if (items.count() == 1) {
        return dynamic_cast<Node *>(items.first());
    } else {
        return 0;
    }
}

Link *DiagramWindow::selectedLink() const
{
    QList<QGraphicsItem *> items = scene->selectedItems();
    if (items.count() == 1) {
        return dynamic_cast<Link *>(items.first());
    } else {
        return 0;
    }
}

DiagramWindow::NodePair DiagramWindow::selectedNodePair() const
{
    QList<QGraphicsItem *> items = scene->selectedItems();
    if (items.count() == 2) {
        Node *first = dynamic_cast<Node *>(items.first());
        Node *second = dynamic_cast<Node *>(items.last());
        if (first && second)
            return NodePair(first, second);
    }
    return NodePair();
}
