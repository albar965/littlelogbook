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

#include "gui/constants.h"

namespace ll {
namespace constants {

const char *SETTINGS_TABLE = "MainWindow/TableView";
const char *SETTINGS_LOGBOOK_FILE_DIALOG = "MainWindow/Logbook";
const char *SETTINGS_LANGUAGE = "MainWindow/Language";
const char *SETTINGS_SHOW_QUIT = "Actions/ShowQuit";
const char *SETTINGS_SHOW_RELOAD = "Actions/ShowReloadLogbook";
const char *SETTINGS_SHOW_NO_RUNWAYS = "Actions/ShowRunwaysNotFound";
const char *SETTINGS_SHOW_RELOAD_RUNWAYS = "Actions/ShowReloadRunways";
const char *SETTINGS_SHOW_RESET_VIEW = "Actions/ShowResetView";
const char *SETTINGS_SHOW_FILTER_RELOAD = "Actions/ShowFilterReload";
const char *SETTINGS_LOGBOOK_FILE = "File/Logbook";
const char *SETTINGS_MAINWINDOW_SIZE = "MainWindow/Size";
const char *SETTINGS_MAINWINDOW_STATE = "MainWindow/Properties";
const char *SETTINGS_SHOW_STATUSBAR = "MainWindow/StatusBar";
const char *SETTINGS_SHOW_SEARCHOOL = "MainWindow/SearchTool";
const char *SETTINGS_LOGBOOK_FILE_TIMESTAMP = "File/LogbookTimestamp";
const char *SETTINGS_RUNWAYS_FILE = "File/Runways";
const char *SETTINGS_RUNWAYS_FILE_TIMESTAMP = "File/RunwaysFileTimestamp";

const char *SETTINGS_FILTER_ENTRIES = "Filter/FilterEntries";
const char *SETTINGS_FILTER_INVALID_DATE = "Filter/InvalidDate";
const char *SETTINGS_FILTER_START_AND_DEST_EMPTY = "Filter/StartAndDestEmpty";
const char *SETTINGS_FILTER_START_OR_DEST_EMPTY = "Filter/StartOrDestEmpty";
const char *SETTINGS_FILTER_START_DEST_SAME = "Filter/StartAndDestSame";
const char *SETTINGS_FILTER_MIN_FLIGH_TIME = "Filter/MinFlightTime";

const char *SETTINGS_EXPORT_OPEN = "File/OpenAfterExport";
const char *SETTINGS_EXPORT_FILE_DIALOG = "MainWindow/Export";

const char *SETTINGS_FSX_PATH = "File/FsxPath";

const char *FSX_REGISTRY_PATH = "HKEY_CURRENT_USER\\Software\\Microsoft\\Microsoft Games\\Flight Simulator";
const char *FSX_REGISTRY_KEY = "10.0/AppPath";

const char *EXPORT_HTML_CODEC = "UTF-8";
const char *EXPORT_HTML_CSS_FILE = ":/littlelogbook/resources/css/export.css";

const char *LOGBOOK_FILENAME = "Logbook.BIN";

const char *DEFAULT_HELP_LANG = "en";

const char *DATABASE_TYPE = "QSQLITE";

const char *QUERY_PLACEHOLDER_CHAR = "*";
const char *QUERY_NEGATE_CHAR = "-";

} // namespace CONSTANTS
} // namespace ll
