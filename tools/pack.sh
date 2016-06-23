#!/bin/sh

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
# TexturePacker --trim-mode None --format xml --data "$DIR"/../spritesheets/hero/hero.xml --sheet "$DIR"/../spritesheets/hero/hero.png "$DIR"/../images/hero/clips
# TexturePacker --trim-mode None --format xml --data "$DIR"/../spritesheets/priest/priest.xml --sheet "$DIR"/../spritesheets/priest/priest.png "$DIR"/../images/priest/clips
TexturePacker "$DIR"/../assets/images/hero/hero.tps
TexturePacker "$DIR"/../assets/images/priest/priest.tps

