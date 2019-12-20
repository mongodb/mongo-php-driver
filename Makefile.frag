.PHONY: coverage test-clean package package.xml format format-changed

DATE=`date +%Y-%m-%d--%H-%M-%S`
MONGODB_VERSION=$(shell php -n -dextension=modules/mongodb.so -r 'echo MONGODB_VERSION;')
MONGODB_MINOR=$(shell echo $(MONGODB_VERSION) | cut -d. -f1,2)
MONGODB_STABILITY=$(shell php -n -dextension=modules/mongodb.so -r 'echo MONGODB_STABILITY;')

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

vm:
	@command -v vagrant >/dev/null 2>&1 || { echo >&2 "Vagrant needs to be installed to run vms"; exit 1; }
	@vagrant up ldap mo

list-servers:
	php scripts/list-servers.php

test-bootstrap:
	vagrant reload mo
	vagrant ssh mo -c 'sudo rm -f /home/vagrant/server.pid'
	vagrant ssh mo -c 'sudo mongo-orchestration -f mongo-orchestration-config.json -b 192.168.112.10 --enable-majority-read-concern start'
	php scripts/start-servers.php

format:
	$(top_srcdir)/scripts/clang-format.sh

format-changed:
	$(top_srcdir)/scripts/clang-format.sh changed

distcheck: package test-virtual

test-virtual: package
	sh ./scripts/run-tests-on.sh freebsd
	sh ./scripts/run-tests-on.sh precise32
	sh ./scripts/run-tests-on.sh precise64

test-clean:
	find $(top_srcdir)/tests -not \( -path $(top_srcdir)/tests/utils -prune \) -type f -name "*.diff" -o -name "*.exp" -o -name "*.log" -o -name "*.mem" -o -name "*.out" -o -name "*.php" -o -name "*.sh" | xargs -r rm

package:
	pecl package package.xml

package.xml:
	php bin/prep-release.php $(MONGODB_VERSION) $(MONGODB_STABILITY)

libmongoc-version-current:
	cd src/libmongoc/ && python build/calc_release_version.py > ../LIBMONGOC_VERSION_CURRENT

libmongocrypt-version-current:
	cd src/libmongocrypt/ && cmake -P ./cmake/GetVersion.cmake 2> ../LIBMONGOCRYPT_VERSION_CURRENT
