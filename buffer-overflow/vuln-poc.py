import struct
import os

# Start with 1000 A's, see if it crashes
buffer = "A" * 1000

# Execute and pass buffer as an argument
PROGNAME="./vuln"
os.execv(PROGNAME, [PROGNAME, buffer])