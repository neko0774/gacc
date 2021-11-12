#!/bin/bash
assert() {
	expected="$1"
	input="$2"

    ./gacc "$input" > tmp.s
    cc -o tmp tmp.s
    if [ -e tmp ];then
        ./tmp
    else
        echo "cannot compile the file"
        exit 1
    fi
    actual="$?"

    if [ "$actual" = "$expected" ]; then
        echo "$input => $actual"
	else
        echo "$input => $expected expected, but got $actual"
        exit 1
    fi
}

    assert 0 0
    assert 21 "5+20-4"
    assert 21 "5 +20-4"
    assert 23 "3+4*5"
    assert 4 "6 /3+2"
    assert 12 "(1+ 2)* 3 +3"
    assert 2 "-3+7-2"
    assert 1 "1==1"
    assert 1 "0<1"
    assert 1 "1>0"
    assert 0 "0>1"
    assert 0 "32+4- 6 +23<=53- 5*4"
          
    echo OK
