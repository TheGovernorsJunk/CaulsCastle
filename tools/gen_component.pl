#!/usr/bin/perl -w
use strict;

my $usage = "Usage: gen_component.pl [options] component_name";
my $help = <<EOF;

$usage
Options:
  --help            prints help and exits

Generates to standard output Lua script that interfaces with entity
state machines. For consistency, any Lua file subsequently created
should be named like the argument passed here.

EOF

die $usage, "\n" unless @ARGV;
exit 0 if $ARGV[0] eq "--help" && print $help;

my $component = $ARGV[0];

my $script = <<EOF;
local function execute(entity, dt)
end

-- local function enter(entity)
-- end

-- local function exit(entity)
-- end

$component = {
    -- enter = enter,
    -- exit = exit,
    execute = execute
}

return $component
EOF

print $script;
