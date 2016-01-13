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

#include "gui/globalstats.h"

#include "table/formatter.h"
#include "sql/sqlquery.h"

#include <QApplication>

using atools::sql::SqlQuery;

GlobalStats::GlobalStats(QWidget *parent, atools::sql::SqlDatabase *sqlDb)
  : QObject(parent), db(sqlDb)
{
  // Create darker colors dynamically from default palette
  color = QApplication::palette().color(QPalette::Active, QPalette::Base).
          darker(105).name(QColor::HexRgb);
  colorAlt = QApplication::palette().color(QPalette::Active, QPalette::AlternateBase).
             darker(105).name(QColor::HexRgb);

  tableRow.append("<tr bgcolor=\"" + color + "\"><td>%1</td><td>%2</td></tr>");
  tableRow.append("<tr bgcolor=\"" + colorAlt + "\"><td>%1</td><td>%2</td></tr>");

  tableRowAlignRight.append("<tr bgcolor=\"" + color + "\"><td>%1</td><td align=\"right\">%2</td></tr>");
  tableRowAlignRight.append("<tr bgcolor=\"" + colorAlt + "\"><td>%1</td><td align=\"right\">%2</td></tr>");

  tableRowHeader = "<tr><td>%1</td></tr>";
}

GlobalStats::~GlobalStats()
{

}

QString GlobalStats::createGlobalStatsReport(atools::fs::SimulatorType type,
                                             bool hasLogbook,
                                             bool hasAirports)
{
  QString html(
    "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">"
      "<html>"
        "<head>"
        "</head>"
        "<body style=\"font-family:'sans'; font-size:8pt; font-weight:400; font-style:normal;\">");

  if(hasLogbook)
  {
    QString queryStr = buildQueryStr(hasAirports);

    if(type != atools::fs::ALL_SIMULATORS)
      queryStr += " where simulator_id = " + QString::number(type);

    SqlQuery query(db);
    query.exec(queryStr);

    SqlQuery startDateQuery(db);

    QString countQueryStr = "select "
                            "  min(startdate) as earliest_flight, "
                            "  max(startdate) as latest_flight "
                            "from logbook where startdate > 0";

    if(type != atools::fs::ALL_SIMULATORS)
      countQueryStr += " and simulator_id = " + QString::number(type);

    startDateQuery.exec(countQueryStr);

    if(query.next())
    {
      if(query.value("num_flights").toInt() > 0)
      {
        // For internal use string built HTML is sufficient
        int i = 0;
        html += "<table border=\"0\" cellpadding=\"2\" cellspacing=\"0\"><tbody>";

        html += alt(i++, tableRow).arg(bold(tr("Number of flights:"))).
                arg(query.value("num_flights").toString());

        if(startDateQuery.next())
        {
          html += alt(i++, tableRow).arg(bold(tr("Earliest flight:"))).
                  arg(formatter::formatDateLong(startDateQuery.value("earliest_flight").toInt()));
          html += alt(i++, tableRow).arg(bold(tr("Latest flight:"))).
                  arg(formatter::formatDateLong(startDateQuery.value("latest_flight").toInt()));
        }
        html += "</tbody></table>";

        if(hasAirports)
        {
          int i2 = 0;
          html += header(tr("Distances"));
          html += "<table border=\"0\" cellpadding=\"2\" cellspacing=\"0\"><tbody>";

          html += alt(i2++, tableRowAlignRight).arg(bold(tr("Total:"))).
                  arg(formatter::formatDoubleUnit(query.value("distance_sum").toDouble(),
                                                  tr("NM", "Nautical miles")));
          html += alt(i2++, tableRowAlignRight).arg(bold(tr("Maximum:"))).
                  arg(formatter::formatDoubleUnit(query.value("distance_max").toDouble(),
                                                  tr("NM", "Nautical miles")));
          html += alt(i2++, tableRowAlignRight).arg(bold(tr("Average:"))).
                  arg(formatter::formatDoubleUnit(query.value("distance_avg").toDouble(),
                                                  tr("NM", "Nautical miles")));
          html += "</tbody></table>";
        }

        i = 0;
        html += header(tr("Airports"));
        html += "<table border=\"0\" cellpadding=\"2\" cellspacing=\"0\"><tbody>";
        html += tableRowHeader.arg(bold(tr("Number of distinct:")));
        html += alt(i++, tableRowAlignRight).arg(bold(tr("Start airports:"))).
                arg(query.value("airport_from_icao_cnt").toString());
        if(hasAirports)
          html += alt(i++, tableRowAlignRight).arg(bold(tr("Start countries:"))).
                  arg(query.value("airport_from_country_cnt").toString());

        html += alt(i++, tableRowAlignRight).arg(bold(tr("Destination airports:"))).
                arg(query.value("airport_to_icao_cnt").toString());

        if(hasAirports)
          html += alt(i++, tableRowAlignRight).arg(bold(tr("Destination countries:"))).
                  arg(query.value("airport_to_country_cnt").toString());
        html += "</tbody></table>";

        i = 0;
        html += header(tr("Flight time"));
        html += "<table border=\"0\" cellpadding=\"2\" cellspacing=\"0\"><tbody>";
        html += alt(i++, tableRowAlignRight).arg(bold(tr("Total:"))).
                arg(formatter::formatMinutesHoursDaysLong(query.value("total_time_sum").toDouble()));
        html += alt(i++, tableRowAlignRight).arg(bold(tr("Maximum:"))).
                arg(formatter::formatMinutesHoursDaysLong(query.value("total_time_max").toDouble()));
        html += alt(i++, tableRowAlignRight).arg(bold(tr("Average:"))).
                arg(formatter::formatMinutesHoursDaysLong(query.value("total_time_avg").toDouble()));
        html += "</tbody></table>";

        i = 0;
        html += header(tr("Night flight time"));
        html += "<table border=\"0\" cellpadding=\"2\" cellspacing=\"0\"><tbody>";
        html += alt(i++, tableRowAlignRight).arg(bold(tr("Total:"))).
                arg(formatter::formatMinutesHoursDaysLong(query.value("night_time_sum").toDouble()));
        html += alt(i++, tableRowAlignRight).arg(bold(tr("Maximum:"))).
                arg(formatter::formatMinutesHoursDaysLong(query.value("night_time_max").toDouble()));
        html += "</tbody></table>";

        i = 0;
        html += header(tr("Instrument flight time"));
        html += "<table border=\"0\" cellpadding=\"2\" cellspacing=\"0\"><tbody>";
        html += alt(i++, tableRowAlignRight).arg(bold(tr("Total:"))).
                arg(formatter::formatMinutesHoursDaysLong(query.value("instrument_time_sum").toDouble()));
        html += alt(i++, tableRowAlignRight).arg(bold(tr("Maximum:"))).
                arg(formatter::formatMinutesHoursDaysLong(query.value("instrument_time_max").toDouble()));
        html += "</tbody></table>";

        i = 0;
        html += header(tr("Aircraft"));
        html += "<table border=\"0\" cellpadding=\"2\" cellspacing=\"0\"><tbody>";
        html += tableRowHeader.arg(bold(tr("Number of distinct:")));
        html += alt(i++, tableRowAlignRight).arg(bold(tr("Aircrafts:"))).
                arg(query.value("aircraft_descr_cnt").toString());
        html += alt(i++, tableRowAlignRight).arg(bold(tr("Aircraft registrations:"))).
                arg(query.value("aircraft_reg_cnt").toString());
        html += "</tbody></table>";
      }
      else
        html += tr("No Logbook Entries found");
    } // if(query.next())
  } // if(hasLogbook)
  else
    html += tr("No Logbook loaded");

  html += "</body></html>";
  return html;
}

