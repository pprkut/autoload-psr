--TEST--
Autoload classes without namespace with PSR-0
--EXTENSIONS--
autoload_psr
--INI--
include_path=./tests/files
--FILE--
<?php

$class1 = new TestClass();
$class2 = new \TestClass();

?>
--EXPECT--
