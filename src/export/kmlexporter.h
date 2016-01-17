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

#ifndef LITTLELOGBOOK_KMLEXPORTER_H
#define LITTLELOGBOOK_KMLEXPORTER_H

#include "export/exporter.h"

#include <QColor>
#include <QObject>
#include <QSqlRecord>

namespace atools {
namespace sql {
class SqlQuery;
}
}

class QWidget;
class Controller;
class QFile;
class QXmlStreamWriter;

class KmlExporter :
  public Exporter
{
  Q_OBJECT

public:
  KmlExporter(QWidget *parent, Controller *controller);
  virtual ~KmlExporter();

  /* Export all rows.
   *
   * @param open Open file in default browser after export.
   * @return number of rows exported.
   */
  virtual int exportAll(bool open);

  /* Export only selected rows.
   *
   * @param open Open file in default browser after export.
   * @return number of rows exported.
   */
  virtual int exportSelected(bool open);

private:
  atools::sql::SqlQuery *airportDetailQuery = nullptr;

  QString lineColor, startIcon, destIcon;
  double startScale, destScale;
  int lineWidth, startXHotspot, startYHotspot, destXHotspot, destYHotspot;

  /* Show save file dialog */
  QString saveKmlFileDialog();

  /* Open file and stream and write all header and style information */
  bool startFile(QFile& file, QXmlStreamWriter& stream);

  /* Close file and tags */
  void endFile(QFile& file, QXmlStreamWriter& stream);

  /* Write line style for routes */
  void writeLineStyle(QXmlStreamWriter& stream);

  /* Write start and destination point styles */
  void writePointStyle(QXmlStreamWriter& stream, bool start);

  /* Write flight with three placemarks - start, dest and line */
  void writeFlight(QXmlStreamWriter& stream, QSqlRecord rec);

  /* Get airport information from the database */
  QSqlRecord queryAirportDetail(const QString& icao);

  /* Create the airport details HTML code */
  QString airportDescription(QSqlRecord lbRec, QSqlRecord apRec, const QString& fromTo);

  /* Create the flight details HTML code for the line*/
  QString flightDescription(QSqlRecord rec);

  /* Show the user how many entries were skipped due to incomplete information */
  void skippedEntriesDialog(int skipped);

  /* Prepare the SQL query on demand */
  void prepareQuery();

};

#endif // LITTLELOGBOOK_KMLEXPORTER_H
