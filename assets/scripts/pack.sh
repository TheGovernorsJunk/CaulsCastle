#!/bin/sh

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
TexturePacker --trim-mode None --format xml --data "$DIR"/../spritesheets/hero/hero.xml --sheet "$DIR"/../spritesheets/hero/hero.png "$DIR"/../images/hero/clips

