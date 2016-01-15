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

  QString lineColor = "ff00ffff";
  int lineWidth = 3;
  QString startIcon = "http://maps.google.com/mapfiles/kml/paddle/grn-blank.png";
  QString destIcon = "http://maps.google.com/mapfiles/kml/paddle/red-blank.png";
  double startScale = 1.5, destScale = 1.5;
  int startXHotspot = 32, startYHotspot = 1, destXHotspot = 32, destYHotspot = 1;

  QString saveKmlFileDialog();
  bool startFile(QFile& file, QXmlStreamWriter& stream);
  void endFile(QFile& file, QXmlStreamWriter& stream);
  void writeLineStyle(QXmlStreamWriter& stream);
  void writePointStyle(QXmlStreamWriter& stream, bool start);
  void writeFlight(QXmlStreamWriter& stream, QSqlRecord rec);

  QSqlRecord airportDetail(const QString& icao);
  QString airportDescription(QSqlRecord lbRec, QSqlRecord apRec, const QString& fromTo);
  QString flightDescription(QSqlRecord rec);

};

#endif // LITTLELOGBOOK_KMLEXPORTER_H
