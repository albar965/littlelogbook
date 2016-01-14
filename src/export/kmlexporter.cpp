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

#include <QFile>
#include <QXmlStreamReader>

using atools::gui::ErrorHandler;
using atools::gui::Dialog;
using atools::sql::SqlQuery;
using atools::sql::SqlDatabase;

KmlExporter::KmlExporter(QWidget *parent, Controller *controller)
  : Exporter(parent, controller)
{
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

int KmlExporter::exportAll(bool open)
{
  int exported = 0;
  QString filename = "/home/alex/Temp/_export/export.kml"; /*saveKmlFileDialog();*/
  qDebug() << "exportAllKml" << filename;

  if(filename.isEmpty())
    return 0;

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
      writeFlight(stream, rec);
    exported++;
  }

  endFile(file, stream);
  if(open)
    openDocument(filename);
  return exported;
}

int KmlExporter::exportSelected(bool open)
{
  int exported = 0;
  QString filename = saveKmlFileDialog();
  qDebug() << "exportSelectedKml" << filename;

  if(filename.isEmpty())
    return 0;

  QFile file(filename);
  QXmlStreamWriter stream;
  if(!startFile(file, stream))
    return exported;

  // TODO

  endFile(file, stream);

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
  stream.writeTextElement("name", file.fileName());

  writeLineStyle(stream);
  writePointStyle(stream, true);
  writePointStyle(stream, false);

  stream.writeStartElement("Folder");
  stream.writeTextElement("name", "Exported Flights");
  return true;
}

void KmlExporter::endFile(QFile& file, QXmlStreamWriter& stream)
{
  stream.writeEndElement(); // Folder
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
  QString fromName = rec.value("airport_from_name").toString();
  QString toName = rec.value("airport_to_name").toString();
  QString shortDescr = QString("%1 to %2").arg(fromIcao).arg(toIcao);

  stream.writeStartElement("Folder");
  stream.writeTextElement("name", shortDescr);

  stream.writeStartElement("Placemark");
  stream.writeTextElement("name", shortDescr);
  stream.writeTextElement("description", QString("From %1 (%2) to %3 (%4)").
                          arg(fromIcao).arg(fromName).arg(toIcao).arg(toName));
  stream.writeTextElement("styleUrl", "#line_style");
  stream.writeStartElement("LineString");
  stream.writeTextElement("tessellate", "1");
  stream.writeTextElement("coordinates", fromCoord + " " + toCoord);
  stream.writeEndElement(); // LineString
  stream.writeEndElement(); // Placemark

  stream.writeStartElement("Placemark");
  stream.writeTextElement("name", QString("From %1 (%2)").arg(fromIcao).arg(fromName));
  stream.writeTextElement("description", airportDescription(rec, fromRec, "from"));
  stream.writeTextElement("styleUrl", "#start_style");
  stream.writeStartElement("Point");
  stream.writeTextElement("coordinates", fromCoord);
  stream.writeEndElement(); // Point
  stream.writeEndElement(); // Placemark

  stream.writeStartElement("Placemark");
  stream.writeTextElement("name", QString("To %1 (%2)").arg(toIcao).arg(toName));
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
  QString retval("<table border=\"0\" cellpadding=\"2\" cellspacing=\"0\"><tbody>");

  retval += "<tr><td>ICAO:</td><td>" + lbRec.value("airport_" + fromTo + "_icao").toString() + "</td>";
  retval += "<tr><td>Name:</td><td>" + lbRec.value("airport_" + fromTo + "_name").toString() + "</td>";
  retval += "<tr><td>City:</td><td>" + lbRec.value("airport_" + fromTo + "_city").toString() + "</td>";
  QString state(lbRec.value("airport_" + fromTo + "_state").toString());
  if(!state.isEmpty())
    retval += "<tr><td>State:</td><td>" + state + "</td>";
  retval += "<tr><td>Country:</td><td>" + lbRec.value("airport_" + fromTo + "_country").toString() + "</td>";
  retval += "<tr><td>Longitude:</td><td>" + apRec.value("longitude").toString() + "</td>";
  retval += "<tr><td>Latitude:</td><td>" + apRec.value("latitude").toString() + "</td>";
  retval += "<tr><td>Altitude:</td><td>" + QLocale().toString(apRec.value("altitude").toInt()) + " ft</td>";
  retval += "<tr><td>Max Runway Length:</td><td>" +
            QLocale().toString(apRec.value("max_runway_length").toInt()) + " ft</td>";

  if(apRec.value("has_lights").toBool())
    retval += "<tr><td>Has Lights</td>";
  if(apRec.value("has_ils").toBool())
    retval += "<tr><td>Has ILS</td>";

  retval += "</tbody></table>";
  return retval;
}
