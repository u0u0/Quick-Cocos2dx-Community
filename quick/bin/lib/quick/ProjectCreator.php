<?php

require_once(__DIR__ . '/init.php');

class ProjectCreator
{
    private $config;
    private $options;
    private $validated = false;
    private $vars = array();

    function __construct(array $config, array $options)
    {
        $this->config = $config;
        $this->options = $options;
    }

    function validateConfig()
    {
        // check template
        $templatePath = rtrim($this->config['template'], "/\\");
        $templatePath = $templatePath . DS;
        if (!is_dir($templatePath))
        {
            printf("ERROR: invalid template path \"%s\"\n", $templatePath);
            return false;
        }
        $this->config['template'] = $templatePath;

        // check package name
        $packageName = str_replace('-', '_', strtolower($this->config['package']));
        $parts = explode('.', $packageName);
        $packageName = array();
        for ($i = 0; $i < count($parts); $i++)
        {
            $parts[$i] = preg_replace('/[^a-z0-9_]/', '', $parts[$i]);
            if (!empty($parts[$i])) $packageName[] = $parts[$i];
        }
        if (count($packageName) < 2)
        {
            printf("ERROR: invalid package name \"%s\"\n", implode('.', $packageName));
            return false;
        }

        $lastname = $packageName[count($packageName) - 1];
        array_pop($packageName);
        $packageName = implode('.', $packageName);
        $this->config['package'] = $packageName . '.' . $lastname;
        $this->config['packageLastName'] = $lastname;
        $this->config['packageFullName'] = $packageName . '.' . $lastname;
        $this->config['packageModuleName'] = $packageName;

        // check output path
        if (empty($this->config['output']))
        {
            $curpath = rtrim(getcwd(), '/\\');
            $this->config['output'] = $curpath . DS . $lastname . DS;
            $this->config['cocos_output'] = $curpath;
            $this->config['cocos_project'] = $lastname;
        }
        else
        {
            $outpath = rtrim($this->config['output'], '/\\');
            $this->config['output'] = $outpath . DS;
            $pos = strrpos($outpath, DS);
            if ($pos != false)
            {
                $this->config['cocos_output'] = substr($outpath, 0, $pos);
                $this->config['cocos_project'] = substr($outpath, $pos+1);
            }
            else
            {
                $this->config['cocos_output'] = $outpath;
                $this->config['cocos_project'] = $lastname;
            }
        }
        if (!$this->config['force'] && (is_dir($this->config['output']) || file_exists($this->config['output'])))
        {
            printf("ERROR: project path \"%s\" exists\n", $this->config['output']);
            return false;
        }

        // check screen orientation
        $orientation = strtolower($this->config['orientation']);
        if ($orientation != 'landscape' && $orientation != 'portrait')
        {
            printf("ERROR: invalid screen orientation \"%s\"\n", $orientation);
            return false;
        }
        $this->config['orientation'] = $orientation;

        if (!$this->config['quiet'])
        {
            dumpConfig($this->config, $this->options);
        }
        $this->validated = true;
        return true;
    }

    function run()
    {
        if (!$this->validated)
        {
            print("ERR: invalid config\n");
            return false;
        }

        // prepare contents
        $this->vars['__TEMPLATE_PATH__'] = $this->config['template'];
        $this->vars['__PROJECT_COCOS_NAME__'] = $this->config['cocos_project'];
        $this->vars['__PROJECT_PACKAGE_MODULE_NAME__'] = $this->config['packageModuleName'];
        $this->vars['__PROJECT_PACKAGE_MODULE_NAME_L__'] = strtolower($this->config['packageModuleName']);
        $this->vars['__PROJECT_PACKAGE_FULL_NAME__'] = $this->config['packageFullName'];
        $this->vars['__PROJECT_PACKAGE_FULL_NAME_L__'] = strtolower($this->config['packageFullName']);
        $this->vars['__PROJECT_PACKAGE_LAST_NAME__'] = $this->config['packageLastName'];
        $this->vars['__PROJECT_PACKAGE_LAST_NAME_L__'] = strtolower($this->config['packageLastName']);
        $this->vars['__PROJECT_PACKAGE_LAST_NAME_UF__'] = ucfirst(strtolower($this->config['packageLastName']));
        $this->vars['__PROJECT_PATH__'] = $this->config['output'];
        $this->vars['__SCREEN_ORIENTATION__'] = $this->config['orientation'];
        $this->vars['__SCREEN_ORIENTATION_L__'] = strtolower($this->config['orientation']);
        $this->vars['__SCREEN_ORIENTATION_UF__'] = ucfirst(strtolower($this->config['orientation']));
        if ($this->config['orientation'] == 'landscape')
        {
            $this->vars['__SCREEN_WIDTH__'] = '960';
            $this->vars['__SCREEN_HEIGHT__'] = '640';
            $this->vars['__SCREEN_ORIENTATION_QUICK__'] = 'FIXED_HEIGHT';
            $this->vars['__SCREEN_ORIENTATION_IOS__'] = "<string>UIInterfaceOrientationLandscapeRight</string>\n<string>UIInterfaceOrientationLandscapeLeft</string>";
			$this->vars['__SCREEN_ORIENTATION_IOS5__'] = 'UIInterfaceOrientationIsLandscape';
			$this->vars['__SCREEN_ORIENTATION_IOS6__'] = 'UIInterfaceOrientationMaskLandscape';
			$this->vars['__SCREEN_ORIENTATION_SHOULDAUTOROTATE__'] = 'YES';
        }
        else
        {
            $this->vars['__SCREEN_WIDTH__'] = '640';
            $this->vars['__SCREEN_HEIGHT__'] = '960';
            $this->vars['__SCREEN_ORIENTATION_QUICK__'] = 'FIXED_WIDTH';
            $this->vars['__SCREEN_ORIENTATION_IOS__'] = '<string>UIInterfaceOrientationPortrait</string>';
			$this->vars['__SCREEN_ORIENTATION_IOS5__'] = 'UIInterfaceOrientationIsPortrait';
			$this->vars['__SCREEN_ORIENTATION_IOS6__'] = 'UIInterfaceOrientationMaskPortraitUpsideDown';
			$this->vars['__SCREEN_ORIENTATION_SHOULDAUTOROTATE__'] = 'NO';
        }

        // copy files
        $paths = $this->getPaths($this->config['template']);
        foreach ($paths as $sourcePath)
        {
            $sourceFilename = substr($sourcePath, strlen($this->config['template']));
			if (!$this->copyFile($sourcePath)) {
				return false;
			}
        }

        $this->copyFrameworkFiles();

        print("\n\n");

        return true;
    }

