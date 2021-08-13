<?php

use MyModule\MyStruct;
use MyModule\MyChildStruct;

MyModule\receive_scalar("from php");

$str = MyModule\return_scalar();
echo("return_scalar: ".$str."\n");

$strc = new MyStruct("my php struct string", 99, [
    new MyChildStruct("php key 1", "php value 1"),
    new MyChildStruct("php key 2", "php value 2")
]);

MyModule\receive_struct($strc);

$gostrc = MyModule\return_struct();

var_dump($gostrc);

$strc_array = [
    new MyStruct("my php struct string array 1", 111, [
        new MyChildStruct("php key 1_1", "php value 1_1"),
        new MyChildStruct("php key 1_2", "php value 1_2")
    ]),
    new MyStruct("my php struct string array 2", 222, [
        new MyChildStruct("php key 2_1", "php value 2_1"),
        new MyChildStruct("php key 2_2", "php value 2_2")
    ]),
];

MyModule\receive_struct_array($strc_array);

$gostrc_array = MyModule\return_struct_array();
var_dump($gostrc_array);
