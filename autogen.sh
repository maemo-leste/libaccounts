#!/bin/sh

set -u
set -e

mkdir -p m4

gtkdocize --copy --docdir doc/
autoreconf -v -f -i