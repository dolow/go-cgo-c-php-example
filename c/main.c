// include the PHP API itself
#include <php.h>

#include "main.h"
#include "libmymodule.h"

#define PHP_MY_MODULE_EXTNAME "my_module"
#define PHP_MY_MODULE_VERSION "0.0.1"

static zend_class_entry *my_struct_class_entry = NULL;
static zend_class_entry *my_child_struct_class_entry = NULL;

void cMyChildStructToPhpMyChildStruct(MyChildStruct* c, zval* php) {
    zend_class_entry *ce = my_child_struct_class_entry;
    object_init_ex(php, my_child_struct_class_entry);

    zend_update_property_string(my_child_struct_class_entry, Z_OBJ_P(php), "key", sizeof("key") - 1, c->key);
    zend_update_property_string(my_child_struct_class_entry, Z_OBJ_P(php), "value", sizeof("value") - 1, c->value);
}

void cMyStructToPhpMyStruct(MyStruct* c, zval* php) {
    zend_class_entry *ce = my_struct_class_entry;
    object_init_ex(php, my_struct_class_entry);
    zval childArray;
    array_init(&childArray);

    zend_update_property_string(my_struct_class_entry, Z_OBJ_P(php), "string", sizeof("string") - 1, c->string);
    zend_update_property_long(my_struct_class_entry, Z_OBJ_P(php), "int", sizeof("int") - 1, c->integer);
    zend_update_property(my_struct_class_entry, Z_OBJ_P(php), "child", sizeof("child") - 1, &childArray);

    if (c->child != NULL) {
        MyChildStruct childElem;
        for (int j = 0; ; j++) {
            childElem = c->child[j];
            if (childElem.key == NULL) {
                break;
            }

            zval childRef;
            cMyChildStructToPhpMyChildStruct(&childElem, &childRef);
            add_next_index_zval(&childArray, &childRef);
        }
    }
}

void phpMyChildStructToCMyChildStruct(zend_object* php, MyChildStruct* c) {
    zval _;

    zval *zkey = zend_read_property(my_child_struct_class_entry, php, "key", sizeof("key") - 1, 0, &_);
    zval *zval = zend_read_property(my_child_struct_class_entry, php, "value", sizeof("value") - 1, 0, &_);
    c->key = Z_STRVAL(*zkey);
    c->value = Z_STRVAL(*zval);
}

void phpMyStructToCMyStruct(zend_object* php, MyStruct* c) {
    zval _;

    zval *zstr = zend_read_property(my_struct_class_entry, php, "string", sizeof("string") - 1, 0, &_);
    zval *zint = zend_read_property(my_struct_class_entry, php, "int", sizeof("int") - 1, 0, &_);
    zval *zchild = zend_read_property(my_struct_class_entry, php, "child", sizeof("child") - 1, 0, &_);

    c->string = Z_STRVAL(*zstr);
    c->integer = zval_get_long(zint);

    uint32_t count = zend_array_count(Z_ARR_P(zchild));

    c->child = (MyChildStruct*)malloc(sizeof(MyChildStruct) * (count + 1));

    zend_ulong i;
    void *_key;
    zval *val;
    ZEND_HASH_FOREACH_KEY_VAL(Z_ARR_P(zchild), i, _key, val) {
        MyChildStruct elem = c->child[i];
        phpMyChildStructToCMyChildStruct(Z_OBJ_P(val), &elem);
        c->child[i] = elem;
    } ZEND_HASH_FOREACH_END();

    MyChildStruct nullStruct;
    nullStruct.key = NULL;
    nullStruct.value = NULL;
    c->child[count] = nullStruct;
}


PHP_METHOD(MyStruct, __construct)
{
    char *str;
    size_t strlen = 256;
    long integer;
    zval *child;

    ZEND_PARSE_PARAMETERS_START(3, 3)
        Z_PARAM_STRING(str, strlen)
        Z_PARAM_LONG(integer)
        Z_PARAM_ARRAY(child)
    ZEND_PARSE_PARAMETERS_END();

    zend_update_property_string(my_struct_class_entry, Z_OBJ_P(ZEND_THIS), "string", sizeof("string") - 1, str);
    zend_update_property_long(my_struct_class_entry, Z_OBJ_P(ZEND_THIS), "int", sizeof("int") - 1, integer);
    zend_update_property(my_struct_class_entry, Z_OBJ_P(ZEND_THIS), "child", sizeof("child") - 1, child);
}
ZEND_BEGIN_ARG_INFO(arginfo_my_struct_construct, 2)
    ZEND_ARG_INFO(0, str)
    ZEND_ARG_INFO(0, integer)
    ZEND_ARG_INFO(0, child)
ZEND_END_ARG_INFO()
const zend_function_entry my_struct_functions[] = {
    //  public methods
    PHP_ME(MyStruct, __construct, arginfo_my_struct_construct, ZEND_ACC_PUBLIC)
    PHP_FE_END
};