    private function copyFile($sourcePath)
    {
        // check filename
        $sourceFilename = substr($sourcePath, strlen($this->config['template']));
        $destinationFilename = $sourceFilename;

        foreach ($this->vars as $key => $value)
        {
            $value = str_replace('.', DS, $value);
            $destinationFilename = str_replace($key, $value, $destinationFilename);
        }

        printf("create file \"%s\" ... ", $destinationFilename);
        $dirname = pathinfo($destinationFilename, PATHINFO_DIRNAME);
        $destinationDir = $this->config['output'] . $dirname;

        if (!is_dir($destinationDir))
        {
            mkdir($destinationDir, 0755, true);
        }
        if (!is_dir($destinationDir))
        {
            printf("ERROR: mkdir failure\n");
            return false;
        }

        $destinationPath = $this->config['output'] . $destinationFilename;
        $contents = file_get_contents($sourcePath);
        if ($contents == false)
        {
            printf("ERROR: file_get_contents failure\n");
            return false;
        }
        $stat = stat($sourcePath);

        foreach ($this->vars as $key => $value)
        {
            $contents = str_replace($key, $value, $contents);
        }

        if (file_put_contents($destinationPath, $contents) == false)
        {
            printf("ERROR: file_put_contents failure\n");
            return false;
        }
        chmod($destinationPath, $stat['mode']);

        printf("OK\n");
        return true;
    }

    private function getPaths($dir)
    {
        $files = array();
        $dir = rtrim($dir, "/\\") . DS;
        $dh = opendir($dir);
        if ($dh == false)
        {
            return $files;
        }

        while (($file = readdir($dh)) !== false)
        {
            if ($file == "." || $file == ".." || $file == ".DS_Store")
            {
                continue;
            }

            $path = $dir . $file;
            if (is_dir($path))
            {
                $files = array_merge($files, $this->getPaths($path));
            }
            elseif (is_file($path))
            {
                $files[] = $path;
            }
        }
        closedir($dh);
        return $files;
    }

    private function replaceFile($src, $dest, $cmd, $flagCheck)
    {
        foreach ($this->vars as $key => $value)
        {
            $value = str_replace('.', DS, $value);
            $dest = str_replace($key, $value, $dest);
        }
        printf($cmd . " file \"%s\" ... ", $dest);
        $destinationDir = pathinfo($dest, PATHINFO_DIRNAME);

        if (!is_dir($destinationDir))
        {
            mkdir($destinationDir, 0755, true);
        }
        if (!is_dir($destinationDir))
        {
            printf("ERROR: mkdir failure\n");
            return false;
        }

        $contents = file_get_contents($src);
        if ($contents == false)
        {
            printf("ERROR: file_get_contents failure\n");
            return false;
        }
        $stat = stat($src);

        if ($flagCheck)
        {
            foreach ($this->vars as $key => $value)
            {
                $contents = str_replace($key, $value, $contents);
            }
        }

        if (file_put_contents($dest, $contents) == false)
        {
            printf("ERROR: file_put_contents failure\n");
            return false;
        }
        chmod($dest, $stat['mode']);

        printf("OK\n");
        return true;
    }

    private function copyDir($srcPath, $dstPath, $flagCheck)
    {
        $files = array();
        findFiles($srcPath, $files);
        foreach ($files as $src) 
        {
            $dest = str_replace($srcPath, $dstPath, $src);
            $this->replaceFile($src, $dest, "create", $flagCheck);
        }
    }

    private function copyFrameworkFiles()
    {
		# find "\quick\" from template path for get QUICK_V3_ROOT.
		$pos = strrpos($this->config['template'], DS . "quick" . DS);
		$quickRoot = substr($this->config['template'], 0, $pos);
        $quickPath = $quickRoot . DS . "quick";
        $cocosPath = $this->config['output'] . "src";

        $dirname = DS . "cocos";
        $src = $quickPath . $dirname;
        $dst = $cocosPath . $dirname;
        $this->copyDir($src, $dst, false);

        $dirname = DS . "framework";
        $src = $quickPath . $dirname;
        $dst = $cocosPath . $dirname;
        $this->copyDir($src, $dst, false);

        return true;
    }
}
