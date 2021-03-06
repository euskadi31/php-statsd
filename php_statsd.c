/**
 * php-statsd
 * 
 * @author Axel Etcheverry (http://twitter.com/euskadi31)
 * @copyright Copyright (c) 2013 Axel Etcheverry
 * @license http://creativecommons.org/licenses/MIT/deed.fr MIT
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"


#if HAVE_STATSD

#include "ext/standard/info.h"
#include "ext/standard/file.h"

#include "php_statsd.h"
#include "lib/statsd/src/statsd.h"

static int le_statsdbuf;
#define le_statsdbuf_name "StatsD Buffer"

ZEND_BEGIN_ARG_INFO_EX(arginfo_statsd_connect, 0, 0, 1)
    ZEND_ARG_INFO(0, host)
    ZEND_ARG_INFO(0, port)
    ZEND_ARG_INFO(0, ns)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_statsd_send, 0, 0, 2)
    ZEND_ARG_INFO(0, statsd)
    ZEND_ARG_INFO(0, message)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_statsd_inc, 0, 0, 3)
    ZEND_ARG_INFO(0, statsd)
    ZEND_ARG_INFO(0, stat)
    ZEND_ARG_INFO(0, sample_rate)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_statsd_dec, 0, 0, 3)
    ZEND_ARG_INFO(0, statsd)
    ZEND_ARG_INFO(0, stat)
    ZEND_ARG_INFO(0, sample_rate)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_statsd_count, 0, 0, 4)
    ZEND_ARG_INFO(0, statsd)
    ZEND_ARG_INFO(0, stat)
    ZEND_ARG_INFO(0, count)
    ZEND_ARG_INFO(0, sample_rate)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_statsd_gauge, 0, 0, 3)
    ZEND_ARG_INFO(0, statsd)
    ZEND_ARG_INFO(0, stat)
    ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_statsd_timing, 0, 0, 3)
    ZEND_ARG_INFO(0, statsd)
    ZEND_ARG_INFO(0, stat)
    ZEND_ARG_INFO(0, ms)
ZEND_END_ARG_INFO()


const zend_function_entry php_statsd_functions[] = {
    PHP_FE(statsd_connect,  arginfo_statsd_connect)
    PHP_FE(statsd_send,     arginfo_statsd_send)
    PHP_FE(statsd_inc,      arginfo_statsd_inc)
    PHP_FE(statsd_dec,      arginfo_statsd_dec)
    PHP_FE(statsd_count,    arginfo_statsd_count)
    PHP_FE(statsd_gauge,    arginfo_statsd_gauge)
    PHP_FE(statsd_timing,   arginfo_statsd_timing)
    {NULL, NULL, NULL}
    //PHP_FE_END
};

zend_module_entry php_statsd_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
    STANDARD_MODULE_HEADER,
#endif
    "statsd",
    php_statsd_functions,
    PHP_MINIT(statsd),
    NULL,
    NULL,
    NULL,
    PHP_MINFO(statsd),
#if ZEND_MODULE_API_NO >= 20010901
    PHP_STATSD_VERSION,
#endif
    STANDARD_MODULE_PROPERTIES
};

#if COMPILE_DL_STATSD
ZEND_GET_MODULE(php_statsd)
#endif

static void statsd_destructor_statsdbuf(zend_rsrc_list_entry *rsrc TSRMLS_DC) {
    statsd_t *statsd = (statsd_t *)rsrc->ptr;

    statsd_finalize(statsd);
}

PHP_MINIT_FUNCTION(statsd) {
    le_statsdbuf = zend_register_list_destructors_ex(statsd_destructor_statsdbuf, NULL, le_statsdbuf_name, module_number);
    return SUCCESS;
}

PHP_MINFO_FUNCTION(statsd) {
    php_info_print_table_start();
    php_info_print_table_row(2, "StatsD support", "enabled");
    php_info_print_table_row(2, "StatsD Version", PHP_STATSD_VERSION);
    php_info_print_table_end();
}


PHP_FUNCTION(statsd_connect) {
    statsd_t    *statsd;
    char        *host;
    int         host_len;
    long        port = STATSD_DEFAULT_PORT;
    char        *ns;
    int         ns_len = 0;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|ls", &host, &host_len, &port, &ns, &ns_len) == FAILURE) {
        return;
    }

    if (ns_len > 0) {
        statsd = statsd_init_with_namespace(host, (int)port, ns);
    } else {
        statsd = statsd_init(host, (int)port);
    }

    if (!statsd) {
        RETURN_FALSE;
    }

    ZEND_REGISTER_RESOURCE(return_value, statsd, le_statsdbuf);
}

PHP_FUNCTION(statsd_send) {
    zval        *z_statsd;
    statsd_t    *statsd;
    char        *message;
    int         message_len;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rs", &z_statsd, &message, &message_len) == FAILURE) {
        return;
    }

    ZEND_FETCH_RESOURCE(statsd, statsd_t*, &z_statsd, -1, le_statsdbuf_name, le_statsdbuf);

    if (statsd_send(statsd, message) == 0) {
        RETURN_TRUE;
    } else {
        RETURN_FALSE;
    }
}

PHP_FUNCTION(statsd_inc) {
    zval        *z_statsd;
    statsd_t    *statsd;
    char        *stat;
    int         stat_len;
    zval        **sample_rate;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rsZ", &z_statsd, &stat, &stat_len, &sample_rate) == FAILURE) {
        return;
    }

    ZEND_FETCH_RESOURCE(statsd, statsd_t*, &z_statsd, -1, le_statsdbuf_name, le_statsdbuf);

    convert_scalar_to_number_ex(sample_rate);

    if (statsd_inc(statsd, stat, (float)Z_DVAL_PP(sample_rate)) == 0) {
        RETURN_TRUE;
    } else {
        RETURN_FALSE;
    }
}

PHP_FUNCTION(statsd_dec) {
    zval        *z_statsd;
    statsd_t    *statsd;
    char        *stat;
    int         stat_len;
    zval        **sample_rate;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rsZ", &z_statsd, &stat, &stat_len, &sample_rate) == FAILURE) {
        return;
    }

    ZEND_FETCH_RESOURCE(statsd, statsd_t*, &z_statsd, -1, le_statsdbuf_name, le_statsdbuf);

    convert_scalar_to_number_ex(sample_rate);

    if (statsd_dec(statsd, stat, (float)Z_DVAL_PP(sample_rate)) == 0) {
        RETURN_TRUE;
    } else {
        RETURN_FALSE;
    }
}

PHP_FUNCTION(statsd_count) {
    zval        *z_statsd;
    statsd_t    *statsd;
    char        *stat;
    int         stat_len;
    long        value;
    zval        **sample_rate;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rslZ", &z_statsd, &stat, &stat_len, &value, &sample_rate) == FAILURE) {
        return;
    }

    ZEND_FETCH_RESOURCE(statsd, statsd_t*, &z_statsd, -1, le_statsdbuf_name, le_statsdbuf);

    convert_scalar_to_number_ex(sample_rate);

    if (statsd_count(statsd, stat, value, (float)Z_DVAL_PP(sample_rate)) == 0) {
        RETURN_TRUE;
    } else {
        RETURN_FALSE;
    }
}

PHP_FUNCTION(statsd_gauge) {
    zval        *z_statsd;
    statsd_t    *statsd;
    char        *stat;
    int         stat_len;
    long        value;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rsl", &z_statsd, &stat, &stat_len, &value) == FAILURE) {
        return;
    }

    ZEND_FETCH_RESOURCE(statsd, statsd_t*, &z_statsd, -1, le_statsdbuf_name, le_statsdbuf);

    if (statsd_gauge(statsd, stat, value) == 0) {
        RETURN_TRUE;
    } else {
        RETURN_FALSE;
    }
}

PHP_FUNCTION(statsd_timing) {
    zval        *z_statsd;
    statsd_t    *statsd;
    char        *stat;
    int         stat_len;
    long        ms;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rsl", &z_statsd, &stat, &stat_len, &ms) == FAILURE) {
        return;
    }

    ZEND_FETCH_RESOURCE(statsd, statsd_t*, &z_statsd, -1, le_statsdbuf_name, le_statsdbuf);

    if (statsd_timing(statsd, stat, ms) == 0) {
        RETURN_TRUE;
    } else {
        RETURN_FALSE;
    }
}
#endif /* HAVE_STATSD */