#include "openrasp_hook.h"

static inline void hook_directory(INTERNAL_FUNCTION_PARAMETERS)
{
    zval **path;
    int argc = MIN(1, ZEND_NUM_ARGS());
    if (!openrasp_check_type_ignored(ZEND_STRL("directory") TSRMLS_CC) &&
        argc > 0 &&
        zend_get_parameters_ex(argc, &path) == SUCCESS &&
        Z_TYPE_PP(path) == IS_STRING)
    {
        char resolved_path_buff[MAXPATHLEN];
        zval *params;
        MAKE_STD_ZVAL(params);
        array_init(params);
        add_assoc_zval(params, "path", *path);
        Z_ADDREF_PP(path);
        char *real_path = VCWD_REALPATH(Z_STRVAL_PP(path), resolved_path_buff);
        if (real_path)
        {
            add_assoc_string(params, "realpath", real_path, 1);
            zval *stack = NULL;
            MAKE_STD_ZVAL(stack);
            array_init(stack);
            format_debug_backtrace_arr(stack TSRMLS_CC);
            add_assoc_zval(params, "stack", stack);
            check("directory", params TSRMLS_CC);
        }
        else
        {
            Z_ADDREF_PP(path);
        }
    }
}

void pre_global_dir(INTERNAL_FUNCTION_PARAMETERS)
{
    hook_directory(INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
void pre_global_opendir(INTERNAL_FUNCTION_PARAMETERS)
{
    hook_directory(INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
void pre_global_scandir(INTERNAL_FUNCTION_PARAMETERS)
{
    hook_directory(INTERNAL_FUNCTION_PARAM_PASSTHRU);
}