project.name = "wxShapeFramework"

-- Configurations. All I am doing is reordering them so that
-- it defaults to a Release build.
project.configs = { "Release", "Debug" }

-- Add packages here.
dopackage( "../src" )
dopackage( "../samples/wxSFDemo" )
dopackage( "../samples/wxSFSample1" )
dopackage( "../samples/wxSFSample2" )
dopackage( "../samples/wxSFSample3" )
dopackage( "../samples/wxSFSample4" )

