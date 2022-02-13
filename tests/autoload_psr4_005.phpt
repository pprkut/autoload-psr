--TEST--
Autoload classes without namespace with PSR-4
--EXTENSIONS--
autoload_psr
--FILE--
<?php

$class1 = new TestClass();
$class2 = new \TestClass();

?>
--EXPECTF--
Fatal error: Uncaught Error: Class 'TestClass' not found in %sautoload_psr4_005.php:%d
Stack trace:
#0 {main}
  thrown in %sautoload_psr4_005.php on line %d
