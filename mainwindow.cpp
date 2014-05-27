/*
  Copyright 2013 Mike Bergmann, Bones AG

  This file is part of Registerer.

  Registerer is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 2 of the License, or
  (at your option) any later version.

  Registerer is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Registerer.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <qmath.h>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  ui->lVersion->setText(tr("Version") + " " + __DATE__ + " " + __TIME__);
  ui->lLink->setOpenExternalLinks(true);
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
  QMainWindow::changeEvent(e);
  switch (e->type()) {
  case QEvent::LanguageChange:
    ui->retranslateUi(this);
    break;
  default:
    break;
  }
}

void MainWindow::on_lineEdit_editingFinished()
{
  bool ok;
  unsigned long long number;

  if(ui->lineEdit->text().startsWith("0b")) {
    number = ui->lineEdit->text().right(ui->lineEdit->text().length()-2).toULongLong(&ok, 2);
    ui->lBitAsHex->setText(QString("0b")+QString::number((unsigned long long)qPow(2,ui->spinBox->value()),2));
  } else if(ui->lineEdit->text().startsWith("0x")) {
    number = ui->lineEdit->text().right(ui->lineEdit->text().length()-2).toULongLong(&ok, 16);
    ui->lBitAsHex->setText(QString("0x")+QString::number((unsigned long long)qPow(2,ui->spinBox->value()),16));
  } else {
    number = ui->lineEdit->text().toULongLong(&ok, 10);
    ui->lBitAsHex->setText(QString::number((unsigned long long)qPow(2,ui->spinBox->value())));
  }

  ui->lBinary->setText("");
  ui->lPos->setText("");
  ui->lHex->setText("");

  QString bin = QString().setNum(number,  2);
  QString hex = QString().setNum(number, 16);
  QString zero("00000");

  if(bin.length()%4)
    bin.insert(0,zero.data(),4-bin.length()%4);

  QString pos;
  pos.fill(' ',bin.length());

  if(ui->spinBox->value() < bin.length())
    pos[pos.length() - ui->spinBox->value() - 1] = '^';

  for(int i=0;i<bin.length();++i) {
    ui->lBinary->setText(ui->lBinary->text()+bin.at(i));
    ui->lPos->setText(ui->lPos->text()+pos.at(i));
    if((i+1)%4==0) {
      ui->lHex->setText(ui->lHex->text()+hex.at((i-3)/4));
    } else {
      ui->lHex->setText(ui->lHex->text()+" ");
    }

    if((i+1)%4==0) {
      ui->lBinary->setText(ui->lBinary->text()+QString(" "));
      ui->lPos->setText(ui->lPos->text()+QString(" "));
      ui->lHex->setText(ui->lHex->text()+QString(" "));
    }
  }

  QString dec;
  dec.fill(' ',ui->lBinary->text().length()-QString().setNum(number, 10).length()-1);
  dec.append(QString().setNum(number, 10));

  ui->lDec->setText(dec);

}

void MainWindow::on_spinBox_valueChanged(int)
{
  on_lineEdit_editingFinished();
}

void MainWindow::on_tbToggle_clicked()
{
  QString tmp = ui->lBinary->text().remove(" ");

  if(ui->spinBox->value() >= tmp.length()) {
    tmp.fill('0',ui->spinBox->value()-tmp.length()+1);
    tmp.append(ui->lBinary->text().remove(" "));
  }

  if(ui->spinBox->value() < tmp.length()) {

    if(tmp[tmp.length() - 1 - ui->spinBox->value()] == QChar('0'))
      tmp[tmp.length() - 1 - ui->spinBox->value()] = QChar('1');
    else
      tmp[tmp.length() - 1 - ui->spinBox->value()] = QChar('0');

    bool ok;
    unsigned long long num = tmp.toULongLong(&ok,2);
    if(ok) {
      if(ui->lineEdit->text().startsWith("0b")) {
        ui->lineEdit->setText(QString("0b")+QString::number(num,2));
      } else if(ui->lineEdit->text().startsWith("0x")) {
        ui->lineEdit->setText(QString("0x")+QString::number(num,16));
      } else {
        ui->lineEdit->setText(QString::number(num));
      }
      on_lineEdit_editingFinished();
    }
  }
}

void MainWindow::on_tbDisplay_clicked()
{
  unsigned long long number;
  bool ok;

  if(ui->lineEdit->text().startsWith("0b")) {
    number = ui->lineEdit->text().right(ui->lineEdit->text().length()-2).toULongLong(&ok, 2);
  } else if(ui->lineEdit->text().startsWith("0x")) {
    number = ui->lineEdit->text().right(ui->lineEdit->text().length()-2).toULongLong(&ok, 16);
  } else {
    number = ui->lineEdit->text().toULongLong(&ok, 10);
  }

  if(ui->lineEdit->text().startsWith("0b")) {
    ui->lineEdit->setText(QString::number(number, 10));
  } else if(ui->lineEdit->text().startsWith("0x")) {
    ui->lineEdit->setText(QString("0b")+QString::number(number,2));
  } else {
    ui->lineEdit->setText(QString("0x")+QString::number(number,16));
  }

  if(ui->lineEdit->text().startsWith("0b")) {
    ui->lBitAsHex->setText(QString("0b")+QString::number((unsigned long long)qPow(2,ui->spinBox->value()),2));
  } else if(ui->lineEdit->text().startsWith("0x")) {
    ui->lBitAsHex->setText(QString("0x")+QString::number((unsigned long long)qPow(2,ui->spinBox->value()),16));
  } else {
    ui->lBitAsHex->setText(QString::number((unsigned long long)qPow(2,ui->spinBox->value())));
  }

  on_lineEdit_editingFinished();
}
