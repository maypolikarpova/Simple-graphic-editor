#include <QtGui>

#include "node.h"
#include "propertiesdialog.h"

PropertiesDialog::PropertiesDialog(Node *node, QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);

    this->node = node;
    xSpinBox->setValue(int(node->x()));
    ySpinBox->setValue(int(node->y()));
    widthSpin->setValue(int(node->width));
    heightSpin->setValue(int(node->height));
    backgroundColor = node->backgroundColor();
    outlineColor = node->outlineColor();
    updateColorLabel(backgroundColorLabel, backgroundColor);
    updateColorLabel(outlineColorLabel, outlineColor);

}

void PropertiesDialog::on_buttonBox_accepted()
{
    node->setPos(xSpinBox->value(), ySpinBox->value());
    node->setBackgroundColor(backgroundColor);

    node->setOutlineColor(outlineColor);

    node->setSize(widthSpin->value(),heightSpin->value());
    node->update();
    QDialog::accept();
}

void PropertiesDialog::on_backgroundColorButton_clicked()
{
    chooseColor(backgroundColorLabel, &backgroundColor);
}

void PropertiesDialog::on_oulineColorButton_clicked()
{
    chooseColor(outlineColorLabel, &outlineColor);
}

void PropertiesDialog::updateColorLabel(QLabel *label,
                                        const QColor &color)
{
    QPixmap pixmap(16, 16);
    pixmap.fill(color);
    label->setPixmap(pixmap);
}

void PropertiesDialog::chooseColor(QLabel *label, QColor *color)
{
    QColor newColor = QColorDialog::getColor(*color, this);
    if (newColor.isValid()) {
        *color = newColor;
        updateColorLabel(label, *color);
    }
}
