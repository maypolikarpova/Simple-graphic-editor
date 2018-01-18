#ifndef DIAGRAMWINDOW_H
#define DIAGRAMWINDOW_H

#include <QMainWindow>
#include <QPair>

class QAction;
class QGraphicsItem;
class QGraphicsScene;
class QGraphicsView;
class Link;
class Node;

class DiagramWindow : public QMainWindow
{
    Q_OBJECT

public:
    DiagramWindow();

private slots:
    void addTriangle();
    void addOval();
    void addLine();
    void addRect();
    void addLink();
    void del();
    void cut();
    void copy();
    void paste();
    void bringToFront();
    void sendToBack();
    void properties();
    void updateActions();
    void clo();
    void help();
private:
    typedef QPair<Node *, Node *> NodePair;

    void createActions();
    void createMenus();
    void createToolBars();
    void setZValue(int z);
    void setupNode( Node *tria);
    Node *selectedNode() const;
    Link *selectedLink() const;
    NodePair selectedNodePair() const;
    QString path;
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *adddMenu;
    QAction *save;
    QAction *triangle;
    QAction *oval;
    QAction *rect;
    QAction *line;
    QToolBar *editToolBar;
    QAction *exitAction;
    QAction *helpAction;
    QAction *addLinkAction;
    QAction *deleteAction;
    QAction *cutAction;
    QAction *copyAction;
    QAction *pasteAction;
    QAction *bringToFrontAction;
    QAction *sendToBackAction;
    QAction *propertiesAction;
    QGraphicsScene *scene;
    QGraphicsView *view;
    int i;
    int minZ;
    int maxZ;
    int seqNumber;
};

#endif
