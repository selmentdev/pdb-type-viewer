@echo off

set "TARGET_VERSION=vs2019"
if not "%1"=="" set "TARGET_VERSION=vs2017"

tools\premake\premake5 %TARGET_VERSION%