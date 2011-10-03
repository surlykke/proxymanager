/*
  Copyright 2010, 2011 Christian Surlykke

  This file is part of ProxyManager.

  PitchTrainer is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, version 3

  PitchTrainer is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with PitchTrainer.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef PROXY_MANAGER_CONSTS_H
#define PROXY_MANAGER_CONSTS_H
#include <QString>

/*#define VAR_RUN_DIR "/var/run/proxymanagerd"
#define PID_FILE_PATH "/var/run/proxymanagerd/pid"
#define SQUID_CONF_PATH "/var/run/proxymanagerd/squid.conf"*/

const static char* RUN_DIR_PATH =    "/var/run/proxymanagerd";
const static char* PID_FILE_PATH =   "/var/run/proxymanagerd/pid";
const static char* PROXY_RULES_PATH = "/var/run/proxymanagerd/rules";

#endif // PROXY_MANAGER_CONSTS_H
