install:
    xcopy "$(SRCROOT)\vsprops\*.vsprops" "$(DSTROOT)\AppleInternal\tools\vsprops" /e/v/i/h/y
    xcopy "$(SRCROOT)\scripts\*" "$(DSTROOT)\AppleInternal\tools\scripts" /e/v/i/h/y
