#!/bin/bash

set -Cefu -o pipefail

function do_it {
    # local -
    set +C
    echo 100
}

declare THE_VAR="$(do_it these are arguments)"
echo "THE_VAR ($THE_VAR)"


declare CPP_VAR="$(do_it.out these are arguments)"
echo "CPP_VAR ($CPP_VAR)"
