/*
  +----------------------------------------------------------------------+
  | XlsWriter Extension                                                  |
  +----------------------------------------------------------------------+
  | Copyright (c) 2017-2018 The Viest                                    |
  +----------------------------------------------------------------------+
  | http://www.viest.me                                                  |
  +----------------------------------------------------------------------+
  | Author: viest <dev@service.viest.me>                                 |
  +----------------------------------------------------------------------+
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "include.h"
#include "ext/standard/php_var.h"

zend_class_entry *vtiful_format_ce;

/* {{{ ARG_INFO
 */
ZEND_BEGIN_ARG_INFO_EX(format_bold_arginfo, 0, 0, 1)
                ZEND_ARG_INFO(0, handle)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(format_italic_arginfo, 0, 0, 1)
                ZEND_ARG_INFO(0, handle)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(format_underline_arginfo, 0, 0, 2)
                ZEND_ARG_INFO(0, handle)
                ZEND_ARG_INFO(0, style)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(format_align_arginfo, 0, 0, 2)
                ZEND_ARG_INFO(0, handle)
                ZEND_ARG_INFO(0, style)
ZEND_END_ARG_INFO()
/* }}} */

/** {{{ \Vtiful\Kernel\Format::bold()
 */
PHP_METHOD(vtiful_format, bold)
{
    zval *handle;
    lxw_format *bold_format;
    xls_resource_t *xls_res;

    ZEND_PARSE_PARAMETERS_START(1, 1)
            Z_PARAM_RESOURCE(handle)
    ZEND_PARSE_PARAMETERS_END();

    xls_res   = zval_get_resource(handle);
    bold_format = workbook_add_format(xls_res->workbook);

    format_set_bold(bold_format);

    RETURN_RES(zend_register_resource(bold_format, le_xls_writer));
}
/* }}} */

/** {{{ \Vtiful\Kernel\Format::italic()
 */
PHP_METHOD(vtiful_format, italic)
{
    zval *handle;
    lxw_format *italic_format;
    xls_resource_t *xls_res;

    ZEND_PARSE_PARAMETERS_START(1, 1)
            Z_PARAM_RESOURCE(handle)
    ZEND_PARSE_PARAMETERS_END();

    xls_res   = zval_get_resource(handle);
    italic_format = workbook_add_format(xls_res->workbook);

    format_set_italic(italic_format);

    RETURN_RES(zend_register_resource(italic_format, le_xls_writer));
}
/* }}} */

/** {{{ \Vtiful\Kernel\Format::underline()
 */
PHP_METHOD(vtiful_format, underline)
{
    zval *handle;
    zend_long style;
    lxw_format *underline_format;
    xls_resource_t *xls_res;

    ZEND_PARSE_PARAMETERS_START(2, 2)
            Z_PARAM_RESOURCE(handle)
            Z_PARAM_LONG(style)
    ZEND_PARSE_PARAMETERS_END();

    xls_res = zval_get_resource(handle);
    underline_format = workbook_add_format(xls_res->workbook);

    format_set_underline(underline_format, style);

    RETURN_RES(zend_register_resource(underline_format, le_xls_writer));
}
/* }}} */

/** {{{ \Vtiful\Kernel\Format::align()
 */
PHP_METHOD(vtiful_format, align)
{
    zval *handle = NULL, *args = NULL;
    int argc, i;

    lxw_format *align_format;
    xls_resource_t *xls_res;

    ZEND_PARSE_PARAMETERS_START(2, -1)
            Z_PARAM_RESOURCE(handle)
            Z_PARAM_VARIADIC('+', args, argc)
    ZEND_PARSE_PARAMETERS_END();

    xls_res = zval_get_resource(handle);
    align_format = workbook_add_format(xls_res->workbook);

    for (i = 0; i < argc; ++i) {
        zval *arg = args + i;

        if (Z_TYPE_P(arg) != IS_LONG) {
            zend_throw_exception(vtiful_exception_ce, "Format exception, please view the manual", 150);
        }

        format_set_align(align_format, Z_LVAL_P(arg));
    }

    RETURN_RES(zend_register_resource(align_format, le_xls_writer));
}
/* }}} */


