#ifndef _UI_DEBUG_
#define _UI_DEBUG_

#include "QtLog.h"

#ifdef _DEBUG
#define UI_DEBUG_1(file, line, msg) {QString strLine = tr("%1--").arg(line);QString str("UI: File:"); str+= file;str+=" Line:";str+=strLine; str+=msg; str+="\r\n"; QtLog::instance()->appendLog(str);}
#else
#define UI_DEBUG_1(file, line, msg)
#endif

#endif