#ifndef LOGOUT_H
#define LOGOUT_H

/*@brief:该模块的日志接入映射，
 *
 *
 */

#include "Log.h"


///
#define LOG_OUT(int, int, char*, ...) CLog::instance()->write_log();

#endif // LOGOUT_H

