--TEST--
Autoload classes with PSR-4, multiple prefixes with different content
--EXTENSIONS--
autoload_psr
--FILE--
<?php

autoload_register_psr4_prefix('Foo', './tests/files/Test/');
autoload_register_psr4_prefix('Foo\Bar', './tests/files/Foo/Bar/');

$class1 = new Foo\Bar\Foo();
$class2 = new \Foo\Bar\Foo();

$files = get_included_files();

foreach ($files as $file)
{
    var_dump(str_replace(__DIR__, '', $file));
}

?>
--EXPECT--
string(22) "/autoload_psr4_006.php"
string(23) "/files/Test/Bar/Foo.php"
