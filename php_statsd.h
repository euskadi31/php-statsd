/**
 * php-statsd
 * 
 * @author Axel Etcheverry (http://twitter.com/euskadi31)
 * @copyright Copyright (c) 2013 Axel Etcheverry
 * @license http://creativecommons.org/licenses/MIT/deed.fr MIT
 */
#ifndef PHP_STATSD_H
#define PHP_STATSD_H

#if HAVE_STATSD

extern zend_module_entry php_statsd_module_entry;
#define php_statsd_module_ptr &php_statsd_module_entry

PHP_MINIT_FUNCTION(statsd);
PHP_MINFO_FUNCTION(statsd);

PHP_FUNCTION(statsd_connect);
PHP_FUNCTION(statsd_send);
PHP_FUNCTION(statsd_inc);
PHP_FUNCTION(statsd_dec);
PHP_FUNCTION(statsd_count);
PHP_FUNCTION(statsd_gauge);
PHP_FUNCTION(statsd_timing);

#define phpext_statsd_ptr php_statsd_module_ptr

#else
#define php_statsd_module_ptr NULL
#endif  /* HAVE_STATSD */

#define PHP_STATSD_VERSION "0.1.0"

#endif /* PHP_STATSD_H */