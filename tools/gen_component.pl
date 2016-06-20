#!/usr/bin/perl -w
use strict;

if (scalar @ARGV == 0) {
    print "Must supply component name.\n";
    exit;
}

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