/** {{{ xls_methods
*/
zend_function_entry format_methods[] = {
        PHP_ME(vtiful_format, bold,      format_bold_arginfo,      ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
        PHP_ME(vtiful_format, italic,    format_italic_arginfo,    ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
        PHP_ME(vtiful_format, underline, format_underline_arginfo, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
        PHP_ME(vtiful_format, align,     format_align_arginfo,     ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
        PHP_FE_END
};
/* }}} */

/** {{{ VTIFUL_STARTUP_FUNCTION
*/
VTIFUL_STARTUP_FUNCTION(format) {
    zend_class_entry ce;

    INIT_NS_CLASS_ENTRY(ce, "Vtiful\\Kernel", "Format", format_methods);

    vtiful_format_ce = zend_register_internal_class(&ce);

    REGISTER_CLASS_CONST_LONG(vtiful_format_ce, "UNDERLINE_SINGLE",            LXW_UNDERLINE_SINGLE)
    REGISTER_CLASS_CONST_LONG(vtiful_format_ce, "UNDERLINE_DOUBLE ",           LXW_UNDERLINE_DOUBLE)
    REGISTER_CLASS_CONST_LONG(vtiful_format_ce, "UNDERLINE_SINGLE_ACCOUNTING", LXW_UNDERLINE_SINGLE_ACCOUNTING)
    REGISTER_CLASS_CONST_LONG(vtiful_format_ce, "UNDERLINE_DOUBLE_ACCOUNTING", LXW_UNDERLINE_DOUBLE_ACCOUNTING)

    REGISTER_CLASS_CONST_LONG(vtiful_format_ce, "FORMAT_ALIGN_LEFT",                 LXW_ALIGN_LEFT)
    REGISTER_CLASS_CONST_LONG(vtiful_format_ce, "FORMAT_ALIGN_CENTER",               LXW_ALIGN_CENTER)
    REGISTER_CLASS_CONST_LONG(vtiful_format_ce, "FORMAT_ALIGN_RIGHT",                LXW_ALIGN_RIGHT)
    REGISTER_CLASS_CONST_LONG(vtiful_format_ce, "FORMAT_ALIGN_FILL",                 LXW_ALIGN_FILL)
    REGISTER_CLASS_CONST_LONG(vtiful_format_ce, "FORMAT_ALIGN_JUSTIFY",              LXW_ALIGN_JUSTIFY)
    REGISTER_CLASS_CONST_LONG(vtiful_format_ce, "FORMAT_ALIGN_CENTER_ACROSS",        LXW_ALIGN_CENTER_ACROSS)
    REGISTER_CLASS_CONST_LONG(vtiful_format_ce, "FORMAT_ALIGN_DISTRIBUTED",          LXW_ALIGN_DISTRIBUTED)
    REGISTER_CLASS_CONST_LONG(vtiful_format_ce, "FORMAT_ALIGN_VERTICAL_TOP",         LXW_ALIGN_VERTICAL_TOP)
    REGISTER_CLASS_CONST_LONG(vtiful_format_ce, "FORMAT_ALIGN_VERTICAL_BOTTOM",      LXW_ALIGN_VERTICAL_BOTTOM)
    REGISTER_CLASS_CONST_LONG(vtiful_format_ce, "FORMAT_ALIGN_VERTICAL_CENTER",      LXW_ALIGN_VERTICAL_CENTER)
    REGISTER_CLASS_CONST_LONG(vtiful_format_ce, "FORMAT_ALIGN_VERTICAL_JUSTIFY",     LXW_ALIGN_VERTICAL_JUSTIFY)
    REGISTER_CLASS_CONST_LONG(vtiful_format_ce, "FORMAT_ALIGN_VERTICAL_DISTRIBUTED", LXW_ALIGN_VERTICAL_DISTRIBUTED)

    return SUCCESS;
}
/* }}} */
