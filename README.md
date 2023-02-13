
# SPERF
| | **Architecture** |
|---|:---:|
| **macOS**        | ARM_64 |
| **macOS**        | x86_64 |
| **Ubuntu 18.04** | x86_64 |

## Overview
SPERF measures delays of data transmitted periodically from a sender to a receiver. 

## Requirement
```shell
$ ./metadata.txt 
  
  [data size]KB [frequency]Hz [duration]s [timeliness (r/n)]
```

## Usage
### compile
```shell
$ ./compile.sh
```

### receiver
```shell
$ ./receiver [n_nodes] # [n_nodes: the number of nodes]
```

### sender
```shell
$ ./sender [offset] # [offset: the offset of the sender (2...101)]
```

## Output
```shell
[delay_2_0] [delay_2_1] ... [delay_2_fd] # delays of 2nd sender
[delay_3_0] [delay_3_1] ... [delay_3_fd] # delays of 3rd sender
```