#pragma once

#include <syslog.h>
#include <stdio.h>

namespace zlog {

template< typename... args_t >
inline void zlog_base(int priority, const char *format, args_t... args)
{
	syslog(priority, format, args...);
}

template< typename... args_t >
inline void zlog_emerg(const char *format, args_t... args)
{
	zlog_base(LOG_EMERG, format, args...);
}

template< typename... args_t >
inline void zlog_alert(const char *format, args_t... args)
{
	zlog_base(LOG_ALERT, format, args...);
}

template< typename... args_t >
inline void zlog_crit(const char *format, args_t... args)
{
	zlog_base(LOG_CRIT, format, args...);
}

template< typename... args_t >
inline void zlog_err(const char *format, args_t... args)
{
	zlog_base(LOG_ERR, format, args...);
}

template< typename... args_t >
inline void zlog_warning(const char *format, args_t... args)
{
	zlog_base(LOG_WARNING, format, args...);
}

template< typename... args_t >
inline void zlog_notice(const char *format, args_t... args)
{
	zlog_base(LOG_NOTICE, format, args...);
}

template< typename... args_t >
inline void zlog_info(const char *format, args_t... args)
{
	zlog_base(LOG_INFO, format, args...);
}

template< typename... args_t >
inline void zlog_debug(const char *format, args_t... args)
{
	zlog_base(LOG_DEBUG, format, args...);
}

} /* namespace zlog */
