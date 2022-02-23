#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <fstream>

using namespace std;

struct Connection;

struct Location
{
  int location;
  float longitude;
  float latitude;
  string state;
  string name;
  float fdestination;
  vector <Connection*> r;
  float sdestination;
  Location *prev;
  int check;

  Location(int lo, float ln, float la, string nm, float dis, string st) 
  {
    location = lo;
    longitude = ln;
    latitude = la;
    name = nm;
    fdestination = dis;
    state = st;
  }
};

struct Connection
{
  string placename;
  float distance;
  Location * begin;
  Location * end;
  string type;
	
  Connection(Location * b, Location * e, string pn, float d, string t)
  {
    begin = b; 
    end = e; 
    placename = pn; 
    distance = d;
    type = t;
  }
};

struct PQ
{
  vector<Location*> L;
  int add(Location* c)
  {
    L.push_back(c);
    return 0;
  }
  Location* pop()
  {
    if (L.empty())
      return NULL;
    Location *c = L.front();
    L.erase(L.begin());
      return c;
  }
    int empty()
  {
    return L.empty();
  }
};

struct Graph
{
  Location * begin;
  Location * end;
  double total;
  vector <Location*> L;
};

Graph * read_graph(const char* inf1, const char* inf2)
{
  const int size = 100;
  Graph * g = new Graph;
  g->begin = NULL;
  g->end = NULL;
  ifstream intxn(inf1);
  ifstream cnxn(inf2);
  float x, y, dis;
  char state[size];
  char name[size];
  char buffer[size];
  char type[size];
  int location = 0;
  while (!intxn.eof())
  {
    intxn.getline(buffer, size);
    sscanf(buffer, "%f%f%f%s", &x, &y, &dis, state);
    int sp = 0;
    int i, check = 0;
    for (i = 0; i < strlen(buffer); i++)
    {
      if (check == 0)
      {
        if (buffer[i]!=' ')
          check = 1;
        continue;
      }
        if (buffer[i] == ' ')
        {
          sp++;
        }
        while (buffer[i] == ' ')
          i++;
          if (sp == 4)
            break;
      }
      strcpy(name, &buffer[i]);
      string city_state(state);
      string city_name(name);
      Location *c = new Location(location++, x, y, city_name, dis, city_state);
      g->L.push_back(c);
  }
  while (!cnxn.eof())
  {
     cnxn.getline(buffer, size);
     int a;
     int b;
     sscanf(buffer, "%s%s%d%d%f", name, type, &a, &b, &dis);
     string road_name(name);
     string road_type(type);
     Connection  * c1 = new Connection(g->L[a], g->L[b], road_name, dis, road_type);
     Connection *c2 = new Connection(g->L[b], g->L[a], road_name, dis, road_type);
     g->L[a]->r.push_back(c1);
     g->L[b]->r.push_back(c2);
   }
  return g;
}

float get_distance(Location *start, Location *end)
{
  if (start == end)
    return 0;
  for (int i = 0; i < start->r.size(); i++)
  {
    if (end->location == start->r[i]->end->location)
    {
      return start->r[i]->distance;
    }
  }
  return -1;
}

void print_steps(Location* c)
{
  cout << "Intersection " << c->location << ", " << c->fdestination 
       << " miles from " << c->name << ", " << c->state;
}

void show_path(Graph * g, Location * l)
{
  if (l == NULL)
    return;
  else
    show_path(g, l->prev);
  if (l->prev != NULL)
  {
    int x;
    float y = -1;
    for (int i = 0; i < l->r.size(); i++)
    {
      if (l->prev == l->r[i]->begin || l->prev == l->r[i]->end)
      {
         if (y < 0) 
         {
            y = l->r[i]->distance;
            x = i;
         }
         else if (y > l->r[i]->distance)
         {
            y = l->r[i]->distance;
            x = i;
         }
       }
      if (y > 0)
      {
        cout << ". Take " << l->r[x]->placename << " to:" << endl;
        g->total += l->r[x]->distance;
          break;
      }
    }
  }
  print_steps(l);
}

int main()
{
  Graph* g = read_graph("/home/www/class/een318/intersections.txt", "/home/www/class/een318/connections.txt");
  int start, dest;
  cout << "From intersection: ";
  cin >> start;
  cout << "To intersection: ";
  cin >> dest;
  cout << endl;
  g->begin = g->L[start];
  g->end = g->L[dest];
  g->total = 0;

  PQ q;

  for (vector<Location*>::iterator j = g->L.begin(); j != g->L.end(); j++)
  {
     if (g->begin == * j)
     {
       g->begin->prev = NULL;
       (* j)->sdestination = 0;
     }
     else if (get_distance(g->begin, * j) > 0)
     {
       (* j)->prev = g->begin;
       (* j)->sdestination = get_distance(g->begin, *j);
     }
     else
     {
       (* j)->prev = NULL;
       (* j)->sdestination = -1;
     }
   }
   g->begin->check = 1;
   q.add(g->begin);
   while (!(q.empty()))
   {
     Location *c = q.pop();
     for (vector<Connection *>::iterator i = c->r.begin(); i != c->r.end(); i++)
     {
       if ((*i)->end->check == 0) 
       {
         (*i)->end->check = 1;
         q.add((*i)->end);
       }
       float a = (*i)->end->sdestination;
       float b = get_distance(c, (*i)->end);
       if (b == -1)
       ;
       else if (a == -1 && b > 0)
       {
         (*i)->end->sdestination = b + c->sdestination;
         (*i)->end->prev = c;
       }
       else if (a > b + b + c->sdestination)
       {
         (*i)->end->sdestination = b + c->sdestination;
         (*i)->end->prev = c;
       }
      }
    }
 show_path(g, g->end);
 cout << "\n";
}
