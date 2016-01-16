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

#include "export/kmlexporter.h"
#include "gui/dialog.h"
#include "gui/constants.h"
#include "logging/loggingdefs.h"
#include "gui/errorhandler.h"
#include "table/controller.h"
#include "sql/sqlquery.h"
#include "sql/sqldatabase.h"
#include "table/formatter.h"
#include "settings/settings.h"

#include <QFile>
#include <QSqlField>
#include <QXmlStreamReader>
#include <QApplication>
#include <QDateTime>

using atools::gui::ErrorHandler;
using atools::gui::Dialog;
using atools::sql::SqlQuery;
using atools::sql::SqlDatabase;
using atools::settings::Settings;

KmlExporter::KmlExporter(QWidget *parent, Controller *controller)
  : Exporter(parent, controller)
{
  Settings& s = Settings::instance();

  // Store defaults on first access to allow user configuration
  lineColor = s.getAndStoreValue(ll::constants::SETTINGS_EXPORT_KML_LINE_COLOR, "ff00ffff").toString();
  lineWidth = s.getAndStoreValue(ll::constants::SETTINGS_EXPORT_KML_LINE_WIDTH, 2).toInt();

  startIcon = s.getAndStoreValue(ll::constants::SETTINGS_EXPORT_KML_START_ICON,
                                 "http://maps.google.com/mapfiles/kml/paddle/grn-blank.png").toString();
  destIcon = s.getAndStoreValue(ll::constants::SETTINGS_EXPORT_KML_DEST_ICON,
                                "http://maps.google.com/mapfiles/kml/paddle/red-blank.png").toString();

  startScale = s.getAndStoreValue(ll::constants::SETTINGS_EXPORT_KML_START_ICON_SCALE, 1.5).toDouble();
  destScale = s.getAndStoreValue(ll::constants::SETTINGS_EXPORT_KML_DEST_ICON_SCALE, 1.5).toDouble();

  startXHotspot = s.getAndStoreValue(ll::constants::SETTINGS_EXPORT_KML_START_X_HOTSPOT, 32).toInt();
  startYHotspot = s.getAndStoreValue(ll::constants::SETTINGS_EXPORT_KML_START_Y_HOTSPOT, 1).toInt();
  destXHotspot = s.getAndStoreValue(ll::constants::SETTINGS_EXPORT_KML_DEST_X_HOTSPOT, 32).toInt();
  destYHotspot = s.getAndStoreValue(ll::constants::SETTINGS_EXPORT_KML_DEST_Y_HOTSPOT, 1).toInt();

  airportDetailQuery = new SqlQuery(controller->getSqlDatabase());
  airportDetailQuery->prepare("select longitude, latitude, altitude, max_runway_length, has_lights, has_ils "
                              "from airport "
                              "where icao = :icao");
}

KmlExporter::~KmlExporter()
{
  delete airportDetailQuery;
}

QString KmlExporter::saveKmlFileDialog()
{
  return dialog->saveFileDialog(tr("Export Google Earth KML Document"),
                                tr("KML Documents (*.kml);;All Files (*)"),
                                "kml", ll::constants::SETTINGS_EXPORT_FILE_DIALOG);
}

void KmlExporter::skippedEntriesDialog(int skipped)
{
  if(skipped > 0)
    dialog->showInfoMsgBox(ll::constants::SETTINGS_SHOW_SKIPPED_KML,
                           QString(tr("%1 logbook entries were not exported due to "
                                      "missing start or destination airport coordinates.")).arg(skipped),
                           tr("Do not &show this dialog again."));
}

