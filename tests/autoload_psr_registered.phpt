--TEST--
Check that autoload_psr is registered as an autoloader
--EXTENSIONS--
autoload_psr
--FILE--
<?php

var_dump(spl_autoload_functions());

?>
--EXPECT--
array(1) {
  [0]=>
  string(12) "autoload_psr"
}
