from libosso cimport osso_context_t

cdef extern from "time.h":
    ctypedef long time_t
    struct tm:
        int tm_sec
        int tm_min
        int tm_hour
        int tm_mday
        int tm_mon
        int tm_year
        int tm_wday
        int tm_yday
        int tm_isdst

    time_t mktime(tm *tm)

cdef class TimeNotification:
    cdef osso_context_t *ctx
    cdef public object cb_data
