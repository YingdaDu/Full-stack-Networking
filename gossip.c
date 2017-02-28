#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "global.h"

struct gossip {
    struct gossip *next;
    struct sockaddr_in src;
    long counter;
    char *latest;
};
static struct gossip *gossip;

extern struct sockaddr_in my_addr;

struct gossip* gossip_next(struct gossip* gossip) {
    return gossip->next;
}

struct sockaddr_in gossip_src(struct gossip* gossip) {
    return gossip->src;
}

char* gossip_latest(struct gossip* gossip) {
    return gossip->latest;
}

/* A gossip message has the following format:
 *
 *  G<src_addr:src_port>/counter/payload\n
 *
 * Here <src_addr:src_port>/counter uniquely identify a message from
 * the given source.
 */
void gossip_received(struct file_info *fi, char *line){
    char *port = index(line, ':');
    if (port == 0) {
        fprintf(stderr, "do_gossip: format is G<addr>:<port>/counter/payload\n");
        return;
    }
    *port++ = 0;

    char *ctr = index(port, '/');
    if (ctr == 0) {
        fprintf(stderr, "do_gossip: no counter\n");
        return;
    }
    *ctr++ = 0;

    char *payload = index(ctr, '/');
    if (payload == 0) {
        fprintf(stderr, "do_gossip: no payload\n");
        return;
    }
    *payload++ = 0;
 
    /* Get the source and message identifier.
     */
    struct sockaddr_in addr;
    if (addr_get(&addr, line, atoi(port)) < 0) {
        return;
    }
    long counter = atol(ctr);

    // char *is_from = addr_to_string (addr);
    // printf("%s\n", neighbor_addrs);
    // if (strstr(neighbor_addrs, is_from)) {
    /* See if we already have this gossip.
     */
    struct gossip *g;
    for (g = gossip; g != 0; g = g->next) {
        if (addr_cmp(g->src, addr) != 0) {
            continue;
        }
     

        if (g->counter >= counter) {
            printf("already know about this gossip\n");
            return;
        }
        free(g->latest);
        break;
    }
    if (g == 0) {
        g = calloc(1, sizeof(*g));
        g->src = addr;
        g->next = gossip;
        gossip = g;
    }

    /* Restore the line.
     */
    *--port = ':';
    *--ctr = '/';
    *--payload = '/';
    printf("gossip receive : %s\n", line);
    /* Save the gossip.
     */
    int len = strlen(line);
    g->latest = malloc(len + 1);
    memcpy(g->latest, line, len + 1);
    g->counter = counter;
    
    /* Send the gossip to all connections except the one it came in on.
     */
    char *msg = malloc(len + 3);
    sprintf(msg, "G%s\n", g->latest);
    file_broadcast(msg, len + 2, fi);
    free(msg);
}
 

/* Send all gossip I have to the given peer.
 */
void gossip_to_peer(struct file_info *fi){
    struct gossip *g;

    for (g = gossip; g != 0; g = g->next) {
        file_info_send(fi, "G", 1);
        file_info_send(fi, g->latest, strlen(g->latest));
        file_info_send(fi, "\n", 1);
    }
}

struct node_list *create_graph_list(char *neighbor_addrs,struct sockaddr_in my_addr){
   char *set = malloc(40);
   char *port = malloc(40);
   sprintf(port, "%s:%d", inet_ntoa(my_addr.sin_addr), ntohs(my_addr.sin_port));
   graph_list = nl_create();
   nl_add(graph_list, port);
   strcpy(set, port);
   free(port);
   char *addr_tmp = malloc(strlen(neighbor_addrs));
   sprintf(addr_tmp, "%s", neighbor_addrs);
   char *addrs = index(addr_tmp, ';'); 
   const char s[2] = ";";
   char *token;
   token = strtok(addrs, s);
   while (token != NULL) {
    if (!strstr(set, token)){
      set = realloc(set, strlen(set) + 40);
      strcat(set, token);
      char *as = token;
      nl_add(graph_list, as);
    }
    token = strtok(NULL, s);
 }//added neighbors and itself
char *src = malloc(512);
char *tmp = malloc(512);
 struct gossip *g;
 for (g = gossip; g != 0; g = g->next) {
     const char a[2] = "/";
     memset(src, 0, 512 );
     memset(tmp, 0, 512);
     memmove(src, g->latest, strlen(g->latest));
     memmove(tmp, g->latest, strlen(g->latest));
     src = strtok(src, a);
  if (!strstr(set, src)){
     nl_add(graph_list, src);
     set = realloc(set, strlen(set) + 40);
     strcat(set, src);
     }
     char *neigh = index(tmp, ';'); 
     char *token1;
     token1 = strtok(neigh, s);
     while (token1 != NULL) {
      if (!strstr(set, token1)){
       nl_add(graph_list, token1);
       set = realloc(set, strlen(set) + 40);
       strcat(set, token1); 
      }
       token1 = strtok(NULL, s);
     }
 }
    free(tmp);
    free(src);
    free(set);
    free(addr_tmp);
    return graph_list;
}
void set_graph(char *neighbor_addrs, struct sockaddr_in my_addr, 
              int graph[], struct node_list *graph_list, int size){
  char *src = malloc(40);
  char *addr_tmp = malloc(strlen(neighbor_addrs));
  sprintf(addr_tmp, "%s", neighbor_addrs);
  sprintf(src, "%s:%d", inet_ntoa(my_addr.sin_addr), ntohs(my_addr.sin_port));
 // printf("set graph %s to %s as 0\n", src, src);
  set_dist(graph_list, graph, size, src, src, 0);
  char *addrs = index(addr_tmp, ';'); 
  const char s[2] = ";";
  char *token;
  token = strtok(addrs, s);
  while (token != NULL) {
   // printf("set graph %s to %s as 1\n", src, token);
    set_dist(graph_list, graph, size, src, token, 1);
    token = strtok(NULL, s);
 }// set neighbors
 char *tmp = malloc(512);
 char *tmp1 = malloc(512);
 struct gossip *g;
 for (g = gossip; g != 0; g = g->next) {
    // printf("gossip set graph %s\n", g->latest);
      const char a[2] = "/";
      char *connects;
      memset(tmp, 0, 512);
      memmove(tmp, g->latest, strlen(g->latest));
      memset(tmp1, 0, 512);
      memmove(tmp1, g->latest, strlen(g->latest));
      connects = strtok(tmp, a);
      char *f_neigh = index(tmp1, ';');
      char *token1;
      token1 = strtok(f_neigh, s);
     while (token1 != NULL) { 
      //  printf("set graph %s to %s as 1\n", connects, token1);
        set_dist(graph_list, graph, size, connects, token1, 1); 
        token1 = strtok(NULL, s);
       
     }  
  }
  free(tmp);
  free(tmp1);
  free(src);
}


