.PHONY: coverage testclean package package.xml

DATE=`date +%Y-%m-%d--%H-%M-%S`
MONGODB_VERSION=$(shell php -n -dextension=modules/mongodb.so -r 'echo MONGODB_VERSION;')
MONGODB_MINOR=$(shell echo $(MONGODB_VERSION) | cut -d. -f1,2)
MONGODB_STABILITY=$(shell php -n -dextension=modules/mongodb.so -r 'echo MONGODB_STABILITY;')
LIB_PATH=vendor/mongodb/mongodb
COMPOSER_ARGS=update --no-interaction --prefer-source
PHPUNIT_ARGS=--process-isolation

help:
	@echo -e "\t$$ make vm"
	@echo -e "\t       - Launches VMs for running multiple MongoDB variations"
	@echo -e "\t$$ make list-servers"
	@echo -e "\t       - Lists running servers, and their URIs"
	@echo -e "\t$$ make test-bootstrap"
	@echo -e "\t       - Starts up MongoDB through mongo-orchestration"

	@echo ""
	@echo -e "\t$$ make coveralls"
	@echo -e "\t       - Creates code coverage report using coveralls"
	@echo -e "\t$$ make coverage"
	@echo -e "\t       - Creates code coverage report using gcov"

	@echo ""
	@echo -e "\t$$ make composer"
	@echo -e "\t       - Installs test dependencies using composer"

	@echo ""
	@echo -e "\t$$ make distcheck"
	@echo -e "\t       - Builds the archive, runs the virtual tests"

	@echo ""
	@echo -e "\t$$ make package.xml"
	@echo -e "\t       - Creates a package.xml file with empty release notes"
	@echo -e "\t$$ make package"
	@echo -e "\t       - Creates the pecl archive to use for provisioning"
	@echo -e "\t$$ make test-virtual"
	@echo -e "\t       - Provisions some VMs, installs the pecl archive and executes the tests"


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

composer:
	@command -v composer >/dev/null 2>&1; \
	if test $$? -eq 0; then \
		composer $(COMPOSER_ARGS) ;\
		if test -d $(LIB_PATH); then \
			composer $(COMPOSER_ARGS) --working-dir $(LIB_PATH) ;\
		fi \
	elif test -r composer.phar; then \
		php composer.phar $(COMPOSER_ARGS); \
		if test -d $(LIB_PATH); then \
			php $(top_srcdir)/composer.phar $(COMPOSER_ARGS) --working-dir $(LIB_PATH) ;\
		fi \
	else \
		echo "Cannot find composer :("; \
		echo "Aborting."; \
		exit 1; \
	fi

vm:
	@command -v vagrant >/dev/null 2>&1 || { echo >&2 "Vagrant needs to be installed to run vms"; exit 1; }
	@vagrant up ldap mo

list-servers:
	php scripts/list-servers.php

test-bootstrap:
	php scripts/start-servers.php

distcheck: package test-virtual

test-virtual: package
	sh ./scripts/run-tests-on.sh freebsd
	sh ./scripts/run-tests-on.sh precise32
	sh ./scripts/run-tests-on.sh precise64

testunit: composer
	@command -v phpunit >/dev/null 2>&1; \
	if test $$? -eq 0; then \
		pushd $(LIB_PATH) ;\
		phpunit $(PHPUNIT_ARGS) ;\
		popd ;\
	elif test -r phpunit.phar; then \
		pushd $(LIB_PATH) ;\
		php $(top_srcdir)/phpunit.phar $(PHPUNIT_ARGS) ;\
		popd ;\
	else \
		echo "Cannot find phpunit :("; \
		echo "Aborting."; \
		exit 1; \
	fi

testall: composer test testunit

testclean:
	@for group in generic standalone; do \
		find $(top_srcdir)/tests/$$group -type f -name "*.diff" -o -name "*.exp" -o -name "*.log" -o -name "*.mem" -o -name "*.out" -o -name "*.php" -o -name "*.sh" | xargs rm -f; \
	done;

package:
	pecl package package.xml

package.xml:
	php bin/prep-release.php $(MONGODB_VERSION) $(MONGODB_STABILITY)
