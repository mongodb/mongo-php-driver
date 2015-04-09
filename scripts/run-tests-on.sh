#!/bin/sh

VMNAME=$1

vagrant status $VMNAME | grep -q "$VMNAME.*running"
if test $? -eq 0; then
	vagrant provision $VMNAME > .$VMNAME
else
	vagrant up $VMNAME > .$VMNAME
fi

cat .tests | grep -q -E "FAIL|WARN"
if test $? -eq 0; then
	echo "$VMNAME FAILED"
	cat .tests
	exit 2
else
	echo "$VMNAME OK"
fi

