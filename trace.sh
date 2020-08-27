#!/bin/bash

traceroute lenta.ru 40 | grep -oe "([0-9]*\.[0-9]*\.[0-9]*\.[0-9]*)" | grep -oe "[0-9]*\.[0-9]*\.[0-9]*\.[0-9]*"
