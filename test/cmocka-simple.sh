#!/bin/sh

TEST_DIR=`mktemp -d`

cp -v mocks/conf1 $TEST_DIR
cp .libs/simple $TEST_DIR

echo "To run the test do: cd $TEST_DIR ; ./simple"

