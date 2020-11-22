<a name="overview"/>


TLS Cache
=========
A Final Project for CS165 Computer Security designed by Jacob Tan and Richard Duong<br>
[Link to the Github Repository here](https://github.com/richard-duong/TLSCache)<br>

## Objective:
Implement a secure proxy application using TLS protocol to provide simple authentication and secure file transmission. With this, we can demonstrate how a large scale system with a server caching objects inside of proxies can interact with a client and provide objects upon request in a secure and optimal manner.<br>
[The full assignment specifications](docs/finalproject.pdf)<br><br>

___
<a name="table-of-contents"/>


Table of Contents
=================
1. [Overview](#overview)<br>
2. [Table of Contents](#table-of-contents)<br>
3. [How to use](#how-to-use)<br>
4. [Phase Design](#phase-design)<br>
        a) [Initialization](#init-phase)<br>
        b) [Application without False Positive](#no-fp-phase)<br>
        c) [Application with False Positive](#fp-phase)<br>
5. [System Design](#system-design)<br>
        a) [Client](#client-design)<br>
        b) [Server](#server-design)<br>
        c) [Proxy](#proxy-design)<br>
        d) [Blacklist](#blacklist-design)<br>
6. [Component Design](#component-design)<br>
        a) [Packet](#packet-design)<br>
        b) [TLS](#tls-design)<br>
        c) [Rendezvous Hashing](#rendezvous-hashing-design)<br>
        d) [Bloom Filter](#bloom-filter-design)<br>
7. [Distribution of Work](#distribution-of-work)<br>
        a) [Jacob's Contributions](#jacob-contribution)<br>
        b) [Richard's Contributions](#richard-contribution)<br>
8. [Final Words](#final-words)<br>
9. [References](#references)<br>



___
<a name="how-to-use"/>

How to use
==========



___
<a name="phase-design"/>

Phase Design
============

**1) Initialization Phase**<br>
This phase prepares the server and proxies with the appropriate blacklists & bloom filters<br>

**2) Application without False Positive Phase**<br>
This phase evaluates our clients' requests w/o a match on the bloom filter<br>

**3) Application with False Positive Phase**<br>
This phase evaluates our clients' requests w/ a match on the bloom filter<br><br>

___
<a name="init-phase"/>

## 1) Initialization Phase

### Server Initialization<br>
**Has access to:**
- Entire object file<br>
- Blacklist object file<br>
- Proxy name/port list<br>
- HRW hash key<br>
- BF hash keys<br>

**Steps:**
- Read Objects from File<br>
- Generate bloom filter for each proxy to use<br>
- Wait for all proxies to connect<br><br>

### Proxy Initialization<br>
**Has access to:**
- Server port<br>
- TLS certificate<br>
- Relevant blacklist object file<br>

**Steps:**
- Generate trie from reading relevant blacklist<br>
- Request the bloom filter from the server<br>
- Retrieve and copy the bloom filter from the buffer<br><br>

![Proxy Initialization Image](img/init-proxy.png)

### Application deploy<br>
**Steps:**
- Clients can now be instantiated and the hashes become public<br>

<br><br>
<a name="no-fp-phase"/>

## 2) Application without False Positive Phase

### Scenario 1: Client finds request on proxy cache
**Steps:**
- Client runs HRW on object to determine which proxy holds the cached object<br>
- Client requests object from result proxy<br>
- Proxy runs object on Bloom Filter for blacklisted object, finds nothing<br>
- Proxy checks local cache for object, finds object<br>
- Proxy returns object to Client<br>

![Client finds request on proxy cache no FP Image](img/find-proxy-no-fp.png)<br><br>

### Scenario 2: Client finds request on server
**Steps:**
- Client runs HRW on object to determine which proxy holds the cached object<br>
- Client requests object from result proxy<br>
- Proxy runs object on Bloom Filter for blacklisted object, finds nothing<br>
- Proxy checks local cache for object, finds nothing<br>
- Proxy requests object from server<br>
- Server checks locally for object, finds object<br>
- Server returns object to Proxy<br>
- Proxy returns object to Client<br>

![Client finds request on server no FP Image](img/find-server-no-fp.png)<br><br>

### Scenario 3: Client does not find object
**Steps:**
- Client runs HRW on object to determine which proxy holds the cached object<br>
- Client requests object from result proxy<br>
- Proxy runs object on Bloom filter for blacklisted object, finds nothing<br>
- Proxy checks local cache for object, finds nothing<br>
- Proxy requests object from server<br>
- Server checks locally for object, finds nothing<br>
- Server returns no object found to Proxy<br>
- Proxy returns no object found to Client<br>

![Client does not find object no FP Image](img/find-none-no-fp.png)<br><br>


<a name="fp-phase"/>

## 3) Application with False Positive Phase




___
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


___
<a name="component-design"/>

Component Design
================




<a name="packet-design"/>

## Packet Design




<a name="tls-design"/>

## TLS Design




<a name="rendezvous-hashing-design"/>

## Rendezvous Hashing Design
The [Rendezvous Hash](https://en.wikipedia.org/wiki/Rendezvous_hashing) is used to solve the distributed hash table problem. In this case, we have many objects distributed across multiple proxies. In order to ensure a relatively even distribution of objects, let **O** denote the object name, **P** denote the proxy name, and **h(S)** be the [Polynomial Rolling Hash Function](https://en.wikipedia.org/wiki/Rolling_hash). To determine which proxy to send the object to, we run **h(O||P)** for each proxy, and select the largest hash value. Whichever proxy was used in that largest hash value, is the proxy that will receive the object.<br><br>


<a name="bloom-filter"/>

## Bloom Filter Design


___
<a name="distribution-of-work"/>

Distribution of work
====================




<a name="jacob-contribution"/>
  
## Jacob's Contributions


<a name="richard-contribution"/>  

## Richard's Contributions

  
+ Designed the [README](README.md)<br>
+ 
  
___ 
<a name="final-words"/>

Final Words
===========


___
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


