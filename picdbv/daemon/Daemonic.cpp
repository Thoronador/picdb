/*
 -------------------------------------------------------------------------------
    This file is part of picdbv.
    Copyright (C) 2014  Thoronador

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 -------------------------------------------------------------------------------
*/

#include "Daemonic.hpp"
#include <cstdlib> //for exit()
#include <iostream>
#include <signal.h> //for signal()
#include <syslog.h> //for openlog() (and possibly syslog())
#include <unistd.h> //for fork(), setsid(), chdir(), sysconf()

Daemonic::Daemonic()
: hasOpenSyslog(false)
{
  //empty
}

Daemonic::~Daemonic()
{
  //empty
  if (hasOpenSyslog)
    closelog();
}

Daemonic& Daemonic::get()
{
  static Daemonic Instance;
  return Instance;
}

bool Daemonic::daemonize(const std::string& logName)
{
  #ifndef DO_NOT_FORK
  pid_t procID = fork();
  if (procID < 0)
  {
    //fork() failed!
    return false;
  } //if pid < 0
  if (procID>0)
  {
    //We are in the parent process.
    // -> stop/exit parent process
    exit(0);
    return false; //should never get to this point
  } //if pid > 0

  //PID is zero -> child process
  // session leader
  procID = setsid();
  if (procID == ((pid_t) -1))
  {
    //failed to become session leader
    std::cout << "Failed to become session leader!" << std::endl;
    return false;
  }

  //ignore signal HUP
  if (signal(SIGHUP, SIG_IGN) == SIG_ERR)
  {
    std::cout << "Could not ignore SIGHUP!" << std::endl;
    return false;
  }

  //second fork()
  procID = fork();
  if (procID < 0)
  {
    //fork() failed!
    return false;
  } //if pid < 0
  if (procID>0)
  {
    //parent process
    // -> stop/exit parent process
    exit(0);
    return false; //should never get to this point
  } //if pid > 0
  #endif //ifndef DO_NOT_FORK

  /* pid is zero second fork() succeeded - now we are running the "real" daemon process. */

  //change to root directory
  chdir("/");

  //TODO: umask(0)?

  //close all inherited file descriptors
  long max_fd = sysconf(_SC_OPEN_MAX);
  if (max_fd>1024)
    max_fd = 1023;
  else if (max_fd<0)
    max_fd = 255; // 255 is just a guess what could be a reasonable limit
  long i = 0;
  for (i=max_fd; i>=0; --i)
  {
    close(i);
  } //for i

  //open system log
  openlog(logName.c_str(), LOG_PID, LOG_DAEMON);
  hasOpenSyslog = true;
  return true; //success!
}
