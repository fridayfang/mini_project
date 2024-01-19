#! /usr/bin/env python3
"""
test simple math functions
"""
# test_simplemath.py

import pytest
from pytest_mini_project.simplemath import add, subtract, multiply, divide

def test_add():
    """test add function"""
    assert add(1, 2) == 3

def test_subtract():
    """test subtract function"""
    assert subtract(2, 1) == 1

def test_multiply():
    """test multiply function"""
    assert multiply(2, 3) == 6

def test_divide():
    """test divide function"""
    assert divide(6, 2) == 3

def test_divide_by_zero():
    with pytest.raises(ValueError):
        divide(1, 0)
