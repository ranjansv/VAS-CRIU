## VAS-CRIU 

VAS-CRIU is built for fast in-memory checkpoint/restore for processes.
It snapshots memory state of a process into a process indepedent address space
which can used for restore. 

Dependencies of VAS-CRIU
- Linux 4.10+ kernel with support Multiple Virtual Address Spaces(MVAS)
- libmvas, a library containing wrappers for VAS system calls
- mvas CLI, a command line to tool to list, create, remove VASes

VAS-CRIU supports the same CLI options as CRIU.

Eg:
$>criu dump -t PID  -vvv dump.log

List VASes in the system, each new checkpoint will be stored in a VAs with a unique ID. This VAS ID 
is store in mm.img file.

$>mvas -la
/sys/kernel/vas/1/
user: 0
mode: 0600
group: 0
name: CP-24694-4576337492

/sys/kernel/vas/2/
user: 0
mode: 0600
group: 0
name: CP-3374-4576337560


$>criu restore -vvv restore.log




## CRIU (Checkpoint and Restore in Userspace)

An utility to checkpoint/restore tasks. Using this tool, you can freeze a
running application (or part of it) and checkpoint it to a hard drive as a
collection of files. You can then use the files to restore and run the
application from the point it was frozen at. The distinctive feature of the CRIU
project is that it is mainly implemented in user space.

The project home is at http://criu.org.

Pages worth starting with are:
- [Kernel configuration, compilation, etc](http://criu.org/Installation)
- [A simple example of usage](http://criu.org/Simple_loop)
- [More sophisticated example with graphical app](http://criu.org/VNC)

### A video tour on basic CRIU features
[![CRIU introduction](https://asciinema.org/a/7fnt2prsumvxiwf3ng61fgct3.png)](https://asciinema.org/a/7fnt2prsumvxiwf3ng61fgct3)

### How to contribute

* [How to submit patches](http://criu.org/How_to_submit_patches);
* Send all bug reports to [mailing
list](https://lists.openvz.org/mailman/listinfo/criu);
* Spread the word about CRIU in [social networks](http://criu.org/Contacts);
