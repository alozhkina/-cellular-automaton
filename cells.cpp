#include "cells.h"


Cells::Cells(QWidget *parent)
    : QLabel(parent),m_isSelected(false)
{
    setFixedSize(40, 40);
    setStyleSheet("border: 1px solid black;");  // Граница клетки
    updateColor();
}

bool Cells::isSelected() const {
    return m_isSelected;
}

void Cells::setSelected(bool selected) {
    m_isSelected = selected;
    updateColor();
}

void Cells::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        m_isSelected = !m_isSelected;

        updateColor();
    }
}

void Cells::updateColor() {
    if (m_isSelected) {
        setStyleSheet("background-color: lightpink; border: 1px solid black;");
    } else {
        setStyleSheet("background-color: white; border: 1px solid black;");
    }
}




