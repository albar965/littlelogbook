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

#ifndef LITTLELOGBOOK_AIRPORTINFO_H
#define LITTLELOGBOOK_AIRPORTINFO_H

#include <QVariant>

namespace atools {
namespace sql {
class SqlDatabase;
}
}

class QWidget;

/*
 * Creates a small HTML table that contains information for a given airport ICAO
 * code.
 */
class AirportInfo :
  public QObject
{
  Q_OBJECT

public:
  AirportInfo(QWidget *parent, atools::sql::SqlDatabase *sqlDb);
  virtual ~AirportInfo();

  /*
   * Needs airport table present and populated in the database.
   * @return String containing HTML code describing the airport
   */
  QString createAirportHtml(const QString& icao) const;

private:
  QWidget *parentWidget;
  atools::sql::SqlDatabase *db;

};

#endif // LITTLELOGBOOK_AIRPORTINFO_H
