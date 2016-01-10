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

#ifndef PATHSETTINGS_H
#define PATHSETTINGS_H

#include "fs/fspaths.h"

#include <QDateTime>
#include <QList>

class PathSettings
{
public:
  PathSettings();

  bool hasSimulator(atools::fs::SimulatorType type);
  QString getLogbookFile(atools::fs::SimulatorType type);
  QString getRunwaysFile(atools::fs::SimulatorType type);

  void setLogbookFileLoaded(atools::fs::SimulatorType type);
  void setRunwaysFileLoaded(atools::fs::SimulatorType type);

  bool hasLogbookFileChanged(atools::fs::SimulatorType type);
  bool hasRunwaysFileChanged(atools::fs::SimulatorType type);

  void writeSettings();
  void readSettings();

  bool isLogbookFileValid(atools::fs::SimulatorType type);
  bool isRunwaysFileValid(atools::fs::SimulatorType type);

private:
  friend class PathDialog;

  const QStringList settingsLogbookFilePaths =
  {
    "Paths/FileLogbookFsx", "Paths/FileLogbookFsxSe",
    "Paths/FileLogbookP3dV2", "Paths/FileLogbookP3dV3"
  };

  const QStringList settingsRunwayFilePaths =
  {
    "Paths/FileRunwaysFsx", "Paths/FileRunwaysFsxSe",
    "Paths/FileRunwaysP3dV2", "Paths/FileRunwaysP3dV3"
  };

  const QStringList settingsLogbookTimestamps =
  {
    "Paths/TimestampLogbookFsx", "Paths/TimestampLogbookFsxSe",
    "Paths/TimestampLogbookP3dV2", "Paths/TimestampLogbookP3dV3"
  };

  const QStringList settingsRunwayTimestamps =
  {
    "Paths/TimestampRunwaysFsx", "Paths/TimestampRunwaysFsxSe",
    "Paths/TimestampRunwaysP3dV2", "Paths/TimestampRunwaysP3dV3"
  };

  QList<bool> simulators;

  QStringList logbookPaths;
  QStringList runwayPaths;

  QList<QDateTime> logbookTimestamps;
  QList<QDateTime> runwayTimestamps;

  void storeSim(atools::fs::SimulatorType type);
  void loadSim(atools::fs::SimulatorType type);

};

#endif // PATHSETTINGS_H
