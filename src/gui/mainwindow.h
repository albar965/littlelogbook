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

#ifndef LITTLELOGBOOK_MAINWINDOW_H
#define LITTLELOGBOOK_MAINWINDOW_H

#include "sql/sqldatabase.h"
#include "export/htmlexporter.h"
#include "export/csvexporter.h"

#include <QDateTime>
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

namespace atools {
namespace gui {
class Dialog;
class ErrorHandler;
} // namespace gui

} // namespace atools

class CsvExporter;
class Controller;
class GlobalStats;
class HelpHandler;

class QItemSelection;
class QLabel;

class MainWindow :
  public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow();
  virtual ~MainWindow();

private:
  Controller *controller = nullptr;
  CsvExporter *csvExporter = nullptr;
  HtmlExporter *htmlExporter = nullptr;
  GlobalStats *globalStats;
  HelpHandler *helpHandler;

  atools::gui::Dialog *dialog;
  atools::gui::ErrorHandler *errorHandler;

  QLabel *selectionLabel = nullptr;

  atools::sql::SqlDatabase db;
  QString logbookFilename, runwaysFilename, databaseFile, fsxPath;
  QDateTime logbookFileTimestamp, runwaysFileTimestamp;
  bool hasAirports = false;
  bool hasLogbook = false;

  QString selectionLabelText;

  Ui::MainWindow *ui = nullptr;

  /* Reads logbook into database and handles exceptions */
  bool loadLogbookDatabase();

  /* Reads logbook from stored path or shows a file dialog, loads the file and
   *refreshes the GUI. Connected to action only. */
  void openLogbook();

  /* Reads FSX path from registry or from settings if not running
   * under Windows */
  void readFsxPath();

  /* Reload logbook and update GUI */
  void reloadLogbook();

  /* Shows or hides the search bar */
  void showSearchBar(bool visible);

  /* Displays and executes the context menu of the table view */
  void tableContextMenu(const QPoint& pos);

  /* Read all settings from the settings singleton */
  void readSettings();

  /* Write all settings to the settings singleton */
  void writeSettings();

  /* Work on the close event that also catches clicking the close button
   * in the window frame */
  virtual void closeEvent(QCloseEvent *event) override;

  /* Show file dialog if file not set or show dialog about logbook found */
  QString openLogbookFile(const QString& dir = QString());

  /* Tries to find the logbook at the default location */
  QString findFsxDocuments();

  /* Looks for runways.xml and display dialogs about found or not
   *  @return true if the file is new or was changed */
  bool checkRunwaysFile();

  /* Load runway.xml into database and handles exceptions */
  bool loadAirports();

  /* Assigns the runway.xml relevant line edits to the respecive column
   * descriptors */
  void assignAirportLineEdits();
  void cleanAirportLineEdits();

  /* Test if the logbook timestamp has changed and display a dialog */
  void checkLogbookFile(bool airportsChanged);

  /* Open and close database and handle exceptions */
  void openDatabase();
  void closeDatabase();

  /* Connect everything */
  void connectAllSlots();

  /* Connect all controller slots */
  void connectControllerSlots();

  /* Export methods */
  void exportAllCsv();
  void exportAllHtml();
  void exportSelectedCsv();
  void exportSelectedHtml();

  /* Fill view fully */
  void loadAllRowsIntoView();

  /* Reset "Do not show dialog again" message back to default */
  void resetMessages();

  /* Clear search */
  void resetSearch();

  /* Reset table view to default values after question dialog */
  void resetView();

  /* Shows or hides search fields depending on runways.xml */
  void showHideAirportLineEdits();

  /* Update widget and action status when selection in table has changed */
  void tableSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected);
  void updateWidgetsOnSelection();

  /* Release table group by */
  void ungroup();

  /* Update show toolbar, statusbar, etc. action states */
  void updateActionStates();

  /* Update the statistics dock window */
  void updateGlobalStats();

  /* Update actions depending on loaded logbook, runways.xml etc. */
  void updateWidgetStatus();

};

#endif // LITTLELOGBOOK_MAINWINDOW_H
