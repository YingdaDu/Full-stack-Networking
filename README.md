CS 4410 Operating Systems Fall 2016
___

10-P3: Full Stack Networking
=====================

**Due**: Wednesday, November 2 @ 11:59 PM

Overview
--------

In this project, I built a full networking stack using the C socket interface, and implemented the socket level calls in order to form point to point connections. 
Flooding (gossip) mechanism was implemented to flood messages to all other nodes in the network. A link-state
routing protocol was applied to broadcast whatever data it received to all of its neighbors. A message sending interface that will allow nodes to send messages using the shortest path based on dijkstra's algorithm.


The Details
-----------

There are several distinct components to this project:
    
1. In `connect.c` `main()` method, 
        
    * Create a non-blocking TCP socket using the `socket()` and
      `fcntl()` system calls.
    * Set the `SO_REUSEADDR` option on the socket using the
      `setsockopt()`
    * Use `bind()` to bind the socket to `bind_port`
    * make a non-blocking socket in `try_connect` and `server_handler` 
      where indicated. After this part, you should be able to compile 
      and run your code.  At the prompt, you should be able to connect 
      two machines together using the connect command, which has the form:
        
        > C192.168.2.248:54292
        
    Achieved gossip messages to each other. Gossip messages have the form:
            
    > G&lt;src_addr:port&gt;/&lt;counter&gt;/&lt;payload&gt;\n



3. Implemented a flooding mechanism that will broadcast active connections 
every time connection state changes. Used the gossip mechanism mentioned in 
part 1. A gossip message has the form:

    > G&lt;src_addr:port&gt;/&lt;counter&gt;/&lt;payload&gt;\n

    where the payload is

    > ";&lt;addr1:port1&gt;;&lt;addr2:port2&gt;;&lt;addr3:port3&gt;...\n"

    Essentially, the payload is a list of hosts to which you are
    connected. 

    There are two optimizations.

    1. Don't tell the person who sent you the message.
    2. Don't spread around messages you have seen already.

  Note that node names are strings. However, we give you ways to convert 
  from a string version of a node name to a `sockaddr_in` and back in 
  `link_state.c`.

4. Implemented a link-state routing algorithm and a
new 'send' message. The send message has a similar format as the
gossip format. The main difference is that the address belongs to the
destination, not the source, and the TTL is the maximum number of hops
for this packet (TTL >= 0).

    > S&lt;dst_addr:port&gt;/&lt;TTL&gt;/&lt;payload&gt;\n

    Remember, the link-state algorithm recomputes the shortest path
    every time it receives a new routing update, so the message will
    be sent through the current shortest path. When operating on a
    send message, 
    1. If the send message belongs to you, then print out the payload.
    2. If the send message belongs to someone else, then decrement the
    TTL by 1. If TTL <= 0 then drop the message. Otherwise, forward it
    according to the shortest path.





