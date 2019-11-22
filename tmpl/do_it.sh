#!/bin/bash

set -Cefu -o pipefail

function do_it {
    # local -
    set +C
    echo 100
}

declare THE_VAR="$(do_it these are arguments)"
echo "THE_VAR ($THE_VAR)"

command=do_it.out
declare CPP_VAR="$($command)"
echo "CPP_VAR ($CPP_VAR)"
