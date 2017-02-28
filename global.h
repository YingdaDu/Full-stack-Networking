#include <arpa/inet.h>

double timer_now(void);
void timer_start(double when, void (*handler)(void *arg), void *arg);
int timer_check(void);

struct file_info;
struct sockaddr_in;
struct node_list;
struct node_list *graph_list;
struct gossip;

int counter;
char *neighbor_addrs;
void gossip_to_peer(struct file_info *fi);
void gossip_received(struct file_info *fi, char *line);
struct gossip* gossip_next(struct gossip* gossip);
struct sockaddr_in gossip_src(struct gossip* gossip);
char* gossip_latest(struct gossip* gossip);

void file_info_send(struct file_info *fi, char *buf, int size);
void file_broadcast(char *buf, int size, struct file_info *fi);
struct file_info* sockaddr_to_file(struct sockaddr_in dst);

char* addr_to_string (struct sockaddr_in addr);
struct sockaddr_in string_to_addr(char* string);
void dijkstra(int graph[], int nnodes, int src, int dist[], int prev[]);
void set_dist(struct node_list *nl, int graph[], int nnodes, char *src, char *dst, int dist);
struct node_list *create_graph_list(char *neighbor_addrs,struct sockaddr_in my_addr);
void set_graph(char *neighbor_addrs, struct sockaddr_in my_addr, 
              int graph[], struct node_list *graph_list, int size);

int nl_index(struct node_list *nl, char *node);
struct node_list *nl_create(void);
void nl_add(struct node_list *nl, char *node);
char *nl_name(struct node_list *nl, int index);
void nl_destroy(struct node_list *nl);
int nl_nsites(struct node_list *nl);


int addr_get(struct sockaddr_in *sin, const char *addr, int port);
int addr_cmp(struct sockaddr_in a1, struct sockaddr_in a2);

