#pragma once

#include <syslog.h>
#include <stdio.h>

#define zlog_emerg(...) syslog(LOG_EMERG, __VA_ARGS__)
#define zlog_alert(...) syslog(LOG_ALERT, __VA_ARGS__)
#define zlog_crit(...) syslog(LOG_CRIT, __VA_ARGS__)
#define zlog_err(...) syslog(LOG_ERR, __VA_ARGS__)
#define zlog_warning(...) syslog(LOG_WARNING, __VA_ARGS__)
#define zlog_notice(...) syslog(LOG_NOTICE, __VA_ARGS__)
#define zlog_info(...) syslog(LOG_INFO, __VA_ARGS__)
#define zlog_debug(...) syslog(LOG_DEBUG, __VA_ARGS__)
