#include "nx_tri3.h"

nx_tri3::nx_tri3()
{
  node1Index = 0;
  node2Index = 0;
  node3Index = 0;
}

nx_tri3::nx_tri3(int node1Index, int node2Index, int node3Index)
  : node1Index(node1Index), node2Index(node2Index), node3Index(node3Index)
{
}