int KmlExporter::exportAll(bool open)
{
  int exported = 0, skipped = 0;
  QString filename = saveKmlFileDialog();
  qDebug() << "exportAllKml" << filename;

  if(filename.isEmpty())
    return 0;

  // Open file and write all headers including styles
  QFile file(filename);
  QXmlStreamWriter stream;
  if(!startFile(file, stream))
    return exported;

  // Run the current query to get all results - not only the visible
  SqlDatabase *db = controller->getSqlDatabase();
  SqlQuery query(db);
  query.exec(controller->getCurrentSqlQuery());

  while(query.next())
  {
    QSqlRecord rec = query.record();
    if(!(rec.isNull("airport_from_name") || rec.isNull("airport_to_name")))
    {
      writeFlight(stream, rec);
      exported++;
    }
    else
      skipped++;
  }

  endFile(file, stream);

  skippedEntriesDialog(skipped);

  if(open)
    openDocument(filename);
  return exported;
}

int KmlExporter::exportSelected(bool open)
{
  int exported = 0, skipped = 0;
  QString filename = saveKmlFileDialog();
  qDebug() << "exportSelectedKml" << filename;

  if(filename.isEmpty())
    return 0;

  // Open file
  QFile file(filename);
  QXmlStreamWriter stream;
  if(!startFile(file, stream))
    return exported;

  QSqlRecord rec;
  const QItemSelection sel = controller->getSelection();
  for(QItemSelectionRange rng : sel)
    for(int row = rng.top(); row <= rng.bottom(); ++row)
    {
      fillRecord(controller->getRawModelData(row), controller->getRawModelColumns(), rec);
      if(!(rec.isNull("airport_from_name") || rec.isNull("airport_to_name")))
      {
        writeFlight(stream, rec);
        exported++;
      }
      else
        skipped++;
    }

  endFile(file, stream);

  skippedEntriesDialog(skipped);

  if(open)
    openDocument(filename);
  return exported;
}

bool KmlExporter::startFile(QFile& file, QXmlStreamWriter& stream)
{
  if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
  {
    errorHandler->handleIOError(file);
    return false;
  }

  stream.setDevice(&file);
  stream.setAutoFormatting(true);
  stream.setAutoFormattingIndent(2);
  stream.setCodec(ll::constants::EXPORT_KML_CODEC);
  stream.writeStartDocument();
  stream.writeStartElement("kml");
  stream.writeDefaultNamespace("http://www.opengis.net/kml/2.2");
  stream.writeStartElement("Document");
  stream.writeTextElement("name", QApplication::applicationName());
  stream.writeTextElement("description", QString(tr("Version %1 (revision %2)<br/>"
                                                    "by Alexander Barthel.<br/>"
                                                    "Created on %3.")).
                          arg(QApplication::applicationVersion()).
                          arg(GIT_REVISION).
                          arg(QDateTime::currentDateTime().toString(Qt::DefaultLocaleLongDate)));

  writeLineStyle(stream);
  writePointStyle(stream, true);
  writePointStyle(stream, false);

  return true;
}

void KmlExporter::endFile(QFile& file, QXmlStreamWriter& stream)
{
  stream.writeEndElement(); // Document
  stream.writeEndElement(); // kml
  stream.writeEndDocument();

  file.close();
}

void KmlExporter::writeLineStyle(QXmlStreamWriter& stream)
{
  stream.writeStartElement("Style");
  stream.writeAttribute("id", "line_style");
  stream.writeStartElement("LineStyle");
  stream.writeTextElement("color", lineColor);
  stream.writeTextElement("width", QString::number(lineWidth));
  stream.writeEndElement(); // LineStyle
  stream.writeEndElement(); // Style
}

void KmlExporter::writePointStyle(QXmlStreamWriter& stream, bool start)
{
  stream.writeStartElement("Style");
  stream.writeAttribute("id", start ? "start_style" : "dest_style");
  stream.writeStartElement("IconStyle");
  stream.writeTextElement("scale", QString::number(start ? startScale : destScale));
  stream.writeStartElement("Icon");
  stream.writeTextElement("href", start ? startIcon : destIcon);
  stream.writeEndElement(); // Icon
  stream.writeStartElement("hotSpot");
  stream.writeAttribute("x", QString::number(start ? startXHotspot : destXHotspot));
  stream.writeAttribute("y", QString::number(start ? startYHotspot : destYHotspot));
  stream.writeAttribute("xunits", "pixels");
  stream.writeAttribute("yunits", "pixels");
  stream.writeEndElement(); // hotspot
  stream.writeEndElement(); // IconStyle
  stream.writeEndElement(); // Style

}

