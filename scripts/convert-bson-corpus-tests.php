<?php

require_once __DIR__ . '/../tests/utils/tools.php';

$outputPath = realpath(__DIR__ . '/../tests') . '/bson-corpus/';

if ( ! is_dir($outputPath) && ! mkdir($outputPath, 0755, true)) {
    printf("Error creating output path: %s\n", $outputPath);
}

foreach (array_slice($argv, 1) as $inputFile) {
    if ( ! is_readable($inputFile) || ! is_file($inputFile)) {
        printf("Error reading %s\n", $inputFile);
        continue;
    }

    $test = json_decode(file_get_contents($inputFile), true);

    if (json_last_error() !== JSON_ERROR_NONE) {
        printf("Error decoding %s: %s\n", $inputFile, json_last_error_msg());
        continue;
    }

    if ( ! empty($test['deprecated'])) {
        printf("Skipping deprecated test file: %s\n", $inputFile);
        continue;
    }

    if ( ! empty($test['valid'])) {
        foreach ($test['valid'] as $i => $case) {
            $outputFile = sprintf('%s-valid-%03d.phpt', pathinfo($inputFile, PATHINFO_FILENAME), $i + 1);
            $output = renderPhpt(getParamsForValid($test, $case));

            if (false === file_put_contents($outputPath . '/' . $outputFile, $output)) {
                printf("Error writing valid[%d] in %s\n", $i, $inputFile);
                continue;
            }
        }
    }

    if ( ! empty($test['decodeErrors'])) {
        foreach ($test['decodeErrors'] as $i => $case) {
            $outputFile = sprintf('%s-decodeError-%03d.phpt', pathinfo($inputFile, PATHINFO_FILENAME), $i + 1);
            $output = renderPhpt(getParamsForDecodeError($test, $case));

            if (false === file_put_contents($outputPath . '/' . $outputFile, $output)) {
                printf("Error writing decodeErrors[%d] in %s\n", $i, $inputFile);
                continue;
            }
        }
    }

    if ( ! empty($test['parseErrors'])) {
        foreach ($test['parseErrors'] as $i => $case) {
            $outputFile = sprintf('%s-parseError-%03d.phpt', pathinfo($inputFile, PATHINFO_FILENAME), $i + 1);
            try {
                $output = renderPhpt(getParamsForParseError($test, $case));
            } catch (UnexpectedValueException $e) {
                printf("Parse errors not supported for BSON type: %s\n", $test['bson_type']);
                continue;
            }

            if (false === file_put_contents($outputPath . '/' . $outputFile, $output)) {
                printf("Error writing parseErrors[%d] in %s\n", $i, $inputFile);
                continue;
            }
        }
    }
}

