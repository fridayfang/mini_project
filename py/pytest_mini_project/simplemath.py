#! /usr/bin/env python3
"""
simple math functions
"""
# simplemath.py

def add(a, b):
    """add two numbers"""
    return a + b

def subtract(a, b):
    """subtract two numbers"""
    return a - b

def multiply(a, b):
    """multiply two numbers"""
    return a * b

def divide(a, b):
    """divide two numbers"""
    if b == 0:
        raise ValueError("Cannot divide by zero")
    return a / b
