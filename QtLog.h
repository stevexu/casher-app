/*
 * QtLog.h
 *
 *  Created on: 2009-7-7
 *      Author: Steve Xu
 */

#ifndef QTLOG_H_
#define QTLOG_H_

#include <QtCore>

class QtLog {
public:
	static QtLog* instance()
	{
		static QMutex mutex;
		if(!m_instance)
		{
			mutex.lock();

			if(!m_instance)
			{
				m_instance = new QtLog;
			}

			mutex.unlock();
		}

		return m_instance;
	}

	~QtLog();

public:
	void appendLog(const QString& log);

private:
	QtLog();
	QtLog(const QtLog&);
	QtLog& operator=(const QtLog&);
	static QtLog* m_instance;
	QFile* m_file;
	QMutex m_appendLock;
};

#endif /* QTLOG_H_ */
