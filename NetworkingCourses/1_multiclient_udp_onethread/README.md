## Compile:
gcc server.c -o server  
gcc client.c -o client  

## Usage:  
./server  

./client ip port  


## Example server output (two clients):  
./server  
[SERVER] IP addr = 0.0.0.0 port = 57589  
[SERVER] NEW CLIENT! port 64224  
[SERVER] Received message from client 0: 1 port 57594  
[SERVER] Sent to client 0: 1  
[SERVER] NEW CLIENT! port 64480  
[SERVER] Received message from client 1: 1 port 57595  
[SERVER] Sent to client 1: 1  
[SERVER] Received message from client 0: 2 port 57594  
[SERVER] Sent to client 0: 2  
[SERVER] Received message from client 1: 2 port 57595  
[SERVER] Sent to client 1: 2  
[SERVER] Received message from client 0: 3 port 57594  
[SERVER] Sent to client 0: 3  
[SERVER] Received message from client 1: 3 port 57595  
[SERVER] Sent to client 1: 3  
[SERVER] Retransmission to client_id 0 port 57594 last_recv = 3  
[SERVER] Sent to client 0: 3  
[SERVER] Retransmission to client_id 1 port 57595 last_recv = 3  
[SERVER] Sent to client 1: 3  
[SERVER] Received message from client 0: 4 port 57594  
[SERVER] Sent to client 0: 4  
[SERVER] Received message from client 1: 5 port 57595  
[SERVER] Sent to client 1: 4  
[SERVER] Received message from client 0: 5 port 57594  
[SERVER] Sent to client 0: 5  
[SERVER] Received message from client 1: 6 port 57595  
[SERVER] Sent to client 1: 5  
[SERVER] Received message from client 0: 6 port 57594  
[SERVER] Sent to client 0: 6  
[SERVER] Received message from client 1:  port 57595  
[SERVER] Sent to client 1: 6  
[SERVER] Retransmission to client_id 0 port 57594 last_recv = 6  
[SERVER] Sent to client 0: 6  
[SERVER] Retransmission to client_id 1 port 57595 last_recv = 6  
[SERVER] Sent to client 1: 6  
[SERVER] Received message from client 0: 7 port 57594  
[SERVER] Sent to client 0: 7  
[SERVER] client 0 is free  
[SERVER] Received message from client 1:  port 57595  
[SERVER] Sent to client 1: 7  
[SERVER] client 1 is free  
[SERVER] Max client (2) limit has been reached  
  
## First and second client output:  
./client 0.0.0.0 57589  
[CLIENT] Sent to server message: 0 (connect)  
[CLIENT] Sent to server message: 1  
[CLIENT] Received message from server: 1  
[CLIENT] Sent to server message: 2  
[CLIENT] Received message from server: 2  
[CLIENT] Sent to server message: 3  
[CLIENT] Received message from server: 3  
[CLIENT] Lost packet on purpose, waiting for retransmission of 3  
[CLIENT] Received message from server: 3  
[CLIENT] Sent to server message: 4  
[CLIENT] Received message from server: 4  
[CLIENT] Sent to server message: 5  
[CLIENT] Received message from server: 5  
[CLIENT] Sent to server message: 6  
[CLIENT] Received message from server: 6  
[CLIENT] Lost packet on purpose, waiting for retransmission of 6  
[CLIENT] Received message from server: 6  
[CLIENT] Sent to server message: 7  
[CLIENT] Received message from server: 7  
