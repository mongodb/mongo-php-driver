.PHONY: coverage testclean ChangeLog RELEASE package.xml docs

DATE=`date +%Y-%m-%d--%H-%M-%S`
PHONGO_VERSION=`php -n -dextension=modules/phongo.so -r 'echo PHONGO_VERSION;'`
PHONGO_STABILITY=`php -n -dextension=modules/phongo.so -r 'echo PHONGO_STABILITY;'`
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
	@echo -e "\t$$ make patch"
	@echo -e "\t       - When building phongo from VCS, you must run this once"

	@echo ""
	@echo -e "\t$$ make release"
	@echo -e "\t       - Runs the tests and creates the pecl archive on success"


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
	genhtml .coverage.lcov --legend --title "phongo code coverage" --output-directory coverage


coveralls: mv-coverage lcov-coveralls
	coveralls --exclude src/libbson --exclude src/libmongoc --exclude lib --exclude tests --exclude src/MongoDB/php_array.h

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

phongodep:

release: testall ChangeLog RELEASE package.xml
	pecl package package.xml
	@echo "Please run:"
	@echo "		" git add RELEASE-$(PHONGO_VERSION)
	@echo "		" git commit -m \"Add $(PHONGO_VERSION) release notes\"
	@echo "		" git tag -a -m \"Release $(PHONGO_VERSION)\" $(PHONGO_VERSION)
	@echo "		" git push --tags
	@echo "		" make release-docs
	@echo "And don't forget to bump version in php_phongo.h"

docs:
	mkdocs build --clean

release-docs: docs
	mkdocs gh-deploy --clean

patch:
	@if ! test -e $(top_srcdir)/.patched; then \
		for file in `/bin/ls -1 $(top_srcdir)/patches/*.patch | sort -n`; do \
			patch -p0 --dry-run -f -s < $$file;\
			if test $$? -eq 0; then \
				patch -p0 -f -s < $$file;\
				echo "$$file patched"; \
			else \
				echo "$$file cannot be applied!"; \
				exit 1; \
			fi \
		done; \
		touch $(top_srcdir)/.patched; \
	fi

package.xml:
	php bin/prep-release.php $(PHONGO_VERSION)-$(PHONGO_STABILITY)

RELEASE:
	@git log --pretty=format:"%ad  %an  <%ae>%n%x09* %s%n" --date short --since="$$(git show -s --format=%ad `git rev-list --tags --max-count=1`)" > RELEASE-$(PHONGO_VERSION)

ChangeLog:
	@git log --pretty=format:"%ad  %an  <%ae>%n%x09* %s%n" --date short > ChangeLog

