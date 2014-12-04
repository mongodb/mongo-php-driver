<?php
function verify_stability($stability) {
    $stabilities = array(
        "snapshot",
        "devel",
        "alpha",
        "beta",
        "stable",
    );

    if (!in_array($stability, $stabilities)) {
        echo "Invalid stability: $stability\n";
        echo "Must be one of: ", join(", ", $stabilities), "\n";
        usage();
    }
}
function verify_version($version, $stability) {
    if (3 != sscanf($version, "%d.%d.%d", $major, $minor, $patch)) {
        var_dump($major, $minor, $patch);
        echo "Invalid version schema, expected 'major.minor.patch' (1.2.3), got $version\n";
        usage();
    }

    if ($major < 0 && $stability == "stable") {
        echo "Invalid stability for major version $major ($stability)\n";
        echo "Major versions before 1.0.0 cannot be marked as stable\n";
        usage();
    }
}
function verify_changelog($filename) {
    if (!file_exists($filename)) {
        echo "Missing ChangeLog file!\n";
        echo "Need pre-populated $filename\n";
        echo "Did you forget to run $ make ChangeLog ?\n";
        usage();
    }
}
function get_role($file) {
    switch(pathinfo($file, PATHINFO_EXTENSION)) {
    case "defs":
    case "def":
    case "m4":
    case "c":
    case "h":
        return "src";

    case "phpt":
    case "php":
    case "inc":
        return "test";

    default:
        return "doc";
    }
}

function get_files() {
    $dirs = array(
        "php_*.{h,c}",
        "config.m4",
        "Makefile.frag",
        "README*",
        "RELEASE*",
        "ChangeLog*",
        "src/*.{c,h}",
        "src/MongoDB/*.{c,h}",
        "src/BSON/*.{c,h}",
        "src/libbson/build/autotools/*.{m4}",
        "src/libbson/build/autotools/m4/*.{m4}",
        "src/libbson/src/bson/*.{c,h}",
        "src/libbson/src/yajl/*.{c,h}",
        "src/libmongoc/build/autotools/*.{m4}",
        "src/libmongoc/build/autotools/m4/*.{m4}",
        "src/libmongoc/src/mongoc/*.{c,h,def,defs}",
        "tests/batch/*.{phpt}",
        "tests/bson/*.{phpt}",
        "tests/functional/*.{phpt}",
        "tests/generic/*.{phpt}",
        "tests/readPreference/*.{phpt}",
        "tests/standalone/*.{phpt}",
        "tests/utils/*.{inc,php}",
        "tests/writeConcern/*.{phpt}",
    );
    $files = array();
    foreach($dirs as $pattern) {
        $files += array_merge($files, glob($pattern, GLOB_BRACE));
    }
    sort($files);
    return $files;
}

function format_open_dir($dir, $tab) {
    return sprintf('%s<dir name="%s">', str_repeat(" ", $tab), $dir);
}
function format_close_dir($tab) {
    return sprintf("%s</dir>", str_repeat(" ", $tab));
}
function format_file($filename, $tab) {
    return sprintf('%s<file role="%s" name="%s"/>', str_repeat(" ", $tab+1), get_role($filename), $filename);
}
function make_tree($files) {
    $retval = array();
    $lastdir = ".";
    $tab = 2;

    $retval[] = format_open_dir("/", $tab);
    foreach($files as $file) {
        $dir = dirname($file);
        $filename = basename($file);
        if ($dir != $lastdir) {
            $currdir = explode("/", $dir);
            $prevdir = explode("/", $lastdir);
            foreach($currdir as $n => $d) {
                if (isset($prevdir[$n]) && $prevdir[$n] == $d) {
                    /* In case we are shorter then previous */
                    $n++;
                    continue;
                }
                break;
            }
            if ($lastdir != ".") {
                foreach(array_reverse(array_slice($prevdir, $n)) as $close) {
                    $retval[] = format_close_dir($tab--);
                }
            }
            foreach(array_slice($currdir, $n) as $open) {
                $retval[] = format_open_dir($open, ++$tab);
            }
        }
        $retval[] = format_file($filename, $tab);
        $lastdir = $dir;
    }
    foreach(array_reverse(explode("/", $lastdir)) as $close) {
        $retval[] = format_close_dir($tab--);
    }
    $retval[] = format_close_dir($tab);

    return $retval;
}

function usage() {
    global $argv;

    echo "Usage:\n\t";
    echo $argv[0], " <version><-stability>\n";

    exit(1);
}

if ($argc != 2) {
    usage();
}

if (strpos($argv[1], "-")) {
    list($VERSION, $STABILITY) = explode("-", $argv[1], 2);
} else {
    $VERSION = $argv[1];

    /* 0.x.y. are developmental releases and cannot be stable */
    if ((int)$VERSION < 1) {
        $STABILITY = "devel";
    }
    /* A release candidate is a "beta" stability in terms of PECL */
    $rc = substr($VERSION, -3, 2);
    if (strcasecmp($rc, "rc") == 0) {
        $STABILITY = "beta";
    } else {
        $STABILITY = "stable";
    }
}

verify_stability($STABILITY);
verify_version($VERSION, $STABILITY);
$changelog = __DIR__ . "/../RELEASE-".$VERSION;
verify_changelog($changelog);


$currtime = $_SERVER["REQUEST_TIME"];
$DATE = date("Y-m-d", $currtime);
$TIME = date("H:i:s", $currtime);

$fullnotes = file($changelog, FILE_IGNORE_NEW_LINES|FILE_SKIP_EMPTY_LINES);
$NOTES = array();
foreach($fullnotes as $note) {
    $note = "    " . str_replace("&", "&amp;", trim($note));
    /* PHP JIRA Project */
    if (strstr($note, "PHP-") !== false) {
        $NOTES[] = $note;
        continue;
    }
    /* Coverity ID */
    if (strstr($note, "CID-") !== false) {
        $NOTES[] = $note;
        continue;
    }
}


$TREE = make_tree(get_files());

$contents = file_get_contents(__DIR__ . "/package.xml.in");

$REPLACE = array(
    "%RELEASE_DATE%" => $DATE,
    "%RELEASE_TIME%" => $TIME,
    "%RELEASE_VERSION%" => $VERSION,
    "%RELEASE_STABILITY%" => $STABILITY,
    "%RELEASE_NOTES%" => join("\n", $NOTES),
    "%RELEASE_FILES%" => join("\n", $TREE),
);

$contents = str_replace(array_keys($REPLACE), array_values($REPLACE), $contents);

file_put_contents(__DIR__ . "/../package.xml", $contents);
echo "Wrote package.xml\n";

