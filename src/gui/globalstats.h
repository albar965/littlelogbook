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

#ifndef LITTLELOGBOOK_GLOBALSTATS_H
#define LITTLELOGBOOK_GLOBALSTATS_H

#include <QObject>
#include <QWidget>

namespace atools {
namespace sql {
class SqlDatabase;
} // namespace sql
} // namespace atools

/*
 * Runs a query in the logbook table and creates an HTML document with
 * overall logbook statistics.
 */
class GlobalStats :
  public QObject
{
  Q_OBJECT

public:
  GlobalStats(QWidget *parent, atools::sql::SqlDatabase *sqlDb);
  virtual ~GlobalStats();

  /*
   * Creates a HTML report containing overall statistics from the logbook table.
   *
   * @param hasLogbook true if logbook is populated. Otherwise returns a short
   * message.
   * @param hasAirports true if additional airport information is available
   * @return String containing a HTML report
   *
   */
  QString createGlobalStatsReport(bool hasLogbook, bool hasAirports);

private:
  atools::sql::SqlDatabase *db;

  QString color, colorAlt, tableRowHeader;
  QStringList tableRow, tableRowAlignRight;

  /* Select alternating entries based on the index from the string list */
  const QString& alt(int index, const QStringList& list) const;

  /* Get HTML header */
  QString header(const QString& str);

  /* Return string in bold */
  QString bold(const QString& str);

  /* Return string enclosed in a paragraph */
  QString paragraph(const QString& str);

  QString buildQueryStr(bool hasAirports);

};

#endif // LITTLELOGBOOK_GLOBALSTATS_H
