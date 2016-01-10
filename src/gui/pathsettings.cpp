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

#include "gui/pathsettings.h"
#include "settings/settings.h"

#include <QSettings>

using atools::settings::Settings;
using atools::fs::SimulatorType;

PathSettings::PathSettings()
{
  for(int i = 0; i < 4; i++)
  {
    simulators.append(false);
    logbookPaths.append(QString());
    runwayPaths.append(QString());
  }
}

bool PathSettings::hasSimulator(atools::fs::SimulatorType type)
{
  return simulators.at(type);
}

QString PathSettings::logbookPath(atools::fs::SimulatorType type)
{
  return logbookPaths.at(type);
}

QString PathSettings::runwayPath(atools::fs::SimulatorType type)
{
  return runwayPaths.at(type);
}

void PathSettings::store()
{
  storeSim(atools::fs::FSX);
  storeSim(atools::fs::FSX_SE);
  storeSim(atools::fs::P3D_V2);
  storeSim(atools::fs::P3D_V3);

  Settings::instance().syncSettings();
}

void PathSettings::load()
{
  loadSim(atools::fs::FSX);
  loadSim(atools::fs::FSX_SE);
  loadSim(atools::fs::P3D_V2);
  loadSim(atools::fs::P3D_V3);
}

void PathSettings::loadSim(SimulatorType type)
{
  Settings& s = Settings::instance();

  if(!atools::fs::FsPaths::getBasePath(type).isEmpty())
  {
    simulators[type] = true;
    logbookPaths[type] = s->value(lbFilePaths.at(type)).toString();
    runwayPaths[type] = s->value(rwFilePaths.at(type)).toString();
  }
  else
    simulators[type] = false;
}

void PathSettings::storeSim(SimulatorType type)
{
  Settings& s = Settings::instance();

  if(simulators.at(type))
  {
    s->setValue(lbFilePaths.at(type), logbookPaths.at(type));
    s->setValue(rwFilePaths.at(type), runwayPaths.at(type));
  }
}
