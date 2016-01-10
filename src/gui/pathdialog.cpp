/*****************************************************************************
* Copyright 2015-2016 Alexander Barthel albar965@mailbox.org
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#include "gui/pathdialog.h"
#include "gui/constants.h"
#include "logging/loggingdefs.h"
#include "settings/settings.h"

#include <QSettings>
#include <QDir>
#include <QFileInfo>
#include <ui_pathdialog.h>

#include "gui/dialog.h"
#include "gui/errorhandler.h"

using atools::fs::FsPaths;
using atools::fs::SimulatorType;
using atools::settings::Settings;

PathDialog::PathDialog(QWidget *parentWidget, PathSettings *paths) :
  QDialog(parentWidget), ui(new Ui::PathDialog), pathSettings(paths)
{
  dialog = new atools::gui::Dialog(this);
  errorHandler = new atools::gui::ErrorHandler(this);

  ui->setupUi(this);

  pixmapCheckmark = new QPixmap(":/littlelogbook/resources/icons/checkmark.svg");
  pixmapExclamation = new QPixmap(":/littlelogbook/resources/icons/exclamation.svg");

  for(int i = 0; i < 4; i++)
  {
    changedLogbooks.append(false);
    changedRunways.append(false);
  }

  initTab(ui->lineEditFsxLogbook, ui->lineEditFsxRunways,
          ui->labelIconFsxLogbook, ui->labelIconFsxRunways, atools::fs::FSX);

  initTab(ui->lineEditFsxSeLogbook, ui->lineEditFsxSeRunways,
          ui->labelIconFsxSeLogbook, ui->labelIconFsxSeRunways, atools::fs::FSX_SE);

  initTab(ui->lineEditP3dV2Logbook, ui->lineEditP3dV2Runways,
          ui->labelIconP3dV2Logbook, ui->labelIconP3dV2Runways, atools::fs::P3D_V2);

  initTab(ui->lineEditP3dV3Logbook, ui->lineEditP3dV3Runways,
          ui->labelIconP3dV3Logbook, ui->labelIconP3dV3Runways, atools::fs::P3D_V3);

  /* *INDENT-OFF* */
  // Logbook line edits
  connect(ui->lineEditFsxLogbook, &QLineEdit::textEdited,
          [=](const QString &text) {logbookTextEdited(text, ui->labelIconFsxLogbook, atools::fs::FSX); });
  connect(ui->lineEditFsxSeLogbook, &QLineEdit::textEdited,
          [=](const QString &text) {logbookTextEdited(text, ui->labelIconFsxLogbook, atools::fs::FSX_SE); });
  connect(ui->lineEditP3dV2Logbook, &QLineEdit::textEdited,
          [=](const QString &text) {logbookTextEdited(text, ui->labelIconP3dV2Logbook, atools::fs::P3D_V2); });
  connect(ui->lineEditP3dV3Logbook, &QLineEdit::textEdited,
          [=](const QString &text) {logbookTextEdited(text, ui->labelIconP3dV3Logbook, atools::fs::P3D_V3); });

  // Logbook runways/airport line edits
  connect(ui->lineEditFsxRunways, &QLineEdit::textEdited,
          [=](const QString &text) {runwaysTextEdited(text, ui->labelIconFsxRunways, atools::fs::FSX); });
  connect(ui->lineEditFsxSeRunways, &QLineEdit::textEdited,
          [=](const QString &text) {runwaysTextEdited(text, ui->labelIconFsxSeRunways, atools::fs::FSX_SE); });
  connect(ui->lineEditP3dV2Runways, &QLineEdit::textEdited,
          [=](const QString &text) {runwaysTextEdited(text, ui->labelIconP3dV2Runways, atools::fs::P3D_V2); });
  connect(ui->lineEditP3dV3Runways, &QLineEdit::textEdited,
          [=](const QString &text) {runwaysTextEdited(text, ui->labelIconP3dV3Runways, atools::fs::P3D_V3); });

  // Logbook push buttons
  connect(ui->pushButtonFsxLogbook, &QPushButton::clicked,
          [=]() {logbookButtonClicked(ui->lineEditFsxLogbook, ui->labelIconFsxLogbook, atools::fs::FSX); });
  connect(ui->pushButtonFsxSeLogbook, &QPushButton::clicked,
          [=]() {logbookButtonClicked(ui->lineEditFsxSeLogbook, ui->labelIconFsxSeLogbook, atools::fs::FSX_SE); });
  connect(ui->pushButtonP3dV2Logbook, &QPushButton::clicked,
          [=]() {logbookButtonClicked(ui->lineEditP3dV2Logbook, ui->labelIconP3dV2Logbook, atools::fs::P3D_V2); });
  connect(ui->pushButtonP3dV3Logbook, &QPushButton::clicked,
          [=]() {logbookButtonClicked(ui->lineEditP3dV3Logbook, ui->labelIconP3dV3Logbook, atools::fs::P3D_V3); });

  // Logbook runways/airport push puttons
  connect(ui->pushButtonFsxRunways, &QPushButton::clicked,
          [=]() {runwaysButtonClicked(ui->lineEditFsxRunways, ui->labelIconFsxRunways, atools::fs::FSX); });
  connect(ui->pushButtonFsxSeRunways, &QPushButton::clicked,
          [=]() {runwaysButtonClicked(ui->lineEditFsxSeRunways, ui->labelIconFsxSeRunways, atools::fs::FSX_SE); });
  connect(ui->pushButtonP3dV2Runways, &QPushButton::clicked,
          [=]() {runwaysButtonClicked(ui->lineEditP3dV2Runways, ui->labelIconP3dV2Runways, atools::fs::P3D_V2); });
  connect(ui->pushButtonP3dV3Runways, &QPushButton::clicked,
          [=]() {runwaysButtonClicked(ui->lineEditP3dV3Runways, ui->labelIconP3dV3Runways, atools::fs::P3D_V3); });
  /* *INDENT-ON* */
}

