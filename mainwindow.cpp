/*
  Copyright 2013-2026 Mike Bergmann

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

#include "bitformat.h"
#include "ui_mainwindow.h"

#include <QEvent>
#include <QFontDatabase>
#include <QLabel>

namespace {

QString toQt(const std::string &s)
{
    return QString::fromStdString(s);
}

} // namespace

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(std::make_unique<Ui::MainWindow>())
{
    ui->setupUi(this);
    ui->lVersion->setText(tr("Version") + " " __DATE__ " " __TIME__);
    ui->lLink->setOpenExternalLinks(true);

    // The rows only line up under a fixed-width font, and the "Courier New" of
    // the .ui file is not installed everywhere.
    const QFont mono = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    for (QLabel *row : { ui->lBinary, ui->lPos, ui->lHex, ui->lDec })
        row->setFont(mono);

    refresh();
}

MainWindow::~MainWindow() = default;

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    if (e->type() == QEvent::LanguageChange)
        ui->retranslateUi(this);
}

void MainWindow::refresh()
{
    const auto value = registerer::parse(ui->lineEdit->text().toStdString());
    const int bit = ui->spinBox->value();

    if (!value) {
        for (QLabel *row : { ui->lBinary, ui->lPos, ui->lHex, ui->lDec, ui->lBitAsHex })
            row->clear();
        return;
    }

    ui->lBitAsHex->setText(
            toQt(registerer::format({ .number = registerer::mask(bit), .base = value->base })));

    const registerer::Rows rows = registerer::layout(value->number, bit);
    ui->lBinary->setText(toQt(rows.bin));
    ui->lPos->setText(toQt(rows.pos));
    ui->lHex->setText(toQt(rows.hex));
    ui->lDec->setText(toQt(rows.dec));
}

void MainWindow::on_lineEdit_editingFinished()
{
    refresh();
}

void MainWindow::on_spinBox_valueChanged(int)
{
    refresh();
}

void MainWindow::on_tbToggle_clicked()
{
    if (const auto v = registerer::parse(ui->lineEdit->text().toStdString()))
        ui->lineEdit->setText(toQt(
                registerer::format({ .number = registerer::toggle(v->number, ui->spinBox->value()),
                                     .base = v->base })));
    refresh();
}

void MainWindow::on_tbDisplay_clicked()
{
    if (const auto v = registerer::parse(ui->lineEdit->text().toStdString()))
        ui->lineEdit->setText(
                toQt(registerer::format({ .number = v->number, .base = next(v->base) })));
    refresh();
}
