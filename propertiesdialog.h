#ifndef PROPERTIESDIALOG_H
#define PROPERTIESDIALOG_H

#include "ui_propertiesdialog.h"

class Node;

class PropertiesDialog : public QDialog, private Ui::PropertiesDialog
{
    Q_OBJECT

public:
    PropertiesDialog(Node *node, QWidget *parent = 0);

private slots:
    void on_buttonBox_accepted();
    void on_backgroundColorButton_clicked();
    void on_oulineColorButton_clicked();

private:
    void updateColorLabel(QLabel *label, const QColor &color);
    void chooseColor(QLabel *label, QColor *color);

    Node *node;
    QColor backgroundColor;
    QColor outlineColor;
};

#endif