PHP_METHOD(MyChildStruct, __construct)
{
    char *key;
    char *value;
    size_t strlen = 256;

    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_STRING(key, strlen)
        Z_PARAM_STRING(value, strlen)
    ZEND_PARSE_PARAMETERS_END();

    zend_update_property_string(my_child_struct_class_entry, Z_OBJ_P(ZEND_THIS), "key", sizeof("key") - 1, key);
    zend_update_property_string(my_child_struct_class_entry, Z_OBJ_P(ZEND_THIS), "value", sizeof("value") - 1, value);
}
ZEND_BEGIN_ARG_INFO(arginfo_my_child_struct_construct, 0)
ZEND_END_ARG_INFO()
const zend_function_entry my_child_struct_functions[] = {
    //  public methods
    PHP_ME(MyChildStruct, __construct, arginfo_my_child_struct_construct, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

PHP_MINIT_FUNCTION(MyModule)
{
    zend_class_entry myStructEntry;
    zend_class_entry myChildStructEntry;

    INIT_NS_CLASS_ENTRY(myStructEntry, "MyModule", "MyStruct", my_struct_functions);
    INIT_NS_CLASS_ENTRY(myChildStructEntry, "MyModule", "MyChildStruct", my_child_struct_functions);

    my_struct_class_entry = zend_register_internal_class(&myStructEntry);
    my_child_struct_class_entry = zend_register_internal_class(&myChildStructEntry);

    /**
     * MyStruct
     */
    zval *childRef = (zval*)malloc(sizeof(zval)); // empty zval
    zend_declare_property_string(my_struct_class_entry, "string", strlen("string"), "", ZEND_ACC_PUBLIC);
    zend_declare_property_long(my_struct_class_entry, "int", strlen("int"), 0, ZEND_ACC_PUBLIC);
    zend_declare_property(my_struct_class_entry, "child", strlen("child"), childRef, ZEND_ACC_PUBLIC);

    /**
     * MyChildStruct
     */
    zend_declare_property_string(my_child_struct_class_entry, "key", strlen("key"), "", ZEND_ACC_PUBLIC);
    zend_declare_property_string(my_child_struct_class_entry, "value", strlen("value"), "", ZEND_ACC_PUBLIC);

    return SUCCESS;
}


/**
 * declarations of functions
 */

ZEND_BEGIN_ARG_INFO_EX(arginfo_receive_scalar, 0, 0, 0)
    ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()
ZEND_FUNCTION(receive_scalar) {
    char *str;
    size_t strlen = 256;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_STRING(str, strlen)
    ZEND_PARSE_PARAMETERS_END();

    ReceiveScalar(str);
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_return_scalar, 0, 0, 0)
ZEND_END_ARG_INFO()
ZEND_FUNCTION(return_scalar) {
    char* str = ReturnScalar();
    zend_string *result = zend_string_init(str, strlen(str), 0);

    RETURN_STR(result);
}


ZEND_BEGIN_ARG_INFO_EX(arginfo_receive_struct, 0, 0, 1)
    ZEND_ARG_INFO(0, struct)
ZEND_END_ARG_INFO()
ZEND_FUNCTION(receive_struct) {
    zend_object *strc;
    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_OBJ_OF_CLASS(strc, my_struct_class_entry)
    ZEND_PARSE_PARAMETERS_END();

    MyStruct myStrc;
    phpMyStructToCMyStruct(strc, &myStrc);

    ReceiveStruct(&myStrc);
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_return_struct, 0, 0, 0)
ZEND_END_ARG_INFO()
ZEND_FUNCTION(return_struct) {
    MyStruct* strc = ReturnStruct();
    cMyStructToPhpMyStruct(strc, return_value);
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_receive_struct_array, 0, 0, 1)
    ZEND_ARG_INFO(0, arr)
ZEND_END_ARG_INFO()
ZEND_FUNCTION(receive_struct_array) {
    zval *arr;
    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_ARRAY(arr)
    ZEND_PARSE_PARAMETERS_END();

    uint32_t count = zend_array_count(Z_ARR_P(arr));

    MyStruct *structArray = (MyStruct*)malloc(sizeof(MyStruct) * (count + 1));

    zend_ulong i;
    void *_key;
    zval *val;
    ZEND_HASH_FOREACH_KEY_VAL(Z_ARR_P(arr), i, _key, val) {
        MyStruct strc = structArray[i];
        phpMyStructToCMyStruct(Z_OBJ_P(val), &strc);
        structArray[i] = strc;
    } ZEND_HASH_FOREACH_END();

    MyStruct nullStruct;
    nullStruct.string = NULL;
    nullStruct.integer = 0;
    nullStruct.child = NULL;
    structArray[count] = nullStruct;

    ReceiveStructArray(structArray);

    free(structArray);
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_return_struct_array, 0, 0, 0)
ZEND_END_ARG_INFO()
ZEND_FUNCTION(return_struct_array) {
    MyStruct* structArray = ReturnStructArray();

    array_init(return_value);
    if (structArray == NULL) {
        return;
    }

    MyStruct strc;
    for (int i = 0; ; i++) {
        strc = structArray[i];
        if (strc.string == NULL) {
            break;
        }

        zval structRef;
        cMyStructToPhpMyStruct(&strc, &structRef);
        add_next_index_zval(return_value, &structRef);
    }

    free(structArray);
}

// register our function to the PHP API
// so that PHP knows, which functions are in this module
zend_function_entry my_module_functions[] = {
    ZEND_NS_FE("MyModule", receive_scalar, arginfo_receive_scalar)
    ZEND_NS_FE("MyModule", return_scalar, arginfo_return_scalar)
    ZEND_NS_FE("MyModule", receive_struct, arginfo_receive_struct)
    ZEND_NS_FE("MyModule", return_struct, arginfo_return_struct)
    ZEND_NS_FE("MyModule", receive_struct_array, arginfo_receive_struct_array)
    ZEND_NS_FE("MyModule", return_struct_array, arginfo_return_struct_array)
    PHP_FE_END // null termination
};


// some pieces of information about our module
zend_module_entry my_module_module_entry = {
    STANDARD_MODULE_HEADER,
    PHP_MY_MODULE_EXTNAME,
    my_module_functions,
    PHP_MINIT(MyModule),
    NULL,
    NULL,
    NULL,
    NULL,
    PHP_MY_MODULE_VERSION,
    STANDARD_MODULE_PROPERTIES
};

// argument must be prefix of *_module_entry
ZEND_GET_MODULE(my_module);