QString GlobalStats::buildQueryStr(bool hasAirports)
{
  QString queryStr;

  queryStr += "select "
              "  count(distinct airport_from_icao) as airport_from_icao_cnt, "
              "  count(distinct airport_to_icao) as airport_to_icao_cnt, ";

  if(hasAirports)
    queryStr += "  count(distinct airport_from_country) as airport_from_country_cnt, "
                "  count(distinct airport_to_country) as airport_to_country_cnt, "
                "  max(distance) as distance_max, "
                "  avg(distance) as distance_avg, "
                "  sum(distance) as distance_sum, ";

  queryStr += "  max(total_time) as total_time_max, "
              "  avg(total_time) as total_time_avg, "
              "  sum(total_time) as total_time_sum, "
              "  max(night_time) as night_time_max, "
              "  avg(night_time) as night_time_avg, "
              "  sum(night_time) as night_time_sum, "
              "  max(instrument_time) as instrument_time_max, "
              "  avg(instrument_time) as instrument_time_avg, "
              "  sum(instrument_time) as instrument_time_sum, "
              "  count(distinct aircraft_reg) as aircraft_reg_cnt, "
              "  count(distinct aircraft_descr) as aircraft_descr_cnt, "
              "  count(1) as num_flights "
              "from logbook";

  return queryStr;
}

const QString& GlobalStats::alt(int index, const QStringList& list) const
{
  return list.at(index % list.size());
}

QString GlobalStats::header(const QString& str)
{
  return "<h4>" + str + "</h4>";
}

QString GlobalStats::bold(const QString& str)
{
  return "<b>" + str + "</b>";
}

QString GlobalStats::paragraph(const QString& str)
{
  return "<p>" + str + "</p>";
}
