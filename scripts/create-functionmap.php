<?php

$filter = function (string $name): bool {
    $namespaces = ['MongoDB\BSON\\', 'MongoDB\Driver\\'];

    foreach ($namespaces as $namespace) {
        if (str_starts_with($name, $namespace) || str_starts_with($name, strtolower($namespace))) {
            return true;
        }
    }

    return false;
};

$getFunctionCallMapEntry = function(ReflectionFunctionAbstract $function): array {
    $returnType = match(true) {
        $function->hasReturnType() => (string) $function->getReturnType(),
        $function->hasTentativeReturnType() => (string) $function->getTentativeReturnType(),
        default => 'void',
    };
    $callmapEntry = [$returnType];

    foreach ($function->getParameters() as $parameter) {
        $parameterKey = $parameter->getName();
        if ($parameter->isOptional()) {
            $parameterKey .= '=';
        }

        $parameterType = (string) $parameter->getType();
        if ($function->getName() === 'unserialize' && $parameter->getName() === 'serialized') {
            $parameterType = 'string';
        }

        $callmapEntry[$parameterKey] = $parameterType;
    }

    return $callmapEntry;
};

$classes = array_filter(get_declared_classes(), $filter);
$interfaces = array_filter(get_declared_interfaces(), $filter);
$functions = array_filter(get_defined_functions()['internal'], $filter);

$callmap = [];

// Generate call map for functions
foreach ($functions as $functionName) {
    $reflectionFunction = new ReflectionFunction($functionName);
    $callmap[$reflectionFunction->getName()] = $getFunctionCallMapEntry($reflectionFunction);
}

// Generate call map for classes and interfaces
$members = array_merge($classes, $interfaces);
sort($members);

$skippedMethods = ['__set_state', '__wakeup', '__serialize', '__unserialize'];

foreach ($members as $member) {
    $reflectionClass = new ReflectionClass($member);

    foreach ($reflectionClass->getMethods(ReflectionMethod::IS_PUBLIC) as $method) {
        if ($method->getDeclaringClass() != $reflectionClass && $method->getName() != '__toString') {
            continue;
        }

        if (in_array($method->getName(), $skippedMethods, true)) {
            continue;
        }

        $methodKey = $reflectionClass->getName() . '::' . $method->getName();
        $callmap[$methodKey] = $getFunctionCallMapEntry($method);
    }
}

$map = var_export($callmap, true);

// Format output

$replacements = [
    '#array \(#' => '[',     // Short array syntax (opening)
    '#\)#' => ']',           // Short array syntax (closing)
    '#0 => #' => '',         // Numeric index for return type
    '#\\\\{2}#' => '\\',     // Double backslash
    '# => $\s+#m' => ' => ', // Newlines at start of array
    '#\s+\[$\s+#m' => ' [',  // Newlines at start of array
    '#,$\s+#m' => ', ',      // Newline after array elements
    '#, \],\s+#' => "],\n",  // Newline after array elements
    '#^\s*\'#m' => '    \'', // Fix indentation
];

echo preg_replace(
    array_keys($replacements),
    array_values($replacements),
    $map,
);