function getParamsForValid(array $test, array $case)
{
    $code = '';
    $expect = '';

    $bson = $case['bson'];
    $canonicalBson = isset($case['canonical_bson']) ? $case['canonical_bson'] : $bson;
    $expectedCanonicalBson = strtolower($canonicalBson);

    $code .= sprintf('$bson = hex2bin(%s);', var_export($bson, true)) . "\n";

    $code .= "\n// BSON to Canonical BSON\n";
    $code .= 'echo bin2hex(fromPHP(toPHP($bson))), "\n";' . "\n";
    $expect .= $expectedCanonicalBson . "\n";

    if ($bson !== $canonicalBson) {
        $code .= "\n" . sprintf('$canonicalBson = hex2bin(%s);', var_export($canonicalBson, true)) . "\n";
        $code .= "\n// Canonical BSON to Canonical BSON\n";
        $code .= 'echo bin2hex(fromPHP(toPHP($canonicalBson))), "\n";' . "\n";
        $expect .= $expectedCanonicalBson . "\n";
    }

    if (isset($case['extjson'])) {
        $json = $case['extjson'];
        $canonicalJson = isset($case['canonical_extjson']) ? $case['canonical_extjson'] : $json;
        $expectedCanonicalJson = json_canonicalize($canonicalJson);
        $lossy = isset($case['lossy']) ? (boolean) $case['lossy'] : false;

        $code .= "\n// BSON to Canonical extJSON\n";
        $code .= 'echo json_canonicalize(toJSON($bson)), "\n";' . "\n";;
        $expect .= $expectedCanonicalJson . "\n";

        $code .= "\n" . sprintf('$json = %s;', var_export($json, true)) . "\n";
        $code .= "\n// extJSON to Canonical extJSON\n";
        $code .= 'echo json_canonicalize(toJSON(fromJSON($json))), "\n";' . "\n";;
        $expect .= $expectedCanonicalJson . "\n";

        if ($bson !== $canonicalBson) {
            $code .= "\n// Canonical BSON to Canonical extJSON\n";
            $code .= 'echo json_canonicalize(toJSON($canonicalBson)), "\n";' . "\n";;
            $expect .= $expectedCanonicalJson . "\n";
        }

        if ($json !== $canonicalJson) {
            $code .= "\n" . sprintf('$canonicalJson = %s;', var_export($canonicalJson, true)) . "\n";
            $code .= "\n// Canonical extJSON to Canonical extJSON\n";
            $code .= 'echo json_canonicalize(toJSON(fromJSON($canonicalJson))), "\n";' . "\n";;
            $expect .= $expectedCanonicalJson . "\n";
        }

        if ( ! $lossy) {
            $code .= "\n// extJSON to Canonical BSON\n";
            $code .= 'echo bin2hex(fromJSON($json)), "\n";' . "\n";;
            $expect .= $expectedCanonicalBson . "\n";

            if ($json !== $canonicalJson) {
                $code .= "\n// Canonical extJSON to Canonical BSON\n";
                $code .= 'echo bin2hex(fromJSON($canonicalJson)), "\n";' . "\n";;
                $expect .= $expectedCanonicalBson . "\n";
            }
        }
    }

    return [
        '%NAME%' => sprintf('%s: %s', trim($test['description']), trim($case['description'])),
        '%CODE%' => trim($code),
        '%EXPECT%' => trim($expect),
    ];
}

function getParamsForDecodeError(array $test, array $case)
{
    $code = sprintf('$bson = hex2bin(%s);', var_export($case['bson'], true)) . "\n\n";
    $code .= "throws(function() use (\$bson) {\n";
    $code .= "    var_dump(toPHP(\$bson));\n";
    $code .= "}, 'MongoDB\Driver\Exception\UnexpectedValueException');";

    /* We do not test for the exception message, since that may differ based on
     * the nature of the decoding error. */
    $expect = "OK: Got MongoDB\Driver\Exception\UnexpectedValueException";

    return [
        '%NAME%' => sprintf('%s: %s', trim($test['description']), trim($case['description'])),
        '%CODE%' => trim($code),
        '%EXPECT%' => trim($expect),
    ];
}

function getParamsForParseError(array $test, array $case)
{
    $code = '';
    $expect = '';

    switch ($test['bson_type']) {
        case '0x13': // Decimal128
            $code = "throws(function() {\n";
            $code .= sprintf("    new MongoDB\BSON\Decimal128(%s);\n", var_export($case['string'], true));
            $code .= "}, 'MongoDB\Driver\Exception\InvalidArgumentException');";

            /* We do not test for the exception message, since that may differ
             * based on the nature of the parse error. */
            $expect = "OK: Got MongoDB\Driver\Exception\InvalidArgumentException";
            break;

        default:
            throw new UnexpectedValueException;
    }

    return [
        '%NAME%' => sprintf('%s: %s', trim($test['description']), trim($case['description'])),
        '%CODE%' => trim($code),
        '%EXPECT%' => trim($expect),
    ];
}

function renderPhpt(array $params)
{
    $template = <<< 'TEMPLATE'
--TEST--
%NAME%
--DESCRIPTION--
Generated by scripts/convert-bson-corpus-tests.php

DO NOT EDIT THIS FILE
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

%CODE%

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
%EXPECT%
===DONE===
TEMPLATE;

    return str_replace(array_keys($params), array_values($params), $template);
}
