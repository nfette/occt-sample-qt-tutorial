#ifndef NX_TRI3_H
#define NX_TRI3_H

class nx_tri3
{
public:
  nx_tri3();
  nx_tri3(int node1Index, int node2Index, int node3Index);
  int node1Index;
  int node2Index;
  int node3Index;
};

#endif // NX_TRI3_H