PathDialog::~PathDialog()
{
  delete pixmapCheckmark;
  delete pixmapExclamation;

  delete dialog;
  delete errorHandler;
}

void PathDialog::logbookTextEdited(const QString& text, QLabel *iconLabel, SimulatorType type)
{
  changedLogbooks[type] = true;
  updateIcon(text, iconLabel);
}

void PathDialog::runwaysTextEdited(const QString& text, QLabel *iconLabel, SimulatorType type)
{
  changedRunways[type] = true;
  updateIcon(text, iconLabel);
}

void PathDialog::logbookButtonClicked(QLineEdit *edit, QLabel *iconLabel, SimulatorType type)
{
  QString text = dialog->openFileDialog(tr("Select the Logbook File (Logbook.BIN)"),
                                        tr("Logbook Files (*.bin *.BIN);;All Files (*)"),
                                        lbDialogPaths.at(type),
                                        edit->text());
  if(!text.isEmpty())
  {
    changedLogbooks[type] = true;
    edit->setText(text);
  }
  updateIcon(text, iconLabel);
}

void PathDialog::runwaysButtonClicked(QLineEdit *edit, QLabel *iconLabel, SimulatorType type)
{
  QString text = dialog->openFileDialog(tr("Select the Runways File (runways.xml)"),
                                        tr("Runways File (*.xml);;All Files (*)"),
                                        rwDialogPaths.at(type),
                                        edit->text());
  if(!text.isEmpty())
  {
    changedRunways[type] = true;
    edit->setText(text);
  }
  updateIcon(text, iconLabel);
}

void PathDialog::updateIcon(const QString& text, QLabel *iconLabel)
{
  QFileInfo fi(text);
  if(fi.exists() && fi.isReadable() && fi.isFile())
    iconLabel->setPixmap(*pixmapCheckmark);
  else
    iconLabel->setPixmap(*pixmapExclamation);
}

void PathDialog::initTab(QLineEdit *logbookEdit,
                         QLineEdit *runwaysEdit,
                         QLabel *logbookLabel,
                         QLabel *runwaysLabel,
                         SimulatorType type)
{
  if(FsPaths::getBasePath(type).isEmpty())
    ui->tabWidget->setTabEnabled(type, false);
  else
  {
    ui->tabWidget->setTabEnabled(type, true);

    Settings& s = Settings::instance();

    QFileInfo fi;
    QString logbook;
    if(s->contains(pathSettings->lbFilePaths.at(type)))
    {
      logbook = s->value(pathSettings->lbFilePaths.at(type)).toString();
      fi = logbook;
    }
    else
    {
      logbook = FsPaths::getFilesPath(type);
      fi = logbook + QDir::separator() + ll::constants::LOGBOOK_FILENAME;
    }

    if(fi.exists() && fi.isReadable() && fi.isFile())
    {
      logbookLabel->setPixmap(*pixmapCheckmark);
      logbookEdit->setText(fi.absoluteFilePath());
    }
    else
    {
      logbookLabel->setPixmap(*pixmapExclamation);
      logbookEdit->setText(logbook);
    }

    QString runways;
    if(s->contains(pathSettings->rwFilePaths.at(type)))
    {
      runways = s->value(pathSettings->rwFilePaths.at(type)).toString();
      fi = runways;
    }
    else
    {
      runways = FsPaths::getBasePath(type);
      fi = runways + QDir::separator() + ll::constants::RUNWAYS_FILENAME;
    }

    if(fi.exists() && fi.isReadable() && fi.isFile())
    {
      runwaysLabel->setPixmap(*pixmapCheckmark);
      runwaysEdit->setText(fi.absoluteFilePath());
    }
    else
    {
      runwaysLabel->setPixmap(*pixmapExclamation);
      runwaysEdit->setText(runways);
    }
  }
}

void PathDialog::accept()
{
  pathSettings->simulators[atools::fs::FSX] = ui->tabWidget->isTabEnabled(atools::fs::FSX);
  pathSettings->logbookPaths[atools::fs::FSX] = ui->lineEditFsxLogbook->text();
  pathSettings->runwayPaths[atools::fs::FSX] = ui->lineEditFsxRunways->text();

  pathSettings->simulators[atools::fs::FSX_SE] = ui->tabWidget->isTabEnabled(atools::fs::FSX_SE);
  pathSettings->logbookPaths[atools::fs::FSX_SE] = ui->lineEditFsxSeLogbook->text();
  pathSettings->runwayPaths[atools::fs::FSX_SE] = ui->lineEditFsxSeRunways->text();

  pathSettings->simulators[atools::fs::P3D_V2] = ui->tabWidget->isTabEnabled(atools::fs::P3D_V2);
  pathSettings->logbookPaths[atools::fs::P3D_V2] = ui->lineEditP3dV2Logbook->text();
  pathSettings->runwayPaths[atools::fs::P3D_V2] = ui->lineEditP3dV2Runways->text();

  pathSettings->simulators[atools::fs::P3D_V3] = ui->tabWidget->isTabEnabled(atools::fs::P3D_V3);
  pathSettings->logbookPaths[atools::fs::P3D_V3] = ui->lineEditP3dV3Logbook->text();
  pathSettings->runwayPaths[atools::fs::P3D_V3] = ui->lineEditP3dV3Runways->text();

  pathSettings->store();
  QDialog::accept();
}
