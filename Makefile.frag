.PHONY: coverage testclean ChangeLog RELEASE package.xml

DATE=`date +%Y-%m-%d--%H-%M-%S`
PHONGO_VERSION=`php -n -dextension=modules/phongo.so -r 'echo PHONGO_VERSION;'`
PHONGO_STABILITY=`php -n -dextension=modules/phongo.so -r 'echo PHONGO_STABILITY;'`

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
	composer install --dev --no-interaction --prefer-source

testclean:
	@for group in generic standalone; do \
		find $(top_srcdir)/tests/$$group -type f -name "*.diff" -o -name "*.exp" -o -name "*.log" -o -name "*.mem" -o -name "*.out" -o -name "*.php" -o -name "*.sh" | xargs rm -f; \
	done;

phongodep:

release: test ChangeLog RELEASE package.xml
	pecl package package.xml
	@echo "Please run:"
	@echo "		" git commit -m \"Add $(PHONGO_VERSION) release notes\" $(PHONGO_VERSION)
	@echo "		" git tag -a -m \"Release $(PHONGO_VERSION)\" $(PHONGO_VERSION)
	@echo "		" git push --tags
	@echo "And don't forget to pump version in php_phongo.h"

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

