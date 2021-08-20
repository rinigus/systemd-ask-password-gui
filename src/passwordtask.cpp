#include "passwordtask.h"
#include "passwordtasklist.h"

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

  int split = m_message.indexOf(QLatin1Char(':'));
  m_title = m_message.left(split);
  if (split > 0)
    m_description = m_message.mid(split+1);

  qDebug() << "New task:" << m_pid << m_id << m_socket << m_echo << m_not_after << m_message << expired();
}

PasswordTask::~PasswordTask()
{
  emit removed();
  PasswordTaskList::instance()->passwordRemoved(m_id);
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

void PasswordTask::set(QString password)
{
  QStringList arguments;
  arguments << "1" << m_socket;
  m_process = new QProcess(this);

  connect(m_process, &QProcess::started,
          this, &PasswordTask::processStarted);
  connect(m_process,
          static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
          this, &PasswordTask::processCallback);
  connect(m_process, &QProcess::errorOccurred,
          this, &PasswordTask::processCallbackForError);

  m_password = password.toLatin1();
  m_process->start(REPLY_PASSWORD_CMD, arguments);
}

void PasswordTask::processStarted()
{
  m_process->write(m_password);
  m_process->closeWriteChannel();
  qDebug() << "Sending password";
}

void PasswordTask::processCallback(int exitCode, QProcess::ExitStatus /*exitStatus*/)
{
  qDebug() << m_id << "Set password completed with exit code" << exitCode;
}

void PasswordTask::processCallbackForError(QProcess::ProcessError error)
{
  qDebug() << m_id << "Set password error";
}

