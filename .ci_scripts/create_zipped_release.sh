#!/usr/bin/env bash

build_dir="$(pwd)/build"

if [ "$DEPLOY" = "switch" ]
then
chmod 777 $build_dir
mkdir -p $build_dir/release/julius
cp -f $build_dir/julius.nro $build_dir/release/julius/julius.nro
cd $build_dir/release && zip -r ../julius_switch.zip julius -x "*.DS_Store" -x "*__MAC*"
fi
