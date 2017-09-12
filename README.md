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
    
1. For the first part, you will be working in `connect.c` and
utilizing several standard socket API calls, which (along with their
respective options) may be found in the man pages on your terminal or
online. In the `main()` method, you will have to insert code where
indicated that does the following:
        
    * Create a non-blocking TCP socket using the `socket()` and
      `fcntl()` system calls.
    * Set the `SO_REUSEADDR` option on the socket using the
      `setsockopt()`
    * Use `bind()` to bind the socket to `bind_port`
    * In addition, you will have to make a non-blocking socket in
      `try_connect` and `server_handler` where indicated. After this
      part, you should be able to compile and run your code.  At the
      prompt, you should be able to connect two machines together
      using the connect command, which has the form:
        
        > C192.168.2.248:54292
        
        This will connect your node to another node with IP address 
        192.168.2.248 and port 54292. You can also use domain names 
        instead of IP addresses in this command.
    
    In addition, you should be able to gossip messages to each
    other. Gossip messages have the form:
            
    > G&lt;src_addr:port&gt;/&lt;counter&gt;/&lt;payload&gt;\n

    For now, you will be able to put arbitrary payloads in them. You
    may have to put a print statement in `gossip.c` in order to see
    the gossip protocol working. Later on, you **will** be changing
    the gossip protocol so that only link-state messages will be
    flooded. The `counter` field in the gossip message is simply there
    for versioning: a higher counter from the same source means that
    the message has been updated. **A node will only keep track of the
    most recent gossip**.



3. Now, you must implement a flooding mechanism that will broadcast
your active connections every time your connection state changes. This
will use the gossip mechanism mentioned in part 1. A gossip message
will have the form:

    > G&lt;src_addr:port&gt;/&lt;counter&gt;/&lt;payload&gt;\n

    where the payload is

    > ";&lt;addr1:port1&gt;;&lt;addr2:port2&gt;;&lt;addr3:port3&gt;...\n"

    Essentially, the payload is a list of hosts to which you are
    connected. From this point onwards, your gossip messages **must**
    be of this format in order to be interoperable with other
    solutions.

    The simplest way to implement the flooding mechanism is by simply
    broadcasting the message you have received to everyone you
    know. However, these "broadcast storms" are not good, so we make
    two optimizations.

    1. Don't tell the person who sent you the message.
    2. Don't spread around messages you have seen already.

  Note that node names are strings. However, we give you ways to convert 
  from a string version of a node name to a `sockaddr_in` and back in 
  `link_state.c`.

4. Finally, you must implement a link-state routing algorithm and a
new 'send' message. The send message has a similar format as the
gossip format. The main difference is that the address belongs to the
destination, not the source, and the TTL is the maximum number of hops
for this packet (TTL >= 0).

    > S&lt;dst_addr:port&gt;/&lt;TTL&gt;/&lt;payload&gt;\n

    Remember, the link-state algorithm recomputes the shortest path
    every time it receives a new routing update, so the message will
    be sent through the current shortest path. When operating on a
    send message, you must do the following:

    1. If the send message belongs to you, then print out the payload.
    2. If the send message belongs to someone else, then decrement the
    TTL by 1. If TTL <= 0 then drop the message. Otherwise, forward it
    according to the shortest path.

    Note: Though it is not required, the idea of routing tables as seen in 
    Problem Solving Sessions and lecture might be helpful for this part.





