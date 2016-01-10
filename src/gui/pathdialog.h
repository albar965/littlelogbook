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

#ifndef PATHDIALOG_H
#define PATHDIALOG_H

#include "fs/fspaths.h"

#include "gui/pathsettings.h"

#include <QDialog>

class QLineEdit;
class QPixmap;
class QLabel;

namespace Ui {
class PathDialog;
}

namespace  atools {
namespace  gui {
class ErrorHandler;
class Dialog;
}
}

class PathDialog :
  public QDialog
{
  Q_OBJECT

public:
  explicit PathDialog(QWidget *parentWidget, PathSettings *paths);
  virtual ~PathDialog();

  bool hasLogbookFileChanged(atools::fs::SimulatorType type);
  bool hasRunwaysFileChanged(atools::fs::SimulatorType type);
private:
  QList<bool> changedLogbooks;
  QList<bool> changedRunways;

  Ui::PathDialog *ui = nullptr;
  PathSettings *pathSettings;

  atools::gui::ErrorHandler *errorHandler;
  atools::gui::Dialog *dialog;

  virtual void accept() override;

  void initTab(QLineEdit *logbookEdit,
               QLineEdit *runwaysEdit,
               QLabel *logbookLabel,
               QLabel *runwaysLabel,
               atools::fs::SimulatorType type);

  QPixmap *pixmapCheckmark;
  QPixmap *pixmapExclamation;

  const QVector<QString> lbDialogPaths =
  {
    "Paths/DialogLogbookFsx", "Paths/DialogLogbookFsxSe",
    "Paths/DialogLogbookP3dV2", "Paths/DialogLogbookP3dV3"
  };

  const QVector<QString> rwDialogPaths =
  {
    "Paths/DialogRunwaysFsx", "Paths/DialogRunwaysFsxSe",
    "Paths/DialogRunwaysP3dV2", "Paths/DialogRunwaysP3dV3"
  };

  void logbookTextEdited(const QString& text, QLabel *iconLabel);
  void runwaysTextEdited(const QString& text, QLabel *iconLabel);
  void logbookButtonClicked(QLineEdit *edit, QLabel *iconLabel, atools::fs::SimulatorType type);
  void runwaysButtonClicked(QLineEdit *edit, QLabel *iconLabel, atools::fs::SimulatorType type);
  void updateIcon(const QString& text, QLabel *iconLabel);

  void dialogToSettings(QLineEdit* logbookEdit, QLineEdit* runwaysEdit, atools::fs::SimulatorType type);
};

#endif // PATHDIALOG_H
