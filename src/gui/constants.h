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

#ifndef LITTLELOGBOOK_CONSTANTS_H
#define LITTLELOGBOOK_CONSTANTS_H

namespace ll {
namespace constants {

/* Settings keys */
extern const char *SETTINGS_TABLE;
extern const char *SETTINGS_LOGBOOK_FILE_DIALOG;
extern const char *SETTINGS_LANGUAGE;
extern const char *SETTINGS_VERSION;
extern const char *SETTINGS_SHOW_QUIT;
extern const char *SETTINGS_SHOW_RELOAD;
extern const char *SETTINGS_SHOW_NO_RUNWAYS;
extern const char *SETTINGS_SHOW_RELOAD_RUNWAYS;
extern const char *SETTINGS_SHOW_RESET_VIEW;
extern const char *SETTINGS_SHOW_FILTER_RELOAD;
extern const char *SETTINGS_LOGBOOK_FILE;
extern const char *SETTINGS_MAINWINDOW_SIZE;
extern const char *SETTINGS_MAINWINDOW_STATE;
extern const char *SETTINGS_SHOW_STATUSBAR;
extern const char *SETTINGS_SHOW_SEARCHOOL;
extern const char *SETTINGS_LOGBOOK_FILE_TIMESTAMP;
extern const char *SETTINGS_RUNWAYS_FILE;
extern const char *SETTINGS_RUNWAYS_FILE_TIMESTAMP;
extern const char *SETTINGS_EXPORT_OPEN;
extern const char *SETTINGS_EXPORT_FILE_DIALOG;

extern const char *SETTINGS_FILTER_ENTRIES;
extern const char *SETTINGS_FILTER_INVALID_DATE;
extern const char *SETTINGS_FILTER_START_AND_DEST_EMPTY;
extern const char *SETTINGS_FILTER_START_OR_DEST_EMPTY;
extern const char *SETTINGS_FILTER_START_DEST_SAME;
extern const char *SETTINGS_FILTER_MIN_FLIGH_TIME;

/* Export constants */
extern const char *EXPORT_HTML_CODEC;
extern const char *EXPORT_HTML_CSS_FILE;

/* FSX logbook filename */
extern const char *LOGBOOK_FILENAME;
extern const char *RUNWAYS_FILENAME;

/* Default language for help */
extern const char *DEFAULT_HELP_LANG;

/* Default database */
extern const char *DATABASE_TYPE;

/* Query constants */
extern const char *QUERY_PLACEHOLDER_CHAR;
extern const char *QUERY_NEGATE_CHAR;

} // namespace CONSTANTS
} // namespace ll

#endif // LITTLELOGBOOK_CONSTANTS_H
