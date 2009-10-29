/*
 * QtLog.cpp
 *
 *  Created on: 2009-7-7
 *      Author: Teleca
 */

#include "QtLog.h"
#include <qfileinfo.h>
#include <qfile.h>
#include <QtGui>

QtLog* QtLog::m_instance = 0;

QtLog::QtLog() {
	QString appPath = QCoreApplication::applicationDirPath();
	QString logPath = appPath + "/logs";
	QFileInfo fileInfo(logPath);
	if(fileInfo.exists() && fileInfo.isWritable())
	{
		QString appName = QCoreApplication::applicationName();
		QString logFilePath = logPath + "/debug.log";
		m_file = new QFile(logFilePath);
		if(m_file->open(QIODevice::WriteOnly))
		{
			QTextStream out(m_file);
			out << "Debug Log File, Created at:"
				<< QDate::currentDate().toString(Qt::SystemLocaleDate)
				<< " "
				<< QTime::currentTime().toString(Qt::SystemLocaleDate)
				<< "\r\n" << endl;
		}
		else
		{
			m_file->close();
			delete m_file;
			m_file = NULL;
		}
	}
	else
	{
		m_file = NULL;
	}
}

void QtLog::appendLog(const QString& log)
{
	if (m_file == 0)
	{
		return;
	}
	m_appendLock.lock();
	QTextStream out(m_file);
	out << log << endl;
	m_appendLock.unlock();
}

QtLog::~QtLog() {
	if(m_file)
	{
		m_file->close();
		delete m_file;
		m_file = NULL;
	}

	if(m_instance)
	{
		delete m_instance;
		m_instance = NULL;
	}
}
