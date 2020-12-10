<a name="overview"/>


TLS Cache
=========
A Final Project for CS165 Computer Security designed by Jacob Tan and Richard Duong<br>
[Link to the Github Repository here](https://github.com/richard-duong/TLSCache)<br>

## Objective:
Implement a secure proxy application using TLS protocol to provide simple authentication and secure file transmission. With this, we can demonstrate how a large scale system with a server caching objects inside of proxies can interact with a client and provide objects upon request in a secure and optimal manner.<br>
[The full assignment specifications](docs/finalproject.pdf)<br><br>


-----------------------------
<a name="table-of-contents"/>


Table of Contents
=================
1. [Overview](#overview)<br>
2. [Table of Contents](#table-of-contents)<br>
3. [How to use](#how-to-use)<br>
4. [Phase Design](#phase-design)<br>
        a) [Initialization](#initial-phase)<br>
        b) [Standard Application Process](#standard-phase)<br>
        c) [Nonstandard Application Process with False Positives](#nonstandard-phase)<br>
5. [Component Design](#component-design)<br>
        a) [Packet](#packet-design)<br>
        b) [TLS](#tls-design)<br>
        c) [Rendezvous Hashing](#rendezvous-hashing-design)<br>
        d) [Bloom Filter](#bloom-filter-design)<br>
        e) [Trie](#trie-design)<br>
6. [System Design](#system-design)<br>
        a) [Client](#client-design)<br>
        b) [Server](#server-design)<br>
        c) [Proxy](#proxy-design)<br>
        d) [Blacklist](#blacklist-design)<br>
7. [Distribution of Work](#distribution-of-work)<br>
        a) [Jacob's Contributions](#jacob-contribution)<br>
        b) [Richard's Contributions](#richard-contribution)<br>
8. [Final Words](#final-words)<br>
9. [References](#references)<br>



----------------------
<a name="how-to-use"/>

How to use
==========
This repository contains the starter code for the CS165 project. The directory structure is as follows:
```
certificates/	// Contains CA and server certificates.
scripts/	// Helper scripts.
src/		// Client and Server code. Add your code here.
cmake/		// CMake find script. 
extern/		// Required third party tools and libraries- LibreSSL & CMake.
licenses/	// Open source licenses for code used.
```


### Steps
1. Download and extract the code.
2. Run the following commands:
```
$ cd TCPSocket_iii
$ source scripts/setup.sh

Generate the server and client certificates
$ cd certificates
$ make
```
3. The plaintext server and client can be used as follows:
```
$ cd TCPSocket_iii

Run the server:
$ ./build/src/server 9999

Run the client (in another terminal):
$ ./build/src/client 127.0.0.1 9999
```

### How to build and run code
1. Add your code in `src/client` or `src/server`. 
2. Go to `build/`
3. Run `make`


### Scripts included
1. `setup.sh` should be run exactly once after you have downloaded code, and never again. It extracts and builds the dependencies in extern/, and builds and links the code in src/ with LibreSSL.
2. `reset.sh` reverts the directory to its initial state. It does not touch `src/` or `certificates/`. Run `make clean` in `certificates/` to delete the generated certificates.


### FAQ
1. How to generate CA, server and client certificates?

Go to `certificates/` and run `make` to generate all three certificates. 
```
root.pem	// Root CA certificate, the root of trust
server.crt	// Server's certificate, signed by the root CA using an intermediate CA certificate 
server.key	// Server's private key
```

2. The given starter code has only two files, `server/server.c` and `client/client.c`. I want to add another file to implement the proxy. How do I do it?

This project uses CMake to build code, and therefore has a `CMakeLists.txt` file located in `src/`. You can read the file as follows:
```
set(CLIENT_SRC	client/client.c)	# The CLIENT_SRC variable holds the names of all files that are a part of client's implementation. This is a client that has only one file in its implementation.
add_executable(client ${CLIENT_SRC})    # Tells CMake to compile all files listed in CLIENT_SRC into a binary named 'client'
target_link_libraries(client LibreSSL::TLS) # Asks CMake to link our executable to libtls
```
If you want to split your client's code into multiple files, you can modify `src/CMakeLists.txt` as follows:
```
set(CLIENT_SRC client/client_1.c client/client_2.c)
```
Your code can be split into any number of files as necessary, but remember that they are all compiled to a single runnable binary. 
If you want to create more binaries, you can copy the three lines explained above and change the variable and file names as necesary.



------------------------
<a name="phase-design"/>

Phase Design
============

**1) Initialization**<br>
This phase prepares the server and proxies with the appropriate blacklists & Bloom Filters<br>

**2) Standard Application Process**<br>
This phase evaluates our clients' requests without a match on the Bloom Filter<br>

**3) Nonstandard Application Process with False Positives**<br>
This phase evaluates our clients' requests with a match on the Bloom Filter<br><br><br><br>


<a name="initial-phase"/>

## Initialization
Before being able to run the TLS application, we need to be able to set up the system so that we instantiate the bloom filters and trie blacklists on each proxy. On top of that, because proxies might be set at different times, so what we can do instead, is allow the server to wait for connections from the proxy, and once all of the connections are made, we can pass out the hash keys and allow clients to finally start requesting objects.<br>
+ **Server Initialization**<br>
+ **Proxy Initialization**<br>
+ **Client Initialization**<br><br>


<a name="initial-phase-server"/>

### Server Initialization
**Has access to:**
- Entire object file<br>
- Blacklist object file<br>
- Proxy name/port list<br>
- HRW hash key<br>
- BF hash keys<br>

**Steps:**
- Read objects from file<br>
- Generate Bloom Filter for each proxy to use<br>
- Wait for all proxies to connect<br><br>
![Server Initialization Image](docs/initialize-server.png)<br><br>
![Packet Transmission: Server Initialization Image](docs/packet-initialize-server.png)<br><br><br><br>


<a name="initial-phase-proxy"/>

### Proxy Initialization
**Has access to:**
- Server port<br>
- TLS certificate<br>
- Relevant blacklist object file<br>

**Steps:**
- Generate trie from reading relevant blacklist<br>
- Request the Bloom Filter from the server<br>
- Retrieve and copy the Bloom Filter from the buffer<br><br>
![Proxy Initialization Image](docs/initialize-proxy.png)<br><br>
![Packet Transmission: Proxy Initialization Image](docs/packet-initialize-proxy.png)<br><br><br><br>


<a name="initial-phase-client"/>

### Client Initialization
**Steps:**
- Wait for server to finish receiving initial connections from all proxies<br>
- Generate proxies.txt and keys.txt for client applications to use later<br>
- Read and store proxies.txt and keys.txt<br><br>
![Client Initialization Image](docs/initialize-client.png)<br><br>
![Packet Transmissino: Client Initialization Image](docs/packet-initialize-client.png)<br><br><br><br>



<a name="standard-phase"/>

## Standard Application Process
This phase is the standard application process. In the standard application process, we are anticipating an object request that does not produce a false positive in the Bloom Filter. If the client makes a request to the application, they're expected to encounter one of these four scenarios.<br>
+ **Scenario 1: Client requests object on proxy**<br>
+ **Scenario 2: Client requests object on server**<br>
+ **Scenario 3: Client requests nonexistent object**<br>
+ **Scenario 4: Client requests blacklisted object**<br><br>



<a name="standard-phase-scene-1"/>

### Scenario 1: Client requests object on proxy
**Steps:**
- Client runs HRW on object to determine which proxy holds the cached object<br>
- Client requests object from result proxy<br>
- Proxy checks if object is on Bloom Filter for blacklisted objects, finds no match<br>
- Proxy checks local cache for object, finds object<br>
- Proxy returns object to Client<br><br>
![Client requests object on proxy no FP Image](docs/standard-request-proxy.png)<br><br>
![Packet Transmission: Client requests object on proxy no FP Image](docs/packet-standard-request-proxy.png)<br><br><br><br>


<a name="standard-phase-scene-2"/>

### Scenario 2: Client requests object on server
**Steps:**
- Client runs HRW on object to determine which proxy holds the cached object<br>
- Client requests object from result proxy<br>
- Proxy checks if object is on Bloom Filter for blacklisted objects, finds no match<br>
- Proxy checks local cache for object, finds no match<br>
- Proxy requests object from server<br>
- Server checks locally for object, finds object<br>
- Server returns object to Proxy<br>
- Proxy returns object to Client<br><br>
![Client requests object on server no FP Image](docs/standard-request-server.png)<br><br>
![Packet Transmission: Client requests object on proxy no FP Image](docs/standard-request-server-transmission.png)<br><br><br><br>


<a name="standard-phase-scene-3"/>

### Scenario 3: Client requests nonexistent object
**Steps:**
- Client runs HRW on object to determine which proxy holds the cached object<br>
- Client requests object from result proxy<br>
- Proxy checks if object is on Bloom Filter of blacklisted objects, finds no match<br>
- Proxy checks local cache for object, finds no match<br>
- Proxy requests object from server<br>
- Server checks locally for object, finds no match<br>
- Server returns *no object found* to Proxy<br>
- Proxy returns *no object found* to Client<br><br>
![Client requests nonexistent object no FP Image](docs/standard-request-none.png)<br><br>
![Packet Transmission: Client requests nonexistent object no FP Image](docs/standard-request-none-transmission.png)<br><br><br><br>


<a name="standard-phase-scene-4"/>

### Scenario 4: Client requests blacklisted object
**Steps:**
- Client runs HRW on object to determine which proxy holds the cached object<br>
- Client requests object from result proxy<br>
- Proxy checks if object is on Bloom Filter of blacklisted objects, finds match<br>
- Proxy checks if object is on Trie of blacklisted objects, finds match<br>
- Proxy returns *access deny* to Client<br><br>
![Client requests blacklisted object no FP Image](docs/standard-request-blacklist.png)<br><br>
![Packet Transmission: Client requests blacklisted object no FP Image](docs/standard-request-blacklist-transmission.png)<br><br><br><br>



<a name="nonstandard-phase"/>

## Nonstandard Application Process with False Positives
This phase is the nonstandard application process. In the nonstandard application process, we are anticipating an object request that produces false positive on the Bloom Filter. With a false positive, that means the situation where the client requests a blacklisted object is excluded. If the client makes a request to the application, they're expected to encounter one of these three scenarios.<br>
+ **Scenario 1: Client requests object on proxy**<br>
+ **Scenario 2: Client requests object on server**<br>
+ **Scenario 3: Client requests nonexistent object**<br><br>


<a name="nonstandard-phase-scene-1"/>

### Scenario 1: Client requests object on proxy
**Steps:**
- Client runs HRW on object to determine which proxy holds the cached object<br>
- Client requests object from result proxy<br>
- Proxy checks if object is on Bloom Filter for blacklisted objects, finds match<br>
- Proxy checks if object is on Trie of blacklisted objects, finds no match (false positive)<br>
- Proxy checks local cache for object, finds object<br>
- Proxy returns object to Client<br><br>
![Client requests object on proxy FP Image](docs/nonstandard-request-proxy.png)<br><br>
![Packet Transmission: Client requests object on proxy FP Image](docs/packet-nonstandard-request-proxy.png)<br><br><br><br>


<a name="nonstandard-phase-scene-2"/>

### Scenario 2: Client requests object on server
**Steps:**
- Client runs HRW on object to determine which proxy holds the cached object<br>
- Client requests object from result proxy<br>
- Proxy checks if object is on Bloom Filter for blacklisted objects, finds match<br>
- Proxy checks if object is on Trie of blacklisted objects, finds no match (false positive)<br>
- Proxy checks local cache for object, finds object<br>
- Proxy requests object from server<br>
- Server checks locally for object, finds object<br>
- Server returns object to Proxy<br>
- Proxy returns object to Client<br><br>
![Client requests object on server FP Image](docs/nonstandard-request-server.png)<br><br>
![Packet Transmission: Client requests object on server FP Image](docs/packet-nonstandard-request-server.png)<br><br><br><br>


<a name="nonstandard-phase-scene-3"/>

### Scenario 3: Client requests nonexistent object
**Steps:**
- Client runs HRW on object to determine which proxy holds the cached object<br>
- Client requests object from result proxy<br>
- Proxy checks if object is on Bloom Filter for blacklisted objects, finds match<br>
- Proxy checks if object is on Trie of blacklisted objects, finds no match (false positive)<br>
- Proxy checks local cache for object, finds no match<br>
- Proxy requests object from server<br>
- Server checks locally for object, finds no match<br>
- Server returns *no object found* to Proxy<br>
- Proxy returns *no object found* to Client<br><br>
![Client requests nonexistent object FP Image](docs/nonstandard-request-none.png)<br><br>
![Packet Transmission: Client requests nonexistent object FP Image](docs/packet-nonstandard-request-none.png)<br><br><br><br>




----------------------------
<a name="component-design"/>

Component Design
================

+ **Packet Design**<br>
+ **TLS Design**<br>
+ **Rendezvous Hashing Design**<br>
+ **Bloom Filter Design**<br>
+ **Trie Design**<br><br>


<a name="packet-design"/>

## Packet Design
The packet design when using TLS is much simpler than if one were to incorporate a checksum and manually encrypt, where you can just send . However, since proxies and clients will be receiving different types of packets, we need some way of specifying the type of specification. Therefore we've reduced it down to 5 prefixes on the packet that will specify what to anticipate with packet requests coming in and packets sent out. The 5 packet prefixes are: **INI**, **GET**, **PUT**, **NON**, **DEN**.<br><br>

### INI
+ Used by the proxy during [Proxy Initialization](#initial-phase-proxy) to notify the server that the proxy is ready, and to request the bloom filter from the server.<br> 
+ Used by the server to respond to the proxy during [Proxy Initialization](#initial-phase-proxy) to receive the bloom filter from the server.<br><br>

### GET
+ Used by the client during [Standard Application Process](#standard-phase) and [Nonstandard Application Process](#nonstandard-phase) to send an object request to the proxy<br>
+ Used by the proxy during [Standard Application Process](#standard-phase) and [Nonstandard Application Process](#nonstandard-phase) to send an object request to the server<br>

### PUT
+ Used by the proxy during [Standard Application Process](#standard-phase) and [Nonstandard Application Process](#nonstandard-phase) to return an object to the client<br>
+ Used by the server during [Standard Application Process](#standard-phase) and [Nonstandard Application Process](#nonstandard-phase) to return an object to the proxy<br>

### NON
+ Used by the server during [Standard Client requests nonexistent object](#standard-phase-scene-3) and [Nonstandard Client requests nonexistent object](#nonstandard-phase-scene-3) to notify the proxy that the object requested does not exist on the server<br>
+ Used by the proxy during [Standard Client requests nonexistent object](#standard-phase-scene-3) and [Nonstandard Client requests nonexistent object](#nonstandard-phase-scene-3) to notify the client that the object requested does not exist on the proxy or the server<br>

### DEN
+ Used by the proxy during [Standard Client requests blacklisted object](#standard-phase-scene-4) to notify the client that the object requested was a blacklisted object<br><br>

<a name="tls-design"/>

## TLS Design





<a name="rendezvous-hashing-design"/>

## Rendezvous Hashing Design
The [Rendezvous (HRW) Hash](https://en.wikipedia.org/wiki/Rendezvous_hashing) is a standardized optimization of [Consistent Hashing](https://en.wikipedia.org/wiki/Consistent_hashing) used to solve a distributed hash table problem. In this case, we have many objects distributed across multiple proxies. In order to ensure a relatively even distribution of objects, let **O** denote the object name, **P** denote the proxy name, **S** be some string to hash, **k** be some integer key, and **h(S, k)** be the [Murmur3 Hash Function](https://en.wikipedia.org/wiki/MurmurHash) (Also see the [SMHasher Repository](https://github.com/rurban/smhasher) for the full implementation). To determine which proxy to send the object to, we run **h(O||P, k)** with static k for each proxy, and select the largest hash value. Whichever proxy was used in that largest hash value, is the proxy that we will distribute or receive the object from. The original paper <br><br>


<a name="bloom-filter-design"/>

## Bloom Filter Design





<a name="trie-design"/>

## Trie Design




-------------------------
<a name="system-design"/>

System Design
=============




<a name="client-design"/>

## Client Design





<a name="server-design"/>

## Server Design





<a name="proxy-design"/>

## Proxy Design






<a name="blacklist-design"/>

## Blacklist Design





--------------------------------
<a name="distribution-of-work"/>

Distribution of work
====================





<a name="jacob-contribution"/>
  
## Jacob's Contributions
+ Created Rendezvous hashing design<br>
+ Created Bloom Filter design<br>



<a name="richard-contribution"/>  

## Richard's Contributions  
+ Designed the [README](README.md)<br>
+ Created Server/Proxy/Client Architecture<br>
+ Created the Packet design<br>



-----------------------
<a name="final-words"/>

Final Words
===========



----------------------
<a name="references"/>

References
==========

[Murmur Hash](https://en.wikipedia.org/wiki/MurmurHash)<br>
[Murmur Hash Header](https://github.com/rurban/smhasher/blob/master/MurmurHash3.h)<br>
[Murmur Hash Implementation](https://github.com/rurban/smhasher/blob/master/MurmurHash3.cpp)<br>
[Bloom Filter Analysis](https://www.cs.ucr.edu/~ravi/Papers/Jrnl/BloomFilterAnalysis.pdf)<br>
[TLS Manual](https://man.openbsd.org/tls_init.3)<br>
[Bob Beck TLS Tutorial](https://github.com/bob-beck/libtls/blob/master/TUTORIAL.md)<br>
[Certificate Authority](https://en.wikipedia.org/wiki/Certificate_authority)<br>
[Consistent Hashing](https://en.wikipedia.org/wiki/Consistent_hashing)<br>
