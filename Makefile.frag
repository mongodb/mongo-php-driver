.PHONY: mv-coverage lcov-coveralls lcov-local coverage coveralls format format-changed format-check test-clean package package.xml libmongoc-version-current libmongocrypt-version-current generate-function-map

ifneq (,$(realpath $(EXTENSION_DIR)/json.so))
PHP_TEST_SHARED_EXTENSIONS := "-d" "extension=$(EXTENSION_DIR)/json.so" $(PHP_TEST_SHARED_EXTENSIONS)
PHP_TEST_SETTINGS := "-d" "extension=$(EXTENSION_DIR)/json.so" $(PHP_TEST_SETTINGS)
endif

DATE=`date +%Y-%m-%d--%H-%M-%S`
MONGODB_VERSION=$(shell php -n -dextension=modules/mongodb.so -r 'echo MONGODB_VERSION;')
MONGODB_MINOR=$(shell echo $(MONGODB_VERSION) | cut -d. -f1,2)
MONGODB_STABILITY=$(shell php -n -dextension=modules/mongodb.so -r 'echo MONGODB_STABILITY;')

help:
	@echo ""
	@echo -e "\t$$ make coveralls"
	@echo -e "\t       - Creates code coverage report using coveralls"
	@echo -e "\t$$ make coverage"
	@echo -e "\t       - Creates code coverage report using gcov"

	@echo ""
	@echo -e "\t$$ make package.xml"
	@echo -e "\t       - Creates a package.xml file with empty release notes"
	@echo -e "\t$$ make package"
	@echo -e "\t       - Creates the pecl archive to use for provisioning"


mv-coverage:
	@if test -e $(top_srcdir)/coverage; then \
		echo "Moving previous coverage run to coverage-$(DATE)"; \
		mv coverage coverage-$(DATE); \
	fi

lcov-coveralls:
	lcov --gcov-tool $(top_srcdir)/.llvm-cov.sh --capture --directory . --output-file .coverage.lcov --no-external

lcov-local:
	lcov --gcov-tool $(top_srcdir)/.llvm-cov.sh --capture --derive-func-data --directory . --output-file .coverage.lcov --no-external

coverage: mv-coverage lcov-local
	genhtml .coverage.lcov --legend --title "mongodb code coverage" --output-directory coverage


coveralls: mv-coverage lcov-coveralls
	coveralls --exclude src/libbson --exclude src/libmongoc --exclude src/contrib --exclude lib --exclude tests

format:
	$(top_srcdir)/scripts/clang-format.sh

format-changed:
	$(top_srcdir)/scripts/clang-format.sh changed

format-check:
	$(top_srcdir)/scripts/clang-format.sh check

test-clean:
	find $(top_srcdir)/tests -not \( -path $(top_srcdir)/tests/utils -prune \) -type f -name "*.diff" -o -name "*.exp" -o -name "*.log" -o -name "*.mem" -o -name "*.out" -o -name "*.php" -o -name "*.sh" | xargs -r rm

package:
	pecl package package.xml

package.xml:
	php bin/prep-release.php $(MONGODB_VERSION) $(MONGODB_STABILITY)

libmongoc-version-current:
	cd src/libmongoc/ && python build/calc_release_version.py > ../LIBMONGOC_VERSION_CURRENT

libmongocrypt-version-current:
	cd src/libmongocrypt/ && python etc/calc_release_version.py > ../LIBMONGOCRYPT_VERSION_CURRENT

generate-function-map: all
	@if test ! -z "$(PHP_EXECUTABLE)" && test -x "$(PHP_EXECUTABLE)"; then \
		INI_FILE=`$(PHP_EXECUTABLE) -d 'display_errors=stderr' -r 'echo php_ini_loaded_file();' 2> /dev/null`; \
		if test "$$INI_FILE"; then \
			$(EGREP) -h -v $(PHP_DEPRECATED_DIRECTIVES_REGEX) "$$INI_FILE" > $(top_builddir)/tmp-php.ini; \
		else \
			echo > $(top_builddir)/tmp-php.ini; \
		fi; \
		INI_SCANNED_PATH=`$(PHP_EXECUTABLE) -d 'display_errors=stderr' -r '$$a = explode(",\n", trim(php_ini_scanned_files())); echo $$a[0];' 2> /dev/null`; \
		if test "$$INI_SCANNED_PATH"; then \
			INI_SCANNED_PATH=`$(top_srcdir)/build/shtool path -d $$INI_SCANNED_PATH`; \
			$(EGREP) -h -v $(PHP_DEPRECATED_DIRECTIVES_REGEX) "$$INI_SCANNED_PATH"/*.ini >> $(top_builddir)/tmp-php.ini; \
		fi; \
		CC="$(CC)" \
			$(PHP_EXECUTABLE) -n -c $(top_builddir)/tmp-php.ini -n -c $(top_builddir)/tmp-php.ini -d extension_dir=$(top_builddir)/modules/ $(PHP_TEST_SHARED_EXTENSIONS) $(top_srcdir)/scripts/generate-functionmap.php; \
		RESULT_EXIT_CODE=$$?; \
		rm $(top_builddir)/tmp-php.ini; \
		exit $$RESULT_EXIT_CODE; \
	else \
		echo "ERROR: Cannot generate function maps without CLI sapi."; \
	fi