void KmlExporter::writeFlight(QXmlStreamWriter& stream, QSqlRecord rec)
{
  QString fromIcao(rec.value("airport_from_icao").toString());
  QString toIcao(rec.value("airport_to_icao").toString());
  QSqlRecord fromRec(airportDetail(fromIcao));
  QSqlRecord toRec(airportDetail(toIcao));

  QString fromCoord = QString("%1,%2,0").
                      arg(fromRec.value("longitude").toDouble()).
                      arg(fromRec.value("latitude").toDouble());
  QString toCoord = QString("%1,%2,0").
                    arg(toRec.value("longitude").toDouble()).
                    arg(toRec.value("latitude").toDouble());

  QString shortDescr = QString(tr("%1 to %2")).arg(fromIcao).arg(toIcao);

  stream.writeStartElement("Folder");
  stream.writeTextElement("name", shortDescr);
  stream.writeTextElement("description", flightDescription(rec));

  // Line
  stream.writeStartElement("Placemark");
  stream.writeTextElement("name", shortDescr);
  stream.writeTextElement("description", flightDescription(rec));
  stream.writeTextElement("styleUrl", "#line_style");
  stream.writeStartElement("LineString");
  stream.writeTextElement("tessellate", "1");
  stream.writeTextElement("coordinates", fromCoord + " " + toCoord);
  stream.writeEndElement(); // LineString
  stream.writeEndElement(); // Placemark

  // Start icon
  stream.writeStartElement("Placemark");
  stream.writeTextElement("name", QString(tr("From %1 (%2)")).
                          arg(fromIcao).arg(rec.value("airport_from_name").toString()));
  stream.writeTextElement("description", airportDescription(rec, fromRec, "from"));
  stream.writeTextElement("styleUrl", "#start_style");
  stream.writeStartElement("Point");
  stream.writeTextElement("coordinates", fromCoord);
  stream.writeEndElement(); // Point
  stream.writeEndElement(); // Placemark

  // Destination icon
  stream.writeStartElement("Placemark");
  stream.writeTextElement("name", QString(tr("To %1 (%2)")).
                          arg(toIcao).arg(rec.value("airport_to_name").toString()));
  stream.writeTextElement("description", airportDescription(rec, toRec, "to"));
  stream.writeTextElement("styleUrl", "#dest_style");
  stream.writeStartElement("Point");
  stream.writeTextElement("coordinates", toCoord);
  stream.writeEndElement(); // Point
  stream.writeEndElement(); // Placemark

  stream.writeEndElement(); // Folder
}

QSqlRecord KmlExporter::airportDetail(const QString& icao)
{
  airportDetailQuery->bindValue(":icao", icao);
  airportDetailQuery->exec();
  if(airportDetailQuery->next())
    return airportDetailQuery->record();

  return QSqlRecord();
}

