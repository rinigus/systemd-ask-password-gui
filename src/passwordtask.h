#ifndef PASSWORDTASK_H
#define PASSWORDTASK_H

#include <QObject>

#include <QProcess>
#include <QString>

typedef uint64_t usec_t;

class PasswordTask : public QObject
{
  Q_OBJECT

  Q_PROPERTY(QString id READ id)
  Q_PROPERTY(QString title READ title)
  Q_PROPERTY(QString description READ description)

public:
  PasswordTask(QObject *parent = nullptr);
  PasswordTask(QString inifname, QObject *parent = nullptr);
  ~PasswordTask();

  operator bool() const;
  bool expired() const;
  double dtExpiration() const;

  QString id() const { return m_id; }
  QString ini() const { return m_ini; }
  QString description() const { return m_description; }
  QString title() const { return m_title; }

  Q_INVOKABLE void set(QString password);

signals:
  void removed();

private:
  void processStarted();
  void processCallback(int exitCode, QProcess::ExitStatus exitStatus);
  void processCallbackForError(QProcess::ProcessError error);

private:
  int     m_pid{-1};
  int     m_echo{0};
  usec_t  m_not_after{0};
  QString m_id;
  QString m_ini;
  QString m_message;
  QString m_socket;

  QString m_description;
  QString m_title;

  QByteArray m_password;

  QProcess* m_process{nullptr};
};

#endif // PASSWORDTASK_H
