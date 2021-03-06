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
#include "export/kmlexporter.h"
#include "export/csvexporter.h"
#include "gui/pathsettings.h"

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
class QComboBox;

class MainWindow :
  public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow();
  virtual ~MainWindow();

signals:
  /* Emitted when window is shown the first time */
  void windowShown();

private:
  bool firstStart = true;
  Controller *controller = nullptr;
  CsvExporter *csvExporter = nullptr;
  HtmlExporter *htmlExporter = nullptr;
  KmlExporter *kmlExporter = nullptr;
  GlobalStats *globalStats;
  HelpHandler *helpHandler;

  PathSettings pathSettings;

  atools::gui::Dialog *dialog;
  atools::gui::ErrorHandler *errorHandler;

  QLabel *selectionLabel = nullptr;
  QComboBox *simulatorComboBox = nullptr;

  atools::sql::SqlDatabase db;
  QString databaseFile;

  bool hasAirports = false;
  bool hasLogbook = false;
  bool hasDatabaseLoadStatus = false;

  QString selectionLabelText;
  int defaultTableViewFontPointSize;

  Ui::MainWindow *ui = nullptr;

  /* Work on the close event that also catches clicking the close button
   * in the window frame */
  virtual void closeEvent(QCloseEvent *event) override;

  /* Emit a signal windowShown after first appearance */
  virtual void showEvent(QShowEvent *event) override;

  /* Reads logbook into database and handles exceptions */
  bool loadLogbookDatabase(atools::fs::SimulatorType type);

  /* Shows or hides the search bar */
  void showSearchBar(bool visible);

  /* Displays and executes the context menu of the table view */
  void tableContextMenu(const QPoint& pos);

  /* Read all settings from the settings singleton */
  void readSettings();

  /* Write all settings to the settings singleton */
  void writeSettings();

  /* Looks for runways.xml and display dialogs about found or not
   *  @return true if the file is new or was changed */
  void checkRunwaysFile(atools::fs::SimulatorType type, bool notifyChange);

  /* Load runway.xml into database and handles exceptions */
  bool loadAirports(atools::fs::SimulatorType type);

  /* Assigns the runway.xml relevant line edits to the respecive column
   * descriptors */
  void assignAirportLineEdits();
  void cleanAirportLineEdits();

  /* Test if the logbook timestamp has changed and display a dialog */
  void checkLogbookFile(atools::fs::SimulatorType type, bool notifyChange);

  /* Open and close database and handle exceptions */
  void openDatabase();
  void closeDatabase();

  /* Connect everything */
  void connectAllSlots();

  /* Connect all controller slots */
  void connectControllerSlots();

  /* Export methods */
  void exportAllCsv();
  void exportSelectedCsv();
  void exportAllHtml();
  void exportSelectedHtml();
  void exportAllKml();
  void exportSelectedKml();

  /* Fill view fully */
  void loadAllRowsIntoView();

  /* Reset "Do not show dialog again" message back to default */
  void resetMessages();

  /* Clear search */
  void resetSearch();

  /* Reset table view to default values after question dialog */
  void resetView();

  /* Shows or hides search fields depending on runways.xml */
  void showHideAirportLineEdits(bool visible);

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

  /* Show notification dialog about reloading */
  void filterLogbookEntries();

  /* Open the path management dialog and reload database if needed */
  void pathDialog();

  /* Clean model and queries before changing the database to avoid locks */
  void preDatabaseLoad();

  /* Restore view and model after database changes */
  void postDatabaseLoad(bool force = false);

  /* Check for first start (show dialog then) or file changes (reload these) */
  void startupChecks();

  void assignSearchFieldsToController();

  /* Update flags based on table presence */
  void updateDatabaseStatus();

  /* Copy CSV like text of selected rows to clipboard */
  void tableCopyCipboard();

  /* Change font size and store in settings */
  void zoomTableView(int value);

  /* Update action status (enabled/disabled) */
  void enableDisableZoomActions();

  /* Init table row height according to font size (smaller than default) */
  void initTableViewZoom();

  /* Change font size and adjust row height accordingly */
  void setTableViewFontSize(int pointSize);

  /* Reload all changed files */
  void checkAllFiles(bool notifyReload);
  void reloadChanged();

  /* Drop all table and reload available files */
  void resetDatabase();

  /* Private setup UI for all that cannot be done in designer */
  void setupUi();

  /* Check if runways file has changed and reload if needed */
  void checkRunwaysFile();

};

#endif // LITTLELOGBOOK_MAINWINDOW_H
