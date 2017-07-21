#!/usr/bin/env sh

PGDATA="/mnt/data/database"

SELFDIR="`readlink -f $0 | xargs dirname`"

rm -rf ${PGDATA}
mkdir ${PGDATA}
chown postgres:postgres ${PGDATA}

su -l postgres -s /bin/sh -c "initdb --no-locale -E UTF8 -D $PGDATA"

su -l postgres -s /bin/sh -c "cat $SELFDIR/initdb.sql | postgres --single -D $PGDATA -j"
