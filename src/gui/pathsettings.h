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

#include <QList>

class PathSettings
{
public:
  PathSettings();

  bool hasSimulator(atools::fs::SimulatorType type);
  QString logbookPath(atools::fs::SimulatorType type);
  QString runwayPath(atools::fs::SimulatorType type);

  void store();
  void load();

private:
  friend class PathDialog;

  const QStringList lbFilePaths =
  {
    "Paths/FileLogbookFsx", "Paths/FileLogbookFsxSe",
    "Paths/FileLogbookP3dV2", "Paths/FileLogbookP3dV3"
  };

  const QStringList rwFilePaths =
  {
    "Paths/FileRunwaysFsx", "Paths/FileRunwaysFsxSe",
    "Paths/FileRunwaysP3dV2", "Paths/FileRunwaysP3dV3"
  };

  QList<bool> simulators;
  QStringList logbookPaths;
  QStringList runwayPaths;
  void storeSim(atools::fs::SimulatorType type);
  void loadSim(atools::fs::SimulatorType type);
};

#endif // PATHSETTINGS_H
