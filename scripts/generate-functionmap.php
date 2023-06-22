<?php

if (PHP_VERSION_ID < 80000) {
    echo 'This script requires PHP 8.0 or higher';
    exit(1);
}

$filename = __DIR__ . '/functionmap.php';
(new FunctionMapGenerator)->createFunctionMap($filename);
printf("Created call map in %s\n", $filename);

class FunctionMapGenerator
{
    public function createFunctionMap(string $filename): void
    {
        $this->writeFunctionMap($filename, $this->getFunctionMap());
    }

    private function getFunctionMap(): array
    {
        $classes = array_filter(get_declared_classes(), $this->filterItems(...));
        $interfaces = array_filter(get_declared_interfaces(), $this->filterItems(...));
        $functions = array_filter(get_defined_functions()['internal'], $this->filterItems(...));

        $functionMap = [];

        // Generate call map for functions
        foreach ($functions as $functionName) {
            $reflectionFunction = new ReflectionFunction($functionName);
            $functionMap[$reflectionFunction->getName()] = $this->getFunctionMapEntry($reflectionFunction);
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
                $functionMap[$methodKey] = $this->getFunctionMapEntry($method);
            }
        }

        return $functionMap;
    }

    private function writeFunctionMap(string $filename, array $functionMap): void
    {
        $lines = [];
        foreach ($functionMap as $methodName => $typeInfo) {
            $generatedTypeInfo = implode(
                ', ',
                array_map(
                    function (string|int $key, string $value): string {
                        if (is_int($key)) {
                            return $this->removeDoubleBackslash(var_export($value, true));
                        }

                        return sprintf('%s => %s', var_export($key, true), $this->removeDoubleBackslash(var_export($value, true)));
                    },
                    array_keys($typeInfo),
                    array_values($typeInfo)
                )
            );

            $lines[] = sprintf(
                '    %s => [%s],',
                $this->removeDoubleBackslash(var_export($methodName, true)),
                $generatedTypeInfo
            );
        }

        $fileTemplate = <<<'PHP'
<?php

$mongoDBFunctionMap = [
%s
];

PHP;

        file_put_contents($filename, sprintf($fileTemplate, implode("\n", $lines)));
    }

    private function filterItems(string $name): bool {
        $namespaces = ['MongoDB\BSON\\', 'MongoDB\Driver\\'];

        $name = strtolower($name);

        foreach ($namespaces as $namespace) {
            // Always compare lowercase names, as get_defined_functions lowercases function names by default
            if (str_starts_with($name, strtolower($namespace))) {
                return true;
            }
        }

        return false;
    }

    private function getFunctionMapEntry(ReflectionFunctionAbstract $function): array
    {
        $returnType = match(true) {
            $function->hasReturnType() => (string) $function->getReturnType(),
            $function->hasTentativeReturnType() => (string) $function->getTentativeReturnType(),
            default => 'void',
        };

        $functionMapEntry = [$returnType];

        foreach ($function->getParameters() as $parameter) {
            $parameterKey = $parameter->getName();
            if ($parameter->isOptional()) {
                $parameterKey .= '=';
            }

            $functionMapEntry[$parameterKey] = (string) $parameter->getType();
        }

        return $functionMapEntry;
    }

    private function removeDoubleBackslash(string $string): string
    {
        return str_replace('\\\\', '\\', $string);
    }
}
