#!/usr/bin/perl -w
use strict;

my $usage = "usage: gen_component.pl component_name\n";
die $usage if scalar @ARGV == 0;

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
