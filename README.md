Autoload PSR
====
*PHP PSR-0/PSR-4 Autoloader*


[![Build Status](https://travis-ci.org/pprkut/autoload-psr.svg?branch=master)](https://travis-ci.org/pprkut/autoload-psr)

The ```autoload_psr``` extension provides an integrated autoloader for PHP to load classes
according to [PSR-0](https://www.php-fig.org/psr/psr-0/) and/or [PSR-4](https://www.php-fig.org/psr/psr-4/).

The autoloader registers itself with SPL, so you no longer need to care about specifically setting up
an autoloader for your project. PSR-0 works out of the box, no further setup needed. To use PSR-4 you need
to register the respective namespace prefixes that you would like to use in your project.

API
===
*The PHP API for ```autoload_psr```*

```php
/**
* Register a namespace prefix for PSR-4.
* @param string prefix
* @param string path
**/
function autoload_register_psr4_prefix(string prefix, string path) : void;
```
