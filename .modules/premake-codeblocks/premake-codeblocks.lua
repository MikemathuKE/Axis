-- This is a dummy module.

local err = [[


┌─────────────────────────────╖
│                             ║
│    REMOTE CODE EXECUTION    ║
│                             ║
╘═════════════════════════════╝

Your premake shouldn't be running this script!
Update premake-core or remove the 'moduledownloader' from it.
https://github.com/premake/premake-core/issues/1381
]]

error(err);