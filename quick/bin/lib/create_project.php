<?php

ini_set('memory_limit','256M');
require_once(__DIR__ . '/quick/ProjectCreator.php');

$options = array(
    array('h',   'help',        0,      false,       'show help'),
    array('p',   'package',     1,      null,        'package name'),
    array('o',   'output',      1,      '',          'project path (default is "current_dir/last_package_name")'),
    array('r',   'orientation', 1,      'portrait',  'screen orientation (default is "portrait")'),
    array('f',   'force',       0,      false,       'overwrite exists files'),
    array('q',   'quiet',       0,      false,       'quiet'),
    array('t',   'template',    1,      '',          'template dir'),
);

function errorhelp()
{
    print("\nshow help:\n    create_project -h\n\n");
}

function help()
{
    global $options;

    echo <<<EOT

usage: create_project -p package_name

options:

EOT;

    for ($i = 0; $i < count($options); $i++)
    {
        $o = $options[$i];
        printf("    -%s %s\n", $o[0], $o[4]);
    }

    echo <<<EOT

screen orientation:
    -r portrait (default)
    -r landscape

examples:

    # create new project folder "hello", package name is com.quick2dx.samples.hello
    create_project -p com.quick2dx.samples.hello

EOT;

}

// ----

print("\n");
if ($argc < 2)
{
    help();
    exit(1);
}

$config = fetchCommandLineArguments($argv, $options, 4);
if (!$config)
{
    errorhelp();
    exit(1);
}

if ($config['help'])
{
    help();
    exit(0);
}

$creator = new ProjectCreator($config, $options);
if ($creator->validateConfig())
{
    if ($creator->run() == true)
    {
        exit(0);
    }
    else
    {
        exit(1);
    }
}
else
{
    errorhelp();
    exit(1);
}
