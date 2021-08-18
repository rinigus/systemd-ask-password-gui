#include "passwordtask.h"

#include <QDebug>
#include <QSettings>

#include <signal.h>
#include <time.h>

#define USEC_PER_SEC  ((uint64_t) 1000000ULL)
#define NSEC_PER_USEC ((uint64_t) 1000ULL)
#define TINFINITY 1e10

PasswordTask::PasswordTask(QObject *parent) : QObject(parent)
{
}

PasswordTask::PasswordTask(QString inifname, QObject *parent) : QObject(parent)
{
  QSettings tini(inifname, QSettings::IniFormat);
  tini.beginGroup("Ask");
  m_pid = tini.value("PID", -1).toInt();
  m_socket = tini.value("Socket").toString();
  m_echo = tini.value("Echo", 0).toInt();
  m_not_after = tini.value("NotAfter", 0).toULongLong();
  m_message = tini.value("Message").toString();
  m_id = tini.value("Id", inifname).toString();
  m_ini = inifname;

  qDebug() << "New task:" << m_pid << m_id << m_socket << m_echo << m_not_after << m_message << expired();
}

PasswordTask::~PasswordTask()
{
  qDebug() << "Removing task:" << m_id << m_socket;
}

PasswordTask::operator bool() const
{
  if (m_pid < 0 || kill(m_pid, 0)==ESRCH || m_socket.isEmpty())
    return false;
  return true;
}

bool PasswordTask::expired() const
{
  return dtExpiration() < 0;
}

double PasswordTask::dtExpiration() const
{
  if (m_not_after == 0) return TINFINITY*1.1; // infinity in our context

  struct timespec ts;

  if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0)
    return true; // error in acquiring time

  usec_t now =
      (usec_t) ts.tv_sec * USEC_PER_SEC +
      (usec_t) ts.tv_nsec / NSEC_PER_USEC;

  return (m_not_after - now)*1e-6;
}
