#ifndef LOGOUT_H
#define LOGOUT_H

/*@brief:��ģ�����־����ӳ�䣬
 *
 *
 */

#include "Log.h"


///
#define LOG_OUT(int, int, char*, ...) CLog::instance()->write_log();

#endif // LOGOUT_H

