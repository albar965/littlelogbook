/*****************************************************************************
* Copyright 2015 Alexander Barthel albar965@mailbox.org
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

#include "gui/airportinfo.h"

#include "sql/sqlquery.h"
#include "sql/sqlutil.h"

using atools::sql::SqlQuery;
using atools::sql::SqlUtil;
using atools::sql::SqlDatabase;

AirportInfo::AirportInfo(QWidget *parent, atools::sql::SqlDatabase *sqlDb)
  : parentWidget(parent), db(sqlDb)
{

}

AirportInfo::~AirportInfo()
{

}

QString AirportInfo::createAirportHtml(const QString& icao) const
{
  SqlQuery query(db);
  query.prepare(SqlUtil(db).buildSelectStatement("airport") + " where icao = :icao");
  query.bindValue(":icao", icao);
  query.exec();

  if(query.next())
  {
    // Use plain text to build the HTML code. No need for XML stream classes
    // here
    QString tableHtml("<table border=\"0\" cellpadding=\"2\" cellspacing=\"0\">"
                        "<tbody>");

    QString tableRow("<tr><td>%1</td><td>%2</td></tr>");
    QString tableRowUnit("<tr><td>%1</td><td>%2 %3</td></tr>");

    tableHtml += tableRow.arg(tr("ICAO Id:")).arg(query.value("icao").toString());
    tableHtml += tableRow.arg(tr("Name:")).arg(query.value("name").toString());
    tableHtml += tableRow.arg(tr("City:")).arg(query.value("city").toString());
    if(!query.value("state").isNull())
      tableHtml += tableRow.arg(tr("State:")).arg(query.value("state").toString());
    tableHtml += tableRow.arg(tr("Country:")).arg(query.value("country").toString());
    tableHtml += tableRowUnit.arg(tr("Altitude:")).arg(query.value("altitude").toInt()).arg(tr("ft"));

    int rwyLength = query.value("max_runway_length").toInt();
    if(rwyLength > 0)
      tableHtml += tableRowUnit.arg(tr("Longest runway:")).arg(rwyLength).arg(tr("ft"));

    if(query.value("has_lights").toBool())
      tableHtml += tableRow.arg(tr("Has lights")).arg("");

    if(query.value("has_ils").toBool())
      tableHtml += tableRow.arg(tr("Has ILS")).arg("");

    tableHtml += "</tbody>"
                 "</table>";
    return tableHtml;
  }

  return QString();
}
