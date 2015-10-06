.PHONY: coverage testclean ChangeLog RELEASE package package.xml docs

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
	@echo -e "\t$$ make release"
	@echo -e "\t       - Packages the archive, runs the tests locally and virtual"

	@echo ""
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

mongodbdep:

release: test distcheck
	@echo "Please run:"
	@echo "		" git add RELEASE-$(MONGODB_VERSION)
	@echo "		" git commit -m \"Add $(MONGODB_VERSION) release notes\"
	@echo "		" git tag -a -m \"Release $(MONGODB_VERSION)\" $(MONGODB_VERSION)
	@echo "		" git push --tags
	@echo "		" make release-docs
	@echo "And don't forget to bump version in php_phongo.h"

package: ChangeLog package.xml
	@git checkout RELEASE-$(MONGODB_MINOR)
	pecl package package.xml
	@cat RELEASE-$(MONGODB_MINOR) >> RELEASE-$(MONGODB_VERSION)
	@mv RELEASE-$(MONGODB_VERSION) RELEASE-$(MONGODB_MINOR)

docs:
	mkdocs build --clean

release-docs: docs
	mkdocs gh-deploy --clean

package.xml: RELEASE
	php bin/prep-release.php $(MONGODB_VERSION)-$(MONGODB_STABILITY)

RELEASE:
	@echo "RELEASE $(MONGODB_VERSION)" >> RELEASE-$(MONGODB_VERSION)
	@echo "-------------" >> RELEASE-$(MONGODB_VERSION)
	@git log --pretty=format:"%ad  %an  <%ae>%n%x09* %s%n" --no-merges --date short --since="$$(git show -s --format=%ad `git rev-list --tags --max-count=1`)" >> RELEASE-$(MONGODB_VERSION)
	@echo -e "\n" >> RELEASE-$(MONGODB_VERSION)

ChangeLog:
	@git log --pretty=format:"%ad  %an  <%ae>%n%x09* %s%n" --date short > ChangeLog