QString KmlExporter::airportDescription(QSqlRecord lbRec, QSqlRecord apRec, const QString& fromTo)
{
  QLocale l;
  QString retval("<table border=\"1\" cellpadding=\"2\" cellspacing=\"0\"><tbody>");

  retval += "<tr><td><b>" + tr("ICAO:") + "</b></td><td><b>" +
            lbRec.value("airport_" + fromTo + "_icao").toString() + "</b></td>";
  retval += "<tr><td><b>" + tr("Name:") + "</b></td><td><b>" +
            lbRec.value("airport_" + fromTo + "_name").toString() + "</b></td>";
  retval += "<tr><td>" + tr("City:") + "</td><td>" +
            lbRec.value("airport_" + fromTo + "_city").toString() + "</td>";
  QString state(lbRec.value("airport_" + fromTo + "_state").toString());
  if(!state.isEmpty())
    retval += "<tr><td>" + tr("State:") + "</td><td>" + state + "</td>";
  retval += "<tr><td>" + tr("Country:") + "</td><td>" +
            lbRec.value("airport_" + fromTo + "_country").toString() + "</td>";
  retval += "<tr><td>" + tr("Longitude:") + "</td><td>" +
            l.toString(apRec.value("longitude").toDouble(), 'f', 6) + "</td>";
  retval += "<tr><td>" + tr("Latitude:") + "</td><td>" +
            l.toString(apRec.value("latitude").toDouble(), 'f', 6) + "</td>";
  retval += "<tr><td>" + tr("Altitude:") + "</td><td>" +
            l.toString(apRec.value("altitude").toInt()) + " ft</td>";
  retval += "<tr><td>" + tr("Longest Runway:") + "</td><td>" +
            l.toString(apRec.value("max_runway_length").toInt()) + " ft</td>";

  if(apRec.value("has_lights").toBool())
    retval += "<tr><td>" + tr("Has Lights") + "</td>";
  if(apRec.value("has_ils").toBool())
    retval += "<tr><td>" + tr("Has ILS") + "</td>";

  retval += "</tbody></table>";
  return retval;
}

QString KmlExporter::flightDescription(QSqlRecord rec)
{
  QString fromName(rec.value("airport_from_name").toString());
  QString toName(rec.value("airport_to_name").toString());
  QString fromIcao(rec.value("airport_from_icao").toString());
  QString toIcao(rec.value("airport_to_icao").toString());

  QLocale l;
  QString retval = QString("<p><b>" + tr("From %1 (%2)<br/>to %3 (%4)") + "</b></p>").
                   arg(fromIcao).arg(fromName).arg(toIcao).arg(toName);

  retval += "<table border=\"1\" cellpadding=\"2\" cellspacing=\"0\"><tbody>";

  retval += "<tr><td>" + tr("Simulator:") + "</td><td>" +
            controller->formatModelData("simulator_id", rec.value("simulator_id")) + "</td>";
  retval += "<tr><td>" + tr("Logbook ID:") + "</td><td>" +
            controller->formatModelData("logbook_id", rec.value("logbook_id")) + "</td>";
  retval += "<tr><td>" + tr("Start Time:") + "</td><td>" +
            controller->formatModelData("startdate", rec.value("startdate")) + "</td>";

  retval += "<tr><td>" + tr("Distance:") + "</td><td>" +
            l.toString(rec.value("distance").toDouble(), 'f', 0) + " NM</td>";
  retval += "<tr><td>" + tr("Total Time:") + "</td><td>" +
            formatter::formatMinutesHoursLong(rec.value("total_time").toDouble()) + "</td>";
  retval += "<tr><td>" + tr("Night Time:") + "</td><td>" +
            formatter::formatMinutesHoursLong(rec.value("night_time").toDouble()) + "</td>";
  retval += "<tr><td>" + tr("Instrument Time:") + "</td><td>" +
            formatter::formatMinutesHoursLong(rec.value("instrument_time").toDouble()) + "</td>";

  QString descr = rec.value("description").toString();
  if(!descr.isEmpty())
    retval += "<tr><td>" + tr("Flight Description:") + "</td><td>" + descr + "</td>";
  retval += "<tr><td>" + tr("Aircraft Registration:") + "</td><td>" +
            rec.value("aircraft_reg").toString() + "</td>";
  retval += "<tr><td>" + tr("Aircraft Description:") + "</td><td>" +
            rec.value("aircraft_descr").toString() + "</td>";
  retval += "<tr><td>" + tr("Aircraft Type:") + "</td><td>" +
            controller->formatModelData("aircraft_type", rec.value("aircraft_type")) + "</td>";

  retval += "</tbody></table>";
  return retval;
}
