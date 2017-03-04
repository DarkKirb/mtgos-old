Command:

| Word | Desc |
|------|------|
| 0    | Request ID |
| 1    | Command no |
| 2-15 | args |

Command No:

| No | Desc |
|----|------|
| 0  | ACK  |
| 1  | Unknown Command |
| 2  | Send more data |
| 3  | putChar(int) |
| 4  | puts(uint32_t length,char[]) |

More command ID's will be added as time goes on.
